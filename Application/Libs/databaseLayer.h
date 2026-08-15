#ifndef DATABASELAYER_H
#define DATABASELAYER_H


#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QVariant>
#include <QSqlRecord>

class SqlParameter
{
   public:
      SqlParameter(QString fieldName, QVariant value)
      {
         this->fieldName = fieldName;
         this->value = value;
      }

      QString fieldName;
      QVariant value;
};
typedef QList<SqlParameter> SqlParameters ;

class DatabaseLayer : public QObject
{
      Q_OBJECT
   public:
      explicit DatabaseLayer(QString driverName, QString databaseName, QObject *parent = 0);
      QSqlQuery   exec  (const QString &query);
      qint64      insert(const QString &tableName, QList<SqlParameter> &sqlParameters, const QString &returnField="");
      double      max   (const QString &tableName, const QString &fieldName);
      bool        exists(const QString &tableName, SqlParameter parameter);
      bool update(const QString &tableName, QList<SqlParameter> &updateParameters, QList<SqlParameter> &andWhereParameters);
      QSqlError lastError();
      QSqlQueryModel *filter(const QString &tableName, QList<SqlParameter> andWhereParameters, bool caseSensitive=false);
      qint64 dictionaryValueId(const QString typeName, const QString valueName);
      QString dictionaryValueString(const qint64 dicType, const qint64 dicValue);
   private:
      QSqlDatabase db;
      double aggregate(QString aggregateName, QString tableName, QString fieldName);
   signals:

   public slots:

};

#endif // DATABASELAYER_H
