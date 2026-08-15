#ifndef PROGEEPROM_H
#define PROGEEPROM_H

#include <QObject>
#include "progDevice.h"

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

    virtual void read(int dim, int dim2, int c=0, int d=0);
    virtual void write(int dim,int OscAddr, int c=0, int d=0,int e=0, int f=0);

signals:
    void onProgressChanged(int step, int max);
public slots:

private:
   int sizeEE;


    double hvreg=0;
    
};

#endif // PROGEEPROM_H
