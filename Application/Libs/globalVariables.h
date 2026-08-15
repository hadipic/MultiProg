#ifndef COMMONVARIABLES_H
#define COMMONVARIABLES_H

#include <QObject>
#include <QAction>
#include "QString"

#include <cstring>
#include <unistd.h>
#include <QPlainTextEdit>
#include <getopt.h>

#include "HardwareLayer/Devices/progDevice.h"
#include "HardwareLayer/Devices/Memory/progEEPROM.h"
#include "HardwareLayer/Devices/AVR/progAVR.h"
#include "HardwareLayer/Devices/PIC/progP12.h"
#include "HardwareLayer/Devices/PIC/progP16.h"
#include "HardwareLayer/Devices/PIC/progP18.h"
#include "HardwareLayer/Devices/PIC/progP24.h"
#include "Libs/strings.h"
#include "databaseLayer.h"

class GlobalVariables : public QObject
{
    Q_OBJECT
public:


    static GlobalVariables *getInstance();
    static USBTools *usb;
    static ProgEEPROM *progEEPROM;
    static ProgAVR *progAVR;
    static ProgP12 *progP12;
    static ProgP16 *progP16;
    static ProgP18 *progP18;
    static ProgP24 *progP24;
    static Devices *devices;
    static Strings strings;
    static bool saveLog;
    static DatabaseLayer *db;


private:
    explicit GlobalVariables(QObject *parent = 0);
    static GlobalVariables * globalVariables;
    static QAction *Act1;

signals:
  //void  PrintStatus(QString msg1 ,int j,int i);
public slots:
    
};

#endif // COMMONVARIABLES_H
