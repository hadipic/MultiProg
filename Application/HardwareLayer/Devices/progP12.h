#ifndef PROGP12_H
#define PROGP12_H

#include <QObject>
#include "progDevice.h"

class ProgP12 : public ProgDevice
{
    Q_OBJECT
public:
    explicit ProgP12(USBTools *usb, QObject *parent = 0);
    ProgP12(QObject *parent = 0);
    void Read12F5xx(int dim, int dim2);
    void Write12F5xx(int dim,int OscAddr);
    void Write12C5xx(int dim,int dummy);
signals:
    
public slots:
    
};

#endif // PROGP12_H
