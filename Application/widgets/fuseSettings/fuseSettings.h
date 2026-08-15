#ifndef FUZESETTINGS_H
#define FUZESETTINGS_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include "ui_fuseSettings.h"
#include "avrFuseSettings.h"
#include <HardwareLayer/usbTools.h>

namespace Ui
{
class FuseSettings;
}

class FuseSettings : public QWidget
{
	Q_OBJECT
public:
    explicit FuseSettings(QWidget *parent = 0);
    ~FuseSettings();
    Ui::FuseSettings *getUi();
    Ui::FuseSettings *ui;
    void setDeviceInfo(DeviceInfo *deviceInfo, USBTools *usb);

    void loadAVRFuseSettings();
    void loadPICFuseSettings();
    void loadPicFuseSettingsFromUSB();

    void saveFuseByte(FuseByte *fuseByte);
   private:
    DeviceInfo *deviceInfo;
    USBTools *usb;
public slots:
private slots:

};

#endif // FUZESETTINGS_H
