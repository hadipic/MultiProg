#ifndef READXMLAVRLIST_H
#define READXMLAVRLIST_H
#include <QXmlStreamReader>
#include "avrFuse.h"
#include "Libs/xmlFunctions.h"

class ReadXMLAVRList
{
public:
    ReadXMLAVRList();
    AvrFuse * ReadXML(QString avrName);
private:
    //void addNavigation(quint8 lastMask, int textCount, FuseByte *fuseByte, QXmlStreamReader xmlReader);
    void addNavigations(QXmlStreamReader &xmlReader, FuseByte *fuseByte, int textCount);
};

#endif // READXMLAVRLIST_H
