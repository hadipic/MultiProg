#ifndef READWRITEINFO_H
#define READWRITEINFO_H

#include <QObject>
#include <QDebug>

#include <HardwareLayer/usbTools.h>
#include "HardwareLayer/Devices/Memory/progEEPROM.h"
#include "HardwareLayer/Devices/AVR/progAVR.h"
#include "HardwareLayer/Devices/PIC/progP12.h"
#include "HardwareLayer/Devices/PIC/progP16.h"
#include "HardwareLayer/Devices/PIC/progP18.h"
#include "HardwareLayer/Devices/PIC/progP24.h"

class ProgAVR;
class ProgP12;
class ProgP16;
class ProgP18;
class ProgP24;
class ProgEEPROM;


namespace Family
{
    enum Family {PIC12,PIC16,PIC18,PIC24,AVR,I2CEE,SPIEE,UWEE,OWEE,UNIOEE};
}

namespace ReadFunctions
{
    enum ReadFunctions{ Read12F5xx, Read16Fxxx, Read16F1xxx, Read18Fx, Read24Fx, Read24Ex, ReadI2C, Read93x, Read25xx, ReadOneWireMem, ReadDS1820, Read11xx, ReadAT, ReadAT_HV};
}
namespace WriteFunctions
{
    enum WriteFunctions{Write12F5xx, Write12C5xx, Write12F6xx, Write12F61x, Write12F62x, Write16F7x, Write16F71x, Write16F72x, Write16F62x, Write16F8x, Write16F81x, Write16F87x, Write16F87xA, Write16F88x, Write16F1xxx, Write18Fx,
                    Write24Fx, Write24Ex, WriteI2C, Write93Sx, Write93Cx, Write25xx, WriteOneWireMem, Write11xx, WriteAT, WriteATmega, WriteAT_HV};
}

class ReadWriteInfo : public QObject
{
    Q_OBJECT
public:

    ReadWriteInfo(USBTools *usb);
    ReadWriteInfo(USBTools *usb, int family, double HV, int V33, int readFunctionIndex, qint64 memoryFlashSize, qint64 memoryEESize, QList<int> readParam, int resArea, int writeFunctionIndex, QList<int> writeParam, double writeParamD);
    int family;
    int readFunctionIndex;
    int writeFunctionIndex;
    double HV;                  //High voltage value (-1= turn off HV)
    int V33;                    //3.3V regulator required (0=not required)
    qint64 memoryFlashSize;
    qint64 memoryEESize;
    qint64 addresConfig;
    qint64 addresEE;

    bool hasCode;
    bool hasEE;
    bool eeInCode;

    int readParam[2];           //Read function parameters; -10 = NU
    int resArea;                //reserved area size
    int writeParam[6];			//Write function parameters; -10 = NU
    double writeParamD;			//Write function parameter, double
    void write(int ee);
    //AvrFuse * avrFuse;
    // explicit ReadWriteInfo1(USBTools *usb, QObject *parent = 0);
    void setProgrammersInterface(ProgEEPROM *progEEPROM, ProgAVR *progAVR, ProgP12 *progP12, ProgP16 *progP16,  ProgP18 *progP18, ProgP24 *progP24);
private:
    USBTools *usb;
    ProgEEPROM *progEEPROM;
//    ProgAVR *progAVR;
//    ProgP12 *progP12;
//    ProgP16 *progP16;
//    ProgP18 *progP18;
//    ProgP24 *progP24;
public slots:
    void read(int ee,int r);

signals:
    void readFinished();
};
#endif // READWRITEINFO_H
