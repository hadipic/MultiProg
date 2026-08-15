#ifndef PROGP24_H
#define PROGP24_H
#include "HardwareLayer/Devices/progDevice.h"
#include "HardwareLayer/Devices/devices.h"

class DeviceSelected;
class Devices;

class ProgP24 : public ProgDevice
{
    Q_OBJECT
public:
    ProgP24(USBTools *usb, Devices*devices, QObject *parent = 0);
    void Read24Fx(int dim,int dim2,int options,int appIDaddr,int executiveArea);
    void Read24Ex(int dim,int dim2,int options,int appIDaddr,int executiveArea);

    void Write24Fx(int dim,int dim2,int options,int appIDaddr,int rowSize, double wait);
    void DisplayCODE24F(int dim);
    void DisplayEE24F();
    void Write24Ex(int dim,int dim2,int options,int appIDaddr,int rowSize, double wait);

    void CheckData(int a,int b,int addr,int *err);

    //void PIC24_ID(int id);
    DeviceSelected findByID(quint16 id, bool printMsg=false);
    DeviceSelected detectDevice();

private:
    void initPIC24_ID();
    Devices *devices;
    DeviceSelected detectDevice24Fx();
    DeviceSelected detectDevice24Ex();


signals:
    void onProgressChanged(int step, int max);


};

#endif // PROGP24_H
