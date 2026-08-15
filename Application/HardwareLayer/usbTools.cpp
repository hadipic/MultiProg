#include "usbTools.h"


#include <qmath.h>

#include "Libs/strings.h"
#include "Libs/globalVariables.h"

USBTools::USBTools(QObject *parent) :
    QObject(parent)
{
#ifdef MY_ANDROID
    fd=-1;
    path[0]=0;
#endif
    vid=0x04D8;
    pid=0x0100;
    DIMBUF=65;
    trisa=1,trisb=0,trisc=0x34,latac=0,latb=0;
    _isDeviceDetected=false;
    timer = new QTimer(this);
    max_err=200;
    FWVersion=0x800;
    memCODE_W=NULL;
    memEE=NULL;
    memCODE_W=NULL;
    memCODE=NULL;

    skipV33check=0;
    size=0,sizeW=0,sizeEE=0,sizeCONFIG=0,sizeUSERID=0;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateIo()));
}

#define COL 16

void USBTools::DisplayEE()
{
    char s[256],t[256],v[256],*g;
    int valid=0,empty=1;
    int i,j,max;
    s[0]=0;
    v[0]=0;
    char* aux=(char*)malloc((sizeEE/COL+1)*(16+COL*6));

    // aux=malloc((sizeEE/17)*(16+16*5));
    aux[0]=0;
    //CommonFunctions::printMessage(GlobalVariables::strings[S_EEMem]);	//"\r\nEEPROM memory:\r\n"
    max=sizeEE>7000?7000:sizeEE;
    for(i=0;i<max;i+=COL)
    {
        valid=0;
        for(j=i;j<i+COL&&j<sizeEE;j++)
        {
            sprintf(t,"%02X ",memEE[j]);
            strcat(s,t);
            sprintf(t,"%c",isprint(memEE[j])&&(memEE[j]<0xFF)?memEE[j]:'.');
            //  g=g_locale_to_utf8(t,-1,NULL,NULL,NULL);
            if(g) strcat(v,g);
            free(g);
            if(memEE[j]<0xff)
                valid=1;
        }
        if(valid)
        {
            sprintf(t,"%04X: %s %s\r\n",i,s,v);
            strcat(aux,t);
            empty=0;
        }
        s[0]=0;
        v[0]=0;
    }
    if(empty)
        CommonFunctions::printMessage("empty");	//empty
    else{
        CommonFunctions::printMessage(aux);
        if(sizeEE>max)
            CommonFunctions::printMessage("(...)\r\n");
    }
    free(aux);

}

/*JNIEXPORT void JNICALL USBTools::Java_Test_getByteArray (JNIEnv *env)
{

   /*jbyteArray firstMacArray = env-> NewByteArray (6);
   jbyte * bytes = env-> GetByteArrayElements (firstMacArray, 0);

   for (int i = 0; i < 6; ++i)
      bytes[i]=i;

   env-> SetByteArrayRegion (firstMacArray, 0, 6, bytes);
   return firstMacArray;
}
*/

int writeCount=1;
void USBTools::write()
{

#if defined MY_ANDROID

    //ioctl(fd, HIDIOCSUSAGES, &ref_multi_u);
    //ioctl(fd, HIDIOCSREPORT, &rep_info_u);
    CommonFunctions::printMessage("Start Write");
    QAndroidJniEnvironment env;
    jbyteArray buffer = env->NewByteArray (DIMBUF);
    env->SetByteArrayRegion (buffer, 0, DIMBUF, reinterpret_cast<jbyte*>(bufferU));

#ifdef DEBUG
    QString tx="";
    for (int i = 0; i < DIMBUF; ++i)
        tx+=QString("TX[%0]:%1\n").arg(i, 2).arg((quint8)bufferU[i], 2, 16, QLatin1Char('0')).toUpper();
    CommonFunctions::printMessage(tx);
#endif
    QAndroidJniObject result = QAndroidJniObject::callStaticObjectMethod("org/qtproject/MultiProg/AndroidUSBTools",
                                                                         "SendData",
                                                                         "([B)Ljava/lang/String;", buffer);

#ifdef DEBUG
    CommonFunctions::printMessage("Result:"+result.toString()+" writeCount:"+QString::number(writeCount++));
#endif
    env->DeleteLocalRef(buffer);


#elif defined _WIN32 || defined __CYGWIN__
    WriteFile(WriteHandle,bufferU,DIMBUF,&BytesWritten,NULL);
#endif
}

void USBTools::msDelay(double delay)
{
    int MinDly=1;
#if defined MY_ANDROID
    long x=(int)delay*1000.0;
    usleep(x>MinDly?x:MinDly);
#elif defined _WIN32 || defined __CYGWIN__
    Sleep((long)qCeil(delay)>MinDly?(long)qCeil(delay):MinDly);
#endif
}



void USBTools::read()
{
#if defined MY_ANDROID
    CommonFunctions::printMessage("Start Read");

    QAndroidJniObject result = QAndroidJniObject::callStaticObjectMethod("org/qtproject/MultiProg/AndroidUSBTools",
                                                                         "ReadData",
                                                                         "()Ljava/lang/String;");

    msDelay(5);
    //    jbyteArray array = result.object<jbyteArray>();
    //    int len = env->GetArrayLength (array);
    //    unsigned char* buf = new unsigned char[len];
    //    env->GetByteArrayRegion(array, 0, len, reinterpret_cast<jbyte*>(buf));

    //CommonFunctions::printMessage("Result:"+result.toString());
    QStringList list = result.toString().split(' ');
    DIMBUF = list.length();
    QString rx="";
    for (int i = 0; i < DIMBUF; ++i)
    {
        bufferI[i] =  list[i].toInt();
        //qDebug()<<bufferI[i];
#ifdef DEBUG
        rx+=QString("RX[%0]:%1\n").arg(i, 2).arg((quint8) bufferI[i], 2, 16, QLatin1Char('0')).toUpper();
#endif
    }
#ifdef DEBUG
    CommonFunctions::printMessage(rx);
#endif


#elif defined MY_LINUX
    ioctl(fd, HIDIOCGUSAGES, &ref_multi_i);
    ioctl(fd,HIDIOCGREPORT, &rep_info_i);
#elif defined _WIN32 || defined __CYGWIN__
    ReadFile(ReadHandle,bufferI,DIMBUF,&NumberOfBytesRead,(LPOVERLAPPED) &HIDOverlapped);
#endif
}

int USBTools::findDevice()
{
    _isDeviceDetected = false;

#if defined MY_ANDROID

    QAndroidJniObject deviceInfo = QAndroidJniObject::callStaticObjectMethod("org/qtproject/MultiProg/AndroidUSBTools",
                                                                             "CheckInfo",
                                                                             "(I)Ljava/lang/String;", 10);

    /*QAndroidJniObject deviceInfo = QAndroidJniObject::callStaticObjectMethod("org/qtproject/MultiProg/HidBridge",
                                                                            "OpenDevice",
                                                                            "Ljava/lang/String;");
   */
    QString deviceInfoStr = deviceInfo.toString();

    if(!deviceInfoStr.isEmpty())
    {
        _isDeviceDetected = true;
        CommonFunctions::printMessage("DeviceInfo:"+deviceInfoStr);
        return 1;
    }
    else
    {
        _isDeviceDetected = false;
        CommonFunctions::printMessage("Device not found");
        return 0;
    }

#elif defined MY_LINUX

    /*libusb_context *ctx = NULL;
   int r; //for return values
   int cnt; //holding number of devices in list
   r = libusb_init(&ctx); //initialize a library session
   if(r < 0)
   {
      CommonFunctions::printMessage("Init Error");
      return 0;
   }
   libusb_set_debug(ctx, 3); //set verbosity level to 3, as suggested in the documentation
   cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
   if(cnt < 0)
   {
      CommonFunctions::printMessage("Get Device Error");
   }
   CommonFunctions::printMessage(QString("Devices in list. count=%0").arg(cnt));

   for(ssize_t i = 0; i < cnt; i++)
   {
      //CommonFunctions::printMessage(QString("dev[%0]:%1").arg(i).arg(devs[i][0]));
   }
   libusb_free_device_list(devs, 1); //free the list, unref the devices in it
   libusb_exit(ctx); //close the session
   return 0;
   */

    struct hiddev_devinfo device_info;
    int i;
    strcpy(path, "/dev/usb/hidraw2-1:1.0");
    //strcpy(path, "/sdcard/test.txt");
    //strcpy(path, "/dev/bus/usb/002/033");
    CommonFunctions::printMessage(path);
    if(path[0]==0) 	//search all devices
    {
        for(i=0; i<16; i++)
        {
            sprintf(path,"/dev/usb/hiddev%d",i);
            CommonFunctions::printMessage(path);
            if((fd = open(path, O_RDONLY ))>0)
            {
                CommonFunctions::printMessage("Open");
                ioctl(fd, HIDIOCGDEVINFO, &device_info);
                //CommonFunctions::printMessage(QString("Device info:"));
                if(device_info.vendor==vid&&device_info.product==pid)
                    break;
                else
                    close(fd);
            }
        }
        if(i==16)
        {
            //printMessage((strings[S_noprog]);
            path[0]=0;
            return 0;
        }
    }
    else 	//user supplied path
    {
        if ((fd = open(path, O_RDONLY )) < 0)
        {
            CommonFunctions::printMessage(QString("cannot open %0, make sure you have read permission on it").arg(path));
            return 0;
        }
        ioctl(fd, HIDIOCGDEVINFO, &device_info);
        if(device_info.vendor!=vid||device_info.product!=pid)
        {
            CommonFunctions::printMessage(GlobalVariables::strings[S_noprog]);
            return 0;
        }
    }
    _isDeviceDetected = true;
    rep_info_u.report_type=HID_REPORT_TYPE_OUTPUT;
    rep_info_i.report_type=HID_REPORT_TYPE_INPUT;
    rep_info_u.report_id=rep_info_i.report_id=HID_REPORT_ID_FIRST;
    rep_info_u.num_fields=rep_info_i.num_fields=1;
    ref_multi_u.uref.report_type=HID_REPORT_TYPE_OUTPUT;
    ref_multi_i.uref.report_type=HID_REPORT_TYPE_INPUT;
    ref_multi_u.uref.report_id=ref_multi_i.uref.report_id=HID_REPORT_ID_FIRST;
    ref_multi_u.uref.field_index=ref_multi_i.uref.field_index=0;
    ref_multi_u.uref.usage_index=ref_multi_i.uref.usage_index=0;
    ref_multi_u.num_values=ref_multi_i.num_values=DIMBUF;
    CommonFunctions::printMessage("Programmer Detected");
#elif defined _WIN32 || defined __CYGWIN__
    PSP_DEVICE_INTERFACE_DETAIL_DATA detailData;
    HANDLE DeviceHandle;
    HANDLE hDevInfo;
    GUID HidGuid;
    char MyDevicePathName[1024];
    ULONG Length;
    ULONG Required;
    typedef struct _HIDD_ATTRIBUTES
    {
        ULONG   Size;
        USHORT  VendorID;
        USHORT  ProductID;
        USHORT  VersionNumber;
    } HIDD_ATTRIBUTES, *PHIDD_ATTRIBUTES;

    typedef void (__stdcall*GETHIDGUID) (OUT LPGUID HidGuid);
    typedef BOOLEAN (__stdcall*GETATTRIBUTES)(IN HANDLE HidDeviceObject,OUT PHIDD_ATTRIBUTES Attributes);
    typedef BOOLEAN (__stdcall*SETNUMINPUTBUFFERS)(IN  HANDLE HidDeviceObject,OUT ULONG  NumberBuffers);
    typedef BOOLEAN (__stdcall*GETNUMINPUTBUFFERS)(IN  HANDLE HidDeviceObject,OUT PULONG  NumberBuffers);
    typedef BOOLEAN (__stdcall*GETFEATURE) (IN  HANDLE HidDeviceObject, OUT PVOID ReportBuffer, IN ULONG ReportBufferLength);
    typedef BOOLEAN (__stdcall*SETFEATURE) (IN  HANDLE HidDeviceObject, IN PVOID ReportBuffer, IN ULONG ReportBufferLength);
    typedef BOOLEAN (__stdcall*GETREPORT) (IN  HANDLE HidDeviceObject, OUT PVOID ReportBuffer, IN ULONG ReportBufferLength);
    typedef BOOLEAN (__stdcall*SETREPORT) (IN  HANDLE HidDeviceObject, IN PVOID ReportBuffer, IN ULONG ReportBufferLength);
    typedef BOOLEAN (__stdcall*GETMANUFACTURERSTRING) (IN  HANDLE HidDeviceObject, OUT PVOID ReportBuffer, IN ULONG ReportBufferLength);
    typedef BOOLEAN (__stdcall*GETPRODUCTSTRING) (IN  HANDLE HidDeviceObject, OUT PVOID ReportBuffer, IN ULONG ReportBufferLength);
    typedef BOOLEAN (__stdcall*GETINDEXEDSTRING) (IN  HANDLE HidDeviceObject, IN ULONG  StringIndex, OUT PVOID ReportBuffer, IN ULONG ReportBufferLength);
    HIDD_ATTRIBUTES Attributes;
    SP_DEVICE_INTERFACE_DATA devInfoData;
    int LastDevice = FALSE;
    int MemberIndex = 0;
    LONG Result;
    //	char UsageDescription[256];

    Length=0;
    detailData=NULL;
    DeviceHandle=NULL;

    HMODULE hHID=0;
    GETHIDGUID HidD_GetHidGuid=0;
    GETATTRIBUTES HidD_GetAttributes=0;
    SETNUMINPUTBUFFERS HidD_SetNumInputBuffers=0;
    GETNUMINPUTBUFFERS HidD_GetNumInputBuffers=0;
    GETFEATURE HidD_GetFeature=0;
    SETFEATURE HidD_SetFeature=0;
    GETREPORT HidD_GetInputReport=0;
    SETREPORT HidD_SetOutputReport=0;
    GETMANUFACTURERSTRING HidD_GetManufacturerString=0;
    GETPRODUCTSTRING HidD_GetProductString=0;
    hHID = LoadLibrary(L"hid.dll");
    if(!hHID)
    {
        CommonFunctions::printMessage("Can't find hid.dll");
        return 0;
    }
    HidD_GetHidGuid=(GETHIDGUID)GetProcAddress(hHID,"HidD_GetHidGuid");
    HidD_GetAttributes=(GETATTRIBUTES)GetProcAddress(hHID,"HidD_GetAttributes");
    HidD_SetNumInputBuffers=(SETNUMINPUTBUFFERS)GetProcAddress(hHID,"HidD_SetNumInputBuffers");
    HidD_GetNumInputBuffers=(GETNUMINPUTBUFFERS)GetProcAddress(hHID,"HidD_GetNumInputBuffers");
    HidD_GetFeature=(GETFEATURE)GetProcAddress(hHID,"HidD_GetFeature");
    HidD_SetFeature=(SETFEATURE)GetProcAddress(hHID,"HidD_SetFeature");
    HidD_GetInputReport=(GETREPORT)GetProcAddress(hHID,"HidD_GetInputReport");
    HidD_SetOutputReport=(SETREPORT)GetProcAddress(hHID,"HidD_SetOutputReport");
    HidD_GetManufacturerString=(GETMANUFACTURERSTRING)GetProcAddress(hHID,"HidD_GetManufacturerString");
    HidD_GetProductString=(GETPRODUCTSTRING)GetProcAddress(hHID,"HidD_GetProductString");
    if(HidD_GetHidGuid==NULL\
            ||HidD_GetAttributes==NULL\
            ||HidD_GetFeature==NULL\
            ||HidD_SetFeature==NULL\
            ||HidD_GetInputReport==NULL\
            ||HidD_SetOutputReport==NULL\
            ||HidD_GetManufacturerString==NULL\
            ||HidD_GetProductString==NULL\
            ||HidD_SetNumInputBuffers==NULL\
            ||HidD_GetNumInputBuffers==NULL) return 0;


    HMODULE hSAPI=0;
    hSAPI = LoadLibrary(L"setupapi.dll");
    if(!hSAPI)
    {
        CommonFunctions::printMessage("Can't find setupapi.dll");
        return 0;
    }
    typedef HDEVINFO (WINAPI* SETUPDIGETCLASSDEVS) (CONST GUID*,PCSTR,HWND,quint16);
    typedef BOOL (WINAPI* SETUPDIENUMDEVICEINTERFACES) (HDEVINFO,PSP_DEVINFO_DATA,CONST GUID*,quint16,PSP_DEVICE_INTERFACE_DATA);
    typedef BOOL (WINAPI* SETUPDIGETDEVICEINTERFACEDETAIL) (HDEVINFO,PSP_DEVICE_INTERFACE_DATA,PSP_DEVICE_INTERFACE_DETAIL_DATA_A,WORD,PWORD,PSP_DEVINFO_DATA);
    typedef BOOL (WINAPI* SETUPDIDESTROYDEVICEINFOLIST) (HDEVINFO);
    SETUPDIGETCLASSDEVS SetupDiGetClassDevsA=0;
    SETUPDIENUMDEVICEINTERFACES SetupDiEnumDeviceInterfaces=0;
    SETUPDIGETDEVICEINTERFACEDETAIL SetupDiGetDeviceInterfaceDetailA=0;
    SETUPDIDESTROYDEVICEINFOLIST SetupDiDestroyDeviceInfoList=0;
    SetupDiGetClassDevsA=(SETUPDIGETCLASSDEVS) GetProcAddress(hSAPI,"SetupDiGetClassDevsA");
    SetupDiEnumDeviceInterfaces=(SETUPDIENUMDEVICEINTERFACES) GetProcAddress(hSAPI,"SetupDiEnumDeviceInterfaces");
    SetupDiGetDeviceInterfaceDetailA=(SETUPDIGETDEVICEINTERFACEDETAIL) GetProcAddress(hSAPI,"SetupDiGetDeviceInterfaceDetailA");
    SetupDiDestroyDeviceInfoList=(SETUPDIDESTROYDEVICEINFOLIST) GetProcAddress(hSAPI,"SetupDiDestroyDeviceInfoList");
    if(SetupDiGetClassDevsA==NULL\
            ||SetupDiEnumDeviceInterfaces==NULL\
            ||SetupDiDestroyDeviceInfoList==NULL\
            ||SetupDiGetDeviceInterfaceDetailA==NULL) return 0;


    /*
  The following code is adapted from Usbhidio_vc6 application example by Jan Axelson
  for more information see see http://www.lvr.com/hidpage.htm
  */

    /*
  API function: HidD_GetHidGuid
  Get the GUID for all system HIDs.
  Returns: the GUID in HidGuid.
  */
    HidD_GetHidGuid(&HidGuid);

    /*
  API function: SetupDiGetClassDevs
  Returns: a handle to a device information set for all installed devices.
  Requires: the GUID returned by GetHidGuid.
  */
    hDevInfo=SetupDiGetClassDevs(&HidGuid,NULL,NULL,DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
    devInfoData.cbSize = sizeof(devInfoData);

    //Step through the available devices looking for the one we want.
    //Quit on detecting the desired device or checking all available devices without success.
    MemberIndex = 0;
    LastDevice = FALSE;
    hEventObject=NULL;
    do
    {
        /*
      API function: SetupDiEnumDeviceInterfaces
      On return, MyDeviceInterfaceData contains the handle to a
      SP_DEVICE_INTERFACE_DATA structure for a detected device.
      Requires:
      The DeviceInfoSet returned in SetupDiGetClassDevs.
      The HidGuid returned in GetHidGuid.
      An index to specify a device.
      */
        Result=SetupDiEnumDeviceInterfaces (hDevInfo, 0, &HidGuid, MemberIndex, &devInfoData);
        if (Result != 0)
        {
            //A device has been detected, so get more information about it.
            /*
          API function: SetupDiGetDeviceInterfaceDetail
          Returns: an SP_DEVICE_INTERFACE_DETAIL_DATA structure
          containing information about a device.
          To retrieve the information, call this function twice.
          The first time returns the size of the structure in Length.
          The second time returns a pointer to the data in DeviceInfoSet.
          Requires:
          A DeviceInfoSet returned by SetupDiGetClassDevs
          The SP_DEVICE_INTERFACE_DATA structure returned by SetupDiEnumDeviceInterfaces.

          The final parameter is an optional pointer to an SP_DEV_INFO_DATA structure.
          This application doesn't retrieve or use the structure.
          If retrieving the structure, set
          MyDeviceInfoData.cbSize = length of MyDeviceInfoData.
          and pass the structure's address.
          */
            //Get the Length value.
            //The call will return with a "buffer too small" error which can be ignored.
            Result = SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, NULL, 0, &Length, NULL);

            //Allocate memory for the hDevInfo structure, using the returned Length.
            detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);

            //Set cbSize in the detailData structure.
            detailData -> cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

            //Call the function again, this time passing it the returned buffer size.
            Result = SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, detailData, Length,&Required, NULL);

            // Open a handle to the device.
            // To enable retrieving information about a system mouse or keyboard,
            // don't request Read or Write access for this handle.
            /*
          API function: CreateFile
          Returns: a handle that enables reading and writing to the device.
          Requires:
          The DevicePath in the detailData structure
          returned by SetupDiGetDeviceInterfaceDetail.
          */
            DeviceHandle=CreateFile(detailData->DevicePath,
                                    0, FILE_SHARE_READ|FILE_SHARE_WRITE,
                                    (LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING, 0, NULL);

            /*
          API function: HidD_GetAttributes
          Requests information from the device.
          Requires: the handle returned by CreateFile.
          Returns: a HIDD_ATTRIBUTES structure containing
          the Vendor ID, Product ID, and Product Version Number.
          Use this information to decide if the detected device is
          the one we're looking for.
          */

            //Set the Size to the number of bytes in the structure.
            Attributes.Size = sizeof(Attributes);
            Result = HidD_GetAttributes(DeviceHandle,&Attributes);

            //Is it the desired device?
            _isDeviceDetected = false;
            if (Attributes.VendorID == vid)
            {
                if (Attributes.ProductID == pid)
                {
                    //Both the Vendor ID and Product ID match.
                    _isDeviceDetected = true;
                    //strcpy(MyDevicePathName, detailData.DevicePath ->DevicePath);

                    // Get a handle for writing Output reports.
                    WriteHandle=CreateFile(detailData->DevicePath,
                                           GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                                           (LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,0,NULL);

                    //Get a handle to the device for the overlapped ReadFiles.
                    ReadHandle=CreateFile(detailData->DevicePath,
                                          GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,(LPSECURITY_ATTRIBUTES)NULL,
                                          OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);

                    if (hEventObject)
                        CloseHandle(hEventObject);
                    hEventObject = CreateEvent(NULL,TRUE,TRUE,L"");

                    //Set the members of the overlapped structure.
                    HIDOverlapped.hEvent = hEventObject;
                    HIDOverlapped.Offset = 0;
                    HIDOverlapped.OffsetHigh = 0;
                    Result=HidD_SetNumInputBuffers(DeviceHandle,64);
                }
                else
                    //The Product ID doesn't match.
                    CloseHandle(DeviceHandle);
            }
            else
                //The Vendor ID doesn't match.
                CloseHandle(DeviceHandle);
            //Free the memory used by the detailData structure (no longer needed).
            free(detailData);
        }
        else
            //SetupDiEnumDeviceInterfaces returned 0, so there are no more devices to check.
            LastDevice=TRUE;
        //If we haven't found the device yet, and haven't tried every available device,
        //try the next one.
        MemberIndex = MemberIndex + 1;
    } //do
    while ((LastDevice == FALSE) && (!_isDeviceDetected));

    //Free the memory reserved for hDevInfo by SetupDiClassDevs.
    SetupDiDestroyDeviceInfoList(hDevInfo);

    /*	if(info)
{
          printMessage(3("Device detected: vid=0x%04X pid=0x%04X\nPath: %s\n",vid,pid,MyDevicePathName);
          if(HidD_GetManufacturerString(DeviceHandle,string,sizeof(string))==TRUE) wprintf(L"Manufacturer string: %s\n",string);
          if(HidD_GetProductString(DeviceHandle,string,sizeof(string))==TRUE) wprintf(L"Product string: %s\n",string);
      }*/
#endif
    if (!_isDeviceDetected)
    {
        //printMessage((strings[S_noprog]);	 //"Programmer not detected\r\n"
        CommonFunctions::printMessage("Programmer not detected"); //"Programmer not detected\r\n"
        //gtk_statusbar_push(status_bar,statusID,strings[S_noprog]);
    }
    else
    {
        //printMessage((strings[S_prog]);	//"Programmer detected\r\n");
        CommonFunctions::printMessage("Programmer detected");
        //gtk_statusbar_push(status_bar,statusID,strings[S_prog]);
    }
    return _isDeviceDetected;
}

void USBTools::WriteSample()
{

    int j=0;
    bufferU[j++]=0;
    bufferU[j++]=READ_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x80;	//PORTA
    bufferU[j++]=READ_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x81;	//PORTB
    bufferU[j++]=READ_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x82;	//PORTC
    bufferU[j++]=WRITE_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x92;	//TRISA
    bufferU[j++]=trisa;
    bufferU[j++]=WRITE_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x93;	//TRISB
    bufferU[j++]=trisb;
    bufferU[j++]=WRITE_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x94;	//TRISC
    bufferU[j++]=trisc;
    bufferU[j++]=EXT_PORT;
    bufferU[j++]=latb;
    bufferU[j++]=latac;
    bufferU[j++]=FLUSH;
    for(; j<DIMBUF; j++)
        bufferU[j]=0x0;

    write();
}







void USBTools::IoChange()
{
    if (!_isDeviceDetected)
        return;

    //  qDebug()<<QString::number(latac, 2).toUpper();

    int z;
    int j=0;
    bufferU[j++]=0;
    bufferU[j++]=READ_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x80;	//PORTA
    bufferU[j++]=READ_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x81;	//PORTB
    bufferU[j++]=READ_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x82;	//PORTC
    bufferU[j++]=WRITE_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x92;	//TRISA
    bufferU[j++]=trisa;
    bufferU[j++]=WRITE_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x93;	//TRISB
    bufferU[j++]=trisb;
    bufferU[j++]=WRITE_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x94;	//TRISC
    bufferU[j++]=trisc;
    bufferU[j++]=EXT_PORT;
    bufferU[j++]=latb;
    bufferU[j++]=latac;
    bufferU[j++]=FLUSH;
    for(; j<DIMBUF; j++)
        bufferU[j]=0x0;

    write();
    msDelay(2);
    read();
    for(z=1;z<DIMBUF-3&&bufferI[z]!=READ_RAM;z++) ;
    portA.byte=bufferI[z+3];	//PORTA
    for(z+=4;z<DIMBUF-3&&bufferI[z]!=READ_RAM;z++);
    portB.byte=bufferI[z+3];	//PORTB
    for(z+=4;z<DIMBUF-3&&bufferI[z]!=READ_RAM;z++);
    portC.byte=bufferI[z+3];	//PORTC
    emit onIoChanged();
}

void USBTools::Connect()
{

}
QString USBTools::toBinary(quint32 a)
{
    return QString::number(a, 2);
}

bool USBTools::isDeviceDetected()
{
    return _isDeviceDetected;
}

void USBTools::changeEnableIo(bool canChanged)
{
    if(canChanged)
    {
        if(!timer->isActive())
            timer->start(100);
    }
    else
    {
        timer->stop();
    }
}


void USBTools::updateIo()
{
    IoChange();
}

void  USBTools::DCDCactive(bool checkdcdc ,qint8 valuedc)

{
    if (!_isDeviceDetected)
        return;

    if( checkdcdc)
    {
        int j=0;

        bufferU[j++]=0;
        bufferU[j++]=VREG_EN;			//enable HV regulator
        bufferU[j++]=SET_VPP;
        bufferU[j++]= valuedc;
        bufferU[j++]=FLUSH;
        for(;j<DIMBUF;j++) bufferU[j]=0x0;
        write();
        msDelay(2);
        read();
    }
    else{
        int j=0;
        bufferU[j++]=0;
        bufferU[j++]=VREG_DIS;			//disable HV regulator
        bufferU[j++]=FLUSH;
        for(;j<DIMBUF;j++) bufferU[j]=0x0;
        write();
        msDelay(2);
        read();
    }


}




///Start HV regulator
int USBTools::StartHVReg1(double V)
{
    int j=1,z;

    bufferU[0]=0;


    // bufferU[j++]=VREG_EN;			//enable HV regulator
    // bufferU[j++]=SET_VPP;
    // bufferU[j++]=V;
    bufferU[j++]=SET_PARAMETER;
    bufferU[j++]=SET_T3;
    bufferU[j++]=2000>>8;
    bufferU[j++]=2000&0xff;
    bufferU[j++]=WAIT_T3;
    bufferU[j++]=READ_ADC;
    bufferU[j++]=FLUSH;
    for(;j<DIMBUF;j++) bufferU[j]=0x0;
    write();
    msDelay(30);
    read();
    for(z=1;z<DIMBUF-2&&bufferI[z]!=READ_ADC;z++);
    int v=(bufferI[z+1]<<8)+bufferI[z+2];

    // qDebug()<<"voltage rading:"<<QString::number(v, 10) ;

    return v;
}

///Read programmer ID
void  USBTools::ProgID()
{
    if (!_isDeviceDetected)
        return;
    int j=1;
    bufferU[0]=0;
    bufferU[j++]=PROG_RST;
    bufferU[j++]=FLUSH;
    for(;j<DIMBUF;j++) bufferU[j]=0x0;
    write();
    msDelay(2);
    read();

    //  qDebug()<<  QString("VERSION= ")+ QString::number(bufferI[2],10)+QString(".")+QString::number(bufferI[3],10)+QString(".")+QString::number(bufferI[3],10);
    QString a1= QString("Firmware version: ")+ QString::number(bufferI[2],10)+QString(".")+QString::number(bufferI[3],10)+QString(".")+QString::number(bufferI[3],10);
    CommonFunctions::printMessage(a1 );
    FWVersion=(bufferI[2]<<16)+(bufferI[3]<<8)+bufferI[4];
    a1= QString("Hardware ID: ")+ QString::number(bufferI[5],10)+QString(".")+QString::number(bufferI[6],10)+QString(".")+QString::number(bufferI[7],10);
    // CommonFunctions::printMessage(a1 );
    HwID=bufferI[7];
    if(HwID==1)  CommonFunctions::printMessage( a1+" (18F2550)\r\n\r\n" );
    else if(HwID==2) CommonFunctions::printMessage(a1+" (18F2450)\r\n\r\n" );
    else if(HwID==3)CommonFunctions::printMessage(a1+" (18F2458/2553)\r\n\r\n" );
    else  CommonFunctions::printMessage(a1+" (?)\r\n\r\n" );
}




///
///Check if a 3.3V regulator is present
int USBTools::CheckV33Regulator()
{
    int i,j=1;
    if(skipV33check)
        return 1;
    bufferU[j++]=WRITE_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x93;
    bufferU[j++]=0xFE;	//B0 = output
    bufferU[j++]=EXT_PORT;
    bufferU[j++]=0x01;	//B0=1
    bufferU[j++]=0;
    bufferU[j++]=READ_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x81;	//Check if B1=1
    bufferU[j++]=EXT_PORT;
    bufferU[j++]=0x00;	//B0=0
    bufferU[j++]=0;
    bufferU[j++]=READ_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x81;	//Check if B1=0
    bufferU[j++]=WRITE_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x93;
    bufferU[j++]=0xFF;	//BX = input
    bufferU[j++]=FLUSH;
    for(;j<DIMBUF;j++) bufferU[j]=0x0;
    write();
    msDelay(2);
    read();
    for(j=1;j<DIMBUF-3&&bufferI[j]!=READ_RAM;j++);
    i=bufferI[j+3]&0x2;		//B1 should be high
    for(j+=3;j<DIMBUF-3&&bufferI[j]!=READ_RAM;j++);
    return (i+(bufferI[j+3]&0x2))==2?1:0;
}




///
///Start HV regulator

int USBTools::StartHVReg(double V)
{
    int j=1,z;
    int vreg=(int)(V*10.0);
    bufferU[0]=0;
    quint16 t0,t;
    if(V==-1)
    {
        bufferU[j++]=VREG_DIS;			//disable HV regulator
        bufferU[j++]=FLUSH;
        write();
        msDelay(40);
        read();
        return -1;
    }
    t=t0=CommonFunctions::GetTickCount();
    bufferU[j++]=VREG_EN;			//enable HV regulator
    bufferU[j++]=SET_VPP;
    bufferU[j++]=vreg;
    bufferU[j++]=SET_PARAMETER;
    bufferU[j++]=SET_T3;
    bufferU[j++]=2000>>8;
    bufferU[j++]=2000&0xff;
    bufferU[j++]=WAIT_T3;
    bufferU[j++]=READ_ADC;
    bufferU[j++]=FLUSH;
    for(;j<DIMBUF;j++)
        bufferU[j]=0x0;
    write();
    msDelay(20);
    read();
    for(z=1;z<DIMBUF-2&&bufferI[z]!=READ_ADC;z++);
    int v=(bufferI[z+1]<<8)+bufferI[z+2];
    //	PrintMessage2("v=%d=%fV\n",v,v/G);
    if(v==0)
    {
        //	PrintMessage(strings[S_lowUsbV]);	//"Tensione USB troppo bassa (VUSB<4.5V)\r\n"
        return 0;
    }
    j=1;
    bufferU[j++]=WAIT_T3;
    bufferU[j++]=READ_ADC;
    bufferU[j++]=FLUSH;
    for(;j<DIMBUF;j++)
        bufferU[j]=0x0;
    for(;(v<(vreg/10.0-1)*G||v>(vreg/10.0+1)*G)&&t<t0+1500;t=CommonFunctions::GetTickCount())
    {
        write();
        msDelay(20);
        read();
        for(z=1;z<DIMBUF-2&&bufferI[z]!=READ_ADC;z++);
        v=(bufferI[z+1]<<8)+bufferI[z+2];
        if(HwID==3)
            v>>=2;		//if 12 bit ADC
        //		PrintMessage2("v=%d=%fV\n",v,v/G);
    }
    if(v>(vreg/10.0+1)*G)
    {
        //PrintMessage(strings[S_HiVPP]);	//"Attenzione: tensione regolatore troppo alta\r\n\r\n"
        return 0;
    }
    else if(v<(vreg/10.0-1)*G)
    {
        //  PrintMessage(strings[S_LowVPP]);	//"Attenzione: tensione regolatore troppo bassa\r\n\r\n"
        return 0;
    }
    else if(v==0)
    {
        //  PrintMessage(strings[S_lowUsbV]);	//"Tensione USB troppo bassa (VUSB<4.5V)\r\n"
        return 0;
    }
    else{
        //  PrintMessage2(strings[S_reg],t-t0,v/G);	//"Regolatore avviato e funzionante dopo T=%d ms VPP=%.1f\r\n\r\n"
        return vreg;
    }
}



/// Enable/disable vdd or vpp from IO tab or update DCDC voltage
void USBTools::vdd_vpp(bool vpp_e , bool vcc_e)
{
    if (!_isDeviceDetected)
        return;
    int j=0,vdd_vpp_v=0;
    char str[16]="";
    if(vpp_e)
    {

        vdd_vpp_v+=4;
        // strcat(str,"VPP ");
    }
    if(vcc_e)
    {

        vdd_vpp_v+=1;
        //   strcat(str,"VDD ");
    }

    bufferU[j++]=0;
    bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    bufferU[j++]=vdd_vpp_v;
    bufferU[j++]=FLUSH;
    for(;j<DIMBUF;j++) bufferU[j]=0x0;
    write();
    msDelay(2);
    read();
}

int USBTools::CheckS1()
{
    int i,j=1;
    //quint32 bytesWritten;
    bufferU[j++]=READ_RAM;
    bufferU[j++]=0x0F;
    bufferU[j++]=0x84;	//READ PORTE
    bufferU[j++]=FLUSH;

    for(;j<DIMBUF;j++)
        bufferU[j]=0x0;
    write();
    msDelay(2);
    read();
    for(j=1;j<DIMBUF-3&&bufferI[j]!=READ_RAM;j++);
    i=bufferI[j+3]&0x8;		//i=E3
    return i?0:1;			//S1 open -> E3=1
}
