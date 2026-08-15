#ifndef PROGDEVICE_H
#define PROGDEVICE_H

#include <QObject>
#include <QDebug>
#include <HardwareLayer/usbTools.h>

class ProgDevice : public QObject
{
    Q_OBJECT
public:
    explicit ProgDevice(USBTools *usb, QObject *parent = 0);
protected:
    USBTools *usb;
signals:
    void  PrintStatus(QString msg1 ,int j,int i);
    void  onProgressChanged(int step, int max);

    
};

#endif // PROGDEVICE_H
