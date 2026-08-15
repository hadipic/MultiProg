#ifndef DEVICES_H
#define DEVICES_H

#include <QObject>
#include <QList>
#include <QPair>
#include <HardwareLayer/Devices/PIC/picInfo.h>
#include "readWriteInfo.h"




class DeviceType;
class DeviceInfo;
class DeviceFamily;
class ReadWriteInfo;

class ProgAVR;
class ProgP12;
class ProgP16;
class ProgP18;
class ProgP24;
class ProgEEPROM;

class DeviceSelected
{
   public:
      DeviceInfo * deviceInfo;
      quint16 familyIndex, typeIndex, infoIndex;
};


class DeviceInfo
{
   public:
      quint16       id;
      QString       name;
      quint16       revMask;
      ReadWriteInfo *readWriteInfo;
      DeviceType    *parentDeviceType;
      PicInfo       *picInfo;
      int            bitLength();
};

class DeviceType
{
   public:
      QString              typeName;
      QList<DeviceInfo*>   devicesList;
      DeviceFamily         *parentDeviceFamily;

      DeviceInfo           *addDevice(QString deviceName, ReadWriteInfo *readWriteInfo);
};

class DeviceFamily
{
   public:
      QString              familyName;
      QString              companyName;
      QList<DeviceType*>   deviceTypesList;
      DeviceType*          addDeviceType(QString typeName);
};


class Devices : public QObject
{
      Q_OBJECT
   public:
      explicit Devices(USBTools *usb, QObject *parent = 0);
      DeviceFamily *addDeviceFamily(QString familyName, QString companyName);

      QList<DeviceFamily*> deviceFamilyList;
      QList<QPair<QString, DeviceType *> > deviceTypeTable;
      void  addDeviceType(DeviceFamily *deviceFamily, QString typeNameForShow, QString perfixName);
      void  addDevice(QString deviceNames, int family, double HV, int V33,
                      int readFunctionIndex, QString readParam, int resArea,
                      int writeFunctionIndex, QString writeParam, double writeParamD);
      bool setDeviceID(quint16 id, QString name, quint16 revMask=0);
      void setProgrammersInterface(ProgEEPROM *progEEPROM, ProgAVR *progAVR, ProgP12 *progP12, ProgP16 *progP16, ProgP18 *progP18, ProgP24 *progP24);
      QList<DeviceInfo*> allDevices;
   private:
      USBTools *usb;
      ProgEEPROM *progEEPROM;
      ProgAVR *progAVR;
      ProgP12 *progP12;
      ProgP16 *progP16;
      ProgP18 *progP18;
      ProgP24 *progP24;

      bool deviceInfoLessThan(const DeviceInfo &d1, const DeviceInfo &d2);
      bool deviceNameLessThan(const QString &deviceName, const DeviceInfo &d1);
      int binarySearch(const QList<DeviceInfo*> &devices, QString deviceName);
      void quickSort(QList<DeviceInfo *> *devices, int l, int r);

      void initPIC16_ID();
   signals:

   public slots:

};

#endif // DEVICES_H
