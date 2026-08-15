#ifndef PROGEEPROM_H
#define PROGEEPROM_H

#include <QObject>
#include "HardwareLayer/Devices/progDevice.h"


class ProgEEPROM : public ProgDevice
{
    Q_OBJECT
public:
    explicit ProgEEPROM(USBTools *usb, QObject *parent = 0);
    void ReadI2C(int dim,int addr);
    void WriteI2C(int dim,int addr,int page);
    void Read93x(int dim,int na,int options);
    void Write93Sx(int dim,int na,int page);
    void Write93Cx(int dim,int na, int options);
    void Read25xx(int dim);
    void Write25xx(int dim,int options);
    void ReadOneWireMem(int dim,int options);
    void WriteOneWireMem(int dim,int options);
    void ReadDS1820();
    void Read11xx(int dim);
    void Write11xx(int dim,int page);
    
    void OW_ID(int id);



public slots:

signals:
    //void onProgressChanged(int step, int max);


private:
    double hvreg;
    
};

#endif // PROGEEPROM_H
