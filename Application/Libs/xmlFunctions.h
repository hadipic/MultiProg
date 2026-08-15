#ifndef XMLFUNCTIONS_H
#define XMLFUNCTIONS_H
#include <QXmlStreamReader>
#include <QObject>

class XMLFunctions : public QObject
{
    Q_OBJECT
public:
    explicit XMLFunctions(QObject *parent = 0);
    static bool goToSimilarStartElement(QXmlStreamReader &xmlReader, QString tagName, QString finishTagName="");
    static bool goToStartElement(QXmlStreamReader &xmlReader, QString tagName, QString finishTagName="");

signals:
    
public slots:
    
};

#endif // XMLFUNCTIONS_H
