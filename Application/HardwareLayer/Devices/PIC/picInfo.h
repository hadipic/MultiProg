#ifndef PICINFO_H
#define PICINFO_H

#include <QObject>
#include <QStringList>
#include "../fuseByte.h"

class DeviceTag
{
   public:
      QString name;
      QString status;
      QString memoryTechnology;
      QString architecture;
      QString alternative;
      QString document;
      QString selfWrite;
      quint32 ID;
      quint64 IDLowPower;
      quint64 IDHighVoltage;
      quint64 pc;
      DeviceTag()
      {
         ID=IDLowPower=IDHighVoltage=pc=-1;
      }
};

namespace ChecksumProtectedType
{
   enum ChecksumProtectedType{Off=0, All=1, Part=2, Microprocessor,
                              Microcontroller,
                              ExtendedMicrocontroller,
                              CodeProtectedMicrocontroller, HighSecuriry};
}

class Checksum
{
   public:
      QString protectedType;
      qint8   protectedBlocks;
      qint64 start;
      qint64 end;
      QString type;
      qint64 constant;
      qint64 bchecksum;
      qint64 cchecksum;
      qint64 bbsize;
      inline Checksum()
      {
         protectedBlocks=start = end = constant = bchecksum = cchecksum=bbsize=-1;
         protectedType=type="";
      }
};

class Frequency
{
   public:
      double start;
      double end;
      double vddMin;
      double vddMax;
      double vddMinEnd;
      QString osc;
      inline Frequency()
      {
         start = end = vddMin = vddMax = vddMinEnd = -1;
         osc="";
      }
};
class FrequencyRange
{
   public:
      QString name;
      QString special;
      QList<Frequency> frequencies;
      inline FrequencyRange()
      {
         name = special="";
      }
};

class Voltage
{
   public:
      QString name;
      double min;
      double max;
      double nominal;
      inline Voltage()
      {
         min = max = nominal = -1;
         name="";
      }

};


class Memory
{
   public:
      qint64 start;
      qint64 end;
      bool isValid;
      inline Memory()
      {
         start=end=-1;
         isValid=false;
      }
      void saveToMemCODE_W  (unsigned char *fromMemory);
      void loadFromMemCODE_W(unsigned char *toMemory);
      quint64 size();


};

class CalibrationMemory:public Memory
{
   public:
      quint64 calOpmask;
      quint64 calOpcode;
      inline CalibrationMemory()
      {
         calOpmask = calOpcode = -1;
      }
};

class CodeMemory:public Memory
{
   public:
      quint64 wordWriteAlign;
      quint64 wordEraseAlign;
      CodeMemory()
      {
         wordWriteAlign = wordEraseAlign = -1;
      }
};

class ConfigMemory:public Memory
{
   public:
      quint64 hexfileOffset;
      ConfigMemory()
      {
         hexfileOffset = -1;
      }
};

class EEPromMemory:public Memory
{
   public:
      quint64 rmask;
      quint64 hexfileOffset;
      EEPromMemory()
      {
         rmask = hexfileOffset = -1;
      }
};

class UserIdsMemory:public Memory
{
   public:
      quint64 rmask;
      UserIdsMemory()
      {
         rmask = -1;
      }
};


class MemoriesInfo
{
   public:
      CodeMemory        *code;
      CalibrationMemory *calibration;
      UserIdsMemory     *userIDs;
      Memory            *deviceID;
      ConfigMemory      *config;
      EEPromMemory      *eeprom;
      Memory            *debugVector;
      Memory            *calibrationBackup;
      Memory            *programExecutive;
      MemoriesInfo()
      {
         code = NULL;
         calibration =NULL;
         userIDs= NULL;
         deviceID = NULL;
         config = NULL;
         eeprom = NULL;
         debugVector= NULL;
         calibrationBackup=  NULL;
         programExecutive = NULL;
      }
};


class Package
{
   public:
      QStringList typeNames;
      QStringList pinNames;
};

class PicInfo
{
      //Q_OBJECT
   public:
      //explicit PicInfo(QObject *parent=0);
      PicInfo();
      PicInfo(QString name);
      ~PicInfo();
      DeviceTag            deviceTag;
      QList<Checksum>      checksums;
      QList<FrequencyRange>frequencyRanges;
      QList<Voltage>       voltages;
      MemoriesInfo         memoriesInfo;
      QList<FuseByte>      fuseBytes;
      QList<Package>       packages;

      void saveToDB();
      bool loadFromDB(QString name);

      bool finded();
   private:
      bool _finded;

};

#endif // PICINFO_H
