#include "fuseByte.h"
#include "Libs/databaseLayer.h"
#include "Libs/globalVariables.h"

FuseByte::FuseByte()
{

}

FuseByte::FuseByte(quint32 fuseByteID)
{
   loadFromDB(fuseByteID);
}

FuseByte::FuseByte(const QSqlRecord &record)
{
   loadFromDB(record);
}

quint32 FuseByte::saveToDB(quint32 picDeviceID)
{
   DatabaseLayer *db = GlobalVariables::db;
   SqlParameters parameters;
   qDebug()<<"picDeviceID"<<picDeviceID;

   parameters.clear();
   parameters.append(SqlParameter("PicDeviceID", picDeviceID));
   parameters.append(SqlParameter("Name", name));
   parameters.append(SqlParameter("Value", value));
   parameters.append(SqlParameter("Mask", mask));
   parameters.append(SqlParameter("Defualt", defualt));
   qint64 fuseByteID=db->insert("FuseBytes", parameters, "FuseByteID");
   qDebug()<<"fuseByteID"<<fuseByteID;
   for (int j = 0; j < bitsInfo.length(); ++j)
   {
      parameters.clear();
      parameters.append(SqlParameter("FuseByteID", fuseByteID));
      parameters.append(SqlParameter("BitNumber", bitsInfo[j].bitNumber));
      parameters.append(SqlParameter("BitName", bitsInfo[j].bitName));
      parameters.append(SqlParameter("Description", bitsInfo[j].description));
      db->insert("BitsInfo", parameters);
   }
   for (int j = 0; j < navigationGroups.length(); ++j)
   {
      parameters.clear();
      parameters.append(SqlParameter("FuseByteID", fuseByteID));
      parameters.append(SqlParameter("Name", navigationGroups[j].name));
      parameters.append(SqlParameter("Mask", navigationGroups[j].mask));
      qint64  fuseNavigationGroupID=db->insert("FuseNavigationGroups", parameters, "FuseNavigationGroupID");
      for (int k = 0; k < navigationGroups[j].members.length(); k++)
      {
         parameters.clear();
         parameters.append(SqlParameter("FuseNavigationGroupID", fuseNavigationGroupID));
         parameters.append(SqlParameter("Value", navigationGroups[j].members[k].value));
         parameters.append(SqlParameter("Text", navigationGroups[j].members[k].text));
         db->insert("FuseNavigations", parameters);
      }
   }
   return fuseByteID;
}

void FuseByte::loadFromDB(const QSqlRecord &record)
{
   DatabaseLayer *db = GlobalVariables::db;
   quint32 fuseByteID  = record.value("FuseByteID").toUInt();
   name     = record.value("Name").toString();
   value    = record.value("Value").toInt();
   mask     = record.value("Mask").toInt();
   defualt  = record.value("Defualt").toInt();
   QSqlQueryModel *tblFuseNavigationGroups=db->filter("FuseNavigationGroups", SqlParameters()<<SqlParameter("FuseByteID", fuseByteID));
   for (int i = 0; i < tblFuseNavigationGroups->rowCount(); ++i)
   {
      FuseNavigationGroup fuseNavGroup;
      fuseNavGroup.name = tblFuseNavigationGroups->record(i).value("Name").toString();
      fuseNavGroup.mask = tblFuseNavigationGroups->record(i).value("Mask").toInt();
      quint32 fuseNavigationGroupID = tblFuseNavigationGroups->record(i).value("FuseNavigationGroupID").toInt();
      QSqlQueryModel *tblFuseNavigations=db->filter("FuseNavigations", SqlParameters()<<SqlParameter("FuseNavigationGroupID", fuseNavigationGroupID));
      for (int j = 0; j < tblFuseNavigations->rowCount(); ++j)
      {
         FuseNavigation fuseNav;
         fuseNav.value = tblFuseNavigations->record(j).value("Value").toInt();
         fuseNav.text  = tblFuseNavigations->record(j).value("Text").toString();
         fuseNavGroup.members.append(fuseNav);
      }
      navigationGroups.append(fuseNavGroup);
   }
   QSqlQueryModel *tblBitsInfo=db->filter("BitsInfo", SqlParameters()<<SqlParameter("FuseByteID", fuseByteID));
   for (int i = 0; i < tblBitsInfo->rowCount(); ++i)
   {
      BitInfo bitInfo;
      bitInfo.bitNumber=tblBitsInfo->record(i).value("BitNumber").toInt();
      bitInfo.bitName=tblBitsInfo->record(i).value("BitName").toString();
      bitInfo.description=tblBitsInfo->record(i).value("Description").toString();
      bitsInfo.append(bitInfo);
   }
}

void FuseByte::loadFromDB(quint32 fuseByteID)
{
   DatabaseLayer *db = GlobalVariables::db;
   QSqlQueryModel *tblFuseBytes=db->filter("FuseBytes", SqlParameters()<<SqlParameter("FuseByteID", fuseByteID));
   if(tblFuseBytes->rowCount()>0)
   {
      loadFromDB(tblFuseBytes->record(0));
   }
}
