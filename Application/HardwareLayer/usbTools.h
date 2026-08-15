#ifndef USBTOOLS_H
#define USBTOOLS_H

//#ifdef Q_OS_ANDROID
#if !defined _WIN32 && !defined __CYGWIN__
#define WORD quint16
#define BYTE quint8
#define MY_ANDROID
#endif

#include <QObject>
#include <QString>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>
#include "strings.h"
#include "instructions.h"
#if defined MY_ANDROID
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <asm/types.h>
#include <fcntl.h>
//#include <F:\Softwares\Programming\QT\ForAndroid\OtherTools\android-ndk-r10d\platforms\android-21\arch-arm\usr\include\linux\hiddev.h>
//#include <F:\Softwares\Programming\QT\ForAndroid\OtherTools\android-ndk-r10d\platforms\android-21\arch-arm\usr\include\linux\hidraw.h>
#include <linux/input.h>
#include <linux/usbdevice_fs.h>
#include <sys/timeb.h>

#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>

#elif defined _WIN32 || defined __CYGWIN__
#include "basetsd.h"
#include "windef.h"
#include "wtypes.h"
#include "winbase.h"
#include "winreg.h"
#include "wingdi.h"
#include "winuser.h"
#include "setupapi.h"
#endif

#include <QTimer>
#include "QDebug"
#include "Libs/commonFunctions.h"


struct Bits
{
      quint8 Bit0:1;
      quint8 Bit1:1;
      quint8 Bit2:1;
      quint8 Bit3:1;
      quint8 Bit4:1;
      quint8 Bit5:1;
      quint8 Bit6:1;
      quint8 Bit7:1;
};
union Port
{
      quint8 byte;
      Bits bits;
};

class USBTools : public QObject
{
      Q_OBJECT


   public:


#define COL 16
#define VERSION "0.8.1"
#define G (12.0/34*1024/5)		//=72.2823529412
#define LOCK	1
#define FUSE	2
#define FUSE_H  4
#define FUSE_X	8
#define CAL 	16
#define SLOW	256

      explicit USBTools(QObject *parent = 0);

      int vid;
      int pid;
      quint8 bufferU[128], bufferI[128];
      int trisa, trisb, trisc, latac, latb;
      int DIMBUF;
      int HwID;
      Port portA, portB, portC;
      int MinDly,load_osccal,load_BKosccal;
      bool load_calibword,saveLog;
      bool  ICDenable,ICDaddr;
      int err, max_err;
      int FWVersion;
      int programID;


#if defined MY_ANDROID
      int fd;
      //struct hidraw_report_descriptor rep_info_i,rep_info_u;
      //struct hidraw_devinfo ref_multi_i,ref_multi_u;
      char path[256];
      JNIEXPORT void JNICALL Java_Test_getByteArray (JNIEnv *env);


#elif defined _WIN32 || defined __CYGWIN__
      HANDLE WriteHandle,ReadHandle;
      HANDLE hEventObject;
      OVERLAPPED HIDOverlapped;
      DWORD BytesWritten, NumberOfBytesRead;
#elif defined MY_LINUX
      struct hiddev_report_info rep_info_i,rep_info_u;
      struct hiddev_usage_ref_multi ref_multi_i,ref_multi_u;
#endif

      void DisplayEE();
      void write();
      int  findDevice();

      void WriteSample();
      void Connect();
      void DCDCactive(bool checkdcdc ,qint8 valuedc);
      int  StartHVReg(double V);
      int StartHVReg1(double V);
      void ProgID();
      void  vdd_vpp (bool vpp_e , bool vcc_e);
      int  CheckV33Regulator();

      int  CheckS1();
      void read();
      void msDelay(double delay);
      void IoChange();
      QString toBinary(quint32 a);

      bool isDeviceDetected();

      void changeEnableIo(bool canChanged);

      quint16 *memCODE_W;
      quint8 *memCODE, *memEE, memID[8], memCONFIG[48], memUSERID[8];
      quint16 memCONFIG_W[48];

      int skipV33check;
      quint32 size,sizeW,sizeEE,sizeCONFIG,sizeUSERID;
      int hvreg;
      int stop,start;




   signals:
      void onIoChanged();


   public slots:
      void updateIo();


   private:
      //void timerEvent(QTimerEvent *event);
      QTimer *timer;
      bool _isDeviceDetected;
};

#endif // USBTOOLS_H
