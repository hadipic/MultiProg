#include "QFile"
#include "QMessageBox"
#include "QDebug"
#include "QDomElement"
#include <QtCore/qmath.h>
#include <QDirIterator>
#include <QSettings>
#include <QSqlRecord>
#include "readXMLPIC.h"



//***********************************************
//********** ReadXMLPIC functions ***************
//***********************************************
ReadXMLPIC::ReadXMLPIC()
{

}

void ReadXMLPIC::saveToDB(QString picName)
{
   PicInfo *picInfo=readXML(picName);
   picInfo->saveToDB();
}

void ReadXMLPIC::saveAllDevicesToDB()
{
   QDir dir;
   dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
   dir.setSorting(QDir::Name);
   dir.setPath("./XML/PIC/");
   QFileInfoList list = dir.entryInfoList();
   for (int i = 0; i < list.size(); ++i)
   {
      QFileInfo fileInfo = list.at(i);
      //qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
      //                     .arg(fileInfo.fileName()));
      QString fileName=fileInfo.fileName().toUpper();
      if(fileName.endsWith(".XML"))
      {
         qDebug()<<"Pic:"<<fileName;
         saveToDB(fileName.left(fileName.length()-4));
      }
   }
}

quint64 ReadXMLPIC::hexStrToInt(QString hexStr)
{
   if(hexStr.length()>2)
      return hexStr.mid(2).toUInt(0, 16);
   return 0;
}

bool ReadXMLPIC::strToDouble(double &output, const QString str)
{
   bool ok=false;
   double temp=str.toDouble(&ok);
   if(ok)
      output=temp;
   return ok;
}

PicInfo *ReadXMLPIC::readXML(QString picName)
{
   QFile *xmlFile = new QFile("XML/PIC/"+picName+".xml");
   //PicInfo2 picInfo2;
   //PicInfo2 *picInfo1=&picInfo2;


   if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
   {
      //QMessageBox::critical(this,"Load XML File Problem",
      //                    "Couldn't open xmlfile.xml to load settings for download",
      //                  QMessageBox::Ok);
      return NULL;
   }
   PicInfo *picInfo = new PicInfo();
   QXmlStreamReader xmlReader(xmlFile);

   FuseByte             *fuseConfig    = NULL;
   FuseNavigationGroup  *group         = NULL;
   FuseNavigation       *navigation    = NULL;
   FrequencyRange       *frequencyRange= NULL;
   Package              *package       = NULL;

   int configNumber=0;
   int status=0;
   quint8 shift=8;
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
         if(xmlReader.name()=="device")
         {
            picInfo->deviceTag.name          = xmlReader.attributes().value("name").toString();
            picInfo->deviceTag.status        = xmlReader.attributes().value("status").toString();
            picInfo->deviceTag.memoryTechnology= xmlReader.attributes().value("memory_technology").toString();
            picInfo->deviceTag.architecture  = xmlReader.attributes().value("architecture").toString();
            picInfo->deviceTag.selfWrite     = xmlReader.attributes().value("self_write").toString();
            picInfo->deviceTag.document      = xmlReader.attributes().value("document").toString();
            picInfo->deviceTag.alternative   = xmlReader.attributes().value("alternative").toString();

            picInfo->deviceTag.ID = hexStrToInt(xmlReader.attributes().value("id").toString());
            picInfo->deviceTag.IDHighVoltage = hexStrToInt(xmlReader.attributes().value("id_high_voltage").toString());
            picInfo->deviceTag.IDLowPower = hexStrToInt(xmlReader.attributes().value("id_low_power").toString());
            picInfo->deviceTag.pc = xmlReader.attributes().value("pc").toString().toUInt();
         }
         if(xmlReader.name()=="checksum")
         {
            Checksum checksum;
            checksum.protectedType = xmlReader.attributes().value("protected").toString();
            qint8 colonIndex=checksum.protectedType.indexOf(':');
            if(colonIndex>=0)
            {
               checksum.start = checksum.protectedType.mid(0, colonIndex).toUInt(0, 16);
               checksum.end   = xmlReader.attributes().value("protected").toString().mid(colonIndex+1).toUInt(0, 16);
               checksum.protectedType="";
            }
            checksum.type           = xmlReader.attributes().value("type").toString();
            checksum.bchecksum      = hexStrToInt(xmlReader.attributes().value("bchecksum").toString());
            checksum.cchecksum      = hexStrToInt(xmlReader.attributes().value("cchecksum").toString());
            if(xmlReader.attributes().value("protected_blocks").toString()!="")
               checksum.protectedBlocks= xmlReader.attributes().value("protected_blocks").toString().toInt();
            checksum.constant       = hexStrToInt(xmlReader.attributes().value("constant").toString());
            checksum.bbsize         = hexStrToInt(xmlReader.attributes().value("bbsize").toString());

            picInfo->checksums.append(checksum);
         }
         if(xmlReader.name()=="frequency_range")
         {
            FrequencyRange frqRange;
            frqRange.name     = xmlReader.attributes().value("name").toString();
            frqRange.special  = xmlReader.attributes().value("special").toString();
            picInfo->frequencyRanges.append(frqRange);
            frequencyRange=&picInfo->frequencyRanges.last();
         }
         if(xmlReader.name()=="frequency")
         {
            Frequency frequency;
            strToDouble(frequency.start,     xmlReader.attributes().value("start").toString());
            strToDouble(frequency.end,       xmlReader.attributes().value("end").toString());
            strToDouble(frequency.vddMin,    xmlReader.attributes().value("vdd_min").toString());
            strToDouble(frequency.vddMax,    xmlReader.attributes().value("vdd_max").toString());
            strToDouble(frequency.vddMinEnd, xmlReader.attributes().value("vdd_min_end").toString());
            frequency.osc =   xmlReader.attributes().value("osc").toString();
            frequencyRange->frequencies.append(frequency);
         }
         if(xmlReader.name()=="voltages")
         {
            Voltage voltage;
            voltage.name = xmlReader.attributes().value("name").toString();
            strToDouble(voltage.min, xmlReader.attributes().value("min").toString());
            strToDouble(voltage.max, xmlReader.attributes().value("max").toString());
            strToDouble(voltage.nominal, xmlReader.attributes().value("nominal").toString());
            picInfo->voltages.append(voltage);
         }
         if(xmlReader.name()=="voltages")
         {
            Voltage voltage;
            voltage.name = xmlReader.attributes().value("name").toString();
            strToDouble(voltage.min, xmlReader.attributes().value("min").toString());
            strToDouble(voltage.max, xmlReader.attributes().value("max").toString());
            strToDouble(voltage.nominal, xmlReader.attributes().value("nominal").toString());
            picInfo->voltages.append(voltage);
         }

         if(xmlReader.name()=="memory")
         {
            QString name=xmlReader.attributes().value("name").toString();
            Memory *memory=NULL;
            if     (name=="code")
            {
               picInfo->memoriesInfo.code = new CodeMemory();
               picInfo->memoriesInfo.code->wordWriteAlign=xmlReader.attributes().value("word_write_align").toString().toInt();
               picInfo->memoriesInfo.code->wordEraseAlign=xmlReader.attributes().value("word_erase_align").toString().toInt();
               memory = picInfo->memoriesInfo.code;
            }
            else if(name=="user_ids")
            {
               picInfo->memoriesInfo.userIDs = new UserIdsMemory();
               picInfo->memoriesInfo.userIDs->rmask = xmlReader.attributes().value("rmask").toString().mid(2).toInt(0, 16);
               memory = picInfo->memoriesInfo.userIDs;
            }
            else if(name=="device_id")
            {
               picInfo->memoriesInfo.deviceID = new Memory();
               memory = picInfo->memoriesInfo.deviceID;
            }
            else if(name=="config")
            {
               picInfo->memoriesInfo.config =new  ConfigMemory();
               picInfo->memoriesInfo.config->hexfileOffset = xmlReader.attributes().value("hexfile_offset").toString().mid(2).toInt(0, 16);
               memory = picInfo->memoriesInfo.config;
            }
            else if(name=="eeprom")
            {
               picInfo->memoriesInfo.eeprom = new EEPromMemory();
               picInfo->memoriesInfo.eeprom->rmask = xmlReader.attributes().value("rmask").toString().mid(2).toInt(0, 16);
               picInfo->memoriesInfo.eeprom->hexfileOffset = xmlReader.attributes().value("hexfile_offset").toString().mid(2).toInt(0, 16);
               memory = picInfo->memoriesInfo.eeprom;
            }
            else if(name=="debug_vector")
            {
               picInfo->memoriesInfo.debugVector = new Memory();
               memory = picInfo->memoriesInfo.debugVector;
            }
            else if(name=="calibration")
            {
               picInfo->memoriesInfo.calibration = new CalibrationMemory();
               picInfo->memoriesInfo.calibration->calOpmask = xmlReader.attributes().value("cal_opmask").toString().mid(2).toInt(0, 16);
               picInfo->memoriesInfo.calibration->calOpcode = xmlReader.attributes().value("cal_opcode").toString().mid(2).toInt(0, 16);
               memory = picInfo->memoriesInfo.calibration;
            }
            else if(name=="calibration_backup")
            {
               picInfo->memoriesInfo.calibrationBackup = new Memory();
               memory = picInfo->memoriesInfo.calibrationBackup;
            }
            else if(name=="program_executive")
            {
               picInfo->memoriesInfo.programExecutive = new Memory();
               memory = picInfo->memoriesInfo.programExecutive;
            }
            if(memory)
            {
               memory->start = xmlReader.attributes().value("start").toString().mid(2).toUInt(&memory->isValid, 16);
               memory->end   = xmlReader.attributes().value("end").toString().mid(2).toUInt(&memory->isValid, 16);

               /*if(start.length()>2)
                  memory->start = start.mid(2).toUInt(&memory->isValid, 16);
               if(end.length()>2)
                  memory->end   = end.mid(2).toUInt(&memory->isValid, 16);
               */
            }
         }
         if(xmlReader.name()=="config")
         {
            /*for (int i = 0; i < xmlReader.attributes().count(); ++i)
                {
                    qDebug()<<xmlReader.attributes().at(i).name()<<xmlReader.attributes().at(i).value();
                }
                QString configName=xmlReader.attributes().value("name").toString();
                if(configName=="")
                    configName="Word0";*/
            //if(configNumber)
            //fuseConfigs.append(fuseConfig);
            if(!fuseConfig || fuseConfig->mask>0xFF)
            {
               fuseConfig = new FuseByte;
               picInfo->fuseBytes.append(*fuseConfig);
               delete fuseConfig;
               fuseConfig=NULL;
               fuseConfig = &picInfo->fuseBytes.last();

               fuseConfig->name=xmlReader.attributes().value("name").toString();
               if(fuseConfig->name=="")
                  fuseConfig->name="Config"+QString::number(configNumber);

               fuseConfig->mask=xmlReader.attributes().value("wmask").toString().mid(2).toUInt(0,16);
               fuseConfig->value=xmlReader.attributes().value("bvalue").toString().mid(2).toUInt(0,16);
               configNumber++;
               shift=0;
            }
            else
            {
               //fuseConfig->name=xmlReader.attributes().value("name").toString();
               //if(fuseConfig->name=="")
               fuseConfig->name="Config"+QString::number(configNumber);
               shift=8;
               fuseConfig->mask=xmlReader.attributes().value("wmask").toString().mid(2).toUInt(0,16)<<shift + fuseConfig->mask;
               fuseConfig->value=xmlReader.attributes().value("bvalue").toString().mid(2).toUInt(0,16)<<shift + fuseConfig->value;

            }
            status=1;

            //qDebug()<<"microName:"<<microName<<xmlReader.attributes().count();
         }
         if(status==1 || status==2)
         {
            if(xmlReader.name()=="mask")
            {
               //if(group.members.count())
               group = new FuseNavigationGroup;
               fuseConfig->navigationGroups.append(*group);
               delete group;
               group=&fuseConfig->navigationGroups.last();

               group->name=xmlReader.attributes().value("name").toString();
               group->mask=xmlReader.attributes().value("value").toString().mid(2).toUInt(0, 16)<<shift;
               status=2;
            }
            if(xmlReader.name()=="value" && status==2)
            {
               navigation = new FuseNavigation;
               group->members.append(*navigation);
               delete navigation;
               navigation=&group->members.last();
               //navigation.
               navigation->value=xmlReader.attributes().value("value").toString().mid(2).toUInt(0, 16)<<shift;
               navigation->text=xmlReader.attributes().value("name").toString();
            }
         }
         if(xmlReader.name()=="package")
         {
            Package pkg;
            pkg.typeNames=xmlReader.attributes().value("types").toString().split(' ');
            picInfo->packages.append(pkg);
            package = &picInfo->packages.last();
         }
         if(xmlReader.name()=="pin")
         {
            package->pinNames.append(xmlReader.attributes().value("name").toString());
         }
      }
   }
   /*
   QString str="salam";
   //int size=sizeof(&str);
   int id=qRegisterMetaType<QString>("QString");
   //int size=QMetaType::sizeOf(id);
   int size=10000;
   char *data = new char[size];
   memcpy(data, &str, size);
   QFile file("ProgSettings.dat");
   file.open(QIODevice::WriteOnly);
   file.write(data, size);
   file.close();
   qRegisterMetaType<PicInfo>("PicInfo");
   QString str="salam";
   QVariant var;
   var.setValue(str);
   //QSettings  settings("ProgSettings.dat", QSettings::CustomFormat1);
   QSettings settings("MultiProg", "Devices");
   QColor color;
   //color=settings.value(picName).value<QColor>();;
   color.setRgb(100,100, 100);
   settings.setValue(picName, color);*/
   return picInfo;
}





