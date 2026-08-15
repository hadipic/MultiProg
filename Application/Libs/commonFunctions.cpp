#include "commonFunctions.h"
#include "QDateTime"

QPlainTextEdit *CommonFunctions::txtLog;
CommonFunctions::CommonFunctions(QObject *parent) :
    QObject(parent)
{
    txtLog=NULL;
}

quint32 CommonFunctions::GetTickCount()
{
    //struct timeb now;
    //ftime(&now);
    //return now.time*1000+now.millitm;
   return QDateTime::currentDateTime().currentMSecsSinceEpoch();
}


void CommonFunctions::printMessage(QString msg)
{
    qDebug()<<msg;
    if(txtLog!=NULL)
    {
        txtLog->appendPlainText(msg.replace("\n\n","\n"));
    }
}

void CommonFunctions::printMessage(char *msg)
{
    QString str;
    str.sprintf("%s",msg);
    printMessage(str);
}

void CommonFunctions::printMessage1(QString msg, int p1)
{
    QString str;
    str.sprintf(msg.toUtf8(), p1);
    printMessage(str);
}

void CommonFunctions::printMessage1(QString msg, const char *p1)
{
    QString str;
    str.sprintf(msg.toUtf8(), p1);
    printMessage(str);
}

void CommonFunctions::printMessage2(QString msg, int p1, int p2)
{
    QString str;
    str.sprintf(msg.toUtf8(), p1,p2);
    printMessage(str);
}

void CommonFunctions::printMessage3(QString msg, double p1, int p2, int p3)
{
    QString str;
    str.sprintf(msg.toUtf8(), p1,p2,p3);
    printMessage(str);
}

void CommonFunctions::printMessage3(QString msg, char *p1, int p2, int p3)
{
    QString str;
    str.sprintf(msg.toUtf8(), p1,p2,p3);
    printMessage(str);
}

void CommonFunctions::printMessage4(QString msg, int p1, int p2, int p3, int p4)
{
    QString str;
    str.sprintf(msg.toUtf8(), p1,p2,p3,p4);
    printMessage(str);
}

void CommonFunctions::PrintStatusSetup()
{

}

void CommonFunctions::PrintStatusEnd()
{
}

void CommonFunctions::PrintStatusClear()
{
}
