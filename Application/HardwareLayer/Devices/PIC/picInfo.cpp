#include "picInfo.h"
#include "Libs/globalVariables.h"

//***********************************************
//********** PicInfo   functions  ***************
//***********************************************
/*PicInfo::PicInfo(QObject *parent):QObject(parent)
{
}
*/

PicInfo::PicInfo(QString name)
{
   _finded=loadFromDB(name);
}

PicInfo::PicInfo()
{

}

PicInfo::~PicInfo()
{

}

void PicInfo::saveToDB()
{
   DatabaseLayer *db= GlobalVariables::db;
   QSqlQueryModel *model=db->filter("PicDevices",  SqlParameters()<<SqlParameter("name", deviceTag.name));
   quint64 picDeviceID=0;
   if(model->rowCount()>0)
      picDeviceID=model->record(0).value("PicDeviceID").toInt();
   qint16 status           = db->dictionaryValueId("PicStatuses", deviceTag.status);
   qint16 memoryTechnology = db->dictionaryValueId("PicMemoryTechnologies", deviceTag.memoryTechnology);
   qint16 architecture     = db->dictionaryValueId("PicArchitectures", deviceTag.architecture);
   qint16 selfWrite        = db->dictionaryValueId("PicSelfWrites", deviceTag.selfWrite);

   SqlParameters parameters=SqlParameters()
                            <<SqlParameter("Name", deviceTag.name)
                           <<SqlParameter("HardwareID", deviceTag.ID)
                          <<SqlParameter("Status", status)
                         <<SqlParameter("MemoryTechnology", memoryTechnology)
                        <<SqlParameter("Architecture", architecture)
                       <<SqlParameter("SelfWrite", selfWrite)
                      <<SqlParameter("IDHighVoltage", deviceTag.IDHighVoltage)
                     <<SqlParameter("IDLowPower", deviceTag.IDLowPower)
                    <<SqlParameter("Alternative", deviceTag.alternative)
                   <<SqlParameter("Document", deviceTag.document)
                  <<SqlParameter("Pc", deviceTag.pc);
   if(!picDeviceID)
   {
      //<device>
      picDeviceID=db->insert("PicDevices", parameters, "PicDeviceID");
      //<checksums>
      for (int i = 0; i < checksums.length(); ++i)
      {
         qint16 protectedTypeID=0;
         qint16 typeID=-1;
         if(checksums[i].protectedType!="")
            protectedTypeID = db->dictionaryValueId("PicChecksumProtectedTypes", checksums[i].protectedType);
         if(checksums[i].type!="")
            typeID = db->dictionaryValueId("PicChecksumTypes", checksums[i].type);
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("ProtectedTypeID", protectedTypeID));
         parameters.append(SqlParameter("ProtectedStart", checksums[i].start));
         parameters.append(SqlParameter("ProtectedEnd", checksums[i].end));
         parameters.append(SqlParameter("Constant", checksums[i].constant));
         parameters.append(SqlParameter("bchecksum", checksums[i].bchecksum));
         parameters.append(SqlParameter("cchecksum", checksums[i].cchecksum));
         parameters.append(SqlParameter("TypeID", typeID));
         parameters.append(SqlParameter("ProtectedBlocks", checksums[i].protectedBlocks));
         db->insert("PicChecksums", parameters);
      }

      for (int i = 0; i < frequencyRanges.length(); ++i)
      {
         qint16 nameID     = db->dictionaryValueId("PicFrequencyRangeNames", frequencyRanges[i].name);
         qint16 specialID  = db->dictionaryValueId("PicFrequencyRangeSpecials", frequencyRanges[i].special);

         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("NameID", nameID));
         parameters.append(SqlParameter("SpecialID", specialID));
         qint64 frequencyRangeID=db->insert("PicFrequencyRanges", parameters, "FrequencyRangeID");
         for (int j = 0; j < frequencyRanges[i].frequencies.length(); ++j)
         {
            qint16 oscID = db->dictionaryValueId("PicFrequencyOSCs", frequencyRanges[i].frequencies[j].osc);
            parameters.clear();
            parameters.append(SqlParameter("FrequencyRangeID", frequencyRangeID));
            parameters.append(SqlParameter("Start", frequencyRanges[i].frequencies[j].start));
            parameters.append(SqlParameter("End", frequencyRanges[i].frequencies[j].end));
            parameters.append(SqlParameter("VddMin", frequencyRanges[i].frequencies[j].vddMin));
            parameters.append(SqlParameter("VddMax", frequencyRanges[i].frequencies[j].vddMax));
            parameters.append(SqlParameter("VddMinEnd", frequencyRanges[i].frequencies[j].vddMinEnd));
            parameters.append(SqlParameter("OscID", oscID));
            db->insert("PicFrequencies", parameters);
         }
      }

      for (int i = 0; i < voltages.length(); ++i)
      {
         qint16 voltageNameID = db->dictionaryValueId("PicVoltageNames", voltages[i].name);
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("voltageNameID", voltageNameID));
         parameters.append(SqlParameter("MinVoltage", voltages[i].min));
         parameters.append(SqlParameter("MaxVoltage", voltages[i].max));
         parameters.append(SqlParameter("NominalVoltage", voltages[i].nominal));
         db->insert("PicVoltages", parameters);
      }
      if(memoriesInfo.calibration)
      {
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("Start", memoriesInfo.calibration->start));
         parameters.append(SqlParameter("End", memoriesInfo.calibration->end));
         parameters.append(SqlParameter("CalOpmask", memoriesInfo.calibration->calOpmask));
         parameters.append(SqlParameter("CalOpcode", memoriesInfo.calibration->calOpcode));
         db->insert("PicMemoryCalibrations", parameters);
      }
      if(memoriesInfo.calibrationBackup)
      {
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("Start", memoriesInfo.calibrationBackup->start));
         parameters.append(SqlParameter("End", memoriesInfo.calibrationBackup->end));
         db->insert("PicMemoryCalibrationBackups", parameters);
      }
      if(memoriesInfo.code)
      {
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("Start", memoriesInfo.code->start));
         parameters.append(SqlParameter("End", memoriesInfo.code->end));
         parameters.append(SqlParameter("WordWriteAlign", memoriesInfo.code->wordWriteAlign));
         parameters.append(SqlParameter("WordEraseAlign", memoriesInfo.code->wordEraseAlign));
         db->insert("PicMemoryCodes", parameters);
      }
      if(memoriesInfo.config)
      {
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("Start", memoriesInfo.config->start));
         parameters.append(SqlParameter("End", memoriesInfo.config->end));
         parameters.append(SqlParameter("HexfileOffset", memoriesInfo.config->hexfileOffset));
         db->insert("PicMemoryConfigs", parameters);
      }
      if(memoriesInfo.debugVector)
      {
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("Start", memoriesInfo.debugVector->start));
         parameters.append(SqlParameter("End", memoriesInfo.debugVector->end));
         db->insert("PicMemoryDebugVectors", parameters);
      }
      if(memoriesInfo.deviceID)
      {
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("Start", memoriesInfo.deviceID->start));
         parameters.append(SqlParameter("End", memoriesInfo.deviceID->end));
         db->insert("PicMemoryDeviceIDs", parameters);
      }
      if(memoriesInfo.eeprom)
      {
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("Start", memoriesInfo.eeprom->start));
         parameters.append(SqlParameter("End", memoriesInfo.eeprom->end));
         parameters.append(SqlParameter("RMask", memoriesInfo.eeprom->rmask));
         parameters.append(SqlParameter("HexfileOffset", memoriesInfo.eeprom->hexfileOffset));
         db->insert("PicMemoryEEProms", parameters);
      }
      if(memoriesInfo.programExecutive)
      {
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("Start", memoriesInfo.programExecutive->start));
         parameters.append(SqlParameter("End", memoriesInfo.programExecutive->end));
         db->insert("PicMemoryProgramExecutives", parameters);
      }
      if(memoriesInfo.userIDs)
      {
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         parameters.append(SqlParameter("Start", memoriesInfo.userIDs->start));
         parameters.append(SqlParameter("End", memoriesInfo.userIDs->end));
         parameters.append(SqlParameter("RMask", memoriesInfo.userIDs->rmask));
         db->insert("PicMemoryUserIds", parameters);
      }


      for (int i = 0; i < fuseBytes.length(); ++i)
         fuseBytes[i].saveToDB(picDeviceID);

      for (int i = 0; i < packages.length(); ++i)
      {
         parameters.clear();
         parameters.append(SqlParameter("PicDeviceID", picDeviceID));
         qint64 picPackageID=db->insert("PicPackages", parameters, "PicPackageID");

         for (int j = 0; j < packages[i].typeNames.length(); ++j)
         {
            qint16 picPackageTypeID = db->dictionaryValueId("PicPackageTypes", packages[i].typeNames[j]);
            parameters.clear();
            parameters.append(SqlParameter("PicPackageID ", picPackageID ));
            parameters.append(SqlParameter("PicPackageTypeID ", picPackageTypeID ));
            db->insert("PicPackageTypes", parameters);
         }
         for (int j = 0; j < packages[i].pinNames.length(); ++j)
         {
            parameters.clear();
            parameters.append(SqlParameter("PicPackageID ", picPackageID ));
            parameters.append(SqlParameter("PinName ", packages[i].pinNames[j]));
            db->insert("PicPackagePins", parameters);
         }
      }

   }
   else
      db->update("PicDevices", parameters, SqlParameters()<<SqlParameter("PicDeviceID",picDeviceID));

}


bool PicInfo::loadFromDB(QString name)
{
   DatabaseLayer *db= GlobalVariables::db;
   QSqlQueryModel *tblPicDevices=db->filter("PicDevices",  SqlParameters()<<SqlParameter("Name", name));
   QSqlQueryModel *tblTemp = NULL;
   quint64 picDeviceID=0;
   if(tblPicDevices->rowCount()>0)
   {
      picDeviceID                = tblPicDevices->record(0).value("PicDeviceID").toInt();
      qDebug()<< "tblPicDevices"<<tblPicDevices->record(0).value("Name").toString();
      deviceTag.name             = tblPicDevices->record(0).value("Name").toString();;
      deviceTag.ID               = tblPicDevices->record(0).value("HardwareID").toInt();
      deviceTag.status           = db->dictionaryValueString(1, tblPicDevices->record(0).value("Status").toInt());
      deviceTag.memoryTechnology = db->dictionaryValueString(2, tblPicDevices->record(0).value("MemoryTechnology").toInt());
      deviceTag.architecture     = db->dictionaryValueString(3, tblPicDevices->record(0).value("Architecture").toInt());
      deviceTag.selfWrite        = db->dictionaryValueString(4, tblPicDevices->record(0).value("SelfWrite").toInt());


      delete tblTemp;
      tblTemp = db->filter("PicChecksums", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      for (int i = 0; i < tblTemp->rowCount(); ++i)
      {
         Checksum checksum;
         if(tblTemp->record(i).value("ProtectedTypeID").toInt()>0)
            checksum.protectedType  = db->dictionaryValueString(5, tblTemp->record(i).value("ProtectedTypeID").toInt());
         checksum.start             = tblTemp->record(i).value("ProtectedStart").toInt();
         checksum.end               = tblTemp->record(i).value("ProtectedEnd").toInt();
         checksum.constant          = tblTemp->record(i).value("Constant").toInt();
         checksum.bchecksum         = tblTemp->record(i).value("bchecksum").toInt();
         checksum.cchecksum         = tblTemp->record(i).value("cchecksum").toInt();
         if(tblTemp->record(i).value("TypeID").toInt()>0)
            checksum.type           = db->dictionaryValueString(6, tblTemp->record(i).value("TypeID").toInt());
         checksum.protectedBlocks   = tblTemp->record(i).value("ProtectedBlocks").toInt();
         checksums.append(checksum);
      }

      delete tblTemp;
      tblTemp = db->filter("PicFrequencyRanges", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      for (int i = 0; i < tblTemp->rowCount(); ++i)
      {
         FrequencyRange frqRange;
         if(tblTemp->record(i).value("NameID").toInt()>0)
            frqRange.name         = db->dictionaryValueString(7, tblTemp->record(i).value("NameID").toInt());
         if(tblTemp->record(i).value("SpecialID").toInt()>0)
            frqRange.special      = db->dictionaryValueString(8, tblTemp->record(i).value("SpecialID").toInt());
         QSqlQueryModel *tblPicFrequencies = db->filter("PicFrequencies", SqlParameters()<<SqlParameter("FrequencyRangeID",  tblTemp->record(i).value("FrequencyRangeID").toInt()));
         for (int j = 0; j < tblPicFrequencies->rowCount(); ++j)
         {
            Frequency frequency;
            frequency.start      = tblPicFrequencies->record(j).value("Start").toFloat();
            frequency.end        = tblPicFrequencies->record(j).value("End").toFloat();
            frequency.vddMin     = tblPicFrequencies->record(j).value("VddMin").toFloat();
            frequency.vddMax     = tblPicFrequencies->record(j).value("VddMax").toFloat();
            frequency.vddMinEnd  = tblPicFrequencies->record(j).value("VddMinEnd").toFloat();
            if(tblPicFrequencies->record(j).value("OscID").toInt()>0)
               frequency.osc     = db->dictionaryValueString(9, tblPicFrequencies->record(j).value("OscID").toInt());
            frqRange.frequencies.append(frequency);
         }
         frequencyRanges.append(frqRange);
      }

      delete tblTemp;
      tblTemp = db->filter("PicVoltages", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      for (int i = 0; i < tblTemp->rowCount(); ++i)
      {
         Voltage voltag;
         if(tblTemp->record(i).value("VoltageNameID").toInt()>0)
            voltag.name = db->dictionaryValueString(10, tblTemp->record(i).value("VoltageNameID").toInt());
         voltag.min     = tblTemp->record(i).value("MinVoltage").toFloat();
         voltag.max     = tblTemp->record(i).value("MaxVoltage").toFloat();
         voltag.nominal = tblTemp->record(i).value("NominalVoltage").toFloat();
         voltages.append(voltag);
      }

      delete tblTemp;
      tblTemp = db->filter("PicMemoryCodes", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      if(tblTemp->rowCount()>0)
      {
         memoriesInfo.code = new CodeMemory();
         memoriesInfo.code->start         = tblTemp->record(0).value("Start").toInt();
         memoriesInfo.code->end           = tblTemp->record(0).value("End").toInt();
         memoriesInfo.code->wordWriteAlign= tblTemp->record(0).value("WordWriteAlign").toInt();
         memoriesInfo.code->wordEraseAlign= tblTemp->record(0).value("WordEraseAlign").toInt();
      }

      delete tblTemp;
      tblTemp = db->filter("PicMemoryCalibrations", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      if(tblTemp->rowCount()>0)
      {
         memoriesInfo.calibration = new CalibrationMemory();
         memoriesInfo.calibration->start     = tblTemp->record(0).value("Start").toInt();
         memoriesInfo.calibration->end       = tblTemp->record(0).value("End").toInt();
         memoriesInfo.calibration->calOpmask = tblTemp->record(0).value("CalOpmask").toInt();
         memoriesInfo.calibration->calOpcode = tblTemp->record(0).value("CalOpcode").toInt();
      }

      delete tblTemp;
      tblTemp = db->filter("PicMemoryUserIds", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      if(tblTemp->rowCount()>0)
      {
         memoriesInfo.userIDs = new UserIdsMemory();
         memoriesInfo.userIDs->start   = tblTemp->record(0).value("Start").toInt();
         memoriesInfo.userIDs->end     = tblTemp->record(0).value("End").toInt();
         memoriesInfo.userIDs->rmask   = tblTemp->record(0).value("RMask").toInt();
      }

      delete tblTemp;
      tblTemp = db->filter("PicMemoryDeviceIDs", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      if(tblTemp->rowCount()>0)
      {
         memoriesInfo.deviceID = new Memory();
         memoriesInfo.deviceID->start   = tblTemp->record(0).value("Start").toInt();
         memoriesInfo.deviceID->end     = tblTemp->record(0).value("End").toInt();
      }

      delete tblTemp;
      tblTemp = db->filter("PicMemoryConfigs", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      if(tblTemp->rowCount()>0)
      {
         memoriesInfo.config = new ConfigMemory();
         memoriesInfo.config->start          = tblTemp->record(0).value("Start").toInt();
         memoriesInfo.config->end            = tblTemp->record(0).value("End").toInt();
         memoriesInfo.config->hexfileOffset  = tblTemp->record(0).value("HexfileOffset").toInt();
      }

      delete tblTemp;
      tblTemp = db->filter("PicMemoryEEProms", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      if(tblTemp->rowCount()>0)
      {
         memoriesInfo.eeprom = new EEPromMemory();
         memoriesInfo.eeprom->start          = tblTemp->record(0).value("Start").toInt();
         memoriesInfo.eeprom->end            = tblTemp->record(0).value("End").toInt();
         memoriesInfo.eeprom->rmask          = tblTemp->record(0).value("RMask").toInt();
         memoriesInfo.eeprom->hexfileOffset  = tblTemp->record(0).value("HexfileOffset").toInt();
      }


      delete tblTemp;
      tblTemp = db->filter("PicMemoryDebugVectors", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      if(tblTemp->rowCount()>0)
      {
         memoriesInfo.debugVector = new Memory();
         memoriesInfo.debugVector->start          = tblTemp->record(0).value("Start").toInt();
         memoriesInfo.debugVector->end            = tblTemp->record(0).value("End").toInt();
      }

      delete tblTemp;
      tblTemp = db->filter("PicMemoryCalibrationBackups", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      if(tblTemp->rowCount()>0)
      {
         memoriesInfo.calibrationBackup = new Memory();
         memoriesInfo.calibrationBackup->start          = tblTemp->record(0).value("Start").toInt();
         memoriesInfo.calibrationBackup->end            = tblTemp->record(0).value("End").toInt();
      }

      delete tblTemp;
      tblTemp = db->filter("PicMemoryProgramExecutives", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      if(tblTemp->rowCount()>0)
      {
         memoriesInfo.programExecutive = new Memory();
         memoriesInfo.programExecutive->start          = tblTemp->record(0).value("Start").toInt();
         memoriesInfo.programExecutive->end            = tblTemp->record(0).value("End").toInt();
      }


      QSqlQueryModel *tblFuseBytes=db->filter("FuseBytes",  SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      for (int i = 0; i < tblFuseBytes->rowCount(); ++i)
      {
         FuseByte fuseByte(tblFuseBytes->record(i));
         fuseBytes.append(fuseByte);
      }

      delete tblTemp;
      tblTemp = db->filter("PicPackages", SqlParameters()<<SqlParameter("PicDeviceID", picDeviceID));
      for (int i = 0; i < tblTemp->rowCount(); ++i)
      {
         Package package;
         QSqlQueryModel *tblPicPackageTypes = db->filter("PicPackageTypes", SqlParameters()<<SqlParameter("PicPackageID",  tblTemp->record(i).value("PicPackageID").toInt()));
         for (int j = 0; j < tblPicPackageTypes->rowCount(); ++j)
            package.typeNames.append(db->dictionaryValueString(11, tblPicPackageTypes->record(j).value("PicPackageTypeID").toInt()));

         QSqlQueryModel *tblPicPackagePins = db->filter("PicPackagePins", SqlParameters()<<SqlParameter("PicPackageID",  tblTemp->record(i).value("PicPackageID").toInt()));
         for (int j = 0; j < tblPicPackagePins->rowCount(); ++j)
            package.pinNames.append(tblPicPackagePins->record(j).value("PinName").toString());
         packages.append(package);
      }
      return true;
   }
   return false;
}

bool PicInfo::finded()
{
   return _finded;
}


void Memory::saveToMemCODE_W(unsigned char* fromMemory)
{
   if(isValid)
   {
      for(quint16 i=start;i<=end;i++)
      {
         quint16 index =i-start;
         GlobalVariables::usb->memCODE_W[i] = (fromMemory[index*2+1]<<8) + fromMemory[index*2];
      }
   }
}

void Memory::loadFromMemCODE_W(unsigned char *toMemory)
{
   if(isValid)
   {
      for(quint16 i=start;i<=end;i++)
      {
         quint16 index=i-start;
         toMemory[index*2]   =  GlobalVariables::usb->memCODE_W[i]     & 0x00FF;
         toMemory[index*2+1] = (GlobalVariables::usb->memCODE_W[i]>>8) & 0x00FF;
      }
   }

}

quint64 Memory::size()
{
   if(start==0 && end==0)
      return 0;
   return end-start+1;
}
