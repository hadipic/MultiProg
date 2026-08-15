#include "avrFuse.h"
#include <Libs/databaseLayer.h>
#include <Libs/globalVariables.h>

AvrFuse::AvrFuse()
{
   low.defualt=high.defualt=extend.defualt=lock.defualt=0;
}

AvrFuse::AvrFuse(QString avrName)
{
   loadFromDB(avrName);
}

void AvrFuse::loadFromDB(QString avrName)
{
   DatabaseLayer *db = GlobalVariables::db;
   QSqlQueryModel *tblAvrDevices = db->filter("AvrDevices", SqlParameters()<<SqlParameter("Name", avrName));
   if(tblAvrDevices->rowCount()>0)
   {
      qint32 id=tblAvrDevices->record(0).value("LowFuseByteID").toInt();
      low.loadFromDB(id);
      id=tblAvrDevices->record(0).value("HighFuseByteID").toInt();
      high.loadFromDB(id);
      id=tblAvrDevices->record(0).value("ExtendFuseByteID").toInt();
      extend.loadFromDB(id);
      id=tblAvrDevices->record(0).value("LockFuseByteID").toInt();
      lock.loadFromDB(id);
   }
   //SqlParameters parameters;
   //qDebug()<<"picDeviceID"<<picDeviceID;

}
