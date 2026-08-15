#include "deviceListWidget.h"
#include "ui_deviceListWidget.h"
#include <QDebug>

DeviceListWidget::DeviceListWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::DeviceListWidget)
{
   ui->setupUi(this);
   devices = GlobalVariables::devices;
   ui->cboDeviceFamily->clear();
   for (int i = 0; i < devices->deviceFamilyList.length(); i++)
   {
      ui->cboDeviceFamily->addItem(devices->deviceFamilyList.at(i)->familyName);
   }
   connect(ui->btnDetectDevice, SIGNAL(clicked()), SLOT(detectDevice()));

}

void DeviceListWidget::on_cboDeviceFamily_currentIndexChanged(int index)
{
   ui->cboDeviceTypes->clear();
   if(index>=0)
      for (int i = 0; i < devices->deviceFamilyList.at(index)->deviceTypesList.length(); i++)
         ui->cboDeviceTypes->addItem(devices->deviceFamilyList.at(index)->deviceTypesList.at(i)->typeName);
}

DeviceListWidget::~DeviceListWidget()
{
   delete ui;
}

void DeviceListWidget::setComboIndex(int familyIndex, int typeIndex, int infoIndex)
{
   ui->cboDeviceFamily->setCurrentIndex(familyIndex);
   ui->cboDeviceTypes->setCurrentIndex(typeIndex);
   ui->cboDevicesInfo->setCurrentIndex(infoIndex);
   on_cboDevicesInfo_currentIndexChanged(infoIndex);
}

int DeviceListWidget::familyIndex()
{
   return ui->cboDeviceFamily->currentIndex();
}

int DeviceListWidget::typeIndex()
{
   return ui->cboDeviceTypes->currentIndex();
}

int DeviceListWidget::infoIndex()
{
   return ui->cboDevicesInfo->currentIndex();
}

void DeviceListWidget::on_cboDeviceTypes_currentIndexChanged(int index)
{

   int familyIndex=ui->cboDeviceFamily->currentIndex();
   ui->cboDevicesInfo->clear();
   if(familyIndex>=0 && index>=0)
      for (int i = 0; i < devices->deviceFamilyList.at(familyIndex)->deviceTypesList.at(index)->devicesList.length(); i++)
         ui->cboDevicesInfo->addItem(devices->deviceFamilyList.at(familyIndex)->deviceTypesList.at(index)->devicesList.at(i)->name);
}

void DeviceListWidget::on_cboDevicesInfo_currentIndexChanged(int index)
{
   int familyIndex=ui->cboDeviceFamily->currentIndex();
   int typeIndex=ui->cboDeviceTypes->currentIndex();
   if(familyIndex>=0 && typeIndex>=0 && index>=0)
      emit onDeviceSelected(
            devices->deviceFamilyList.at(familyIndex)
            ,devices->deviceFamilyList.at(familyIndex)->deviceTypesList.at(typeIndex)
            ,devices->deviceFamilyList.at(familyIndex)->deviceTypesList.at(typeIndex)->devicesList.at(index)
            );
}


void DeviceListWidget::detectDevice()
{
   DeviceSelected deviceSelected;
   deviceSelected.deviceInfo=NULL;
   if(ui->cboDeviceFamily->currentText()=="PIC")
   {
      GlobalVariables::progP12->detectDevice();
      deviceSelected=GlobalVariables::progP24->detectDevice();
      if(!deviceSelected.deviceInfo)
      {
         deviceSelected=GlobalVariables::progP18->detectDevice();
         if(!deviceSelected.deviceInfo)
            deviceSelected=GlobalVariables::progP16->detectDevice();
      }
      if(deviceSelected.deviceInfo)
         deviceSelected.deviceInfo->picInfo=new PicInfo(deviceSelected.deviceInfo->name);

   }
   else if(ui->cboDeviceFamily->currentText()=="AVR")
   {
      deviceSelected=GlobalVariables::progAVR->detectDevice();
   }

   if(deviceSelected.deviceInfo)
   {
      setComboIndex(deviceSelected.familyIndex, deviceSelected.typeIndex, deviceSelected.infoIndex);
      CommonFunctions::printMessage("Device is:"+deviceSelected.deviceInfo->name+", id:"+QString::number(deviceSelected.deviceInfo->id, 16).toUpper()+", rev id:"+QString::number(deviceSelected.deviceInfo->revMask, 16).toUpper());
   }
}
