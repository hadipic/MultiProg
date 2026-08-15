#include <QApplication>
#include "mainwindow.h"
#include "widgets/hexEditor/hexPlainTextEdit.h"
#include "Forms/ioCheck.h"
#include "HardwareLayer/fileIO.h"
#include "widgets/scrollTest.h"
#include "HardwareLayer/Devices/AVR/readXMLAVR.h"
#include "HardwareLayer/Devices/PIC/readXMLPIC.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
int main(int argc, char *argv[])
{

   //quint8 bufferI=32;
   //QString rx=QString("RX[%0]:%1\n").arg(0, 2).arg( bufferI, 2, 16, QLatin1Char('0'));
   //Q_INIT_RESOURCE(application);
   QApplication app(argc, argv);
   //app.setOrganizationName("QtProject");
   //app.setApplicationName("Application Example");


   GlobalVariables::getInstance();
   MainWindow w;
   w.show();
   return app.exec();
}
