#ifndef PROGP16_H
#define PROGP16_H
#include "HardwareLayer/Devices/progDevice.h"
#include "HardwareLayer/Devices/devices.h"

class DeviceSelected;
class Devices;
class ProgP16 : public ProgDevice
{
    Q_OBJECT
public:
    ProgP16(USBTools *usb, Devices *devices, QObject *parent = 0);

    //void Read16Fxxx(ReadWriteInfo *deviceInfo, int dim,int dim2,int dim3,int vdd);

    void Read16Fxxx(int dim,int dim2,int dim3,int vdd);
    void Read16F1xxx(int dim,int dim2,int dim3,int options);

    void Write12F6xx(int dim,int dim2);
    void Write12F61x(int dim,int d,int d2);
    void Write12F62x(int dim,int dim2);
    void Write16F7x(int dim,int vdd);
    void Write16F71x(int dim,int vdd);
    void Write16F72x(int dim,int d,int d2);
    void Write16F62x (int dim,int dim2);
    void Write16F8x(int dim,int dim2);
    void Write16F81x(int dim,int dim2);
    void Write16F87x(int dim,int dim2);
    void Write16F87xA(int dim,int dim2,int seq);
    void Write16F88x(int dim,int dim2);

    void Write16F1xxx(int dim,int dim2,int options);

    void DisplayCODE16F(int size);
    void DisplayEE16F(int size);

    DeviceSelected findByID(quint16 id, bool printMsg=false);
    DeviceSelected detectDevice();

private:

    DeviceSelected detectDevice16F1();
    DeviceSelected detectDevice16F2();
    void initPIC16_ID();
    Devices *devices;
    public slots:
signals:
};

#endif // PROGP16_H
