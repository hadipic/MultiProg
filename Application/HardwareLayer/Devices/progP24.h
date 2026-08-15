#ifndef PROGP24_H
#define PROGP24_H

#include <QObject>
#include "progDevice.h"

class ProgP24 : public ProgDevice
{
    Q_OBJECT
public:
    explicit ProgP24(USBTools *usb, QObject *parent = 0);
    void Read24Fx(int dim,int dim2,int options,int appIDaddr,int executiveArea);
    void Write24Fx(int dim,int dim2,int options,int appIDaddr,int rowSize, double wait);
    void DisplayCODE24F(int dim);
    void DisplayEE24F();
    void Read24Ex(int dim,int dim2,int options,int appIDaddr,int executiveArea);
    void Write24Ex(int dim,int dim2,int options,int appIDaddr,int rowSize, double wait);


signals:
    
public slots:
    
};

#endif // PROGP24_H
