#include "readXMLAVR.h"
#include "QFile"
#include "QMessageBox"
#include "QDebug"
#include "QDomElement"
#include <QtCore/qmath.h>
#include "Libs/globalVariables.h"
#include <QDir>
ReadXMLAVR::ReadXMLAVR()
{

}

void ReadXMLAVR::saveAllDevicesToDB()
{
   QDir dir;
   dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
   dir.setSorting(QDir::Name);
   dir.setPath("./XML/AVR/");
   QFileInfoList list = dir.entryInfoList();
   for (int i = 0; i < list.size(); ++i)
   {
      QFileInfo fileInfo = list.at(i);
      //qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
      //                     .arg(fileInfo.fileName()));
      QString fileName=fileInfo.fileName().toUpper();
      if(fileName.endsWith(".XML"))
      {
         QString avrName=fileName.left(fileName.length()-4);
         qDebug()<<"Avr:"<<avrName;
         saveToDB(avrName);
      }
   }
}

void ReadXMLAVR::saveToDB(QString avrName)
{
   AvrFuse *avrFuse=readXML(avrName);
   DatabaseLayer *db=GlobalVariables::db;
   if(avrFuse)
   {
      SqlParameters parameters;
      parameters.append(SqlParameter("Name", avrName));

      QSqlQueryModel *model=db->filter("AvrDevices",  parameters);
      qint32  avrDeviceID=0;
      if(model->rowCount()>0)
         avrDeviceID=model->record(0).value("AvrDeviceID").toInt();
      else
      {
         avrDeviceID = db->insert("AvrDevices", parameters, "AvrDeviceID");
         if(avrDeviceID>0)
         {
            qint32 lowFuseByteID = avrFuse->low.saveToDB(0);
            qint32 highFuseByteID = avrFuse->high.saveToDB(0);
            qint32 extendFuseByteID = avrFuse->extend.saveToDB(0);
            qint32 lockFuseByteID = avrFuse->lock.saveToDB(0);
            db->update("AvrDevices", SqlParameters()
                       <<SqlParameter("LowFuseByteID", lowFuseByteID)
                       <<SqlParameter("HighFuseByteID", highFuseByteID)
                       <<SqlParameter("ExtendFuseByteID", extendFuseByteID)
                       <<SqlParameter("LockFuseByteID", lockFuseByteID)
                       , SqlParameters() <<SqlParameter("AvrDeviceID", avrDeviceID));
         }
      }

   }
}

void ReadXMLAVR::addNavigations(QXmlStreamReader &xmlReader, FuseByte *fuseByte, int textCount)
{
   quint8 lastMask=0;
   FuseNavigationGroup fuseNavigationGroup;
   for(int i=0;i<textCount;i++)
   {
      FuseNavigation fuseNavigation;
      XMLFunctions::goToStartElement(xmlReader, "MASK");

      quint8 mask=xmlReader.text().toString().right(2).toUInt(0, 16);

      XMLFunctions::goToStartElement(xmlReader, "VALUE");
      fuseNavigation.value=xmlReader.text().toString().right(2).toUInt(0, 16);

      XMLFunctions::goToStartElement(xmlReader, "TEXT");
      fuseNavigation.text=xmlReader.text().toString();

      qDebug()<<mask<<fuseNavigation.value<<fuseNavigation.text;

      if(mask!=lastMask && fuseNavigationGroup.members.count()>0)
      {
         fuseByte->navigationGroups.append(fuseNavigationGroup);
         fuseNavigationGroup.members.clear();
      }
      fuseNavigationGroup.members.append(fuseNavigation);
      fuseNavigationGroup.mask=mask;
      lastMask=mask;
   }
   fuseByte->navigationGroups.append(fuseNavigationGroup);
}

AvrFuse *ReadXMLAVR::readXML(QString avrName)
{
   AvrFuseType::AvrFuseType status=AvrFuseType::NONE;

   QFile *xmlFile = new QFile("XML/AVR/"+avrName+".xml");
   if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
   {
      //QMessageBox::critical(this,"Load XML File Problem",
      //                    "Couldn't open xmlfile.xml to load settings for download",
      //                  QMessageBox::Ok);
      return NULL;
   }
   QXmlStreamReader xmlReader(xmlFile);

   AvrFuse *avrFuse = new AvrFuse();
   FuseByte *fuseByte;

   //Parse the XML until we reach end of it
   QString data="";
   int bitCount=0;
   int textCount=0;

   while(!xmlReader.atEnd() && !xmlReader.hasError())
   {
      // Read next element
      QXmlStreamReader::TokenType token = xmlReader.readNext();

      //If token is just StartDocument - go to next
      if(token == QXmlStreamReader::StartDocument)
      {
         continue;
      }
      //If token is StartElement - read it
      if(token == QXmlStreamReader::StartElement)
      {
         if(xmlReader.name()=="PART_NAME")
         {
            xmlReader.readNext();
            QString microName=xmlReader.text().toString();
            qDebug()<<microName;
         }

         if(xmlReader.name()=="LOCKBIT")
         {
            fuseByte=&avrFuse->lock;
            status=AvrFuseType::FuseLock;
         }

         if(status==AvrFuseType::FuseLock && xmlReader.name()=="NMB_LOCK_BITS")
         {
            xmlReader.readNext();
            bitCount=xmlReader.text().toString().toUInt();
         }

         if(status==AvrFuseType::FuseLock && xmlReader.name()=="NMB_TEXT")
         {
            xmlReader.readNext();
            textCount=xmlReader.text().toString().toUInt();
         }

         if(status==AvrFuseType::FuseLock && textCount>0 && xmlReader.name()=="TEXT1")
         {
            addNavigations(xmlReader, fuseByte, textCount);
            textCount=0;
         }

         if(status==AvrFuseType::FuseLock && bitCount>0 && xmlReader.name().startsWith("LOCKBIT"))
         {
            fuseByte->defualt=0xFF;
            fuseByte->value=fuseByte->defualt;

            BitInfo bitInfo;
            QString tagName="";
            for(int i=0;i<bitCount;i++)
            {
               if(i>0)
                  XMLFunctions::goToSimilarStartElement(xmlReader, "LOCKBIT");
               tagName=xmlReader.name().toString();
               bitInfo.bitNumber=tagName.right(tagName.length()-7).toInt();

               XMLFunctions::goToStartElement(xmlReader, "NAME");
               bitInfo.bitName=xmlReader.text().toString();

               XMLFunctions::goToStartElement(xmlReader, "TEXT");
               bitInfo.description=xmlReader.text().toString();
               fuseByte->bitsInfo.append(bitInfo);


            }
            status==AvrFuseType::NONE;
            bitCount=0;
         }

         if(xmlReader.name()=="LOW")
         {
            fuseByte=&avrFuse->low;
            status=AvrFuseType::FuseLow;
         }
         else if(xmlReader.name()=="HIGH")
         {
            fuseByte=&avrFuse->high;
            status=AvrFuseType::FuseHigh;
         }
         else if(xmlReader.name()=="EXTENDED")
         {
            fuseByte=&avrFuse->extend;
            status=AvrFuseType::FuseExtend;
         }


         if((status==AvrFuseType::FuseLow || status==AvrFuseType::FuseHigh || status==AvrFuseType::FuseExtend)
               && xmlReader.name()=="NMB_FUSE_BITS")
         {
            xmlReader.readNext();
            bitCount=xmlReader.text().toString().toUInt();
         }


         if((status==AvrFuseType::FuseLow || status==AvrFuseType::FuseHigh || status==AvrFuseType::FuseExtend)
               && xmlReader.name()=="NMB_TEXT")
         {
            xmlReader.readNext();
            textCount=xmlReader.text().toString().toUInt();
         }

      }




      if((status==AvrFuseType::FuseLow || status==AvrFuseType::FuseHigh || status==AvrFuseType::FuseExtend)
            && (bitCount>0))
      {
         quint8 def=0;
         BitInfo bitInfo;
         QString tagName="";

         for(int i=0;i<bitCount;i++)
         {
            XMLFunctions::goToSimilarStartElement(xmlReader, "FUSE");
            tagName=xmlReader.name().toString();
            bitInfo.bitNumber=tagName.right(tagName.length()-4).toInt();

            XMLFunctions::goToStartElement(xmlReader, "NAME");
            bitInfo.bitName=xmlReader.text().toString();

            XMLFunctions::goToStartElement(xmlReader, "TEXT");
            bitInfo.description=xmlReader.text().toString();
            fuseByte->bitsInfo.insert(0, bitInfo);

            if(XMLFunctions::goToStartElement(xmlReader, "DEFAULT", "FUSE"))
            {
               //def.append(xmlReader.text().toString());
               if(xmlReader.text().toString()=="1")
                  def+=qPow(2,  bitInfo.bitNumber);
            }
         }
         fuseByte->defualt=def;//.toUInt(0, 2);
         fuseByte->value=fuseByte->defualt;
         bitCount=0;
      }



      if((status==AvrFuseType::FuseLow || status==AvrFuseType::FuseHigh || status==AvrFuseType::FuseExtend)
            && (textCount>0))
      {
         addNavigations(xmlReader, fuseByte, textCount);
         textCount=0;
      }
   }

   if(xmlReader.hasError())
   {
      //QMessageBox::critical(this,
      //                    "xmlFile.xml Parse Error",xmlReader.errorString(),
      //                  QMessageBox::Ok);
      qDebug() <<"Parse Error";
      return avrFuse;
   }

   //close reader and flush file
   xmlReader.clear();
   xmlFile->close();
   return avrFuse;
}
