#include "fuseSettings.h"
#include "Libs/globalVariables.h"
#include <QSettings>
#include <qmath.h>

FuseSettings::FuseSettings(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::FuseSettings)
{
   ui->setupUi(this);

}

FuseSettings::~FuseSettings()
{
   delete ui;

}

Ui::FuseSettings *FuseSettings::getUi()
{
   return ui;
}

void FuseSettings::setDeviceInfo(DeviceInfo *deviceInfo, USBTools *usb)
{
   this->deviceInfo=deviceInfo;
   this->usb=usb;
}

void FuseSettings::loadAVRFuseSettings()
{
   ui->pageAvrFuseSettings->setDeviceInfo(deviceInfo);
   ui->pageAvrFuseSettings->loadAVRFuseSettings();
}

void FuseSettings::loadPICFuseSettings()
{
   ui->pagePicFuseSettings->setDeviceInfo(deviceInfo, usb);
   deviceInfo->picInfo = ui->pagePicFuseSettings->loadPicFuseSettings();
}

void FuseSettings::loadPicFuseSettingsFromUSB()
{
   ui->pagePicFuseSettings->loadPicFuseSettingsFromUSB();;
}
