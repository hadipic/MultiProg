#include "globalVariables.h"




USBTools    *GlobalVariables::usb;
ProgEEPROM  *GlobalVariables::progEEPROM;
ProgAVR     *GlobalVariables::progAVR;
ProgP12     *GlobalVariables::progP12;
ProgP16     *GlobalVariables::progP16;
ProgP18     *GlobalVariables::progP18;
ProgP24     *GlobalVariables::progP24;
Devices     *GlobalVariables::devices;

Strings      GlobalVariables::strings;
bool         GlobalVariables::saveLog;
DatabaseLayer *GlobalVariables::db;

GlobalVariables *GlobalVariables::globalVariables;

GlobalVariables::GlobalVariables(QObject *parent) :
    QObject(parent)
{
    usb = new USBTools();
    devices = new Devices(usb, this);
    progEEPROM = new ProgEEPROM(usb, this);
    progAVR = new ProgAVR(usb, devices, this);
    progP12 = new ProgP12(usb, this);
    progP16 = new ProgP16(usb, devices, this);
    progP18 = new ProgP18(usb, devices, this);
    progP24 = new ProgP24(usb, devices, this);
    db = new DatabaseLayer("QSQLITE", "Database/Devices.db");

    //devices->setProgrammersInterface(progEEPROM, progAVR, progP12, progP16, progP18, progP24);
            //4267
    saveLog=false;
}


GlobalVariables *GlobalVariables::getInstance()
{
    if(globalVariables==NULL)
    {
        globalVariables = new GlobalVariables();
    }
    return globalVariables;
}

