#ifndef PROGP18_H
#define PROGP18_H
#include "HardwareLayer/Devices/progDevice.h"
#include "HardwareLayer/Devices/devices.h"

class DeviceSelected;
class Devices;

class ProgP18 : public ProgDevice
{
        Q_OBJECT
public:


    ProgP18(USBTools *usb, Devices *devices, QObject *parent = 0);
    void Read18Fx(int dim,int dim2,int options);
    void Write18Fx(int dim,int dim2,int wbuf,int eraseW1,int eraseW2,int options);
    void DisplayCODE18F(int dim);
    DeviceSelected findByID(quint16 id, bool printMsg=false);
    DeviceSelected detectDevice();

private:
    void initPIC18_ID();
    Devices *devices;
signals:
    void onProgressChanged(int step, int max);


};

#endif // PROGP18_H
