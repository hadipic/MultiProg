#ifndef PROGAVR_H
#define PROGAVR_H
#include "HardwareLayer/Devices/progDevice.h"
#include "HardwareLayer/Devices/devices.h"

class DeviceSelected;
class Devices;
class ProgAVR : public ProgDevice
{
    Q_OBJECT
public:
    quint8 AVRlock, AVRfuse, AVRfuse_h, AVRfuse_x;

    explicit ProgAVR(USBTools *usb, Devices *devices, QObject *parent = 0);

    void ReadAT(int dim, int dim2, int options);
    void ReadAT_Fuse(int options, bool readLock=false);


    void ReadAT_HV(int dim, int dim2, int options);
    void ReadAT_HV_Fuse(int options, bool readLock=false);

    void WriteAT(int dim, int dim2);
    void WriteAT_Fuse();

    void WriteATmega(int dim, int dim2, int page, int options);
    void WriteATmega_Fuse(int options);

    void WriteAT_HV(int dim, int dim2, int page, int options);
    void WriteAT_HV_Fuse(int options);


    void DisplayCODEAVR(int dim);
    DeviceSelected detectDevice();

    DeviceSelected findByID(quint8 id[], bool printMsg=false);


private:
    void initAtmelID();
    Devices *devices;
    DeviceSelected detectDeviceAT();
    DeviceSelected detectDeviceAT_HV();

signals:
    void onProgressChanged(int step, int max);
    void onFuseRead();
};

#endif // PROGAVR_H
