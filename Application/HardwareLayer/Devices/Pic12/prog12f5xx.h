#ifndef PROG12F5XX_H
#define PROG12F5XX_H
#include "HardwareLayer/Devices/progDevice.h"
class Prog12F5xx : public ProgDevice
{
    public:
        Prog12F5xx(USBTools *usb, QObject *parent = 0);
    virtual void read(int dim, int dim2, int c=0, int d=0);
    virtual void write(int dim,int OscAddr, int c=0, int d=0, int e=0, int f=0);
};

#endif // PROG12F5XX_H
