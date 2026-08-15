#include "prog12f5xx.h"

Prog12F5xx::Prog12F5xx(USBTools *usb, QObject *parent):ProgDevice(usb, parent)
{

}

void Prog12F5xx::read(int dim, int dim2, int c, int d)
{
    qDebug()<<"Prog12F5xx Read"<<dim<<dim2;
}

void Prog12F5xx::write(int dim, int OscAddr, int c, int d, int e, int f)
{
    qDebug()<<"Prog12F5xx Write"<<dim<<OscAddr;
}
