#ifndef PROGAVR_H
#define PROGAVR_H
#include "progDevice.h"

class ProgAVR : public ProgDevice
{
    Q_OBJECT
public:
    explicit ProgAVR(USBTools *usb, QObject *parent = 0);

    void ReadAT(int dim, int dim2, int options);
    void ReadAT_HV(int dim, int dim2, int options);
    void WriteAT(int dim, int dim2);
    void WriteATmega(int dim, int dim2, int page, int options);
    void WriteAT_HV(int dim, int dim2, int page, int options);
    void DisplayCODEAVR(int dim);

};

#endif // PROGAVR_H
