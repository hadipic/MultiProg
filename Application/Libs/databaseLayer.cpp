#include "databaseLayer.h"
#include <QDebug>

DatabaseLayer::DatabaseLayer(QString driverName, QString databaseName, QObject *parent) :
   QObject(parent)
{
   db = QSqlDatabase::addDatabase(driverName);
   db.setDatabaseName(databaseName);
   //qDebug()<<driverName<<databaseName;
   db.open();
   /*insert("", QList<SqlParameter>()
          <<SqlParameter("",1)
          );
   */
}

QSqlQuery DatabaseLayer::exec(const QString &query)
{
   QSqlQuery sqlQuery=db.exec(query);
   //qDebug()<<"query:"<< query;
   //qDebug()<< "lastError:"<<db.lastError();
   return sqlQuery;

}

bool DatabaseLayer::exists(const QString &tableName, SqlParameter parameter)
{
   QString fieldValue=parameter.value.toString();
   if(parameter.value.type()==QVariant::String)
      fieldValue="'"+parameter.value.toString()+"'";
   QString query=QString("SELECT count(*) FROM %1 WHERE %2=%3").arg(tableName).arg(parameter.fieldName).arg(fieldValue);
   QSqlQuery  sqlQuery = exec(query);
   if(sqlQuery.next())
      return sqlQuery.value(0) .toInt()>0;
   return false;
}

QSqlQueryModel *DatabaseLayer::filter(const QString &tableName, QList<SqlParameter> andWhereParameters, bool caseSensitive)
{
   QString whereFields="";
   for (int i = 0; i < andWhereParameters.length(); ++i)
   {
      if(i>0)
         whereFields+=" and ";

      if(andWhereParameters[i].value.type()==QVariant::String && !caseSensitive)
         whereFields+="lower("+andWhereParameters[i].fieldName+")=";
      else
         whereFields+=andWhereParameters[i].fieldName+"=";
      if(andWhereParameters[i].value.type()==QVariant::String)
      {
         if(!caseSensitive)
            whereFields+="lower('"+andWhereParameters[i].value.toString()+"')";
         else
            whereFields+="'"+andWhereParameters[i].value.toString()+"'";
      }
      else
         whereFields+=andWhereParameters[i].value.toString();
   }
   QString where="";
   if(whereFields!="")
      where+=" WHERE "+whereFields;

   QString query=QString("SELECT * FROM %1 %2").arg(tableName).arg(where);
   QSqlQueryModel *model = new QSqlQueryModel();
   model->setQuery(query);
   return model;
}

double DatabaseLayer::aggregate(QString aggregateName, QString tableName, QString fieldName)
{
   QString query=QString("SELECT ifnull(%1(%2),0) FROM %3").arg(aggregateName).arg(fieldName).arg(tableName);
   QSqlQuery sqlQuery=exec(query);
   if(sqlQuery.next())
      return sqlQuery.value(0).toDouble();
   return 0;
}

double DatabaseLayer::max(const QString &tableName, const QString &fieldName)
{
   return aggregate("max", tableName, fieldName);
}

qint64 DatabaseLayer::insert(const QString &tableName, QList<SqlParameter> &sqlParameters, const QString &returnField)
{
   QString fieldNames="";
   QString fieldValues="";
   for (int i = 0; i < sqlParameters.length(); ++i)
   {
      if(i>0)
      {
         fieldNames+=", ";
         fieldValues+=", ";
      }
      fieldNames+=sqlParameters[i].fieldName;
      if(sqlParameters[i].value.type()==QVariant::String)
      {
         fieldValues+="'"+sqlParameters[i].value.toString()+"'";
      }
      else
         fieldValues+=sqlParameters[i].value.toString();
   }
   QString query= QString("INSERT INTO %1 (%2) VALUES(%3)").arg(tableName).arg(fieldNames).arg(fieldValues);
   exec(query);
   if(returnField!="")
      return (qint64) max(tableName, returnField);
   return 0;
}

bool DatabaseLayer::update(const QString &tableName, QList<SqlParameter> &updateParameters, QList<SqlParameter> &andWhereParameters)
{
   QString setFields="";
   for (int i = 0; i < updateParameters.length(); ++i)
   {
      if(i>0)
         setFields+=", ";
      setFields+=updateParameters[i].fieldName+"=";
      if(updateParameters[i].value.type()==QVariant::String)
         setFields+="'"+updateParameters[i].value.toString()+"'";
      else
         setFields+=updateParameters[i].value.toString();
   }
   QString whereFields="";
   for (int i = 0; i < andWhereParameters.length(); ++i)
   {
      if(i>0)
         whereFields+=", ";
      whereFields+=andWhereParameters[i].fieldName+"=";
      if(andWhereParameters[i].value.type()==QVariant::String)
         whereFields+="'"+andWhereParameters[i].value.toString()+"'";
      else
         whereFields+=andWhereParameters[i].value.toString();
   }
   QString where="";
   if(whereFields!="")
      where+=" WHERE "+whereFields;
   QString query= QString("UPDATE %1 SET %2 %3").arg(tableName).arg(setFields).arg(where);
   exec(query);
   return lastError().type()==QSqlError::NoError;
}

QSqlError DatabaseLayer::lastError()
{
   return db.lastError();
}

QString DatabaseLayer::dictionaryValueString(const qint64 dicType, const qint64 dicValue)
{
   QSqlQueryModel  *tblDictionary=filter("Dictionary", SqlParameters()<<SqlParameter("DicType", dicType)
                                                                      <<SqlParameter("DicValue", dicValue));
   if(tblDictionary->rowCount()>0)
      return tblDictionary->record(0).value("DicName").toString();
   return "";
}

qint64 DatabaseLayer::dictionaryValueId(const QString typeName, const QString valueName)
{
   int maxType=0;
   QSqlQueryModel  *tblDictionary=filter("Dictionary", SqlParameters()<<SqlParameter("DicName", typeName));
   if(tblDictionary->rowCount()==0)
   {
      QSqlQuery query =exec("SELECT ifnull(max(DicType),0) from Dictionary where DicValue=0");
      if(query.next())
         maxType=query.value(0).toInt()+1;
      insert("Dictionary", SqlParameters()
             <<SqlParameter("DicType", maxType)<<SqlParameter("DicName", typeName)<<SqlParameter("DicValue", 0));
   }
   else
      maxType=tblDictionary->record(0).value("DicType").toInt();

   int dicValue=0;
   delete tblDictionary;
   tblDictionary = NULL;
   tblDictionary=filter("Dictionary",SqlParameters()
                        <<SqlParameter("DicType", maxType)
                        <<SqlParameter("DicName", valueName));
   if(tblDictionary->rowCount()==0)
   {
      QSqlQuery query =exec(QString("SELECT ifnull(max(DicValue),0) from Dictionary where DicType=%1").arg(maxType));
      if(query.next())
         dicValue=query.value(0).toInt()+1;
      insert("Dictionary", SqlParameters()
             <<SqlParameter("DicType", maxType)<<SqlParameter("DicName", valueName)
             <<SqlParameter("DicValue", dicValue));

   }
   else
      dicValue=tblDictionary->record(0).value("DicValue").toInt();
   return dicValue;
}
