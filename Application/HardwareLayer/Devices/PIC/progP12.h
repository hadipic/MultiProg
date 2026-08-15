#ifndef PROGP12_H
#define PROGP12_H
#include "HardwareLayer/Devices/progDevice.h"

class ProgP12 : public ProgDevice
{
public:
    ProgP12(USBTools *usb, QObject *parent = 0);
    void Read12F5xx(int dim, int dim2);
    void Write12F5xx(int dim,int OscAddr);
    void Write12C5xx(int dim,int dummy);

    int use_osccal,use_BKosccal;
    int saveLog, programID, MinDly, load_osccal, load_BKosccal;
    int load_calibword,max_err;

    void detectDevice();
signals:

};

#endif // PROGP12_H
