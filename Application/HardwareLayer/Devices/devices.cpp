#include "devices.h"
#include <QStringList>
using namespace Family;
using namespace ReadFunctions;
using namespace WriteFunctions;

DeviceFamily *Devices::addDeviceFamily(QString familyName, QString companyName)
{
   DeviceFamily *deviceFamily = new DeviceFamily();
   deviceFamily ->familyName = familyName;
   deviceFamily ->companyName = companyName;
   deviceFamilyList.append(deviceFamily);
   return deviceFamily;
}


DeviceType *DeviceFamily::addDeviceType(QString typeName)
{
   DeviceType * deviceType = new DeviceType();
   deviceType->typeName=typeName;
   deviceTypesList.append(deviceType);
   return deviceType;
}


DeviceInfo * DeviceType::addDevice(QString deviceName, ReadWriteInfo *readWriteInfo)
{
   DeviceInfo  *device = new DeviceInfo();
   device->name= deviceName;
   device->readWriteInfo=readWriteInfo;
   devicesList.append(device);
   return device;
}


void Devices::addDevice(QString deviceNames, int family, double HV, int V33, int readFunctionIndex, QString readParam, int resArea, int writeFunctionIndex, QString writeParam, double writeParamD)
{
   QStringList rParams = readParam.split(",");
   QStringList wParams = writeParam.split(",");
   QList<int> rParamList;
   QList<int> wParamList;
   qint64 number;
   bool ok;
   qint64 mFlashSize=0;
   qint64 mEESize=0;

   /*#define LOCK	1
    #define FUSE	2
    #define FUSE_H  4
    #define FUSE_X	8
    #define CAL 	16
    #define SLOW	256
    */
   for(int i=0;i<rParams.length(); i++)
   {
      if(rParams[i].contains("+"))
      {
         QStringList defineList=rParams[i].split('+');
         number=0;
         for (int j = 0; j < defineList.length(); j++)
         {
            if(defineList[j]=="LOCK")
               number+=1;
            else if(defineList[j]=="FUSE")
               number+=2;
            else if(defineList[j]=="FUSE_H")
               number+=4;
            else if(defineList[j]=="FUSE_X")
               number+=8;
            else if(defineList[j]=="CAL")
               number+=16;
            else if(defineList[j]=="SLOW")
               number+=256;
         }
      }
      else if(rParams[i].startsWith("0x"))
         number=rParams[i].mid(2).toLongLong(&ok, 16);
      else
         number=rParams[i].toLongLong(&ok, 10);
      switch (i)
      {
         case 0:mFlashSize=number; break;
         case 1:
            if(rParams[i].startsWith("0x"))
               mEESize=number;
            break;
      }
      if(i>1)
         rParamList.append(number);
   }

   for(int i=0;i<wParams.length(); i++)
   {
      if(wParams[i].startsWith("0x"))
         number=wParams[i].mid(2).toLongLong(&ok, 16);
      else
         number=wParams[i].toLongLong(&ok, 10);
      wParamList.append(number);
   }

   ReadWriteInfo *readWriteInfo = new ReadWriteInfo(usb, family, HV, V33,
                                                    readFunctionIndex, mFlashSize, mEESize, rParamList, resArea,
                                                    writeFunctionIndex, wParamList, writeParamD);
   //readWriteInfo->setProgrammersInterface(progEEPROM, progAVR, progP12, progP16, progP18, progP24);
   QStringList devices = deviceNames.split(",");
   for(int i=0;i<devices.length(); i++)
      for(int j=0;j<deviceTypeTable.length(); j++)
         if(devices[i].startsWith(deviceTypeTable[j].first))
         {
            DeviceInfo *device=deviceTypeTable[j].second->addDevice(devices[i], readWriteInfo);
            device->parentDeviceType = deviceTypeTable[j].second;
            allDevices.append(device);
            break;
         }
}

void Devices::addDeviceType(DeviceFamily *deviceFamily, QString typeNameForShow, QString perfixName)
{
   DeviceType *deviceType=deviceFamily->addDeviceType(typeNameForShow);
   deviceType->parentDeviceFamily = deviceFamily;
   deviceTypeTable.append(qMakePair(perfixName, deviceType));
}


bool Devices::setDeviceID(quint16 id, QString name, quint16 revMask)
{
   DeviceInfo device;
   device.name = name;
   //QList<DeviceInfo*>::iterator index=
   int index=binarySearch(allDevices, name);
   if(index>=0)
   {
      allDevices.at(index)->id = id;
      allDevices.at(index)->revMask = revMask;
      return true;
   }
   return false;
}

void Devices::setProgrammersInterface(ProgEEPROM *progEEPROM, ProgAVR *progAVR, ProgP12 *progP12, ProgP16 *progP16, ProgP18 *progP18, ProgP24 *progP24)
{
   this->progEEPROM = progEEPROM;
   this->progAVR = progAVR;
   this->progP12 = progP12;
   this->progP16= progP16;
   this->progP18 = progP18;
   this->progP24 = progP24;
}

int Devices::binarySearch(const QList<DeviceInfo *> &devices, QString deviceName)
{
   int first = 0;
   int last = devices.count() - 1;
   int middle = (first+last)/2;

   while( first <= last )
   {
      if ( devices[middle]->name < deviceName)
         first = middle + 1;
      else if ( devices[middle]->name == deviceName )
         return middle;
      else
         last = middle - 1;
      middle = (first + last)/2;
   }
   if ( first > last )
      return -1;
}

struct MyComparer
{
   bool operator()(const DeviceInfo & a, const DeviceInfo & b) const
   {
      return a.name.toLower() < b.name.toLower();
   }
};

Devices::Devices(USBTools *usb, QObject *parent) :
   QObject(parent)
{
   this->usb = usb;
   //ReadPtr(10,5);
   DeviceFamily *PIC = addDeviceFamily("PIC", "MicroChip");
   DeviceFamily *AVR_Family = addDeviceFamily("AVR", "ATMEL");
   DeviceFamily *Memory = addDeviceFamily("Memory", "Standard");
   DeviceFamily *OneWire = addDeviceFamily("One Wire", "Standard");


   /*
    DeviceType *typeAVRAT90=AVR->addDeviceType("ATMEL AT90");
    DeviceType *typeAVRATmega=AVR->addDeviceType("ATMEL ATmega");
    DeviceType *typeAVRATtiny=AVR->addDeviceType("ATMEL ATtiny");

    DeviceType *typeI2C24C=Memory->addDeviceType("I2C 24C");
    DeviceType *typeFlash25=Memory->addDeviceType("Flash 25");
    DeviceType *typeSPI93=Memory->addDeviceType("SPI 93");
    DeviceType *typeSPI95=Memory->addDeviceType("SPI 95");

    DeviceType *typeOneWireDS=OneWire->addDeviceType("OneWire DS");
    DeviceType *typeOneWire11=OneWire->addDeviceType("OneWire 11");
    */
   addDeviceType(PIC, "PIC 10F", "10");
   addDeviceType(PIC, "PIC 12CF", "12");
   addDeviceType(PIC, "PIC 16CF", "16");
   // addDeviceType(PIC, "PIC 16F", "16F");

   addDeviceType(PIC, "PIC 18F", "18");
   addDeviceType(PIC, "PIC 24F", "24F");
   addDeviceType(PIC, "PIC 24H", "24H");
   addDeviceType(PIC, "PIC 24EP", "24EP");
   addDeviceType(PIC, "PIC 30F", "30F");
   addDeviceType(PIC, "PIC 33F", "33F");
   addDeviceType(PIC, "PIC 33EP", "33EP");

   addDeviceType(AVR_Family, "AT90",   "AT90S");
   addDeviceType(AVR_Family, "ATtiny", "ATtiny");
   addDeviceType(AVR_Family, "ATmega", "ATmega");

   addDeviceType(Memory, "I2C", "24");
   addDeviceType(Memory, "SPI", "93");
   addDeviceType(Memory, "FLASH", "25");

   addDeviceType(OneWire, "DSX", "DS");
   addDeviceType(OneWire, "11", "11");




   addDevice("10F200,10F204,10F220",
             PIC12,13.0,0,Read12F5xx,"0x100,5",0x40,Write12F5xx,"0x100,0xFF",0);	//256
   addDevice("12C508,12C508A",
             PIC12,13.0,0,Read12F5xx,"0x200,4",0x40,Write12C5xx,"0x200,0",0);	//512
   addDevice("12F508,10F202,10F206,10F222",
             PIC12,13.0,0,Read12F5xx,"0x200,5",0x40,Write12F5xx,"0x200,0x1FF",0);//512
   addDevice("16F54", //4
             PIC12,13.0,0,Read12F5xx,"0x200,4",0x40,Write12F5xx,"0x200,-1",0);	//512, no osccal
   addDevice("12C509,12C509A", //5
             PIC12,13.0,0,Read12F5xx,"0x400,4",0x40,Write12C5xx,"0x400,0",0);	//1K
   addDevice("12F509,12F510,16F505,16F506", //6
             PIC12,13.0,0,Read12F5xx,"0x400,5",0x40,Write12F5xx,"0x400,0x3FF",0);//1K
   addDevice("12F519,16F526,16F527",  // group 7
             PIC12,13.0,0,Read12F5xx,"0x440,8",0x60,Write12F5xx,"0x440,0x3FF",0);//1K + 64
   addDevice("16F57,16F59", //group8
             PIC12,13.0,0,Read12F5xx,"0x800,4",0x40,Write12F5xx,"0x800,-1",0);	//2K
   addDevice("16F627",
             PIC16,13.0,0,Read16Fxxx,"0x400,-0x80,8,0",0x10,Write16F62x,"0x400,0x80,-10",0);	//1K, 128, vpp, ee@0x2200
   addDevice("16F627A",
             PIC16,13.0,0,Read16Fxxx,"0x400,0x80,8,0",0x10,Write12F6xx,"0x400,0x80,-10",0);	//1K, 128, vpp

   addDevice("16F628A",
             PIC16,13.0,0,Read16Fxxx,"0x800,0x80,8,0",0x10,Write12F6xx,"0x800,0x80,-10",0);	//2K, 128, vpp
   addDevice("12F609,16F610,12F615",
             PIC16,13.0,0,Read16Fxxx,"0x400,0,9,0",0x40,Write12F61x,"0x400,0,0",0);		//1K, vpp, cal1


   addDevice("12F629,16F630,12F675,16F676",
             PIC16,13.0,0,Read16Fxxx,"0x400,0x80,8,0",0x20,Write12F62x,"0x400,0x80,-10",0);	//1K, 128, vpp

   addDevice("16F631",
             PIC16,13.0,0,Read16Fxxx,"0x400,0x80,9,0",0x80,Write12F6xx,"0x400,0x80,-10",0);	//1K, 128, vpp, cal1


   addDevice("12F635",
             PIC16,13.0,0,Read16Fxxx,"0x400,0x80,10,0",0x80,Write12F6xx,"0x400,0x80,-10",0);	//1K, 128, vpp, cal1 + cal2


   addDevice("12C671,12CE673",
             PIC16,13.0,0,Read16Fxxx,"0x400,0,0,0",0x100,0,"",0);							//1K, vpp
   addDevice("12C672,12CE674",
             PIC16,13.0,0,Read16Fxxx,"0x800,0,0,0",0x100,0,"",0);							//2K, vpp




   addDevice("12F683,16F684",
             PIC16,13.0,0,Read16Fxxx,"0x800,0x100,9,0",0x40,Write12F6xx,"0x800,0x100,-10",0);//2K, 256, vpp, cal1

   addDevice("16F636,16F639,16F785",
             PIC16,13.0,0,Read16Fxxx,"0x800,0x100,10,0",0x40,Write12F6xx,"0x800,0x100,-10",0);//2K, 256, vpp, cal1 + cal2

   addDevice("16F628",
             PIC16,13.0,0,Read16Fxxx,"0x800,-0x80,8,0",0x10,Write16F62x,"0x800,0x80,-10",0);	//2K, 128, vpp, ee@0x2200
   addDevice("16F616,12F617",
             PIC16,13.0,0,Read16Fxxx,"0x800,0,9,0",0x40,Write12F61x,"0x800,0,0",0);		//2K, vpp, cal1
   addDevice("16F648A",
             PIC16,13.0,0,Read16Fxxx,"0x1000,0x100,8,0",0x10,Write12F6xx,"0x1000,0x100,-10",0);//4K, 256, vpp



   addDevice("16F677,16F687",
             PIC16,13.0,0,Read16Fxxx,"0x800,0x100,9,0",0x80,Write12F6xx,"0x800,0x100,-10",0);//2K, 256, vpp, cal1

   addDevice("16F685,16F689,16F690",
             PIC16,13.0,0,Read16Fxxx,"0x1000,0x100,9,0",0x80,Write12F6xx,"0x1000,0x100,-10",0);//4K, 256, vpp, cal1
   addDevice("16F688",
             PIC16,13.0,0,Read16Fxxx,"0x1000,0x100,9,0",0x40,Write12F6xx,"0x1000,0x100,-10",0);//4K, 256, vpp, cal1


   addDevice("16F716",
             PIC16,13.0,0,Read16Fxxx,"0x800,0,8,2",8,Write16F71x,"0x800,1,-10",0);			//2K, vdd

   addDevice("16F72",
             PIC16,8.5,1,Read16Fxxx,"0x800,0,8,1",0x20,Write16F7x,"0x800,0,-10",0);			//2K, vdd, 3.3V
   addDevice("16F720,16F722,16F722A",
             PIC16,13.0,0,Read16Fxxx,"0x800,0,11,0",0x100,Write16F72x,"0x800,0,0",0);	//2K, vpp, config1-2 + cal1-2, 3.3V

   addDevice("16F721,16F723,16F723A,16F724",
             PIC16,13.0,0,Read16Fxxx,"0x1000,0,11,0",0x100,Write16F72x,"0x1000,0,0",0);	//4K, vpp, config1-2 + cal1-2, 3.3V
   addDevice("16F726,16F727,16F707",
             PIC16,8.5,1,Read16Fxxx,"0x2000,0,11,0",0x100,Write16F72x,"0x2000,0,0",0);	//8K, vpp, config1-2 + cal1-2, 3.3V


   addDevice("16F73,16F74",
             PIC16,13.0,0,Read16Fxxx,"0x1000,0,8,1",0x20,Write16F7x,"0x1000,0,-10",0);		//4K, vdd
   addDevice("16F737,16F747",
             PIC16,13.0,0,Read16Fxxx,"0x1000,0,9,2",9,Write16F7x,"0x1000,1,-10",0);			//4K, vdd short/no delay
   addDevice("16F753",
             PIC16,12.5,0,Read16Fxxx,"0x800,0,10,0",0x80,Write12F61x,"0x800,0,0",0);		//2K, vpp, cal1

   addDevice("16F76,16F77",
             PIC16,13.0,0,Read16Fxxx,"0x2000,0,8,1",0x20,Write16F7x,"0x2000,0,-10",0);		//8K, vdd
   addDevice("16F767,16F777",
             PIC16,13.0,0,Read16Fxxx,"0x2000,0,9,2",0x40,Write16F7x,"0x2000,1,-10",0);		//8K, vdd short delay

   addDevice("16F873A,16F874A",
             PIC16,13.0,0,Read16Fxxx,"0x1000,0x80,8,1",0x100,Write16F87xA,"0x1000,0x80,1",0);//4K, 128, vdd
   addDevice("16F873,16F874",
             PIC16,13.0,0,Read16Fxxx,"0x1000,-0x80,8,1",0x100,Write16F87x,"0x1000,-0x80,-10",0);//4K, 128, vdd, ee@0x2200
   addDevice("16F87,16F88",
             PIC16,13.0,0,Read16Fxxx,"0x1000,0x100,9,2",0x10,Write16F81x,"0x1000,0x100,-10",0);//4K, 256, vdd short delay

   addDevice("16F876A,16F877A",
             PIC16,13.0,0,Read16Fxxx,"0x2000,0x100,8,1",0x100,Write16F87xA,"0x2000,0x100,1",0);//8K, 256, vdd
   //signal
   //addDevice("16F876A,16F877A",
   //            PIC16,8.5,1,Read16Fxxx,"0x2000,0x100,8,1",0x100,Write16F87xA,"0x2000,0x100,1",0);//8K, 256, vdd

   addDevice("16F876,16F877",
             PIC16,13.0,0,Read16Fxxx,"0x2000,-0x100,8,1",0x100,Write16F87x,"0x2000,-0x100,-10",0);//8K, 256, vdd, ee@0x2200

   addDevice("16F883,16F884",
             PIC16,13.0,0,Read16Fxxx,"0x1000,0x100,10,0",0x80,Write16F88x,"0x1000,0x100,-10",0);//4K, 256, vpp, config2 + cal1
   addDevice("16F886,16F887",
             PIC16,13.0,0,Read16Fxxx,"0x2000,0x100,10,0",0x80,Write16F88x,"0x2000,0x100,-10",0);//8K, 256, vpp, config2 + cal1
   addDevice("16F913,16F914",
             PIC16,13.0,0,Read16Fxxx,"0x1000,0x100,10,0",0x40,Write12F6xx,"0x1000,0x100,-10",0);//4K, 256, vpp, cal1 + cal2

   addDevice("16F916,16F917,16F946",
             PIC16,13.0,0,Read16Fxxx,"0x2000,0x100,10,0",0x40,Write12F6xx,"0x2000,0x100,-10",0);//8K, 256, vpp, cal1 + cal2


   addDevice("12F1572",
             PIC16,8.5,1,Read16F1xxx,"0x800,0,12,0",0x200,Write16F1xxx,"0x800,0,0",0);		//2K, vpp, 3.3V
   addDevice("12F1501",
             PIC16,8.5,1,Read16F1xxx,"0x400,0,11,0",0x100,Write16F1xxx,"0x400,0,0",0);		//1K, vpp, 3.3V
   addDevice("12F1571",
             PIC16,8.5,1,Read16F1xxx,"0x400,0,12,0",0x200,Write16F1xxx,"0x400,0,0",0);		//1K, vpp, 3.3V




   addDevice("16F1516,16F1517,16F1526",
             PIC16,8.5,1,Read16F1xxx,"0x2000,0,11,0",0x200,Write16F1xxx,"0x2000,0,0",0);		//8K, vpp, 3.3V
   addDevice("16F1518,16F1519,16F1527",
             PIC16,8.5,1,Read16F1xxx,"0x4000,0,11,0",0x200,Write16F1xxx,"0x4000,0,0",0);		//16K, vpp, 3.3V

   addDevice("12F1822,16F1823,16F1826",
             PIC16,8.5,1,Read16F1xxx,"0x800,0x100,11,0",0x200,Write16F1xxx,"0x800,0x100,0",0);//2K, 256, vpp, 3.3V




   addDevice("16LF1903,16LF1904,16F1508,16F1783,16F1784",
             PIC16,8.5,1,Read16F1xxx,"0x1000,0,11,0",0x200,Write16F1xxx,"0x1000,0,0",0);		//4K, vpp, 3.3V


   addDevice("16LF1902,16F1503,16F1507,16F1782",
             PIC16,8.5,1,Read16F1xxx,"0x800,0,11,0",0x200,Write16F1xxx,"0x800,0,0",0);		//2K, vpp, 3.3V

   addDevice("16LF1906,16LF1907,16F1509,16F1786,16F1787,16F1454,16F1455,16F1459",
             PIC16,8.5,1,Read16F1xxx,"0x2000,0,11,0",0x200,Write16F1xxx,"0x2000,0,0",0);		//8K, vpp, 3.3V

   addDevice("16F1933,16F1934,16F1824,16F1827,16F1828,12F1840",
             PIC16,8.5,1,Read16F1xxx,"0x1000,0x100,11,0",0x200,Write16F1xxx,"0x1000,0x100,0",0);//4K, 256, vpp, 3.3V
   addDevice("16F1936,16F1937,16F1946,16F1825,16F1829,16F1847",
             PIC16,8.5,1,Read16F1xxx,"0x2000,0x100,11,0",0x200,Write16F1xxx,"0x2000,0x100,0",0);//8K, 256, vpp, 3.3V
   addDevice("16F1938,16F1939,16F1947",
             PIC16,8.5,1,Read16F1xxx,"0x4000,0x100,11,0",0x200,Write16F1xxx,"0x4000,0x100,0",0);//16K, 256, vpp, 3.3V
   //-------------PIC18---------------------------------------------------------
   // options:
   //	bit [3:0]
   //     0 = vdd before vpp (12V)
   //     1 = vdd before vpp (9V)
   //     2 = low voltage entry with 32 bit key
   //	bit [7:4]
   //     0 = normal eeprom write algoritm
   //     1 = with unlock sequence 55 AA
   //	bit [11:8]
   //     0 = 5ms erase delay, 1ms code write time, 5ms EE write delay, 5ms config write time
   //     1 = 550ms erase delay, 1.2ms code write time, no config or EEPROM
   //     2 = 550ms erase delay, 3.4ms code write time, no config or EEPROM
   addDevice("18F1230",
             PIC18,12,0,Read18Fx,"0x1000,0x80,0",0,Write18Fx,"0x1000,0x80,8,0x0F0F,0x8787,0",0);	//4K, 128, 8
   addDevice("18F1220,18F2220,18F4220",
             PIC18,12,0,Read18Fx,"0x1000,0x100,0",0,Write18Fx,"0x1000,0x100,8,0x10000,0x80,0x10",0);//4K, 256, 8, EE with unlock
   addDevice("18F1330",
             PIC18,12,0,Read18Fx,"0x2000,0x80,0",0,Write18Fx,"0x2000,0x80,8,0x0F0F,0x8787,0",0);	//8K, 128, 8
   addDevice("18F2221,18F4221",
             PIC18,12,0,Read18Fx,"0x1000,0x100,0",0,Write18Fx,"0x1000,0x100,8,0x3F3F,0x8F8F,0",0);//4K, 256, 8

   addDevice("18F2321,18F4321",
             PIC18,12,0,Read18Fx,"0x2000,0x100,0",0,Write18Fx,"0x2000,0x100,8,0x3F3F,0x8F8F,0",0);//8K, 256, 8
   addDevice("18F1320,18F2320,18F4320,18F2331,18F4331",
             PIC18,12,0,Read18Fx,"0x2000,0x100,0",0,Write18Fx,"0x2000,0x100,8,0x10000,0x80,0x10",0);//8K, 256, 8, EE with unlock
   addDevice("18F13K50,18F13K22",
             PIC18,8.5,1,Read18Fx,"0x2000,0x100,1",0,Write18Fx,"0x2000,0x100,8,0x0F0F,0x8F8F,1",0);//8K, 256, 8, 9V
   addDevice("18F23K20,18F43K20",
             PIC18,8.5,1,Read18Fx,"0x2000,0x100,1",0,Write18Fx,"0x2000,0x100,16,0x0F0F,0x8F8F,1",0);//8K, 256, 16, 9V
   addDevice("18F23K220,18F43K22",
             PIC18,8.5,1,Read18Fx,"0x2000,0x100,1",0,Write18Fx,"0x2000,0x100,64,0x0F0F,0x8F8F,1",0);//8K, 256, 64, 9V
   addDevice("18F2439,18F4439",
             PIC18,12,0,Read18Fx,"0x3000,0x100,0",0,Write18Fx,"0x3000,0x100,8,0x10000,0x80,0x10",0);//12K, 256, 8, EE with unlock
   addDevice("18F2410,18F4410",
             PIC18,12,0,Read18Fx,"0x4000,0,0",0,Write18Fx,"0x4000,0,32,0x3F3F,0x8F8F,0",0);		//16K, 0, 32
   addDevice("18F24J10,18F44J10",
             PIC18,-1,1,Read18Fx,"0x4000,0,2",0,Write18Fx,"0x4000,0,64,0x0101,0x8080,0x202",0);	//16K, 0, 64, LV
   addDevice("18F24J11,18F24J50,18F44J11,18F44J50",
             PIC18,-1,1,Read18Fx,"0x4000,0,2",0,Write18Fx,"0x4000,0,64,0x0101,0x8080,0x102",0);	//16K, 0, 64, LV
   addDevice("18F2420,18F2423,18F4420,18F4423,18F2480,18F4480",
             PIC18,12,0,Read18Fx,"0x4000,0x100,0",0,Write18Fx,"0x4000,0x100,32,0x3F3F,0x8F8F,0",0);//16K, 256, 32
   addDevice("18F2431,18F4431,18F242,18F248,18F442,18F448",
             PIC18,12,0,Read18Fx,"0x4000,0x100,0",0,Write18Fx,"0x4000,0x100,8,0x10000,0x80,0x10",0);//16K, 256, 8, EE with unlock
   addDevice("18F2450,18F4450",
             PIC18,12,0,Read18Fx,"0x4000,0,0",0,Write18Fx,"0x4000,0,16,0x3F3F,0x8F8F,0",0);		//16K, 0, 16
   addDevice("18F14K50,18F14K22",
             PIC18,8.5,1,Read18Fx,"0x4000,0x100,1",0,Write18Fx,"0x4000,0x100,16,0x0F0F,0x8F8F,1",0);//16K, 256, 16, 9V
   addDevice("18F24K20,18F44K20",
             PIC18,8.5,1,Read18Fx,"0x4000,0x100,1",0,Write18Fx,"0x4000,0x100,32,0x0F0F,0x8F8F,1",0);//16K, 256, 32, 9V
   addDevice("18F24K22,18F44K22",
             PIC18,8.5,1,Read18Fx,"0x4000,0x100,1",0,Write18Fx,"0x4000,0x100,64,0x0F0F,0x8F8F,1",0);//16K, 256, 64, 9V
   addDevice("18F2455,18F2458,18F4455,18F4458",
             PIC18,12,0,Read18Fx,"0x6000,0x100,0",0,Write18Fx,"0x6000,0x100,32,0x3F3F,0x8F8F,0",0);//24K, 256, 32
   addDevice("18F2539,18F4539",
             PIC18,12,0,Read18Fx,"0x6000,0x100,0",0,Write18Fx,"0x6000,0x100,8,0x10000,0x80,0x10",0);//24K, 256, 8, EE with unlock
   addDevice("18F2510,18F4510",
             PIC18,12,0,Read18Fx,"0x8000,0,0",0,Write18Fx,"0x8000,0,32,0x3F3F,0x8F8F,0",0);		//32K, 0, 32
   addDevice("18F25J10,18F45J10",
             PIC18,-1,1,Read18Fx,"0x8000,0,2",0,Write18Fx,"0x8000,0,64,0x0101,0x8080,0x202",0);	//32K, 0, 64, LV
   addDevice("18F25J11,18F25J50,18F45J11,18F45J50",
             PIC18,-1,1,Read18Fx,"0x8000,0,2",0,Write18Fx,"0x8000,0,64,0x0101,0x8080,0x102",0);	//32K, 0, 64, LV
   addDevice("18F252,18F258,18F452,18F458",
             PIC18,12,0,Read18Fx,"0x8000,0x100,0",0,Write18Fx,"0x8000,0x100,8,0x10000,0x80,0x10",0);//32K, 256, 8, EE with unlock
   addDevice("18F2550,18F2553,18F4550,18F4553,18F2520,18F2523,18F4520,18F4523,18F2580,18F4580",
             PIC18,12,0,Read18Fx,"0x8000,0x100,0",0,Write18Fx,"0x8000,0x100,32,0x3F3F,0x8F8F,0",0);//32K, 256, 32
   addDevice("18F25K20,18F45K20",
             PIC18,8.5,1,Read18Fx,"0x8000,0x100,1",0,Write18Fx,"0x8000,0x100,32,0x0F0F,0x8F8F,1",0);//32K, 256, 32, 9V
   addDevice("18F25K22,18F45K22",
             PIC18,8.5,1,Read18Fx,"0x8000,0x100,1",0,Write18Fx,"0x8000,0x100,64,0x0F0F,0x8F8F,1",0);//32K, 256, 64, 9V
   addDevice("18F8520",
             PIC18,12,0,Read18Fx,"0x8000,0x400,0",0,Write18Fx,"0x8000,0x400,8,0x10000,0x0080,0x10",0);//32K, 1024, 8, EE with unlock
   addDevice("18F2515,18F4515",
             PIC18,12,0,Read18Fx,"0xC000,0,0",0,Write18Fx,"0xC000,0,64,0x3F3F,0x8F8F,0",0);		//48K, 0, 64
   addDevice("18F2525,18F2585,18F4525,18F4585",
             PIC18,12,0,Read18Fx,"0xC000,0x400,0",0,Write18Fx,"0xC000,0x400,64,0x3F3F,0x8F8F,0",0);//48K, 1K, 64
   addDevice("18F2610,18F4610",
             PIC18,12,0,Read18Fx,"0x10000,0,0",0,Write18Fx,"0x10000,0,64,0x3F3F,0x8F8F,0",0);	//64K, 0, 64
   addDevice("18F26J11,18F26J13,18F26J50,18F26J53,18F46J11,18F46J13,18F46J50,18F46J53",
             PIC18,-1,1,Read18Fx,"0x10000,0,2",0,Write18Fx,"0x10000,0,64,0x0101,0x8080,0x102",0);//64K, 0, 64, LV
   addDevice("18F66J60,18F86J60,18F96J60",
             PIC18,-1,1,Read18Fx,"0x10000,0,2",0,Write18Fx,"0x10000,0,64,0x0101,0x8080,0x202",0);//64K, 0, 64, LV
   addDevice("18F2620,18F2680,18F4620,18F4680",
             PIC18,12,0,Read18Fx,"0x10000,0x400,0",0,Write18Fx,"0x10000,0x400,64,0x3F3F,0x8F8F,0",0);//64K, 1K, 64
   addDevice("18F26K20,18F46K20,18F26K22,18F46K22",
             PIC18,12,0,Read18Fx,"0x10000,0x400,1",0,Write18Fx,"0x10000,0x100,64,0x0F0F,0x8F8F,1",0);//64K, 256, 64, 9V
   addDevice("18F2682,18F4682",
             PIC18,12,0,Read18Fx,"0x14000,0x400,0",0,Write18Fx,"0x14000,0x400,64,0x3F3F,0x8F8F,0",0);//80K, 1K, 64
   addDevice("18F66J65,18F86J65,18F96J65",
             PIC18,-1,1,Read18Fx,"0x18000,0,2",0,Write18Fx,"0x18000,0,64,0x0101,0x8080,0x202",0);//96K, 0, 64, LV
   addDevice("18F2685,18F4685",
             PIC18,12,0,Read18Fx,"0x18000,0x400,0",0,Write18Fx,"0x18000,0x400,64,0x3F3F,0x8F8F,0",0);//96K, 1K, 64
   addDevice("18F27J13,18F27J53,18F47J13,18F47J53",
             PIC18,-1,1,Read18Fx,"0x20000,0,2",0,Write18Fx,"0x20000,0,64,0x0101,0x8080,0x102",0);//128K, 0, 64, LV
   addDevice("18F67J60,18F87J60,18F97J60",
             PIC18,-1,1,Read18Fx,"0x20000,0,2",0,Write18Fx,"0x20000,0,64,0x0101,0x8080,0x202",0);//128K, 0, 64, LV
   addDevice("18F8720,18F8722",
             PIC18,12,0,Read18Fx,"0x20000,0x400,0",0,Write18Fx,"0x20000,0x400,64,0xFFFF,0x8787,0",0);//128K, 1K, 64
   //-------------PIC24---------------------------------------------------------
   // options:
   //	bit [3:0]
   //     0 = low voltage ICSP entry
   //     1 = High voltage ICSP entry (6V)
   //     2 = High voltage ICSP entry (12V) + PIC30F sequence (additional NOPs)
   //     3 = low voltage ICSP entry (5V power supply)
   //	bit [7:4]
   //	   0 = config area in the last 2 program words
   //	   1 = config area in the last 3 program words
   //	   2 = config area in the last 4 program words
   //	   3 = 0xF80000 to 0xF80010 except 02 (24F)
   //     4 = 0xF80000 to 0xF80016 (24H-33F)
   //     5 = 0xF80000 to 0xF8000C (x16 bit, 30F)
   //     6 = 0xF80000 to 0xF8000E (30FSMPS)
   //	bit [11:8]
   //	   0 = code erase word is 0x4064, row write is 0x4004
   //	   1 = code erase word is 0x404F, row write is 0x4001
   //	   2 = code erase word is 0x407F, row write is 0x4001, 55AA unlock and external timing (2 ms)
   //	   3 = code erase word is 0x407F, row write is 0x4001, 55AA unlock and external timing (200 ms)
   //	bit [15:12]
   //	   0 = eeprom erase word is 0x4050, write word is 0x4004
   //	   1 = eeprom erased with bulk erase, write word is 0x4004
   //	   2 = eeprom erased with special sequence, write word is 0x4004
   //	bit [19:16]
   //	   0 = config write is 0x4000
   //	   1 = config write is 0x4003
   //	   2 = config write is 0x4004
   //	   3 = config write is 0x4008
   addDevice("24F04KA200,24F04KA201",
             PIC24,6,1,Read24Fx,"0xB00,0,0x31,0x05BE",0x800,Write24Fx,"0xB00,0,0x20031,0x05BE,32",2.0);			//1.375KW, HV
   addDevice("24F08KA101,24F08KA102",
             PIC24,6,1,Read24Fx,"0x1600,0x200,0x31,0x05BE",0x800,Write24Fx,"0x1600,0x200,0x20031,0x05BE,32",2.0);//2.75KW, HV, 512
   addDevice("24F16KA101,24F16KA102",
             PIC24,6,1,Read24Fx,"0x2C00,0x200,0x31,0x05BE",0x800,Write24Fx,"0x2C00,0x200,0x20031,0x05BE,32",2.0);//5.5KW, HV, 512
   addDevice("24FJ16GA002,24FJ16GA004",
             PIC24,-1,1,Read24Fx,"0x2C00,0,0,0x05BE",0x800,Write24Fx,"0x2C00,0,0x10100,0x05BE,64",2.0);			//5.5KW
   addDevice("24FJ32GA002,24FJ32GA004",
             PIC24,-1,1,Read24Fx,"0x5800,0,0,0x05BE",0x800,Write24Fx,"0x5800,0,0x10100,0x05BE,64",2.0);			//11KW
   addDevice("24FJ32GA102,24FJ32GA104,24FJ32GB002,24FJ32GB004",
             PIC24,-1,1,Read24Fx,"0x5800,0,0x20,0x07F0",0x800,Write24Fx,"0x5800,0,0x10120,0x07F0,64",2.0);			//11KW 4 Config Word
   addDevice("24FJ48GA002,24FJ48GA004",
             PIC24,-1,1,Read24Fx,"0x8400,0,0,0x05BE",0x800,Write24Fx,"0x8400,0,0x10100,0x05BE,64",2.0);			//16.5KW
   addDevice("24FJ64GA002,24FJ64GA004,24FJ64GA006,24FJ64GA008,24FJ64GA010",
             PIC24,-1,1,Read24Fx,"0xAC00,0,0,0x05BE",0x800,Write24Fx,"0xAC00,0,0x10100,0x05BE,64",2.0);			//22KW
   addDevice("24FJ64GA102,24FJ64GA104,24FJ64GB002,24FJ64GB004,24FJ64GA306,24FJ64GA308,24FJ64GA310,24FJ64GC006,24FJ64GC008,24FJ64GC010",
             PIC24,-1,1,Read24Fx,"0xAC00,0,0x20,0x07F0",0x800,Write24Fx,"0xAC00,0,0x10120,0x07F0,64",2.0);			//22KW 4 Config Word
   addDevice("24FJ64GB106,24FJ64GB108,24FJ64GB110",
             PIC24,-1,1,Read24Fx,"0xAC00,0,0x10,0x07F0",0x800,Write24Fx,"0xAC00,0,0x10110,0x07F0,64",2.0);		//22KW 3 Config Word
   addDevice("24FJ96GA006,24FJ96GA008,24FJ96GA010",
             PIC24,-1,1,Read24Fx,"0x10000,0,0,0x05BE",0x800,Write24Fx,"0x10000,0,0x10100,0x05BE,64",2.0);		//32KW
   addDevice("24FJ128GA006,24FJ128GA008,24FJ128GA010",
             PIC24,-1,1,Read24Fx,"0x15800,0,0,0x05BE",0x800,Write24Fx,"0x15800,0,0x10100,0x05BE,64",2.0);		//44KW
   addDevice("24FJ128GA106,24FJ128GA108,24FJ128GA110,24FJ128GB106,24FJ128GB108,24FJ128GB110",
             PIC24,-1,1,Read24Fx,"0x15800,0,0x10,0x07F0",0x800,Write24Fx,"0x15800,0,0x10110,0x07F0,64",2.0);		//44KW 3 Config Word
   addDevice("24FJ128GA306,24FJ128GA308,24FJ128GA310,24FJ128GB206,24FJ128GB210,24FJ128GC006,24FJ128GC008,24FJ128GC010,24FJ128DA106,24FJ128DA110,24FJ128DA206,24FJ128DA210",
             PIC24,-1,1,Read24Fx,"0x15800,0,0x20,0x07F0",0x800,Write24Fx,"0x15800,0,0x10120,0x07F0,64",2.0);		//44KW 4 Config Word
   addDevice("24FJ192GA106,24FJ192GA108,24FJ192GA110,24FJ192GB106,24FJ192GB108,24FJ192GB110",
             PIC24,-1,1,Read24Fx,"0x20C00,0,0x10,0x07F0",0x800,Write24Fx,"0x20C00,0,0x10110,0x07F0,64",2.0);		//68KW 3 Config Word
   addDevice("24FJ256GA106,24FJ256GA108,24FJ256GA110,24FJ256GB106,24FJ256GB108,24FJ256GB110",
             PIC24,-1,1,Read24Fx,"0x2AC00,0,0x10,0x07F0",0x800,Write24Fx,"0x2AC00,0,0x10110,0x07F0,64",2.0);		//88KW 3 Config Word
   addDevice("24FJ256DA106,24FJ256DA110,24FJ256DA206,24FJ256DA210",
             PIC24,-1,1,Read24Fx,"0x2AC00,0,0x20,0x07F0",0x800,Write24Fx,"0x2AC00,0,0x10120,0x07F0,64",2.0);		//88KW 4 Config Word
   addDevice("33FJ06GS101,33FJ06GS102,33FJ06GS202",
             PIC24,-1,1,Read24Fx,"0x1000,0,0x40,0x07F0",0x800,Write24Fx,"0x1000,0,0x00140,0x07F0,64",2.0);		//2KW
   addDevice("24HJ12GP201,24HJ12GP202,33FJ12GP201,33FJ12GP202,33FJ12MC201,33FJ12MC202",
             PIC24,-1,1,Read24Fx,"0x2000,0,0x40,0x07F0",0x800,Write24Fx,"0x2000,0,0x00140,0x07F0,64",2.0);		//4KW
   addDevice("24HJ16GP304,33FJ16GP304,33FJ16GS402,33FJ16GS404,33FJ16GS502,33FJ16GS504,33FJ16MC304",
             PIC24,-1,1,Read24Fx,"0x2C00,0,0x40,0x07F0",0x800,Write24Fx,"0x2C00,0,0x00140,0x07F0,64",2.0);		//5.5KW
   addDevice("24HJ32GP202,24HJ32GP204,24HJ32GP302,24HJ32GP304,33FJ32GP202,33FJ32GP204,33FJ32GP302,33FJ32GP304,33FJ32GS406,33FJ32GS606,33FJ32GS608,33FJ32GS610,33FJ32MC202,33FJ32MC204,33FJ32MC302,33FJ32MC304",		PIC24,-1,1,Read24Fx,"0x5800,0,0x40,0x07F0"
             ,0x1000,Write24Fx,"0x5800,0,0x00140,0x07F0,64",2.0);		//11KW
   addDevice("24HJ64GP202,24HJ64GP204,24HJ64GP206,24HJ64GP210,24HJ64GP502,24HJ64GP504,24HJ64GP506,24HJ64GP510,33FJ64GP202,33FJ64GP204,33FJ64GP206,33FJ64GP306,33FJ64GP310,33FJ64GP706,33FJ64GP708,33FJ64GP710,33FJ64GP802,33FJ64GP804,33FJ64GS406,33FJ64GS606,33FJ64GS608,33FJ64GS610,33FJ64MC202,33FJ64MC204,33FJ64MC506,33FJ64MC508,33FJ64MC510,33FJ64MC706,33FJ64MC710,33FJ64MC802,33FJ64MC804",
             PIC24,-1,1,Read24Fx,"0xAC00,0,0x40,0x07F0",0x1000,Write24Fx,"0xAC00,0,0x00140,0x07F0,64",2.0);		//22KW
   addDevice("24HJ128GP202,24HJ128GP204,24HJ128GP206,24HJ128GP210,24HJ128GP306,24HJ128GP310,24HJ128GP502,24HJ128GP504,24HJ128GP506,24HJ128GP510,33FJ128GP202,33FJ128GP204,33FJ128GP206,33FJ128GP306,33FJ128GP310,33FJ128GP706,33FJ128GP708,33FJ128GP710,33FJ128GP802,33FJ128GP804,33FJ128MC202,33FJ128MC204,33FJ128MC506,33FJ128MC510,33FJ128MC706,33FJ128MC708,33FJ128MC710,33FJ128MC802,33FJ128MC804",
             PIC24,-1,1,Read24Fx,"0x15800,0,0x40,0x07F0",0x1000,Write24Fx,"0x15800,0,0x00140,0x07F0,64",2.0);	//44KW
   addDevice("24HJ256GP206,24HJ256GP210,24HJ256GP610,33FJ256GP506,33FJ256GP510,33FJ256GP710,33FJ256MC510,33FJ256MC710",
             PIC24,-1,1,Read24Fx,"0x2AC00,0,0x40,0x07F0",0x1000,Write24Fx,"0x2AC00,0,0x00140,0x07F0,64",2.0);	//88KW
   addDevice("24EP32GP202,24EP32GP203,24EP32GP204,33EP32GP502,33EP32GP503,33EP32GP504,24EP32MC202,24EP32MC203,24EP32MC204,33EP32MC202,33EP32MC203,33EP32MC204,33EP32MC502,33EP32MC503,33EP32MC504",
             PIC24,-1,1,Read24Ex,"0x5800,0,0,0x0FF0",0x1000,Write24Ex,"0x5800,0,0,0,0,0",2.0);					//11KW 10 Config Word
   addDevice("24EP64GP202,24EP64GP203,24EP64GP204,24EP64GP206,33EP64GP502,33EP64GP503,33EP64GP504,33EP64GP506,24EP64MC202,24EP64MC203,24EP64MC204,24EP64MC206,33EP64MC202,33EP64MC203,33EP64MC204,33EP64MC206,33EP64MC502,33EP64MC503,33EP64MC504,33EP64MC506",
             PIC24,-1,1,Read24Ex,"0xB000,0,0,0x0FF0",0x1000,Write24Ex,"0xB000,0,0,0,0,0",2.0);					//22.5KW 10 Config Word
   addDevice("24EP128GP202,24EP128GP204,24EP128GP206,33EP128GP502,33EP128GP504,33EP128GP506,24EP128MC202,24EP128MC204,24EP128MC206,33EP128MC202,33EP128MC204,33EP128MC206,33EP128MC502,33EP128MC504,33EP128MC506",
             PIC24,-1,1,Read24Ex,"0x15800,0,0,0x0FF0",0x1000,Write24Ex,"0x15800,0,0,0,0,0",2.0);					//44KW 10 Config Word
   addDevice("24EP256GP202,24EP256GP204,24EP256GP206,33EP256GP502,33EP256GP504,33EP256GP506,24EP256MC202,24EP256MC204,24EP256MC206,33EP256MC202,33EP256MC204,33EP256MC206,33EP256MC502,33EP256MC504,33EP256MC506",
             PIC24,-1,1,Read24Ex,"0x2B000,0,0,0x0FF0",0x1000,Write24Ex,"0x2B000,0,0,0,0,0",2.0);					//88KW 10 Config Word
   addDevice("24EP512GP202,24EP512GP204,24EP512GP206,33EP512GP502,33EP512GP504,33EP512GP506,24EP512MC202,24EP512MC204,24EP512MC206,33EP512MC202,33EP512MC204,33EP512MC206,33EP512MC502,33EP512MC504,33EP512MC506",
             PIC24,-1,1,Read24Ex,"0x56000,0,0,0x0FF0",0x1000,Write24Ex,"0x56000,0,0,0,0,0",2.0);					//175KW 10 Config Word
   addDevice("30F1010",
             PIC24,-1,0,Read24Fx,"0x1000,0,0x63,0x05BE",0x600,Write24Fx,"0x1000,0,0x30263,0x05BE,32",2.0);		//1KW, LV5V
   addDevice("30F2020,30F2023",
             PIC24,-1,0,Read24Fx,"0x2000,0,0x63,0x05BE",0x600,Write24Fx,"0x2000,0,0x30263,0x05BE,32",2.0);		//2KW, LV5V
   addDevice("30F2010",
             PIC24,12,0,Read24Fx,"0x2000,0x400,0x52,0x05BE",0x600,Write24Fx,"0x2000,0x400,0x31252,0x05BE,32",2.0);//4KW, 1K, HV12
   addDevice("30F2011,30F2012",
             PIC24,12,0,Read24Fx,"0x2000,0,0x52,0x05BE",0x600,Write24Fx,"0x2000,0,0x31252,0x05BE,32",2.0);		//4KW, HV12
   addDevice("30F3010,30F3011,30F3012,30F3013,30F3014",
             PIC24,12,0,Read24Fx,"0x4000,0x400,0x52,0x05BE",0x600,Write24Fx,"0x4000,0x400,0x31252,0x05BE,32",2.0);//8KW, 1K, HV12
   addDevice("30F4011,30F4012,30F4013",
             PIC24,12,0,Read24Fx,"0x8000,0x400,0x52,0x05BE",0x600,Write24Fx,"0x8000,0x400,0x31252,0x05BE,32",2.0);//16KW, 1K, HV12
   addDevice("30F5011,30F5013,30F5015,30F5016",
             PIC24,12,0,Read24Fx,"0xB000,0x400,0x52,0x05BE",0x600,Write24Fx,"0xB000,0x400,0x31252,0x05BE,32",2.0);//22KW, 1K, HV12
   addDevice("30F6011,30F6013",
             PIC24,12,0,Read24Fx,"0x16000,0x800,0x52,0x05BE",0x600,Write24Fx,"0x16000,0x800,0x31252,0x05BE,32",2.0);//44KW, 2K, HV12
   addDevice("30F6010,30F6012,30F6014,30F6015",
             PIC24,12,0,Read24Fx,"0x18000,0x1000,0x52,0x05BE",0x600,Write24Fx,"0x18000,0x1000,0x31252,0x05BE,32",2.0);//49KW, 4K, HV12
   //-------------ATMEL---------------------------------------------------------
   addDevice("AT90S1200",
             AVR,-1,0,ReadAT,"0x400,0x40,0",0,WriteAT,"0x400,0x40,-10,-10",0);							//1K, 64
   addDevice("ATtiny11",
             AVR,-1,0,ReadAT_HV,"0x400,0,LOCK+FUSE+CAL",0,WriteAT_HV,"0x400,0,0,0",0);					//1K, HV
   addDevice("ATtiny12",
             AVR,-1,0,ReadAT_HV,"0x400,0x40,LOCK+FUSE+CAL",0,WriteAT_HV,"0x400,0x40,0,0",0);				//1K, 64, HV
   addDevice("ATtiny13",
             AVR,-1,0,ReadAT_HV,"0x400,0x40,LOCK+FUSE+FUSE_H+CAL",0,WriteAT_HV,"0x400,0x40,16,0",0);		//1K, 64, HV
   addDevice("AT90S2313,AT90S2323,AT90S2343",
             AVR,-1,0,ReadAT,"0x800,0x80,0",0,WriteAT,"0x800,0x80,-10,-10",0);							//2K, 128
   addDevice("AT90S4414,AT90S4433,AT90S4434,",
             AVR,-1,0,ReadAT,"0x1000,0x100,LOCK+FUSE+FUSE_H+FUSE_X+CAL+SLOW",0,WriteATmega,"0x800,0x80,16,SLOW",0);//2K, 128
   addDevice("ATtiny2313",
             AVR,-1,0,ReadAT,"0x800,0x80,LOCK+FUSE+FUSE_H+FUSE_X+CAL+SLOW",0,WriteATmega,"0x800,0x80,16,SLOW",0);//2K, 128
   addDevice("ATtiny15",
             AVR,-1,0,ReadAT_HV,"0x400,0,LOCK+FUSE+CAL",0,WriteAT_HV,"0x400,0,0,0",0);					//1K, HV
   addDevice("ATtiny22",
             AVR,-1,0,ReadAT_HV,"0x400,0x40,LOCK+FUSE+CAL",0,WriteAT_HV,"0x400,0x40,0,0",0);				//1K, 64, HV
   addDevice("ATtiny24",
             AVR,-1,0,ReadAT_HV,"0x800,0x40,LOCK+FUSE+FUSE_H+CAL",0,WriteAT_HV,"0x400,0x40,16,0",0);		//1K, 64, HV
   addDevice("ATtiny25",
             AVR,-1,0,ReadAT,"0x800,0x80,LOCK+FUSE+FUSE_H+CAL+SLOW",0,WriteATmega,"0x800,0x80,16,SLOW",0);//2K, 128

   addDevice("ATtiny26",
             AVR,-1,0,ReadAT,"0x800,0x80,LOCK+FUSE+FUSE_H+CAL+SLOW",0,WriteATmega,"0x800,0x80,16,SLOW",0);//2K, 128
   addDevice("ATtiny261",
             AVR,-1,0,ReadAT,"0x800,0x80,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteATmega,"0x800,0x80,16,0",0);//2K, 128
   addDevice("ATtiny44,ATtiny45",
             AVR,-1,0,ReadAT_HV,"0x1000,0x100,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteAT_HV,"0x1000,0x100,32,0",0);//4K, 256, HV
   addDevice("ATtiny461,ATtiny4313,ATmega48",
             AVR,-1,0,ReadAT,"0x1000,0x100,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteATmega,"0x1000,0x100,32,0",0);//4K, 256
   addDevice("ATtiny48",
             AVR,-1,0,ReadAT,"0x1000,0x40,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteATmega,"0x1000,0x40,32,0",0);	//4K, 64
   addDevice("AT90S8515,AT90S8535",
             AVR,-1,0,ReadAT,"0x2000,0x100,0",0,WriteAT,"0x2000,0x100,-10,-10",0);							//8K, 256
   addDevice("ATmega8,ATmega8515,ATmega8535",
             AVR,-1,0,ReadAT,"0x2000,0x200,LOCK+FUSE+FUSE_H+CAL",0,WriteATmega,"0x2000,0x200,32,0",0);		//8K, 512
   addDevice("ATmega88,ATtiny861",
             AVR,-1,0,ReadAT,"0x2000,0x200,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteATmega,"0x2000,0x200,32,0",0);//8K, 512
   addDevice("ATtiny84,ATtiny85",
             AVR,-1,0,ReadAT_HV,"0x2000,0x200,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteAT_HV,"0x2000,0x200,32,0",0);//8K, 512, HV
   addDevice("ATtiny88",
             AVR,-1,0,ReadAT,"0x2000,0x40,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteATmega,"0x2000,0x40,32,0",0);	//8K, 64

   addDevice("ATmega16",
             AVR,-1,0,ReadAT,"0x4000,0x200,LOCK+FUSE+FUSE_H+CAL",0,WriteATmega,"0x4000,0x200,64,0",0);		//16K, 512
   addDevice("ATmega164A,ATmega168",
             AVR,-1,0,ReadAT,"0x4000,0x200,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteATmega,"0x4000,0x200,64,0",0);//16K, 512
   addDevice("ATmega32",
             AVR,-1,0,ReadAT,"0x8000,0x400,LOCK+FUSE+FUSE_H+CAL",0,WriteATmega,"0x8000,0x400,64,0",0);		//32K, 1K
   addDevice("ATmega324A,ATmega328",
             AVR,-1,0,ReadAT,"0x8000,0x400,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteATmega,"0x8000,0x400,64,0",0);//32K, 1K
   addDevice("ATmega64,ATmega644A",
             AVR,-1,0,ReadAT,"0x10000,0x800,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteATmega,"0x10000,0x800,128,0",0);//64K, 2K
   addDevice("ATmega128",
             AVR,-1,0,ReadAT,"0x20000,0x1000,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteATmega,"0x20000,0x1000,128,0",0);//128K, 4K
   addDevice("ATmega256",
             AVR,-1,0,ReadAT,"0x40000,0x1000,LOCK+FUSE+FUSE_H+FUSE_X+CAL",0,WriteATmega,"0x40000,0x1000,128,0",0);//256K, 4K
   //-------------I2C---------------------------------------------------------
   addDevice("2400",
             I2CEE,-1,0,ReadI2C,"0x10,0",0,WriteI2C,"0x10,0,1",0);				//16, 1B addr.
   addDevice("2401",
             I2CEE,-1,0,ReadI2C,"0x80,0",0,WriteI2C,"0x80,0,8",0);				//128, 1B addr.
   addDevice("2402",
             I2CEE,-1,0,ReadI2C,"0x100,0",0,WriteI2C,"0x100,0,8",0);				//256, 1B addr.
   addDevice("2404",
             I2CEE,-1,0,ReadI2C,"0x200,0",0,WriteI2C,"0x200,0,16",0);			//512, 1B addr.
   addDevice("2408",
             I2CEE,-1,0,ReadI2C,"0x400,0",0,WriteI2C,"0x400,0,16",0);			//1K, 1B addr.
   addDevice("2416",
             I2CEE,-1,0,ReadI2C,"0x800,0",0,WriteI2C,"0x800,0,16",0);			//2K, 1B addr.
   addDevice("2432",
             I2CEE,-1,0,ReadI2C,"0x1000,1",0,WriteI2C,"0x1000,1,32",0);			//4K, 2B addr.
   addDevice("2464",
             I2CEE,-1,0,ReadI2C,"0x2000,1",0,WriteI2C,"0x2000,1,32",0);			//8K, 2B addr.
   addDevice("24128",
             I2CEE,-1,0,ReadI2C,"0x4000,1",0,WriteI2C,"0x4000,1,64",0);			//16K, 2B addr.
   addDevice("24256",
             I2CEE,-1,0,ReadI2C,"0x8000,1",0,WriteI2C,"0x8000,1,64",0);			//32K, 2B addr.
   addDevice("24512",
             I2CEE,-1,0,ReadI2C,"0x10000,1",0,WriteI2C,"0x10000,1,128",0);		//64K, 2B addr.
   addDevice("241024",
             I2CEE,-1,0,ReadI2C,"0x20000,0x201",0,WriteI2C,"0x20000,0x201,256",0);//128K, 2B addr.
   addDevice("241025",
             I2CEE,-1,0,ReadI2C,"0x20000,0x841",0,WriteI2C,"0x20000,0x841,128",0);//128K, 2B addr.
   //-------------Microwire EEPROM---------------------------------------------------------
   addDevice("93S46",
             UWEE,-1,0,Read93x,"0x80,6,0",0,Write93Sx,"0x80,6,8",0);				//128, 6b addr, 4W page
   addDevice("93x46",
             UWEE,-1,0,Read93x,"0x80,6,0",0,Write93Cx,"0x80,6,0",0);		//128, 6b addr
   addDevice("93x46A",
             UWEE,-1,0,Read93x,"0x80,7,1",0,Write93Cx,"0x80,7,1",0);		//128, 6b addr x8
   addDevice("93S56",
             UWEE,-1,0,Read93x,"0x100,8,0",0,Write93Sx,"0x100,8,8",0);	//256, 8b addr, 4W page
   addDevice("93x56",
             UWEE,-1,0,Read93x,"0x100,8,0",0,Write93Cx,"0x100,8,0",0);	//256, 8b addr
   addDevice("93x56A",
             UWEE,-1,0,Read93x,"0x100,9,1",0,Write93Cx,"0x100,9,1",0);	//256, 8b addr x8
   addDevice("93S66",
             UWEE,-1,0,Read93x,"0x200,8,0",0,Write93Sx,"0x200,8,8",0);	//512, 8b addr, 4W page
   addDevice("93x66",
             UWEE,-1,0,Read93x,"0x200,8,0",0,Write93Cx,"0x200,8,0",0);	//512, 8b addr
   addDevice("93x66A",
             UWEE,-1,0,Read93x,"0x200,9,1",0,Write93Cx,"0x200,9,1",0);	//512, 8b addr x8
   addDevice("93x76",
             UWEE,-1,0,Read93x,"0x400,10,0",0,Write93Cx,"0x400,10,0",0);	//1k, 10b addr
   addDevice("93x76A",
             UWEE,-1,0,Read93x,"0x400,11,1",0,Write93Cx,"0x400,11,1",0);	//1k, 10b addr x8
   addDevice("93x86",
             UWEE,-1,0,Read93x,"0x800,10,0",0,Write93Cx,"0x800,10,0",0);	//2k, 10b addr
   addDevice("93x86A",
             UWEE,-1,0,Read93x,"0x800,11,1",0,Write93Cx,"0x800,11,1",0);	//2k, 10b addr x8
   //-------------SPI---------------------------------------------------------
   addDevice("25010,95010",
             SPIEE,-1,0,Read25xx,"0x80",0,Write25xx,"0x80,16",0);		//128
   addDevice("25020,95020",
             SPIEE,-1,0,Read25xx,"0x100",0,Write25xx,"0x100,16",0);		//256
   addDevice("25040,95040",
             SPIEE,-1,0,Read25xx,"0x200",0,Write25xx,"0x200,16",0);		//512
   addDevice("25080,95080",
             SPIEE,-1,0,Read25xx,"0x400",0,Write25xx,"0x400,16",0);		//1K
   addDevice("25160,95160",
             SPIEE,-1,0,Read25xx,"0x800",0,Write25xx,"0x800,16",0);		//2K
   addDevice("25320,95320",
             SPIEE,-1,0,Read25xx,"0x1000",0,Write25xx,"0x1000,32",0);	//4K
   addDevice("25640,95640",
             SPIEE,-1,0,Read25xx,"0x2000",0,Write25xx,"0x2000,32",0);	//8K
   addDevice("25128,95128",
             SPIEE,-1,0,Read25xx,"0x4000",0,Write25xx,"0x4000,64",0);	//16K
   addDevice("25256,95256",
             SPIEE,-1,0,Read25xx,"0x8000",0,Write25xx,"0x8000,64",0);	//32K
   addDevice("25512,95512",
             SPIEE,-1,0,Read25xx,"0x10000",0,Write25xx,"0x10000,128",0);	//64K
   addDevice("251024,95M01",
             SPIEE,-1,0,Read25xx,"0x20000",0,Write25xx,"0x20000,256",0);	//128K
   addDevice("95M02",
             SPIEE,-1,0,Read25xx,"0x40000",0,Write25xx,"0x40000,256",0);	//256K
   addDevice("25X05",
             SPIEE,-1,1,Read25xx,"0x10000",0,Write25xx,"0x10000,0x1000+256",0);//64K flash 3.3V
   addDevice("251005,25X10",
             SPIEE,-1,1,Read25xx,"0x20000",0,Write25xx,"0x20000,0x1000+256",0);//128K flash 3.3V
   addDevice("252005,25X20",
             SPIEE,-1,1,Read25xx,"0x40000",0,Write25xx,"0x40000,0x1000+256",0);//256K flash 3.3V
   addDevice("254005,25X40",
             SPIEE,-1,1,Read25xx,"0x80000",0,Write25xx,"0x80000,0x1000+256",0);//512K flash 3.3V
   addDevice("258005,25X80",
             SPIEE,-1,1,Read25xx,"0x100000",0,Write25xx,"0x100000,0x1000+256",0);//1M flash 3.3V
   addDevice("251605,25X16",
             SPIEE,-1,1,Read25xx,"0x200000",0,Write25xx,"0x200000,0x1000+256",0);//2M flash 3.3V
   addDevice("25X32",
             SPIEE,-1,1,Read25xx,"0x400000",0,Write25xx,"0x400000,0x1000+256",0);//4M flash 3.3V
   addDevice("25X64",
             SPIEE,-1,1,Read25xx,"0x800000",0,Write25xx,"0x800000,0x1000+256",0);//8M flash 3.3V
   //-------------One wire devices---------------------------------------------------------
   addDevice("DS2430",
             OWEE,-1,0,ReadOneWireMem,"0x20,1",0,WriteOneWireMem,"0x20,1",0);	//32
   addDevice("DS2431",
             OWEE,-1,0,ReadOneWireMem,"0x80,2",0,WriteOneWireMem,"0x80,0",0);	//128
   addDevice("DS2433",
             OWEE,-1,0,ReadOneWireMem,"0x200,0",0,WriteOneWireMem,"0x200,1",0);	//512
   addDevice("DS28EC20",
             OWEE,-1,0,ReadOneWireMem,"0xA00,2",0,WriteOneWireMem,"0xA00,1",0);	//2560
   addDevice("DS1820",
             OWEE,-1,0,ReadDS1820,"-10",0,0,"0",0);	//digital thermometer
   //-------------UNIO devices---------------------------------------------------------
   addDevice("11010",
             UNIOEE,-1,0,Read11xx,"0x80",0,Write11xx,"0x80,16",0);	//128
   addDevice("11020",
             UNIOEE,-1,0,Read11xx,"0x100",0,Write11xx,"0x100,16",0);	//256
   addDevice("11040",
             UNIOEE,-1,0,Read11xx,"0x200",0,Write11xx,"0x200,16",0);	//512
   addDevice("11080",
             UNIOEE,-1,0,Read11xx,"0x400",0,Write11xx,"0x400,16",0);	//1k
   addDevice("11160",
             UNIOEE,-1,0,Read11xx,"0x800",0,Write11xx,"0x800,16",0);		//2k


   //qSort(allDevices.begin(), allDevices.end(), deviceInfoLessThan);
   quickSort(&allDevices, 0, allDevices.length()-1);
   foreach (DeviceFamily *family, deviceFamilyList)
      foreach (DeviceType *type, family->deviceTypesList)
         quickSort(&type->devicesList, 0, type->devicesList.length()-1);
}


void Devices::quickSort(QList<DeviceInfo*> *devices, int l, int r)
{
   if(l<r)
   {
      int i, j;
      DeviceInfo *x, *y;

      i = l;
      j = r;
      x = devices->at((l+r) >> 1);// = (l+r)>>1 is (l+r)/2
      do
      {
         for (; devices->at(i)->name < x->name;i++);
         for (; x->name < devices->at(j)->name; j--);
         if (i <= j)
         {
            y = devices->at(i);
            devices->replace(i, devices->at(j));
            devices->replace(j, y);
            i++;
            j--;
         }
      }while(i <= j);
      if (l < j)
         quickSort(devices, l, j);
      if (i < r)
         quickSort(devices, i, r);
   }
}

bool Devices::deviceInfoLessThan(const DeviceInfo &d1, const DeviceInfo &d2)
{
   return d1.name < d2.name; // sort by name
}

bool Devices::deviceNameLessThan(const QString &deviceName, const DeviceInfo &d1)
{
   return d1.name.toLower() < deviceName.toLower();
}




int DeviceInfo::bitLength()
{
   QString devType=name.mid(0,2);
   int length=0;
   if (devType=="AT")
      length=8;
   else if (devType=="10")
      length=12;
   else
      if (devType=="12" ||devType=="16" )
         length=14;
      else
         if (devType=="18")
            length=16;
         else
            if (devType=="24" ||devType=="30"||devType=="33")
               length=24;
   return length;
}
