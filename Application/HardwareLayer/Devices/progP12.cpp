#include "progP12.h"
#include "QDebug"


ProgP12::ProgP12(USBTools *usb, QObject *parent):ProgDevice(usb, parent)
{
}


void ProgP12::Read12F5xx(int dim, int dim2)
{
    qDebug()<<"ProgP12"<<dim<<dim2;
}

void ProgP12::Write12F5xx(int dim, int OscAddr)
{
}

void ProgP12::Write12C5xx(int dim, int dummy)
{
}
