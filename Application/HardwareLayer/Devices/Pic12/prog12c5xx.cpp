#include "prog12c5xx.h"

Prog12C5xx::Prog12C5xx(USBTools *usb, QObject *parent):ProgDevice(usb, parent)
{

}

void Prog12C5xx::read(int dim, int dim2, int c, int d)
{
    qDebug()<<"Prog12C5xx Read"<<dim<<dim2;
}

void Prog12C5xx::write(int dim, int dummy, int c, int d, int e, int f)
{
    qDebug()<<"Prog12C5xx Write"<<dim<<dummy<<c<<d<<e<<f;
}
