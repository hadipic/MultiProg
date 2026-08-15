#ifndef PROGP18_H
#define PROGP18_H

#include <QObject>
#include "progDevice.h"
class ProgP18 : public ProgDevice
{
    Q_OBJECT
public:
    explicit ProgP18(USBTools *usb, QObject *parent = 0);
    void Read18Fx(int dim,int dim2,int options);
    void Write18Fx(int dim,int dim2,int wbuf,int eraseW1,int eraseW2,int options);
    void DisplayCODE18F(int dim);


signals:
    
public slots:
    
};

#endif // PROGP18_H
