#include "xmlFunctions.h"

XMLFunctions::XMLFunctions(QObject *parent) :
    QObject(parent)
{
}
bool XMLFunctions::goToStartElement(QXmlStreamReader &xmlReader, QString tagName, QString finishTagName)
{
    QXmlStreamReader::TokenType token;
    do
    {
        token = xmlReader.readNext();
        if(finishTagName!="" && token==QXmlStreamReader::EndElement && xmlReader.name().startsWith(finishTagName))
        {
            return false;
        }
    }while(!xmlReader.atEnd() && !xmlReader.hasError()
           && (token!=QXmlStreamReader::StartElement || (token==QXmlStreamReader::StartElement && xmlReader.name()!=tagName)));
    xmlReader.readNext();
    return true;
}

bool XMLFunctions::goToSimilarStartElement(QXmlStreamReader &xmlReader, QString tagName, QString finishTagName)
{
    QXmlStreamReader::TokenType token;
    do
    {
        token = xmlReader.readNext();
        if(finishTagName!="" && token==QXmlStreamReader::EndElement && xmlReader.name().startsWith(finishTagName))
        {
            return false;
        }
    }while(!xmlReader.atEnd() && !xmlReader.hasError()
           && (token!=QXmlStreamReader::StartElement || (token==QXmlStreamReader::StartElement && !xmlReader.name().startsWith(tagName) )));
    return true;
}
