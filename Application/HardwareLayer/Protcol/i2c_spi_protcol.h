#ifndef I2C_SPI_PROTCOL_H
#define I2C_SPI_PROTCOL_H


#include <QObject>
#include <QAction>
#include "QString"
#include <cstring>
#include <unistd.h>
#include <getopt.h>
#include <QPlainTextEdit>

#include "HardwareLayer/Devices/progDevice.h"
#include "Libs/strings.h"

#define CS 8
#define HLD 16

class i2c_spi_protcol: public ProgDevice
{
    Q_OBJECT
public:
   // explicit icd_protcol(QObject *parent = 0);
     i2c_spi_protcol(USBTools *usb, QObject *parent = 0);

      QPlainTextEdit *txtLog;
     void I2CReceive(int mode,int speed,int N,BYTE *buffer);
     void I2CSend(int mode,int speed,int N,BYTE *buffer);

     void printM(char *msg);
     void  printM(QString msg);
     Strings strings;

signals:

public slots:

};

#endif // I2C_SPI_PROTCOL_H
