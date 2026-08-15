#ifndef PICFUSESETTINGS_H
#define PICFUSESETTINGS_H
#include <QWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include "HardwareLayer/Devices/devices.h"
#include "HardwareLayer/Devices/PIC/readXMLPIC.h"


namespace Ui {
class PicFuseSettings;
}

class PicFuseSettings : public QWidget
{
   Q_OBJECT

public:
   explicit PicFuseSettings(QWidget *parent = 0);
   ~PicFuseSettings();
   void setDeviceInfo(DeviceInfo *deviceInfo, USBTools *usb);

   PicInfo* loadPicFuseSettings();
   void loadPicFuseSettingsFromUSB();
   void loadItem(int currentRow);
   void clearObjects();
   private slots:
   void onSettingsChanged();
   void onCheckBoxChanged();
   void onRadioGroupClicked(int id);

   void on_lstConfigs_itemClicked(QListWidgetItem *item);
   void changeMemID();
   void txtMemIDCursorPositionChanged();
   void on_txtMemID_cursorPositionChanged();

   void on_lstConfigs_currentRowChanged(int currentRow);

private:
   DeviceInfo *deviceInfo;
   QWidget *parentWidget;
   QVBoxLayout *verticalLayout;
   Ui::PicFuseSettings *ui;
   PicInfo *picInfo;
   QList<QObject*> objects;
   QList<quint16> fuseByteValueList;
   USBTools *usb;
   quint16 fuseByteValue;
   void showFuseByteValue(quint16 value);
   int selectedRow;
};

#endif // PICFUSESETTINGS_H
