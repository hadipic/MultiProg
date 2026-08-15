#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include <QObject>
#include <QDebug>
#include <QPlainTextEdit>

class CommonFunctions : public QObject
{
    Q_OBJECT
public:
    explicit CommonFunctions(QObject *parent = 0);
    static QPlainTextEdit *txtLog;
    static void printMessage(QString msg);
    static void printMessage1(QString format, int p1);
    static void printMessage1(QString msg, const char *p1);
    static void printMessage2(QString msg, int p1, int p2);
    static void printMessage3(QString msg, double p1, int p2, int p3);
    static void printMessage3(QString msg, char *p1, int p2, int p3);
    static void printMessage4(QString msg, int p1, int p2, int p3, int p4);

    static void	PrintStatusSetup(); //only needed for console version
    static void PrintStatusEnd();     //only needed for console version
    static void PrintStatusClear();   //gtk_statusbar_push(GTK_STATUSBAR(status_bar),statusID,"");
    static void printMessage(char *msg);


    quint32 static GetTickCount();
signals:
    
public slots:
    
};

#endif // COMMONFUNCTIONS_H
