#ifndef READXMLPIC_H
#define READXMLPIC_H
#include <QObject>
#include <QList>
#include <QString>
#include <QXmlStreamReader>

#include "picInfo.h"


class ReadXMLPIC
{
   public:
      ReadXMLPIC();
      PicInfo *readXML(QString picName);
      void saveToDB(QString picName);
      void saveAllDevicesToDB();
      quint64 hexStrToInt(QString hexStr);
      bool strToDouble(double &output, QString const str);
};

#endif // READXMLPIC_H
