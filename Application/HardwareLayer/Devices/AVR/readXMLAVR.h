#ifndef READXMLAVR_H
#define READXMLAVR_H
#include <QXmlStreamReader>
#include "avrFuse.h"
#include "Libs/xmlFunctions.h"

class ReadXMLAVR
{
public:
    ReadXMLAVR();
    AvrFuse * readXML(QString avrName);
    void saveAllDevicesToDB();
    void saveToDB(QString avrName);
   private:
    //void addNavigation(quint8 lastMask, int textCount, FuseByte *fuseByte, QXmlStreamReader xmlReader);
    void addNavigations(QXmlStreamReader &xmlReader, FuseByte *fuseByte, int textCount);
};

#endif // READXMLAVRLIST_H
