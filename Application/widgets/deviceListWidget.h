#ifndef DEVICELISTWIDGET_H
#define DEVICELISTWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <Libs/globalVariables.h>

namespace Ui {
class DeviceListWidget;
}

class DeviceListWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DeviceListWidget(QWidget *parent = 0);
    ~DeviceListWidget();
    Devices *devices;
    void setComboIndex(int familyIndex, int typeIndex, int infoIndex) ;
    int familyIndex();
    int typeIndex();
    int infoIndex();
private slots:
    void on_cboDevicesInfo_currentIndexChanged(int index);
    void on_cboDeviceTypes_currentIndexChanged(int index);
    void on_cboDeviceFamily_currentIndexChanged(int index);
    void detectDevice();

private:
Ui::DeviceListWidget *ui;
signals:
    void onDeviceSelected(DeviceFamily *family, DeviceType *type, DeviceInfo *deviceInfo);
};

#endif // DEVICELISTWIDGET_H
