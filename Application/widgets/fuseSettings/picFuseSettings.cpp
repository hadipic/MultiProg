#include "picFuseSettings.h"
#include "ui_picFuseSettings.h"
#include <QComboBox>
#include <QButtonGroup>
#include <QCheckBox>
#include <QRadioButton>
#include <string>

PicFuseSettings::PicFuseSettings(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::PicFuseSettings)
{
   ui->setupUi(this);
   parentWidget=NULL;
   /*connect(ui->txtID0, SIGNAL(editingFinished()), SLOT(changeMemID()));
   connect(ui->txtID1, SIGNAL(editingFinished()), SLOT(changeMemID()));
   connect(ui->txtID2, SIGNAL(editingFinished()), SLOT(changeMemID()));
   connect(ui->txtID3, SIGNAL(editingFinished()), SLOT(changeMemID()));
   connect(ui->txtID4, SIGNAL(editingFinished()), SLOT(changeMemID()));
   connect(ui->txtID5, SIGNAL(editingFinished()), SLOT(changeMemID()));
   connect(ui->txtID6, SIGNAL(editingFinished()), SLOT(changeMemID()));
   connect(ui->txtID7, SIGNAL(editingFinished()), SLOT(changeMemID()));
*/
   connect(ui->txtMemID, SIGNAL(cursorPositionChanged()), SLOT(txtMemIDCursorPositionChanged()));
   connect(ui->txtMemID, SIGNAL(textChanged()), SLOT(changeMemID()));

}

PicFuseSettings::~PicFuseSettings()
{
   delete ui;
}

void PicFuseSettings::setDeviceInfo(DeviceInfo *deviceInfo, USBTools *usb)
{
   this->deviceInfo = deviceInfo;
   this->usb=usb;
   ui->txtMemID->setBase(deviceInfo->readWriteInfo->addresConfig/8);
}

PicInfo *PicFuseSettings::loadPicFuseSettings()
{
   if(ui->lstConfigs->selectedItems().count())
      ui->lstConfigs->clearSelection();

   if(ui->lstConfigs->count())
   {
      ui->lstConfigs->clear();
      fuseByteValueList.clear();
   }

   ReadXMLPIC readXMLPIC;

   //picInfo=readXMLPIC.readXML(deviceInfo->name);
   picInfo=new PicInfo(deviceInfo->name);

   if(!picInfo || !picInfo->finded())
   {
      //CommonFunctions::printMessage("file "+deviceInfo->name+".xml is not find");
      CommonFunctions::printMessage(deviceInfo->name+" is not in database");
      clearObjects();
      return NULL;
   }

   for (int i = 0; i < picInfo->fuseBytes.count(); ++i)
   {
      QString name  = picInfo->fuseBytes.at(i).name;
      quint16 value = picInfo->fuseBytes.at(i).value;
      name=QString("%1:%3%2").arg(name).arg(value, 5, 16, QLatin1Char('0')).toUpper().arg("0x");
      ui->lstConfigs->addItem(name);
      fuseByteValueList.append(value);
   }
   loadItem(0);
   return picInfo;
}

void PicFuseSettings::loadPicFuseSettingsFromUSB()
{
   if(ui->lstConfigs->selectedItems().count())
      ui->lstConfigs->clearSelection();

   if(ui->lstConfigs->count())
   {
      ui->lstConfigs->clear();
   }
   if(!picInfo->fuseBytes.count())
   {
      CommonFunctions::printMessage("file "+deviceInfo->name+".xml not find");
      return;
   }

   for (int i = 0; i < picInfo->fuseBytes.count(); ++i)
   {
      QString name=picInfo->fuseBytes.at(i).name;
      quint16 value=(usb->memCONFIG[i*2+1]<<8)+usb->memCONFIG[i*2];
      name=QString("%1:%3%2").arg(name).arg(value, 5, 16, QLatin1Char('0')).toUpper().arg("0x");
      ui->lstConfigs->addItem(name);
      fuseByteValueList[i]=value;
   }
   QString hexStr="";
   for (int i = 0; i < 8; ++i)
   {
      QString memID=QString("%1").arg(usb->memID[i], 2, 16, QLatin1Char('0')).toUpper();
      hexStr+=memID;
      if(i<7)
         hexStr+=" ";
   }
   ui->txtMemID->setBitLength(8);
   ui->txtMemID->setPlainText(hexStr+" ");
   loadItem(0);
}

void PicFuseSettings::txtMemIDCursorPositionChanged()
{
   static int lastHexPosition=0;
   int pos=ui->txtMemID->textCursor().position();
   int len=ui->txtMemID->toPlainText().length();


   if(ui->txtMemID->isInSelection())
      return;

   pos=ui->txtMemID->textCursor().position();
   //qDebug()<<pos<<ui->txtMemID->toPlainText().length();
   if(pos>=len-1)
   {
      QTextCursor c = ui->txtMemID->textCursor();
      c.setPosition(len-2);//, QTextCursor::KeepAnchor, 2);
      ui->txtMemID->setTextCursor(c);

   }
   else
   {
      QChar ch=ui->txtMemID->toPlainText().at(pos);
      if ((ch==' ' ||  ch==10))
      {
         QTextCursor c = ui->txtMemID->textCursor();
         if(pos-lastHexPosition>0)
            c.movePosition(QTextCursor::Right);
         else
            c.movePosition(QTextCursor::Left);
         ui->txtMemID->setTextCursor(c);
      }
   }
   lastHexPosition = ui->txtMemID->textCursor().position();
}

void PicFuseSettings::on_txtMemID_cursorPositionChanged()
{
   txtMemIDCursorPositionChanged();
}


void PicFuseSettings::clearObjects()
{
   for (int i = 0; i < objects.count(); ++i)
   {
      QObject *obj=objects.at(i);
      //qDebug()<<"object"<<i<<":"<<obj;
      delete obj;
      obj=NULL;
   }
   objects.clear();
}

void PicFuseSettings::loadItem(int currentRow)
{
   if(currentRow<0)
      return;
   clearObjects();
   parentWidget=ui->scrollAreaAVRNavigation;
   verticalLayout= ui->verticalLayout;
   FuseByte fuseConfig=picInfo->fuseBytes.at(currentRow);


   fuseByteValue= fuseByteValueList[currentRow];
   showFuseByteValue(fuseByteValue);
   selectedRow=currentRow;
   for (int i = 0; i < fuseConfig.navigationGroups.count(); ++i)
   {
      int memberCount=fuseConfig.navigationGroups.at(i).members.count();
      if(memberCount>2)
      {
         quint32 mask = fuseConfig.navigationGroups.at(i).mask;
         int a=mask;
         int bitLen=0;
         for(; a ; a>>=1)
         {
            if(a&1)
               bitLen++;
         }
         quint16 selectedValue=fuseByteValue & mask;
         QLabel *label=new QLabel("\n--"+fuseConfig.navigationGroups.at(i).name+" ["+QString::number(bitLen)+" bits]--", parentWidget);

         verticalLayout->addWidget(label);
         QButtonGroup *radioGroup=new QButtonGroup(parentWidget);
         connect(radioGroup, SIGNAL(buttonClicked(int)), SLOT(onRadioGroupClicked(int)));

         objects.append(label);

         quint16 value=0;
         QString text="";
         for (int j = 0; j < memberCount; ++j)
         {
            value=fuseConfig.navigationGroups.at(i).members.at(j).value;
            text=fuseConfig.navigationGroups.at(i).members.at(j).text;

            //QRadioButton *radio = new QRadioButton(QString("%1 [ %2 : 0x%3 ]").arg(text).arg(value, 16, 2, QLatin1Char('0')).arg(value, 4, 16, QLatin1Char('0')).toUpper(), parentWidget);
            QRadioButton *radio = new QRadioButton(QString("%1 [ %3%2 ]").arg(text).arg(value, 5, 16, QLatin1Char('0')).toUpper().arg("0x"), parentWidget);
            radio->setProperty("Value", value);
            radio->setProperty("Mask", mask);
            //radio->setMaximumHeight(20);
            radioGroup->addButton(radio, j);
            verticalLayout->addWidget(radio);
            objects.append(radio);

            if(selectedValue==value)
               radio->setChecked(true);
         }
         objects.append(radioGroup);
      }
      else if(memberCount==2)
      {
         QCheckBox *checkBoxNav=new QCheckBox(parentWidget);
         QString name=fuseConfig.navigationGroups.at(i).name;
         quint16 mask = fuseConfig.navigationGroups.at(i).mask;
         quint16 selectedValue=fuseByteValue & mask;
         checkBoxNav->setProperty("Name", name);
         checkBoxNav->setProperty("Mask", mask);

         connect(checkBoxNav, SIGNAL(clicked()), this, SLOT(onCheckBoxChanged()));

         verticalLayout->addWidget(checkBoxNav);


         quint16 value=0;
         QString text="";
         for (int j = 0; j < memberCount; ++j)
         {
            value=fuseConfig.navigationGroups.at(i).members.at(j).value;
            text=fuseConfig.navigationGroups.at(i).members.at(j).text;
            if(text=="Off")
               checkBoxNav->setProperty("Off", value);
            else
            {
               checkBoxNav->setProperty("On", value);
               checkBoxNav->setProperty("OnText", text);

               if(selectedValue==value)
               {
                  checkBoxNav->setChecked(true);
                  if(text!="On")
                     name+=":"+text;
               }
            }
         }
         checkBoxNav->setText(name);
         objects.append(checkBoxNav);
      }
   }
}

void PicFuseSettings::on_lstConfigs_itemClicked(QListWidgetItem *item)
{
   qDebug()<<"itemClicked";
   int currentRow=0;
   for (int i = 0; i < ui->lstConfigs->count(); ++i)
   {
      if(item==ui->lstConfigs->item(i))
      {
         currentRow=i;
         break;
      }
   }
   loadItem(currentRow);
}

void PicFuseSettings::changeMemID()
{
   QStringList textIDs=ui->txtMemID->toPlainText().split(" ");
   for (int i = 0; i <textIDs.length() ; ++i)
   {
      bool ok=false;
      quint8 value=textIDs[i].toInt(&ok, 16);
      if(ok)
         usb->memID[i]=value;
   }

}

void PicFuseSettings::showFuseByteValue(quint16 value)
{
   ui->lblByte->setText(QString("%1 : %3%2").arg(value, 16, 2, QLatin1Char('0')).arg(value, 5, 16, QLatin1Char('0')).toUpper().arg("0x"));
}

void PicFuseSettings::onSettingsChanged()
{
   //qDebug()<<"onSettingsChanged";
   quint16 value=0;
   quint16 mask = 0;
   for (int i = 0; i < objects.count(); ++i)
   {
      QObject *obj=objects.at(i);

      //qDebug()<<"object"<<i<<":"<<obj->metaObject()->className();
      if(QString(obj->metaObject()->className())=="QRadioButton")
      {
         QRadioButton *radio=(QRadioButton*)obj;
         if(radio->isChecked())
         {
            mask=radio->property("Mask").toUInt();

            qDebug()<<"rad fuseByteValue1:"<<fuseByteValue;
            value=radio->property("Value").toInt();
            fuseByteValue=(fuseByteValue|mask) & (value | (~mask));
            qDebug()<<"rad fuseByteValue2:"<<fuseByteValue<<" value:"<<value<<"mask"<<mask<<~mask;
         }
      }
      else
         if(QString(obj->metaObject()->className())=="QCheckBox")
         {
            QCheckBox *checkBox=(QCheckBox*)obj;
            mask=checkBox->property("Mask").toUInt();

            QString text=checkBox->property("OnText").toString();
            QString name=checkBox->property("Name").toString();
            if(checkBox->isChecked())
            {
               value=checkBox->property("On").toInt();
               if(text!="On")
                  checkBox->setText(name+":"+text);
            }
            else
            {
               value=checkBox->property("Off").toInt();
               checkBox->setText(name);
            }
            qDebug()<<"check fuseByteValue1:"<<fuseByteValue;
            fuseByteValue=(fuseByteValue|mask) & (value | (~mask));
            qDebug()<<"check fuseByteValue2:"<<fuseByteValue<<" value:"<<value;

            //qDebug()<<checkBox->text()<<checkBox->property("Text").toString()<<checkBox->property("On").toInt()<<checkBox->property("Off").toInt();
         }
   }
   fuseByteValueList[selectedRow]=fuseByteValue;

   usb->memCONFIG[selectedRow*2]  =(quint8)fuseByteValue;
   usb->memCONFIG[selectedRow*2+1]=(quint8)(fuseByteValue>>8);

   showFuseByteValue(fuseByteValue);

   QString name=QString("%1:%3%2").arg(picInfo->fuseBytes.at(selectedRow).name).arg(fuseByteValue, 5, 16, QLatin1Char('0')).toUpper().arg("0x");
   ui->lstConfigs->item(selectedRow)->setText(name);
}

void PicFuseSettings::onCheckBoxChanged()
{
   onSettingsChanged();
}

void PicFuseSettings::onRadioGroupClicked(int id)
{
   onSettingsChanged();
}

void PicFuseSettings::on_lstConfigs_currentRowChanged(int currentRow)
{
   loadItem(currentRow);
}
