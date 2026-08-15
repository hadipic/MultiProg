#include "progP16.h"
#include "Libs/globalVariables.h"



ProgP16::ProgP16(USBTools *usb, Devices *devices, QObject *parent):ProgDevice(usb, parent)
{
    this->devices=devices;
    initPIC16_ID();
}

void ProgP16::DisplayCODE16F(int size)
{
    char s[256]="",t[256]="";
    char* aux=(char*)malloc((usb->size/COL+1)*(16+COL*5));
    aux[0]=0;
    int valid=0,empty=1,i,j;
    for(i=0;i<size&&i<usb->sizeW;i+=COL)
    {
        valid=0;
        for(j=i;j<i+COL&&j<usb->sizeW&&i<usb->size;j++)
        {
            sprintf(t,"%04X ",usb->memCODE_W[j]);
            strcat(s,t);
            if(usb->memCODE_W[j]<0x3fff)
                valid=1;
        }
        if(valid)
        {
            sprintf(t,"%04X: %s\r\n",i,s);
            empty=0;
            strcat(aux,t);
        }
        s[0]=0;
    }
    if(empty)
        CommonFunctions::printMessage(GlobalVariables::strings[S_Empty]);	//empty
    else
        CommonFunctions::printMessage(aux);
    free(aux);
}

void ProgP16::DisplayEE16F(int size)
{
    // display 14 bit PIC EEPROM memory
    int valid=0,empty=1,i,j;
    char s[256]="",t[256]="",v[256]="";
    char* aux=(char*)malloc((size/COL+1)*(16+COL*5));
    aux[0]=0;

    empty=1;
    v[0]=0;
    for(i=0x2100;i<0x2100+size&&i<usb->sizeW;i+=COL)
    {
        valid=0;
        for(j=i;j<i+COL&&j<0x2100+size&&j<usb->sizeW;j++)
        {
            sprintf(t,"%02X ",usb->memCODE_W[j]&0xff);
            strcat(s,t);
            sprintf(t,"%c",isprint(usb->memCODE_W[j]&0xFF)&&(usb->memCODE_W[j]&0xFF)<0xFF?usb->memCODE_W[j]&0xFF:'.');

            strcat(v,t);

            if(usb->memCODE_W[j]<0xff) valid=1;/**/
        }
        if(valid)
        {
            sprintf(t,"%04X: %s %s\r\n",i,s,v);
            empty=0;
            strcat(aux,t);
        }
        s[0]=0;
        v[0]=0;
    }
    if(empty) CommonFunctions::printMessage(GlobalVariables::strings[S_Empty]);	//empty
    else CommonFunctions::printMessage(aux);
    free(aux);

}

DeviceSelected ProgP16::detectDevice()
{
    DeviceSelected deviceSelected=detectDevice16F1();
    if(!deviceSelected.deviceInfo)
        deviceSelected=detectDevice16F2();
    return deviceSelected;
}

DeviceSelected ProgP16::detectDevice16F1()
{
    // read 14 bit enhanced PIC
    // dim=program usb->size 	dim2=eeprom usb->size   dim3=config usb->size
    // options:
    //		bit0=0 -> vpp before vdd
    //		bit0=1 -> vdd before vpp
    //		bit1=1 -> LVP programming
    // DevREV@0x8005
    // DevID@0x8006
    // Config1@0x8007
    // Config2@0x8008
    // Calib1@0x8009
    // Calib2@0x800A
    // Calib3@0x800B
    // eeprom@0x0
    int optionsList[2]={0, 0x100};
    DeviceSelected deviceSelected;
    deviceSelected.deviceInfo=NULL;
    for(int optionsMode=0; optionsMode<2;optionsMode++)
    {
        int z=0,i,j;
        int options= optionsList[optionsMode];

        if((options&2)==0)
        {				//HV entry
            if(!usb->StartHVReg(8.5))
            {
                CommonFunctions::printMessage(GlobalVariables::strings[S_HVregErr]); //"HV regulator error\r\n"
                continue;
            }
            else
                usb->hvreg=8.5;
        }
        else
            usb->StartHVReg(-1);			//LVP mode, turn off HV

        usb->start=CommonFunctions::GetTickCount();
        usb->bufferU[0]=0;
        j=1;
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T1T2;
        usb->bufferU[j++]=1;						//T1=1u
        usb->bufferU[j++]=100;					//T2=100u
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T3;
        usb->bufferU[j++]=2000>>8;
        usb->bufferU[j++]=2000&0xff;
        usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SET_CK_D;
        usb->bufferU[j++]=0x0;
        if((options&2)==0)
        {				//HV entry
            if((options&1)==0)
            {				//VPP before VDD
                usb->bufferU[j++]=EN_VPP_VCC;
                usb->bufferU[j++]=4;				//VPP
                usb->bufferU[j++]=EN_VPP_VCC;
                usb->bufferU[j++]=0x5;			//VDD+VPP
            }
            else{							//VDD before VPP without delay
                usb->bufferU[j++]=EN_VPP_VCC;
                usb->bufferU[j++]=1;				//VDD
                usb->bufferU[j++]=EN_VPP_VCC;
                usb->bufferU[j++]=0x5;			//VDD+VPP
            }
        }
        else{			//Low voltage programming
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=4;				//VPP
            usb->bufferU[j++]=WAIT_T3;
            usb->bufferU[j++]=TX16;			//0000 1010 0001 0010 1100 0010 1011 0010 = 0A12C2B2
            usb->bufferU[j++]=2;
            usb->bufferU[j++]=0x0A;
            usb->bufferU[j++]=0x12;
            usb->bufferU[j++]=0xC2;
            usb->bufferU[j++]=0xB2;
            usb->bufferU[j++]=SET_CK_D;		//Clock pulse
            usb->bufferU[j++]=0x4;
            usb->bufferU[j++]=SET_CK_D;
            usb->bufferU[j++]=0x0;
        }
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(2);
        usb->read();
        //****************** read config area ********************
        j=1;
        CommonFunctions::printMessage(GlobalVariables::strings[S_Read_CONFIG_A]);		//read config ...
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x8000
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=0xFF;
        int k2=0;
        for(i=0x8000; i<=0x800B; i++)
        {
            //Config
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF*2/4-2 || i==0x800B)
            {		//2B cmd -> 4B data
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(5);
                usb->read();
                for(z=1;z<usb->DIMBUF-2;z++)
                {
                    if(usb->bufferI[z]==READ_DATA_PROG)
                    {
                        usb->memCONFIG_W[k2++]=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
                        z+=2;
                    }
                }
                j=1;
            }
        }
        j=1;
        usb->bufferU[j++]=NOP;				//exit program mode
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=1;					//VDD
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SET_CK_D;
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1);
        usb->read();
        usb->stop=CommonFunctions::GetTickCount();
        CommonFunctions::PrintStatusClear();			//clear status report
        //****************** visualize ********************
        for(i=0;i<4;i+=2)
        {
            CommonFunctions::printMessage4("ID%d: 0x%04X\tID%d: 0x%04X\r\n",i,usb->memCONFIG_W[i],i+1,usb->memCONFIG_W[i+1]);
        }
        CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],usb->memCONFIG_W[0x6]);	//"DevID: 0x%04X\r\n"
        if(usb->memCONFIG_W[0x5]<0x3FFF)
            CommonFunctions::printMessage1(GlobalVariables::strings[S_DevREV],usb->memCONFIG_W[0x5]);	//"DevREV: 0x%04X\r\n"
        deviceSelected=findByID(usb->memCONFIG_W[0x06]);
        if(deviceSelected.deviceInfo)
            return deviceSelected;
    }
    return deviceSelected;
}

DeviceSelected ProgP16::detectDevice16F2()
{
    // usb->StartHVReg(13.2);

    // vdd=2;
    DeviceSelected deviceSelected;
    deviceSelected.deviceInfo=NULL;
    for(int vdd=0; vdd<=2; vdd++)
    {
        int k2=0,z=0,i,j;
        usb->start=CommonFunctions::GetTickCount();
        usb->bufferU[0]=0;
        j=1;
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T1T2;
        usb->bufferU[j++]=1;						//T1=1u
        usb->bufferU[j++]=100;					//T2=100u
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T3;
        usb->bufferU[j++]=2000>>8;
        usb->bufferU[j++]=2000&0xff;
        usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SET_CK_D;
        usb->bufferU[j++]=0x0;
        if(vdd==0)
        {						//VPP before VDD
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=4;				//VPP
            usb->bufferU[j++]=NOP;
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=0x5;			//VDD+VPP
        }
        else if(vdd==1)
        {				//VDD before VPP with delay 50ms
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=1;				//VDD
            usb->bufferU[j++]=SET_PARAMETER;
            usb->bufferU[j++]=SET_T3;
            usb->bufferU[j++]=25000>>8;
            usb->bufferU[j++]=25000&0xff;
            usb->bufferU[j++]=WAIT_T3;
            usb->bufferU[j++]=WAIT_T3;
            usb->bufferU[j++]=SET_PARAMETER;
            usb->bufferU[j++]=SET_T3;
            usb->bufferU[j++]=2000>>8;
            usb->bufferU[j++]=2000&0xff;
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=0x5;			//VDD+VPP
        }
        else if(vdd==2)
        {				//VDD before VPP without delay
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=1;				//VDD
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=0x5;			//VDD+VPP
        }
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(2);
        if(vdd)
            usb->msDelay(50);
        usb->read();


        //****************** read config area ********************
        j=1;
        CommonFunctions::printMessage(GlobalVariables::strings[S_Read_CONFIG_A]);		//read config ...
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        for(i=0x2000; i<=0x200B; i++)
        {		//Config
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF*2/4-2||i==0x200B)
            {		//2B cmd -> 4B data
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)
                    usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(5);
                usb->read();
                for(z=1;z<usb->DIMBUF-2;z++)
                {
                    if(usb->bufferI[z]==READ_DATA_PROG)
                    {
                        usb->memCONFIG_W[k2]=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
                        k2++;
                        z+=2;
                    }
                }
                j=1;
            }
        }
        j=1;
        usb->bufferU[j++]=NOP;				//exit program mode
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=1;					//VDD
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SET_CK_D;
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1);
        usb->read();
        usb->stop=CommonFunctions::GetTickCount();
        //PrintStatusClear();			//clear status report
        //****************** visualize ********************
        for(i=0;i<4;i+=2)
        {
            CommonFunctions::printMessage4("ID%d: ",i, usb->memCONFIG_W[i],i+1, usb->memCONFIG_W[i+1]);
        }
        CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID], usb->memCONFIG_W[0x6]);	//"DevID: 0x%04X\r\n"
        if(usb->memCONFIG_W[0x05]<0x3FFF)
            CommonFunctions::printMessage1(GlobalVariables::strings[S_DevREV],usb->memCONFIG_W[0x05]);	//"DevREV: 0x%04X\r\n"

        CommonFunctions::printMessage1(GlobalVariables::strings[S_ConfigWord],usb->memCONFIG_W[0x07]);	//"Configuration word: 0x%04X\r\n"
        deviceSelected=findByID(usb->memCONFIG_W[0x06]);
        if(deviceSelected.deviceInfo)
            return deviceSelected;
    }
    return deviceSelected;
}


DeviceSelected ProgP16::findByID(quint16 id, bool printMsg)
{
    int max= devices->allDevices.count();
    int step=1;

    DeviceSelected deviceSelected;
    deviceSelected.deviceInfo = NULL;
    if(!id)
        return deviceSelected;
    quint16 idr4 = id>>4;
    for (int i = 0; i < devices->deviceFamilyList.count(); ++i)
    {
        DeviceFamily *deviceFamily=devices->deviceFamilyList.at(i);
        if(deviceFamily->familyName=="PIC")
            for (int j = 0; j < deviceFamily->deviceTypesList.count(); ++j)
            {
                DeviceType *deviceType=deviceFamily->deviceTypesList.at(j);
                if(deviceType->typeName=="PIC 16CF" || deviceType->typeName=="PIC 12CF")
                    for (int k = 0; k < deviceType->devicesList.count(); ++k)
                    {
                        emit onProgressChanged(step++, max);

                        DeviceInfo *deviceInfo=deviceType->devicesList.at(k);
                        if(deviceInfo->id==idr4 || deviceInfo->id==id)
                        {
                            if(printMsg)
                                CommonFunctions::printMessage("Device is:"+deviceInfo->name+", id:"+QString::number(deviceInfo->id, 16).toUpper()+", rev id:"+QString::number(deviceInfo->revMask, 16).toUpper());
                            deviceSelected.deviceInfo = deviceInfo;
                            deviceSelected.familyIndex =i;
                            deviceSelected.typeIndex=j;
                            deviceSelected.infoIndex=k;
                            emit onProgressChanged(max, max);
                            return deviceSelected;
                        }
                    }
            }
    }
    emit onProgressChanged(max, max);
    if(printMsg)
        CommonFunctions::printMessage("Unknown device id:"+QString::number(id, 16).toUpper()+"\r\n");
    return deviceSelected;
}

void ProgP16::Read16Fxxx(int dim,int dim2,int dim3,int vdd)
{
    // usb->StartHVReg(13.2);

    // vdd=2;


    int k=0,k2=0,z=0,i,j,ee2200=0;
    char s[512],t[256],*aux;
    if(dim2<0)
    {
        dim2=-dim2;
        ee2200=1;
    }
    if(dim>0x2000||dim<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_CodeLim]);	//"Code usb->size exceeds limits\r\n"
        return;
    }
    if(dim2>0x400||dim2<0)
    {		//Max 1K
        CommonFunctions::printMessage(GlobalVariables::strings[S_EELim]);	//"EEPROM usb->size exceeds limits\r\n"
        return;
    }
    if(dim3>0x100||dim3<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigLim]);	//"Config area usb->size exceeds limits\r\n"
        return;
    }

    if(dim3<8)
       dim3=8;
    //if(saveLog)    {
    //     OpenLogFile();	//"log.txt"
    //    fprintf(logfile,"Read16Fxxx(%d,%d,%d,%d)\n",dim,dim2,dim3,vdd);
    //}

    usb->sizeW=0x2100+dim2;
    if(usb->memCODE_W)
        free(usb->memCODE_W);
    usb->memCODE_W=(WORD*)malloc(sizeof(WORD)*usb->sizeW);
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    if(vdd==0)
    {						//VPP before VDD
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=4;				//VPP
        usb->bufferU[j++]=NOP;
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x5;			//VDD+VPP
    }
    else if(vdd==1)
    {				//VDD before VPP with delay 50ms
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=1;				//VDD
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T3;
        usb->bufferU[j++]=25000>>8;
        usb->bufferU[j++]=25000&0xff;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T3;
        usb->bufferU[j++]=2000>>8;
        usb->bufferU[j++]=2000&0xff;
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x5;			//VDD+VPP
    }
    else if(vdd==2)
    {				//VDD before VPP without delay
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=1;				//VDD
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x5;			//VDD+VPP
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    if(vdd)
        usb->msDelay(50);

    CommonFunctions::printMessage("start read1");		//read code ...
    usb->read();
    CommonFunctions::printMessage("finish read1");		//read code ...

    //if(saveLog)WriteLogIO();
    //****************** usb->read code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_CodeReading1]);		//read code ...
    CommonFunctions::PrintStatusSetup();
    for(i=0,j=1;i<dim;i++)
    {
        usb->bufferU[j++]=READ_DATA_PROG;
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF*2/4-2||i==dim-1)
        {		//2B cmd -> 4B data
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)
                usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(5);
            usb->read();
            for(z=1;z<usb->DIMBUF-2;z++)
            {
                if(usb->bufferI[z]==READ_DATA_PROG)
                {
                    usb->memCODE_W[k++]=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
                    z+=2;
                }
            }
            emit PrintStatus(GlobalVariables::strings[S_CodeReading],(i*100/(dim+dim2+dim3)),i);	//"Read: %d%%, addr. %03X"
            emit onProgressChanged(i, dim+dim2);
            j=1;
            //if(saveLog)            {
            //    fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
            //   // WriteLogIO();
            //}
            qDebug()<<i<<"from"<<dim;
        }
        emit onProgressChanged(i, dim);
    }
    emit onProgressChanged(dim, dim);
    //PrintStatusEnd();
    if(k!=dim)
    {
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ReadCodeErr],dim,k);	//"Error reading code area, requested %d words, read %d\r\n"
    }
    else
        CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);
    for(i=k;i<0x2000;i++)
        usb->memCODE_W[i]=0x3fff;

    j=1;
    //****************** read config area ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_Read_CONFIG_A]);		//read config ...
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    for(i=0x2000;i<0x2000+dim3;i++)
    {		//Config
        usb->bufferU[j++]=READ_DATA_PROG;
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF*2/4-2||i==0x2000+dim3-1)
        {		//2B cmd -> 4B data
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)
                usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(5);
            usb->read();
            for(z=1;z<usb->DIMBUF-2;z++)
            {
                if(usb->bufferI[z]==READ_DATA_PROG)
                {
                    usb->memCODE_W[0x2000+k2]=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
                    //signal group code
                    //if(k0>=7 && k2< 24+7)

                    if(k2>=7 && k2< 24+7)
                    {
                        usb->memCONFIG[(k2-7)*2]=usb->bufferI[z+2];
                        usb->memCONFIG[(k2-7)*2+1]=usb->bufferI[z+1];
                    }
                    k2++;
                    z+=2;
                }
            }
            emit PrintStatus(GlobalVariables::strings[S_CodeReading],(i-0x2000+dim)*100/(dim+dim2+dim3),i);	//"Read: %d%%, addr. %03X"
            j=1;
            // if(saveLog){
            //    fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k2,k2);	//"i=%d(0x%X), k=%d(0x%X)\n"
            //   // WriteLogIO();
            //}
        }
    }
    if(k2!=dim3)
    {
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigErr],dim3,k2);	//"Error reading config area, requested %d words, read %d\r\n"
    }
    else
        CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);
    for(i=0x2000+k2;i<0x2000+dim3;i++)
       usb->memCODE_W[i]=0x3fff;

    //****************** read eeprom ********************
    if(dim2)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_ReadEE]);		//Read EEPROM ...
        //PrintStatusSetup();
        if(ee2200)
        {		//eeprom a 0x2200
            usb->bufferU[j++]=INC_ADDR_N;
            usb->bufferU[j++]=0xFF;
            usb->bufferU[j++]=INC_ADDR;
        }
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=0x100-dim3;
        for(k2=0,i=0x2100;i<0x2100+dim2;i++)
        {
            usb->bufferU[j++]=READ_DATA_DATA;
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF*2/4-2||i==0x2100+dim2-1)
            {		//2B cmd -> 4B data
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)
                   usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(10);
                usb->read();
                for(z=1;z<usb->DIMBUF-1;z++)
                {
                    if(usb->bufferI[z]==READ_DATA_DATA)
                    {
                        usb->memCODE_W[0x2100+k2++]=usb->bufferI[z+1];
                        z++;
                    }
                }
                emit PrintStatus(GlobalVariables::strings[S_CodeReading],(i-0x2100+dim)*100/(dim+dim2+dim3),i);	//"Read: %d%%, addr. %03X"
                emit onProgressChanged(i, dim+dim2+dim3);
                j=1;
                //if(saveLog)                {
                //    fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k2,k2);	//"i=%d(0x%X), k=%d(0x%X)\n"
                //   // WriteLogIO();
                //}
            }
            emit onProgressChanged(dim+dim2+dim3, dim+dim2+dim3);
        }

        CommonFunctions::PrintStatusEnd();
        if(k2!=dim2)
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_ReadEEErr],dim2,k2);	//"Error reading EE area, ..."
            for(i=0x2100+k2;i<0x2100+dim2;i++)
                usb->memCODE_W[i]=0x3fff;
        }
        else
            CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);
    }
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=1;					//VDD
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
       usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    //PrintStatusClear();			//clear status report
    //****************** visualize ********************
    for(i=0;i<4;i+=2)
    {
        CommonFunctions::printMessage4("ID%d: ",i,usb->memCODE_W[0x2000+i],i+1,usb->memCODE_W[0x2000+i+1]);
    }
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],usb->memCODE_W[0x2006]);	//"DevID: 0x%04X\r\n"
    if(usb->memCODE_W[0x2005]<0x3FFF)
        CommonFunctions::printMessage1(GlobalVariables::strings[S_DevREV],usb->memCODE_W[0x2005]);	//"DevREV: 0x%04X\r\n"
    findByID(usb->memCODE_W[0x2006]);
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ConfigWord],usb->memCODE_W[0x2007]);	//"Configuration word: 0x%04X\r\n"
    if(dim3>8)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_Config2Cal1],usb->memCODE_W[0x2008]);	//"Config2 or Cal1: 0x%04X\r\n"
    }
    if(dim3>9)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_Calib1_2],usb->memCODE_W[0x2009]);	//"Calibration word 1 or 2: 0x%04X\r\n"
    }
    CommonFunctions::printMessage(GlobalVariables::strings[S_CodeMem2]);	//"\r\nCode memory\r\n"
    DisplayCODE16F(dim);
    s[0]=0;
    int valid=0,empty=1;
    if(dim3>8)
    {
        aux=(char*)malloc((dim3/COL+1)*(16+COL*5));
        aux[0]=0;
        empty=1;
        CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigResMem]);	//"\r\nConfig and reserved memory:\r\n"
        for(i=0x2000;i<0x2000+dim3;i+=COL)
        {
            valid=0;
            for(j=i;j<i+COL&&j<0x2000+dim3;j++)
            {
                //        sprintf(t,"%04X ",usb->memCODE_W[j]);
                //      strcat(s,t);
                if(usb->memCODE_W[j]<0x3fff) valid=1;
            }
            if(valid)
            {
                //    sprintf(t,"%04X: %s\r\n",i,s);
                empty=0;
                //  strcat(aux,t);
            }
            s[0]=0;
        }
        if(empty)
            CommonFunctions::printMessage(GlobalVariables::strings[S_Empty]);	//empty
        else
            CommonFunctions::printMessage(aux);
        free(aux);
    }
    if(dim2)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEMem]);	//"\r\nEEPROM memory:\r\n"
        DisplayEE16F(dim2);
    }
    //CommonFunctions::printMessage(GlobalVariables::strings[S_End],(stop-start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"
    //   if(saveLog) CloseLogFile();
    //  PrintStatusClear();			//clear status report

}




void ProgP16::Read16F1xxx(int dim,int dim2,int dim3,int options)
{
    // read 14 bit enhanced PIC
    // dim=program usb->size 	dim2=eeprom usb->size   dim3=config usb->size
    // options:
    //		bit0=0 -> vpp before vdd
    //		bit0=1 -> vdd before vpp
    //		bit1=1 -> LVP programming
    // DevREV@0x8005
    // DevID@0x8006
    // Config1@0x8007
    // Config2@0x8008
    // Calib1@0x8009
    // Calib2@0x800A
    // Calib3@0x800B
    // eeprom@0x0
    int k=0,k2=0,z=0,i,j;
    if(!usb->CheckV33Regulator())
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_noV33reg]);	//Can't find 3.3V expansion board
        return;
    }
    char s[256],t[256],*aux;
    if(dim>0x8000||dim<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_CodeLim]);	//"Code usb->size exceeds limits\r\n"
        return;
    }
    if(dim2>0x400||dim2<0)
    {		//Max 1K
        CommonFunctions::printMessage(GlobalVariables::strings[S_EELim]);	//"EEPROM usb->size exceeds limits\r\n"
        return;
    }
    if(dim3>0x200||dim3<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigLim]);	//"Config area usb->size exceeds limits\r\n"
        return;
    }
    if(dim3<11)dim3=11;		//at least config1-2 + calib1-2
    if(usb->saveLog)
    {
        // OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"Read16F1xxx(%d,%d,%d,%d)\n",dim,dim2,dim3,options);
    }
    if((options&2)==0)
    {				//HV entry
        if(!usb->StartHVReg(8.5))
        {
            CommonFunctions::printMessage(GlobalVariables::strings[S_HVregErr]); //"HV regulator error\r\n"
            return;
        }
        else usb->hvreg=8.5;
    }
    else
        usb->StartHVReg(-1);			//LVP mode, turn off HV
    usb->sizeW=0x8000+dim3;
    usb->sizeEE=dim2;
    if(usb->memCODE_W)
        free(usb->memCODE_W);
    usb->memCODE_W=(WORD*)malloc(sizeof(WORD)*usb->sizeW);
    if(usb->memEE)
        free(usb->memEE);
    usb->memEE=(unsigned char*)malloc(usb->sizeEE);			//EEPROM
    for(i=0;i<usb->sizeEE;i++)
        usb->memEE[i]=0xFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    if((options&2)==0)
    {				//HV entry
        if((options&1)==0)
        {				//VPP before VDD
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=4;				//VPP
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=0x5;			//VDD+VPP
        }
        else{							//VDD before VPP without delay
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=1;				//VDD
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=0x5;			//VDD+VPP
        }
    }
    else
    {			//Low voltage programming
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=4;				//VPP
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=TX16;			//0000 1010 0001 0010 1100 0010 1011 0010 = 0A12C2B2
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=0x0A;
        usb->bufferU[j++]=0x12;
        usb->bufferU[j++]=0xC2;
        usb->bufferU[j++]=0xB2;
        usb->bufferU[j++]=SET_CK_D;		//Clock pulse
        usb->bufferU[j++]=0x4;
        usb->bufferU[j++]=SET_CK_D;
        usb->bufferU[j++]=0x0;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    ////iif(usb->saveLog)WriteLogIO();
    //****************** read code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_CodeReading1]);		//read code ...
    // PrintStatusSetup();
    for(i=0,j=1;i<dim;i++)
    {
        usb->bufferU[j++]=READ_DATA_PROG;
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF*2/4-2||i==dim-1)
        {		//2B cmd -> 4B data
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(5);
            usb->read();
            for(z=1;z<usb->DIMBUF-2;z++)
            {
                if(usb->bufferI[z]==READ_DATA_PROG)
                {
                    usb->memCODE_W[k++]=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
                    z+=2;
                }
            }
            emit PrintStatus(GlobalVariables::strings[S_CodeReading],i*100/(dim+dim2+dim3),i);	//"Read: %d%%, addr. %03X"
            emit onProgressChanged(i, dim+dim2+dim3);
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
                // WriteLogIO();
            }
            emit onProgressChanged(i, dim);
        }
        emit onProgressChanged(dim+dim2+dim3, dim+dim2+dim3);
    }
    CommonFunctions::PrintStatusEnd();
    if(k!=dim)
    {
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ReadCodeErr],dim,k);	//"Error reading code area, requested %d words, read %d\r\n"
    }
    else  CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);
    for(i=k;i<0x8000;i++)
       usb->memCODE_W[i]=0x3fff;
    //****************** read config area ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_Read_CONFIG_A]);		//read config ...
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x8000
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=0xFF;
    for(i=0x8000;i<0x8000+dim3;i++)
    {		//Config
        usb->bufferU[j++]=READ_DATA_PROG;
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF*2/4-2||i==0x8000+dim3-1)
        {		//2B cmd -> 4B data
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(5);
            usb->read();
            for(z=1;z<usb->DIMBUF-2;z++)
            {
                if(usb->bufferI[z]==READ_DATA_PROG)
                {
                    usb->memCODE_W[0x8000+k2++]=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
                    z+=2;
                }
            }
            emit PrintStatus(GlobalVariables::strings[S_CodeReading],(i-0x8000+dim)*100/(dim+dim2+dim3),i);	//"Read: %d%%, ind. %03X"
            emit onProgressChanged(i, dim+dim2+dim3);
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k2,k2);	//"i=%d(0x%X), k=%d(0x%X)\n"
                // WriteLogIO();
            }
            emit onProgressChanged(dim+dim2+dim3, dim+dim2+dim3);
        }
    }
    if(k2!=dim3)
    {
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigErr],dim3,k2);	//"Error reading config area, requested %d words, read %d\r\n"
    }
    else  CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);
    for(i=0x8000+k2;i<0x8000+dim3;i++) usb->memCODE_W[i]=0x3fff;
    //****************** read eeprom ********************
    if(dim2)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_ReadEE]);		//Read EEPROM ...
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x16;		//Reset address
        for(i=k=0;i<dim2;i++)
        {
            usb->bufferU[j++]=READ_DATA_DATA;
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF*2/4-2||i==dim2-1)
            {		//2B cmd -> 4B data
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(5);
                usb->read();
                for(z=1;z<usb->DIMBUF-1;z++)
                {
                    if(usb->bufferI[z]==READ_DATA_DATA)
                    {
                        usb->memEE[k++]=usb->bufferI[z+1];
                        z++;
                    }
                }
                emit PrintStatus(GlobalVariables::strings[S_CodeReading],i*100/(dim+dim2+dim3),i);	//"Read: %d%%, ind. %03X"
                emit onProgressChanged(i, dim+dim2+dim3);
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
                    // WriteLogIO();
                }
                emit onProgressChanged(dim+dim2+dim3, dim+dim2+dim3);
            }
        }
        CommonFunctions::PrintStatusEnd();
        if(i!=dim2)
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_ReadEEErr],dim2,i);	//"Error reading EE area, ..."
            for(;i<dim2;i++)
                usb->memEE[i]=0xff;
            emit onProgressChanged(dim+dim2+dim3, dim+dim2+dim3);
        }
        else
            CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);
    }
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=1;					//VDD
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    CommonFunctions::PrintStatusClear();			//clear status report
    //****************** visualize ********************
    for(i=0;i<4;i+=2)
    {
        CommonFunctions::printMessage4("ID%d: 0x%04X\tID%d: 0x%04X\r\n",i,usb->memCODE_W[0x8000+i],i+1,usb->memCODE_W[0x8000+i+1]);
    }
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],usb->memCODE_W[0x8006]);	//"DevID: 0x%04X\r\n"
    if(usb->memCODE_W[0x8005]<0x3FFF)  CommonFunctions::printMessage1(GlobalVariables::strings[S_DevREV],usb->memCODE_W[0x8005]);	//"DevREV: 0x%04X\r\n"
    findByID(usb->memCODE_W[0x8006]);
    CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWordX],1,usb->memCODE_W[0x8007]);	//"Configuration word %d: 0x%04X\r\n"
    CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWordX],2,usb->memCODE_W[0x8008]);	//"Configuration word %d: 0x%04X\r\n"
    CommonFunctions::printMessage2(GlobalVariables::strings[S_CalibWordX],1,usb->memCODE_W[0x8009]);	//"Calibration word %d: 0x%04X\r\n"
    CommonFunctions::printMessage2(GlobalVariables::strings[S_CalibWordX],2,usb->memCODE_W[0x800A]);	//"Calibration word %d: 0x%04X\r\n"
    if(usb->memCODE_W[0x800B]<0x3FFF)  CommonFunctions::printMessage2(GlobalVariables::strings[S_CalibWordX],3,usb->memCODE_W[0x800B]);	//"Calibration word %d: 0x%04X\r\n"
    CommonFunctions::printMessage(GlobalVariables::strings[S_CodeMem2]);	//"\r\nCode memory:\r\n"
    //  DisplayCODE16F(dim);
    if(dim3>11)
    {
        int valid=0,empty=1;
        s[0]=0;
        aux=(char*)malloc(dim3/COL*(16+COL*5));
        aux[0]=0;
        empty=1;
        CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigResMem]);	//"\r\nConfig and reserved memory:\r\n"
        for(i=0x8000;i<0x8000+dim3;i+=COL)
        {
            valid=0;
            for(j=i;j<i+COL&&j<0x8000+dim3;j++)
            {
                sprintf(t,"%04X ",usb->memCODE_W[j]);
                strcat(s,t);
                if(usb->memCODE_W[j]<0x3fff) valid=1;
            }
            if(valid)
            {
                sprintf(t,"%04X: %s\r\n",i,s);
                empty=0;
                strcat(aux,t);
            }
            s[0]=0;
        }
        if(empty)  CommonFunctions::printMessage(GlobalVariables::strings[S_Empty]);	//empty
        else  CommonFunctions::printMessage(aux);
        free(aux);
    }
    //  if(dim2) DisplayEE();	//visualize
    CommonFunctions::printMessage1(GlobalVariables::strings[S_End],(usb->stop-usb->start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"
    // if(usb->saveLog) CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report

}


void ProgP16::Write12F6xx(int dim,int dim2)
{

    // write 14 bit PIC
    // dim=program usb->size 	dim2=eeprom usb->size
    // vpp before vdd
    // DevID@0x2006
    // Config@0x2007
    // Calib1@0x2008 (save)
    // Calib2@0x2009 (save)
    // eeprom@0x2100
    // erase: BULK_ERASE_PROG (1001) +10ms
    // write:LOAD_DATA_PROG (0010) + BEGIN_PROG (1000) + 4ms
    // eeprom:	BULK_ERASE_DATA (1011) + 16ms
    //			LOAD_DATA_DATA (0011) + BEGIN_PROG (1000) + 8ms
    // verify during write

    int err=0;
    WORD devID=0x3fff,calib1=0x3fff,calib2=0x3fff;
    int k=0,z=0,i,j,w;
    if(usb->sizeW<0x2007)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW3]);	//"Can't find CONFIG (0x2007)\r\nEnd\r\n"
        return;
    }
    if(usb->load_calibword)
    {
        if(usb->sizeW>0x2009) usb->load_calibword=2;
        else if(usb->sizeW>0x2008) usb->load_calibword=1;
        else{
            CommonFunctions::printMessage(GlobalVariables::strings[S_NoCalibW]);	//"Can't find calibration data\r\n"
            usb->load_calibword=0;
        }
    }
    if(dim2)
    {
        if(usb->sizeW<0x2100)
        {
            dim2=0;
            CommonFunctions::printMessage(GlobalVariables::strings[S_NoEEMem]);	//"Can't find EEPROM data\r\n"
        }
        else if(dim2>usb->sizeW-0x2100) dim2=usb->sizeW-0x2100;
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write12F6xx(%d,%d)\n",dim,dim2);
    }
    for(i=0;i<0x2009&&i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x06;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Calib1
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Calib2
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=10000>>8;
    usb->bufferU[j++]=10000&0xff;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    findByID(devID);
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    calib1=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    if(calib1<0x3fff)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_CalibWord1],calib1);	//"Calib1: 0x%04X\r\n"
    }
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    calib2=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    if(calib2<0x3fff)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_CalibWord2],calib2);	//"Calib2: 0x%04X\r\n"
    }
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=WAIT_T3;		//necessary when erasing fully written 16F62xA
    //not mentioned in the prog spec!
    if(usb->programID||usb->load_calibword||usb->ICDenable)
    {
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        if(usb->load_calibword)
        {
            usb->bufferU[j++]=INC_ADDR_N;
            if(usb->load_calibword==2)  usb->bufferU[j++]=0x09;
            else  usb->bufferU[j++]=0x08;
        }
    }
    usb->bufferU[j++]=LOAD_DATA_PROG;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=BULK_ERASE_PROG;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
    if(dim2)
    {
        usb->bufferU[j++]=BULK_ERASE_DATA;
        usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
    }
    usb->bufferU[j++]=EN_VPP_VCC;		//exit program mode
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=4000>>8;
    usb->bufferU[j++]=4000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(50);
    usb->read();
    // if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    CommonFunctions::PrintStatusSetup();
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 4ms
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-12||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*5+2);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-5;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+3]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+4]<<8)+usb->bufferI[z+5])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+4]<<8)+usb->bufferI[z+5]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        usb->err++;
                        if(usb->max_err&&usb->err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],usb->err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=6;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d,\n"
                // WriteLogIO();
            }
        }
        emit onProgressChanged(i, dim);
    }
    err+=i-k;
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write eeprom ********************
    if(dim2)
    {
        int err_e=0;
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Writing EEPROM ... "
        CommonFunctions::PrintStatusSetup();
        j=1;
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T3;
        usb->bufferU[j++]=8000>>8;
        usb->bufferU[j++]=8000&0xff;
        usb->bufferU[j++]=BULK_ERASE_DATA;
        usb->bufferU[j++]=WAIT_T3;			// delay T3=8ms
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=INC_ADDR_N;		//use only INC_ADDR_N so verification does not look at it
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=0x2100-0x2001;		//EEPROM area: counter at 0x2100
        for(w=2,i=k=0x2100;i<0x2100+dim2;i++)
        {
            if(usb->memCODE_W[i]<0xff)
            {
                usb->bufferU[j++]=LOAD_DATA_DATA;
                usb->bufferU[j++]=usb->memCODE_W[i]&0xff;
                usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=6ms min
                usb->bufferU[j++]=WAIT_T3;				//Tprogram 8ms
                usb->bufferU[j++]=READ_DATA_DATA;
                w++;
            }
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF-12||i==0x2100+dim2-1)
            {
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],(i-0x2100+dim)*100/(dim+dim2),i);	//"Writing: %d%%, add. %03X"
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(w*9+2);
                w=0;
                usb->read();
                for(z=1;z<usb->DIMBUF-4;z++)
                {
                    if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0xff) k++;
                    else if(usb->bufferI[z]==LOAD_DATA_DATA&&usb->bufferI[z+3]==READ_DATA_DATA)
                    {
                        if (usb->memCODE_W[k]!=usb->bufferI[z+4])
                        {
                            CommonFunctions::printMessage("\r\n");
                            CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError3],k,usb->memCODE_W[k],usb->bufferI[z+4]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                            err_e++;
                            if(usb->max_err&&usb->err+err_e>usb->max_err)
                            {
                                CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],usb->err+err_e);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                                CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                                i=0x2200;
                                z=usb->DIMBUF;
                            }
                        }
                        k++;
                        z+=5;
                    }
                }
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                    // WriteLogIO();
                }
            }
        }
        err_e+=i-k;
        err+=err_e;
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_e);	//"completed, %d errors\r\n"
    }
    //****************** write ID, CONFIG, CALIB ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    int ICDgoto=0x2800+(usb->ICDaddr&0x7FF);		//GOTO ICD routine (0x28xx)
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=4000>>8;
    usb->bufferU[j++]=4000&0xff;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 4ms
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=4;
    }
    if(usb->ICDenable)
    {		//write a GOTO ICD routine (0x28xx)
        usb->bufferU[j++]=LOAD_DATA_PROG;
        usb->bufferU[j++]=ICDgoto>>8;		//MSB
        usb->bufferU[j++]=ICDgoto&0xFF;			//LSB
        usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
        usb->bufferU[j++]=WAIT_T3;				//Tprogram
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007
    usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
    usb->bufferU[j++]=WAIT_T3;				//Tprogram 4ms
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=INC_ADDR;
    if(usb->load_calibword)
    {
        usb->bufferU[j++]=LOAD_DATA_PROG;			//Calib word 1
        usb->bufferU[j++]=usb->memCODE_W[0x2008]>>8;		//MSB
        usb->bufferU[j++]=usb->memCODE_W[0x2008]&0xff;		//LSB
        usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
        usb->bufferU[j++]=WAIT_T3;				//Tprogram 4ms
        usb->bufferU[j++]=READ_DATA_PROG;
        usb->bufferU[j++]=INC_ADDR;
        usb->bufferU[j++]=LOAD_DATA_PROG;			//Calib word 2
        usb->bufferU[j++]=usb->memCODE_W[0x2009]>>8;		//MSB
        usb->bufferU[j++]=usb->memCODE_W[0x2009]&0xff;		//LSB
        usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
        usb->bufferU[j++]=WAIT_T3;				//Tprogram 4ms
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(35);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    if(usb->ICDenable)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (ICDgoto!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage4(GlobalVariables::strings[S_ICDErr],0x2004,i,ICDgoto,(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ICD (0x%X): written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    if(usb->load_calibword)
    {
        for(z+=6;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2008]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_Calib1Err],usb->memCODE_W[0x2008],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing Calib1: written %04X, read %04X\r\n"
            err_c++;
        }
        if(usb->load_calibword==2)
        {
            for(z+=6;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
            if (usb->memCODE_W[0x2009]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
            {
                CommonFunctions::printMessage("\r\n");
                CommonFunctions::printMessage2(GlobalVariables::strings[S_Calib2Err],usb->memCODE_W[0x2009],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing Calib2: written %04X, read %04X\r\n"
                err_c++;
            }
        }
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d \n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    // CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    // if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report

}





void ProgP16::Write16F8x(int dim,int dim2)
{
    // write 14 bit PIC
    // dim=program usb->size 	dim2=eeprom usb->size
    // vdd + 50ms + vdd&vpp
    // DevID@0x2006
    // Config@0x2007
    // eeprom@0x2100
    // erase if protected:
    // LOAD_CONF (0)(0x3FFF) + CUST_CMD (0001) + CUST_CMD (0111)
    // + BEGIN_PROG (1000) + 10ms
    // + CUST_CMD (0001) + CUST_CMD (0111)
    // erase if not protected and DevID=16F84A:
    // LOAD_DATA_PROG (0010)(0x3FFF) + BULK_ERASE_PROG (1001) +10ms
    // LOAD_DATA_DATA (0011)(0xFF) + BULK_ERASE_DATA (1011) + BEGIN_PROG (1000) + 10ms
    // erase if not protected and DevID!=16F84A:
    // LOAD_DATA_PROG (0010)(0x3FFF) + CUST_CMD (0001) + CUST_CMD (0111)
    // + BEGIN_PROG (1000) + 10ms + CUST_CMD (0001) + CUST_CMD (0111)
    // LOAD_DATA_DATA (0011)(0xFF) + CUST_CMD (0001) + CUST_CMD (0111)
    // + BEGIN_PROG (1000) + 10ms + CUST_CMD (0001) + CUST_CMD (0111)
    // write: LOAD_DATA_PROG (0010) + BEGIN_PROG (1000) + 20ms or 8ms(16F84A)
    // write eeprom: LOAD_DATA_DATA (0011) + BEGIN_PROG (1000) + 20ms or 8ms(16F84A)
    // verify during write

    int err=0;
    WORD devID,config;
    int k=0,z=0,i,j,w,r;
    if(usb->sizeW<0x2007)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW3]);	//"Can't find CONFIG (0x2007)\r\nEnd\r\n"
        return;
    }
    if(dim2)
    {
        if(usb->sizeW<0x2100)
        {
            dim2=0;
            CommonFunctions::printMessage(GlobalVariables::strings[S_NoEEMem]);	//"Can't find EEPROM data\r\n"
        }
        else if(dim2>usb->sizeW-0x2100) dim2=usb->sizeW-0x2100;
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write16F8x(%d,%d)\n",dim,dim2);
    }
    for(i=0;i<0x2009&&i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=25000>>8;
    usb->bufferU[j++]=25000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x06;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Config
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;			//50ms
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(140);
    usb->read();
    // if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    findByID(devID);
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    config=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ConfigWord],config);	//"Config word: 0x%04X\r\n"
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=10000>>8;
    usb->bufferU[j++]=10000&0xff;
    if(config<0x3FF0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_ProtErase]);	//"Override write protection\r\n"
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0x3F;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=0x07;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x01;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x07;
        usb->bufferU[j++]=BEGIN_PROG;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x01;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x07;
    }
    else if(devID>>5==0x2B)
    {			//16F84A
        usb->bufferU[j++]=LOAD_DATA_PROG;
        usb->bufferU[j++]=0x3f;				//MSB
        usb->bufferU[j++]=0xff;				//LSB
        usb->bufferU[j++]=BULK_ERASE_PROG;
        usb->bufferU[j++]=BEGIN_PROG;
        usb->bufferU[j++]=WAIT_T3;
        if(dim2)
        {
            usb->bufferU[j++]=LOAD_DATA_DATA;
            usb->bufferU[j++]=0xff;				//LSB
            usb->bufferU[j++]=BULK_ERASE_DATA;
            usb->bufferU[j++]=BEGIN_PROG;
            usb->bufferU[j++]=WAIT_T3;
        }
    }
    else{								//altri
        usb->bufferU[j++]=LOAD_DATA_PROG;
        usb->bufferU[j++]=0x3f;				//MSB
        usb->bufferU[j++]=0xff;				//LSB
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x01;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x07;
        usb->bufferU[j++]=BEGIN_PROG;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x01;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x07;
        if(dim2)
        {
            usb->bufferU[j++]=LOAD_DATA_DATA;	//EEPROM:  spec error?
            usb->bufferU[j++]=0xff;				//LSB
            usb->bufferU[j++]=CUST_CMD;
            usb->bufferU[j++]=0x01;
            usb->bufferU[j++]=CUST_CMD;
            usb->bufferU[j++]=0x07;
            usb->bufferU[j++]=BEGIN_PROG;
            usb->bufferU[j++]=WAIT_T3;
            usb->bufferU[j++]=CUST_CMD;
            usb->bufferU[j++]=0x01;
            usb->bufferU[j++]=CUST_CMD;
            usb->bufferU[j++]=0x07;
        }
    }
    if(!usb->programID)
    {					//back to program memory
        usb->bufferU[j++]=NOP;				//exit program mode
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SET_CK_D;
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=WAIT_T3;			//50ms
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
        usb->bufferU[j++]=0x5;
    }
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    r=(devID>>5==0x2B)?8000:20000;
    usb->bufferU[j++]=r>>8;
    usb->bufferU[j++]=r&0xff;
    usb->bufferU[j++]=FLUSH;
    r/=1000;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(60);
    if(!usb->programID) usb->msDelay(80);
    usb->read();
    // if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    //PrintStatusSetup();
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed
            usb->bufferU[j++]=WAIT_T3;				//Tprogram
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-10||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*r+4);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-5;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+3]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+4]<<8)+usb->bufferI[z+5])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+4]<<8)+usb->bufferI[z+5]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=6;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                // WriteLogIO();
            }
        }
    }
    err+=i-k;
    // PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write eeprom ********************
    if(dim2)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Writing EEPROM ... "
        //PrintStatusSetup();
        j=1;
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=INC_ADDR_N;		//use only INC_ADDR_N so verification does not look at it
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=INC_ADDR_N;	//EEPROM: counter at 0x2100
        usb->bufferU[j++]=1;
        for(w=0,i=k=0x2100;i<0x2100+dim2;i++)
        {
            if(usb->memCODE_W[i]<0xff)
            {
                usb->bufferU[j++]=LOAD_DATA_DATA;
                usb->bufferU[j++]=usb->memCODE_W[i]&0xff;
                usb->bufferU[j++]=BEGIN_PROG;			//internally timed
                usb->bufferU[j++]=WAIT_T3;				//Tprogram
                usb->bufferU[j++]=READ_DATA_DATA;
                w++;
            }
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF-10||i==0x2100+dim2-1)
            {
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],(i-0x2100+dim)*100/(dim+dim2),i);	//"Writing: %d%%, add. %03X"
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(w*r+5);
                w=0;
                usb->read();
                for(z=1;z<usb->DIMBUF-4;z++)
                {
                    if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0xff) k++;
                    else if(usb->bufferI[z]==LOAD_DATA_DATA&&usb->bufferI[z+3]==READ_DATA_DATA)
                    {
                        if (usb->memCODE_W[k]!=usb->bufferI[z+4])
                        {
                            CommonFunctions::printMessage("\r\n");
                            CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError3],k,usb->memCODE_W[k],usb->bufferI[z+4]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                            err++;
                            if(usb->max_err&&err>usb->max_err)
                            {
                                CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                                CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                                i=0x2200;
                                z=usb->DIMBUF;
                            }
                        }
                        k++;
                        z+=5;
                    }
                }
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                    // WriteLogIO();
                }
            }
        }
        err+=i-k;
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],i-k);	//"completed, %d errors\r\n"
    }
    //****************** write ID, CONFIG ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed
            usb->bufferU[j++]=WAIT_T3;				//Tprogram
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=3;
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=7;
    }
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007
    usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG;			//internally timed
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(35);
    if(usb->programID) usb->msDelay(90);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d \n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    // CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report

}




void ProgP16::Write12F61x(int dim,int d,int d2)
{
    // write 14 bit PIC
    // dim=program usb->size
    // d not used
    // vpp before vdd
    // DevREV@0x2005
    // DevID@0x2006
    // Config@0x2007
    // Calib1@0x2008 (save)
    // Calib2@0x2009
    // erase: BULK_ERASE_PROG (1001) +10ms
    // write: LOAD_DATA_PROG (0010) + BEGIN_PROG2 (11000) + 4ms + END_PROG (1010)
    // verify during write

    int err=0;
    WORD devID=0x3fff,devREV=0x3fff,calib1=0x3fff,calib2=0x3fff;
    int k=0,z=0,i,j,w;
    if(usb->sizeW<0x2007)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW3]);	//"Can't find CONFIG (0x2007)\r\nEnd\r\n"
        return;
    }
    if(usb->load_calibword)
    {
        if(usb->sizeW>0x2008) usb->load_calibword=1;
        else{
            CommonFunctions::printMessage(GlobalVariables::strings[S_NoCalibW]);	//"Can't find calibration data\r\n"
            usb->load_calibword=0;
        }
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write12F61x(%d)\n",dim);
    }
    for(i=0;i<0x2009&&i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x05;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevREV
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Calib1
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Calib2
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=10000>>8;
    usb->bufferU[j++]=10000&0xff;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devREV=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    if(devREV<0x3FFF)  CommonFunctions::printMessage1(GlobalVariables::strings[S_DevREV],devREV);	//"DevREV: 0x%04X\r\n"
    findByID(devID);
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    calib1=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    calib2=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    if(calib1<0x3fff)  CommonFunctions::printMessage1(GlobalVariables::strings[S_CalibWord1],calib1);	//"Calib1: 0x%04X\r\n"
    if(calib2<0x3fff)  CommonFunctions::printMessage1(GlobalVariables::strings[S_CalibWord2],calib2);	//"Calib2: 0x%04X\r\n"
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    if(usb->programID||usb->load_calibword)
    {
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        if(usb->load_calibword)
        {
            usb->bufferU[j++]=INC_ADDR_N;
            usb->bufferU[j++]=0x08;
        }
    }
    usb->bufferU[j++]=BULK_ERASE_PROG;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
    usb->bufferU[j++]=EN_VPP_VCC;		//exit program mode
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms after exiting program mode
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=4000>>8;
    usb->bufferU[j++]=4000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(40);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    // PrintStatusSetup();
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG2;			//externally timed, T=3ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 4ms
            usb->bufferU[j++]=END_PROG;
            usb->bufferU[j++]=WAIT_T2;				//Tdischarge 100us
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-12||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*5+2);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-7;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+5]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+6]<<8)+usb->bufferI[z+7])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+4]<<8)+usb->bufferI[z+5]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=8;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d,\n"
                // WriteLogIO();
            }
        }
    }
    err+=i-k;
    //PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write ID, CONFIG, CALIB ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG2;			//externally timed, T=3ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 4ms
            usb->bufferU[j++]=END_PROG;
            usb->bufferU[j++]=WAIT_T2;				//Tdischarge 100us
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=3;
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=7;
    }
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007
    usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG2;			//externally timed, T=3ms min
    usb->bufferU[j++]=WAIT_T3;				//Tprogram 4ms
    usb->bufferU[j++]=END_PROG;
    usb->bufferU[j++]=WAIT_T2;				//Tdischarge 100us
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=INC_ADDR;
    if(usb->load_calibword)
    {
        usb->bufferU[j++]=LOAD_DATA_PROG;			//Calib word 1
        usb->bufferU[j++]=usb->memCODE_W[0x2008]>>8;		//MSB
        usb->bufferU[j++]=usb->memCODE_W[0x2008]&0xff;		//LSB
        usb->bufferU[j++]=BEGIN_PROG2;			//externally timed, T=3ms min
        usb->bufferU[j++]=WAIT_T3;				//Tprogram 4ms
        usb->bufferU[j++]=END_PROG;
        usb->bufferU[j++]=WAIT_T2;				//Tdischarge 100us
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(35);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=8;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    if(usb->load_calibword)
    {
        for(z+=8;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2008]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_Calib1Err],usb->memCODE_W[0x2008],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing Calib1: written %04X, read %04X\r\n"
            err_c++;
        }
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d \n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    // CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    // if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report




}



void ProgP16::Write12F62x(int dim,int dim2)
{

    // write 14 bit PIC
    // dim=program usb->size 	dim2=eeprom usb->size
    // vpp before vdd
    // salva OSCCAL a dim-1
    // CONFIG@0x2007 includes 2  calibration bits
    // DevID@0x2006
    // eeprom@0x2100
    // erase: BULK_ERASE_PROG (1001) +10ms
    // write: LOAD_DATA_PROG (0010) + BEGIN_PROG (1000) + 3ms
    // eeprom: BULK_ERASE_DATA (1011) + 9ms
    // LOAD_DATA_DATA (0011) + BEGIN_PROG (1000) + 6ms
    // verify during write

    int err=0;
    WORD devID,config,osccal;
    int k=0,z=0,i,j,w;
    if(usb->sizeW<0x2007)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW3]);	//"Can't find CONFIG (0x2007)\r\nEnd\r\n"
        return;
    }
    if(dim2)
    {
        if(usb->sizeW<0x2100)
        {
            dim2=0;
            CommonFunctions::printMessage(GlobalVariables::strings[S_NoEEMem]);	//"Can't find EEPROM data\r\n"
        }
        else if(dim2>usb->sizeW-0x2100) dim2=usb->sizeW-0x2100;
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write12F62x(%d,%d)\n",dim,dim2);
    }
    for(i=0;i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    for(i=0;i<dim-0xff;i+=0xff)
    {
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=0xff;
    }
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=dim-1-i;
    usb->bufferU[j++]=READ_DATA_PROG;	// OSCCAL
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x06;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Config
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=10000>>8;
    usb->bufferU[j++]=10000&0xff;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    osccal=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    config=(usb->bufferI[z+1]<<8)+usb->bufferI[z+1];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    findByID(devID);
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    if(usb->programID)
    {
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
    }
    usb->bufferU[j++]=BULK_ERASE_PROG;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
    usb->bufferU[j++]=EN_VPP_VCC;		//exit program mode
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms after exiting program mode
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=3000>>8;
    usb->bufferU[j++]=3000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(40);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    //  PrintStatusSetup();
    if(!usb->load_osccal) usb->memCODE_W[dim-1]=osccal;	//backup osccal
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 3ms
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-10||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*6.5);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-5;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+3]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+4]<<8)+usb->bufferI[z+5])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+4]<<8)+usb->bufferI[z+5]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=6;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                // WriteLogIO();
            }
        }
    }
    CommonFunctions::PrintStatusEnd();
    err+=i-k;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write eeprom ********************
    if(dim2)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Writing EEPROM ... "
        //PrintStatusSetup();
        j=1;
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T3;
        usb->bufferU[j++]=6000>>8;
        usb->bufferU[j++]=6000&0xff;
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=INC_ADDR_N;		//use only INC_ADDR_N so verification does not look at it
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=INC_ADDR_N;	//EEPROM: counter at 0x2100
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=BULK_ERASE_DATA;
        usb->bufferU[j++]=WAIT_T3;			// delay=12ms
        usb->bufferU[j++]=WAIT_T3;
        for(w=3,i=k=0x2100;i<0x2100+dim2;i++)
        {
            if(usb->memCODE_W[i]<0xff)
            {
                usb->bufferU[j++]=LOAD_DATA_DATA;
                usb->bufferU[j++]=usb->memCODE_W[i]&0xff;
                usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=6ms
                usb->bufferU[j++]=WAIT_T3;				//Tprogram 6ms
                usb->bufferU[j++]=READ_DATA_DATA;
                w++;
            }
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF-10||i==0x2100+dim2-1)
            {
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],(i-0x2100+dim)*100/(dim+dim2),i);	//"Writing: %d%%, add. %03X"
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(w*7+2);
                w=0;
                usb->read();
                for(z=1;z<usb->DIMBUF-4;z++)
                {
                    if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0xff) k++;
                    else if(usb->bufferI[z]==LOAD_DATA_DATA&&usb->bufferI[z+3]==READ_DATA_DATA)
                    {
                        if (usb->memCODE_W[k]!=usb->bufferI[z+4])
                        {
                            CommonFunctions::printMessage("\r\n");
                            CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError3],k,usb->memCODE_W[k],usb->bufferI[z+4]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                            err++;
                            if(usb->max_err&&err>usb->max_err)
                            {
                                CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                                CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                                i=0x2200;
                                z=usb->DIMBUF;
                            }
                        }
                        k++;
                        z+=5;
                    }
                }
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                    // WriteLogIO();
                }
            }
        }
        err+=i-k;
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],i-k);	//"completed, %d errors\r\n"
    }
    //****************** write ID, CONFIG, CALIB ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 3ms
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=3;
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=7;
    }
    if(!usb->load_calibword)	usb->memCODE_W[0x2007]=(usb->memCODE_W[0x2007]&0xfff)+(config&0x3000);
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007
    usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms
    usb->bufferU[j++]=WAIT_T3;				//Tprogram 3ms
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(35);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d \n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;		//exit program mode
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    // CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    // if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report


}



void ProgP16::Write16F7x(int dim,int vdd)
{

    // dim=program usb->size
    // write 14 bit PIC
    // vdd=0  vdd +50ms before vpp
    // vdd=1  vdd before vpp
    // DevID@0x2006
    // Config@0x2007
    // Config2@0x2008
    // erase: BULK_ERASE_PROG (1001) +30ms
    // write:LOAD_DATA_PROG (0010) + BEGIN_PROG (1000) + 1ms + END_PROG2(1110)
    // verify during write

    int err=0;
    WORD devID=0x3fff;
    int k=0,z=0,i,j,w;
    if(usb->sizeW<0x2007)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW3]);	//"Can't find CONFIG (0x2007)\r\nEnd\r\n"
        return;
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write16F7x(%d,%d)\n",dim,vdd);
    }
    for(i=0;i<0x2009&&i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=10000>>8;
    usb->bufferU[j++]=10000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    if(vdd==0)
    {
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
    }
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x06;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(20);
    if(vdd==0) usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    findByID(devID);
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    //enter program mode
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    if(vdd==0)
    {
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
    }
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=BULK_ERASE_PROG;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
    usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
    usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(50);
    if(vdd==0) usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    j=1;
    //enter program mode
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    if(vdd==0)
    {
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
    }
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=1000>>8;
    usb->bufferU[j++]=1000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    if(vdd==0) usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    //PrintStatusSetup();
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//externally timed, T=1ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 1ms min
            usb->bufferU[j++]=END_PROG2;
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-10||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*1.5+2);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-6;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+4]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+5]<<8)+usb->bufferI[z+6])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+5]<<8)+usb->bufferI[z+6]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=7;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d,\n"
                // WriteLogIO();
            }
        }
    }
    err+=i-k;
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write ID, CONFIG, CALIB ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//externally timed, T=1ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 1ms
            usb->bufferU[j++]=END_PROG2;
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=3;
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=7;
    }
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007
    usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG;			//externally timed, T=1ms min
    usb->bufferU[j++]=WAIT_T3;				//Tprogram 1ms
    usb->bufferU[j++]=END_PROG2;
    usb->bufferU[j++]=WAIT_T3;				//Tprogram 1ms
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=INC_ADDR;
    if(usb->sizeW>0x2008&&usb->memCODE_W[0x2008]<0x3fff)
    {
        usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 2 0x2008
        usb->bufferU[j++]=usb->memCODE_W[0x2008]>>8;		//MSB
        usb->bufferU[j++]=usb->memCODE_W[0x2008]&0xff;		//LSB
        usb->bufferU[j++]=BEGIN_PROG;			//externally timed, T=1ms min
        usb->bufferU[j++]=WAIT_T3;				//Tprogram 1ms
        usb->bufferU[j++]=END_PROG2;
        usb->bufferU[j++]=WAIT_T3;				//Tprogram 1ms
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    if(usb->sizeW>0x2008&&usb->memCODE_W[0x2008]<0x3fff)
    {
        for(z+=6;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if(~usb->memCODE_W[0x2008]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
        {	//error if written 0 and read 1 (~W&R)
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2008],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
            err_c++;
        }
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d \n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    // CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    // if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report
}






void ProgP16::Write16F71x(int dim,int vdd)
{

    // write 14 bit PIC
    // dim=program usb->size
    // vdd=0  vdd +50ms before vpp
    // vdd=1  vdd before vpp
    // DevID@0x2006
    // Config@0x2007
    // erase: BULK_ERASE_PROG (1001) +6ms
    // write:LOAD_DATA_PROG (0010) + BEGIN_PROG2 (11000) + 2ms + END_PROG2(1110)
    // verify during write

    int err=0;
    WORD devID=0x3fff;
    int k=0,z=0,i,j,w;
    if(usb->sizeW<0x2007)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW3]);	//"Can't find CONFIG (0x2007)\r\nEnd\r\n"
        return;
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write16F71x(%d,%d)\n",dim,vdd);
    }
    for(i=0;i<0x2009&&i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=10000>>8;
    usb->bufferU[j++]=10000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    if(vdd==0)
    {
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
    }
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x06;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(20);
    if(vdd==0) usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    findByID(devID);
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    //enter program mode
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    if(vdd==0)
    {
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
    }
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    if(usb->programID)
    {
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
    }
    usb->bufferU[j++]=BULK_ERASE_PROG;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    //enter program mode
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    if(vdd==0)
    {
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
        usb->bufferU[j++]=WAIT_T3;			//delay between vdd and vpp
    }
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(30);
    if(vdd==0) usb->msDelay(100);
    usb->read();
    // if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    // PrintStatusSetup();
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG2;			//externally timed, T=1ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 1ms min
            usb->bufferU[j++]=END_PROG2;
            usb->bufferU[j++]=WAIT_T2;				//Tdischarge 100us
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-11||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*2.5+2);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-7;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+5]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+6]<<8)+usb->bufferI[z+7])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+6]<<8)+usb->bufferI[z+7]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=8;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d,\n"
                // WriteLogIO();
            }
        }
        emit onProgressChanged(i, dim);
    }
    emit onProgressChanged(dim, dim);
    err+=i-k;
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write ID, CONFIG, CALIB ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG2;			//externally timed, T=1ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 1ms
            usb->bufferU[j++]=END_PROG2;
            usb->bufferU[j++]=WAIT_T2;				//Tdischarge 100us
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=3;
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=7;
    }
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007
    usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG2;			//externally timed, T=1ms min
    usb->bufferU[j++]=WAIT_T3;				//Tprogram 1ms
    usb->bufferU[j++]=END_PROG2;
    usb->bufferU[j++]=WAIT_T2;				//Tdischarge 100us
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=INC_ADDR;
    if(usb->sizeW>0x2008&&usb->memCODE_W[0x2008]<0x3fff)
    {
        usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 2 0x2008
        usb->bufferU[j++]=usb->memCODE_W[0x2008]>>8;		//MSB
        usb->bufferU[j++]=usb->memCODE_W[0x2008]&0xff;		//LSB
        usb->bufferU[j++]=BEGIN_PROG2;			//externally timed, T=1ms min
        usb->bufferU[j++]=WAIT_T3;				//Tprogram 1ms
        usb->bufferU[j++]=END_PROG2;
        usb->bufferU[j++]=WAIT_T2;				//Tdischarge 100us
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    if(usb->sizeW>0x2008&&usb->memCODE_W[0x2008]<0x3fff)
    {
        for(z+=6;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if(~usb->memCODE_W[0x2008]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
        {	//error if written 0 and read 1 (~W&R)
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2008],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
            err_c++;
        }
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d \n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    // CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    // if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report




}






void ProgP16::Write16F72x(int dim,int d,int d2)
{
    // write 14 bit PIC
    // dim=program usb->size
    // d not used
    // vpp before vdd
    // DevID@0x2006
    // Config@0x2007
    // Config2@0x2008 (not used on LF devices)
    // erase: BULK_ERASE_PROG (1001) +6ms
    // write:LOAD_DATA_PROG (0010) + BEGIN_PROG (1000) + 2.5ms
    // verify during write

    int err=0;
    WORD devID=0x3fff;
    int k=0,z=0,i,j,w;
    if(! usb->CheckV33Regulator())
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_noV33reg]);	//Can't find 3.3V expansion board
        return;
    }
    if(!usb->StartHVReg(8.5))
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_HVregErr]); //"HV regulator error\r\n"
        return;
    }
    if(usb->sizeW<0x2008)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW3]);	//"Can't find CONFIG (0x2007)\r\nEnd\r\n"
        return;
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write16F72x(%d)\n",dim);
    }
    for(i=0;i<0x2009&&i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=WAIT_T2;
    usb->bufferU[j++]=WAIT_T2;
    usb->bufferU[j++]=WAIT_T2;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x06;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=CUST_CMD;
    usb->bufferU[j++]=0x16;		//Reset address
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=6000>>8;
    usb->bufferU[j++]=6000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(3);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    findByID(devID);
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    if(usb->programID||usb->ICDenable)
    {
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
    }
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=BULK_ERASE_PROG;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=6ms
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=CUST_CMD;
    usb->bufferU[j++]=0x16;		//Reset address
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2500>>8;
    usb->bufferU[j++]=2500&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(18);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    //PrintStatusSetup();
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 3ms
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-12||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*2.5+2);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-5;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+3]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+4]<<8)+usb->bufferI[z+5])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+4]<<8)+usb->bufferI[z+5]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=6;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d,\n"
                // WriteLogIO();
            }
            emit onProgressChanged(i, dim);
        }
        emit onProgressChanged(dim, dim);
    }
    err+=i-k;
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write ID, CONFIG, CALIB ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    int ICDgoto=0x2800+(usb->ICDaddr&0x7FF);		//GOTO ICD routine (0x28xx)
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=6000>>8;
    usb->bufferU[j++]=6000&0xff;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 3ms
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=4;
    }
    if(usb->ICDenable)
    {		//write a GOTO ICD routine (0x28xx)
        usb->bufferU[j++]=LOAD_DATA_PROG;
        usb->bufferU[j++]=ICDgoto>>8;		//MSB
        usb->bufferU[j++]=ICDgoto&0xFF;			//LSB
        usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
        usb->bufferU[j++]=WAIT_T3;				//Tprogram 3ms
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007
    usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG;				//internally timed, T=5ms min
    usb->bufferU[j++]=WAIT_T3;					//Tprogram 3ms
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=INC_ADDR;
    if(usb->sizeW>0x2008)
    {						//only if Config2 is present
        usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 2 0x2008
        usb->bufferU[j++]=usb->memCODE_W[0x2008]>>8;		//MSB
        usb->bufferU[j++]=usb->memCODE_W[0x2008]&0xff;		//LSB
        usb->bufferU[j++]=BEGIN_PROG;				//internally timed, T=5ms min
        usb->bufferU[j++]=WAIT_T3;					//Tprogram 3ms
        usb->bufferU[j++]=READ_DATA_PROG;
        usb->bufferU[j++]=FLUSH;
    }
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(45);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    if(usb->ICDenable)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (ICDgoto!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage4(GlobalVariables::strings[S_ICDErr],0x2004,i,ICDgoto,(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ICD (0x%X): written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    if(usb->sizeW>0x2008)
    {						//only if Config2 is present
        for(z+=6;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if(~usb->memCODE_W[0x2008]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
        {	//error if written 0 and read 1 (~W&R)
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2008],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
            err_c++;
        }
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d \n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    // CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report



}





void ProgP16::Write16F62x (int dim,int dim2)
{
    // write 14 bit PIC
    // dim=program usb->size 	dim2=eeprom usb->size
    // vpp before vdd
    // DevID@0x2006
    // Config@0x2007
    // eeprom@0x2200
    // erase if protected:
    // LOAD_CONF (0000)(0) + CUST_CMD (0001) + CUST_CMD (0111)
    // + BEGIN_PROG (1000) + 15ms + CUST_CMD (0001) + CUST_CMD (0111)
    // erase if not protected:
    // LOAD_DATA_PROG (0010)(0x3FFF) + BULK_ERASE_PROG (1001) +5ms
    // LOAD_DATA_DATA (0011)(0xFF) + BULK_ERASE_DATA (1011) + BEGIN_PROG (1000) + 5ms
    // write: LOAD_DATA_PROG (0010) + BEGIN_PROG2 (11000) + 8ms
    // write ID: LOAD_DATA_PROG (0010) + BEGIN_PROG (1000) + 16ms
    // write CONFIG: LOAD_DATA_PROG (0010) + BEGIN_PROG2 (11000) + 8ms
    // eeprom: LOAD_DATA_DATA (0011) + BEGIN_PROG2 (11000) + 8ms
    // verify during write

    int err=0;
    WORD devID,config;
    int k=0,z=0,i,j,w;
    if(usb->sizeW<0x2007)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW3]);	//"Can't find CONFIG (0x2007)\r\nEnd\r\n"
        return;
    }
    if(dim2)
    {
        if(usb->sizeW<0x2100)
        {
            dim2=0;
            CommonFunctions::printMessage(GlobalVariables::strings[S_NoEEMem]);	//"Can't find EEPROM data\r\n"
        }
        else if(dim2>usb->sizeW-0x2100) dim2=usb->sizeW-0x2100;
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write16F62x(%d,%d)\n",dim,dim2);
    }
    for(i=0;i<0x2009&&i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=10000>>8;
    usb->bufferU[j++]=10000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x06;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Config
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=EN_VPP_VCC;		//0
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//delay exit-enter prog mode
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(12);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    findByID(devID);
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    config=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ConfigWord],config);	//"Config word: 0x%04X\r\n"
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=5000>>8;
    usb->bufferU[j++]=5000&0xff;
    if(config<0x3C00)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_ProtErase]);	//"Override write protection\r\n"
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0x3F;				//fake config	spec ERROR!!! is written data=0!!
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=0x07;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x01;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x07;
        usb->bufferU[j++]=BEGIN_PROG;		//Tera+Tprog=5+8 ms
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x01;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x07;
    }
    else{
        usb->bufferU[j++]=LOAD_DATA_PROG;
        usb->bufferU[j++]=0x3f;				//MSB
        usb->bufferU[j++]=0xff;				//LSB
        usb->bufferU[j++]=BULK_ERASE_PROG;
        usb->bufferU[j++]=BEGIN_PROG;		//Tera=5ms
        usb->bufferU[j++]=WAIT_T3;
        if(dim2)
        {
            usb->bufferU[j++]=LOAD_DATA_DATA;
            usb->bufferU[j++]=0xff;				//LSB
            usb->bufferU[j++]=BULK_ERASE_DATA;
            usb->bufferU[j++]=BEGIN_PROG;		//Tera=5ms
            usb->bufferU[j++]=WAIT_T3;
        }
    }
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=8000>>8;
    usb->bufferU[j++]=8000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(60);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    //PrintStatusSetup();
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG2;			//program only, internally timed
            usb->bufferU[j++]=WAIT_T3;				//Tprogram=8ms
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-10||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*9+2);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-5;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+3]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+4]<<8)+usb->bufferI[z+5])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+4]<<8)+usb->bufferI[z+5]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=6;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                // WriteLogIO();
            }
        }
    }
    err+=i-k;
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write eeprom ********************
    if(dim2)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Writing EEPROM ... "
        // PrintStatusSetup();
        j=1;
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=INC_ADDR_N;		//use only INC_ADDR_N so verification does not look at it
        usb->bufferU[j++]=0xFF;		//20FF
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=0xFF;		//21FE
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=0x2;		//EEPROM: counter at 0x2200
        for(w=0,i=k=0x2100;i<0x2100+dim2;i++)
        {
            if(usb->memCODE_W[i]<0xff)
            {
                usb->bufferU[j++]=LOAD_DATA_DATA;
                usb->bufferU[j++]=usb->memCODE_W[i]&0xff;
                usb->bufferU[j++]=BEGIN_PROG2;			//internally timed
                usb->bufferU[j++]=WAIT_T3;				//Tprogram=8ms
                usb->bufferU[j++]=READ_DATA_DATA;
                w++;
            }
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF-10||i==0x2100+dim2-1)
            {
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],(i-0x2100+dim)*100/(dim+dim2),i);	//"Writing: %d%%, add. %03X"
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(w*14+1);
                w=0;
                usb->read();
                for(z=1;z<usb->DIMBUF-4;z++)
                {
                    if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0xff) k++;
                    else if(usb->bufferI[z]==LOAD_DATA_DATA&&usb->bufferI[z+3]==READ_DATA_DATA)
                    {
                        if (usb->memCODE_W[k]!=usb->bufferI[z+4])
                        {
                            CommonFunctions::printMessage("\r\n");
                            CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError3],k,usb->memCODE_W[k],usb->bufferI[z+4]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                            err++;
                            if(usb->max_err&&err>usb->max_err)
                            {
                                CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                                CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                                i=0x2200;
                                z=usb->DIMBUF;
                            }
                        }
                        k++;
                        z+=5;
                    }
                }
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                    // WriteLogIO();
                }
            }
        }
        err+=i-k;
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],i-k);	//"completed, %d errors\r\n"
    }
    //****************** write ID, CONFIG ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//erase + prog internally timed, T=8+5 ms
            usb->bufferU[j++]=WAIT_T3;				//Tprogram
            usb->bufferU[j++]=WAIT_T3;				//Tprogram
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=3;
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=7;
    }
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007
    usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG2;				//prog only, internally timed, T=8 ms
    usb->bufferU[j++]=WAIT_T3;					//Tprogram 8ms
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(35);
    if(usb->programID) usb->msDelay(90);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d\n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    // CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    // if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report


}





void ProgP16::Write16F81x(int dim,int dim2)
{
    // write 14 bit PIC
    // dim=program usb->size 	dim2=eeprom usb->size
    // seq=0: vdd + (50ms) + vdd&vpp
    // seq=1: vdd + (50us) + vdd&vpp
    // DevID@0x2006
    // Config@0x2007
    // write CONFIG2@0x2008 if different from 3FFF
    // erase if protected: CHIP ERASE (11111) + 8ms
    // erase if not protected:
    // BULK_ERASE_PROG (1001) + BEGIN_PROG (1001) + 2ms + END_PROGX (10111)
    // BULK_ERASE_DATA (1011) + BEGIN_PROG (1001) + 2ms + END_PROGX (10111)
    // write: LOAD_DATA_PROG (0010) + BEGIN_PROG2 (11000) + 1.5ms + END_PROGX (10111)
    // write eeprom: LOAD_DATA_DATA (0011) + BEGIN_PROG2 (11000) + 1.5ms + END_PROGX (10111)
    // verify during write

    int err=0;
    WORD devID,config;
    int k=0,z=0,i,j,w;
    if(usb->sizeW<0x2007)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW3]);	//"Can't find CONFIG (0x2007)\r\nEnd\r\n"
        return;
    }
    if(dim2)
    {
        if(usb->sizeW<0x2100)
        {
            dim2=0;
            CommonFunctions::printMessage(GlobalVariables::strings[S_NoEEMem]);	//"Can't find EEPROM data\r\n"
        }
        else if(dim2>usb->sizeW-0x2100) dim2=usb->sizeW-0x2100;
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write16F81x(%d,%d)\n",dim,dim2);
    }
    for(i=0;i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=25000>>8;
    usb->bufferU[j++]=25000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x06;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Config
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(40);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    findByID(devID);
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    config=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ConfigWord],config);	//"Config word: 0x%04X\r\n"
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    if(usb->programID||usb->ICDenable)
    {
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0x3F;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
    }
    if((config&0x2100)!=0x2100)
    {
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x1F;					// CHIP_ERASE (11111)
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=WAIT_T3;
    }
    else{
        usb->bufferU[j++]=BULK_ERASE_PROG;
        usb->bufferU[j++]=BEGIN_PROG;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x17;					//END_PROGX (10111)
    }
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    // PrintStatusSetup();
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG2;			//externally timed
            usb->bufferU[j++]=WAIT_T3;				//Tprogram
            usb->bufferU[j++]=CUST_CMD;
            usb->bufferU[j++]=0x17;					//END_PROGX (10111)
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-11||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*2.5+2);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-6;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+4]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+5]<<8)+usb->bufferI[z+6])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+5]<<8)+usb->bufferI[z+6]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=7;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                // WriteLogIO();
            }
            emit onProgressChanged(i, dim);
        }
        emit onProgressChanged(dim, dim);
    }
    err+=i-k;
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write eeprom ********************
    if(dim2)
    {
        int err_e=0;
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Writing EEPROM ... "
        // PrintStatusSetup();
        j=1;
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SET_CK_D;
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
        usb->bufferU[j++]=0x5;
        usb->bufferU[j++]=LOAD_DATA_DATA;
        usb->bufferU[j++]=0x01;
        usb->bufferU[j++]=BULK_ERASE_DATA;
        usb->bufferU[j++]=BEGIN_PROG;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x17;					//END_PROGX (10111)
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(5);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        j=1;
        for(w=0,i=k=0x2100;i<0x2100+dim2;i++)
        {
            if(usb->memCODE_W[i]<0xff)
            {
                usb->bufferU[j++]=LOAD_DATA_DATA;
                usb->bufferU[j++]=usb->memCODE_W[i]&0xff;
                usb->bufferU[j++]=BEGIN_PROG2;			//externally timed
                usb->bufferU[j++]=WAIT_T3;				//Tprogram
                usb->bufferU[j++]=CUST_CMD;
                usb->bufferU[j++]=0x17;					//END_PROGX (10111)
                usb->bufferU[j++]=READ_DATA_DATA;
                w++;
            }
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF-10||i==0x2100+dim2-1)
            {
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],(i-0x2100+dim)*100/(dim+dim2),i);	//"Writing: %d%%, add. %03X"
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(w*2.5+5);
                w=0;
                usb->read();
                for(z=1;z<usb->DIMBUF-5;z++)
                {
                    if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0xff) k++;
                    else if(usb->bufferI[z]==LOAD_DATA_DATA&&usb->bufferI[z+4]==READ_DATA_DATA)
                    {
                        if (usb->memCODE_W[k]!=usb->bufferI[z+5])
                        {
                            CommonFunctions::printMessage("\r\n");
                            CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError3],k,usb->memCODE_W[k],usb->bufferI[z+5]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                            err_e++;
                            if(usb->max_err&&err>usb->max_err)
                            {
                                CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                                CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                                i=0x2200;
                                z=usb->DIMBUF;
                            }
                        }
                        k++;
                        z+=6;
                    }
                }
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                    // WriteLogIO();
                }
            }
        }
        err_e+=i-k;
        err+=err_e;
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_e);	//"completed, %d errors\r\n"
    }
    //****************** write ID, CONFIG, CALIB ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    int ICDgoto=0x2800+(usb->ICDaddr&0x7FF);		//GOTO ICD routine (0x28xx)
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG2;			//externally timed
            usb->bufferU[j++]=WAIT_T3;				//Tprogram
            usb->bufferU[j++]=CUST_CMD;
            usb->bufferU[j++]=0x17;					//END_PROGX (10111)
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=4;
    }
    if(usb->ICDenable)
    {		//write a GOTO ICD routine (0x28xx)
        usb->bufferU[j++]=LOAD_DATA_PROG;
        usb->bufferU[j++]=ICDgoto>>8;		//MSB
        usb->bufferU[j++]=ICDgoto&0xFF;			//LSB
        usb->bufferU[j++]=BEGIN_PROG2;				//externally timed
        usb->bufferU[j++]=WAIT_T3;					//Tprogram
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x17;						//END_PROGX (10111)
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007
    usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG2;				//externally timed
    usb->bufferU[j++]=WAIT_T3;					//Tprogram
    usb->bufferU[j++]=CUST_CMD;
    usb->bufferU[j++]=0x17;						//END_PROGX (10111)
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=INC_ADDR;
    if(usb->sizeW>0x2008&&usb->memCODE_W[0x2008]!=0x3fff)
    {
        usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word2 0x2008
        usb->bufferU[j++]=usb->memCODE_W[0x2008]>>8;		//MSB
        usb->bufferU[j++]=usb->memCODE_W[0x2008]&0xff;		//LSB
        usb->bufferU[j++]=BEGIN_PROG2;				//externally timed
        usb->bufferU[j++]=WAIT_T3;					//Tprogram
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x17;						//END_PROGX (10111)
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=7;
    }
    if(usb->ICDenable)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (ICDgoto!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage4(GlobalVariables::strings[S_ICDErr],0x2004,i,ICDgoto,(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ICD (0x%X): written %04X, read %04X\r\n"
            err_c++;
        }
        z+=7;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    if(usb->sizeW>0x2008&&usb->memCODE_W[0x2008]!=0x3fff)
    {
        for(z+=7;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if(~usb->memCODE_W[0x2008]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
        {	//error if written 0 and read 1 (~W&R)
            CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2008],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
            err_c++;
        }
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d \n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    //CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report


}




void ProgP16::Write16F87x(int dim,int dim2)
{
    // write 14 bit PIC
    // dim=program usb->size 	dim2=eeprom usb->size
    // dim2<0 -> eeprom @ 0x2200
    // vdd + (50ms?) + vdd&vpp
    // DevID@0x2006
    // Config@0x2007
    // eeprom@0x2100
    // erase if protected:
    // LOAD_CONF (0000)(0x3FFF) + CUST_CMD (0001) + CUST_CMD (0111)
    // + BEGIN_PROG (1000) + 8ms
    // + CUST_CMD (0001) + CUST_CMD (0111)
    // erase if not protected:
    // LOAD_DATA_PROG (0010)(0x3FFF) + CUST_CMD (0001) + CUST_CMD (0111)
    // + BEGIN_PROG (1000) + 8ms + CUST_CMD (0001) + CUST_CMD (0111)
    // LOAD_DATA_DATA (0011)(0xFF) + CUST_CMD (0001) + CUST_CMD (0111)
    // + BEGIN_PROG (1000) + 8ms + CUST_CMD (0001) + CUST_CMD (0111)
    // write: LOAD_DATA_PROG (0010) + BEGIN_PROG2 (11000) + 4ms
    // write eeprom: LOAD_DATA_DATA (0011) + BEGIN_PROG (1000) + 8ms
    // verify during write

    int err=0;
    WORD devID,config;
    int k=0,z=0,i,j,w,ee2200=0;
    if(dim2<0)
    {
        dim2=-dim2;
        ee2200=1;
    }
    if(usb->sizeW<0x2007)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW3]);	//"Can't find CONFIG (0x2007)\r\nEnd\r\n"
        return;
    }
    if(dim2)
    {
        if(usb->sizeW<0x2100)
        {
            dim2=0;
            CommonFunctions::printMessage(GlobalVariables::strings[S_NoEEMem]);	//"Can't find EEPROM data\r\n"
        }
        else if(dim2>usb->sizeW-0x2100) dim2=usb->sizeW-0x2100;
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write16F87x(%d,%d)\n",dim,dim2);
    }
    for(i=0;i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x06;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Config
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(60);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    findByID(devID);
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    config=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ConfigWord],config);	//"Config word: 0x%04X\r\n"
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=8000>>8;
    usb->bufferU[j++]=8000&0xff;
    if((config&0x3130)!=0x3130)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_ProtErase]);	//"override write protection\r\n"
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0x3F;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=0x07;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x01;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x07;
        usb->bufferU[j++]=BEGIN_PROG;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x01;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x07;
        usb->bufferU[j++]=NOP;				//exit program mode
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SET_CK_D;
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
        usb->bufferU[j++]=0x5;
    }
    else{
        if(usb->ICDenable||usb->programID)
        {			//erase 0x2000-2004 also
            usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
            usb->bufferU[j++]=0x3F;				//fake config
            usb->bufferU[j++]=0xFF;				//fake config
        }
        usb->bufferU[j++]=LOAD_DATA_PROG;
        usb->bufferU[j++]=0x3f;				//MSB
        usb->bufferU[j++]=0xff;				//LSB
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x01;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x07;
        usb->bufferU[j++]=BEGIN_PROG;
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x01;
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x07;
        if(dim2)
        {
            usb->bufferU[j++]=LOAD_DATA_DATA;	//EEPROM:  spec error?
            usb->bufferU[j++]=0xff;				//LSB
            usb->bufferU[j++]=CUST_CMD;
            usb->bufferU[j++]=0x01;
            usb->bufferU[j++]=CUST_CMD;
            usb->bufferU[j++]=0x07;
            usb->bufferU[j++]=BEGIN_PROG;
            usb->bufferU[j++]=WAIT_T3;
            usb->bufferU[j++]=CUST_CMD;
            usb->bufferU[j++]=0x01;
            usb->bufferU[j++]=CUST_CMD;
            usb->bufferU[j++]=0x07;
        }
        if(usb->ICDenable||usb->programID)
        {			//back to addr 0
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=0x1;
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=0x0;
            usb->bufferU[j++]=SET_CK_D;
            usb->bufferU[j++]=0x0;
            usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
            usb->bufferU[j++]=EN_VPP_VCC;		//VDD
            usb->bufferU[j++]=0x1;
            usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
            usb->bufferU[j++]=0x5;
        }
    }
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=4000>>8;
    usb->bufferU[j++]=4000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(60);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    // PrintStatusSetup();
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG2;			//internally timed
            usb->bufferU[j++]=WAIT_T3;				//Tprogram
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-10||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*5+2);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-5;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+3]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+4]<<8)+usb->bufferI[z+5])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+4]<<8)+usb->bufferI[z+5]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=6;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                // WriteLogIO();
            }
            emit onProgressChanged(i, dim);
        }
    }
    emit onProgressChanged(dim, dim);
    err+=i-k;
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write eeprom ********************
    if(dim2)
    {
        int err_e=0;
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Writing EEPROM ... "
        // PrintStatusSetup();
        j=1;
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=INC_ADDR_N;		//use only INC_ADDR_N so verification does not look at it
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=1;					//EEPROM: counter at 0x2100
        if(ee2200)
        {		//eeprom at 0x2200
            usb->bufferU[j++]=INC_ADDR_N;
            usb->bufferU[j++]=0xFF;
            usb->bufferU[j++]=INC_ADDR_N;
            usb->bufferU[j++]=1;
        }
        for(w=0,i=k=0x2100;i<0x2100+dim2;i++)
        {
            if(usb->memCODE_W[i]<0xff)
            {
                usb->bufferU[j++]=LOAD_DATA_DATA;
                usb->bufferU[j++]=usb->memCODE_W[i]&0xff;
                usb->bufferU[j++]=BEGIN_PROG2;			//internally timed ?????
                usb->bufferU[j++]=WAIT_T3;				//Tprogram         ?????
                usb->bufferU[j++]=READ_DATA_DATA;
                w++;
            }
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF-10||i==0x2100+dim2-1)
            {
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],(i-0x2100+dim)*100/(dim+dim2),i);	//"Writing: %d%%, add. %03X"
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(w*8+5);
                w=0;
                usb->read();
                for(z=1;z<usb->DIMBUF;z++)
                {
                    if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0xff) k++;
                    else if(usb->bufferI[z]==LOAD_DATA_DATA&&usb->bufferI[z+3]==READ_DATA_DATA)
                    {
                        if (usb->memCODE_W[k]!=usb->bufferI[z+4])
                        {
                            CommonFunctions::printMessage("\r\n");
                            CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError3],k,usb->memCODE_W[k],usb->bufferI[z+4]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                            err_e++;
                            if(usb->max_err&&err+err_e>usb->max_err)
                            {
                                CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err+err_e);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                                CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                                i=0x2200;
                                z=usb->DIMBUF;
                            }
                        }
                        k++;
                        z+=5;
                    }
                }
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                    // WriteLogIO();
                }
            }
        }
        err_e+=i-k;
        err+=err_e;
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_e);	//"completed, %d errors\r\n"
    }
    //****************** write ID, CONFIG, CALIB ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    int ICDgoto=0x2800+(usb->ICDaddr&0x7FF);		//GOTO ICD routine (0x28xx)
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=8000>>8;
    usb->bufferU[j++]=8000&0xff;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG2;			//internally timed
            usb->bufferU[j++]=WAIT_T3;				//Tprogram
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=4;
    }
    if(usb->ICDenable)
    {		//write a GOTO ICD routine (0x28xx)
        usb->bufferU[j++]=LOAD_DATA_PROG;
        usb->bufferU[j++]=ICDgoto>>8;		//MSB
        usb->bufferU[j++]=ICDgoto&0xFF;			//LSB
        usb->bufferU[j++]=BEGIN_PROG2;			//internally timed
        usb->bufferU[j++]=WAIT_T3;				//Tprogram
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007
    usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG2;			//internally timed
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(35);
    if(usb->programID) usb->msDelay(90);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    if(usb->ICDenable)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (ICDgoto!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage4(GlobalVariables::strings[S_ICDErr],0x2004,i,ICDgoto,(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ICD (0x%X): written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d \n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    //CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report

}




void ProgP16::Write16F87xA(int dim,int dim2,int seq)
{

    // write 14 bit PIC
    // dim=program usb->size 	dim2=eeprom usb->size
    // seq=0: vdd + (50ms) + vdd&vpp
    // seq=1: vdd + (50us) + vdd&vpp
    // DevID@0x2006
    // Config@0x2007
    // write CONFIG2@0x2008 if different from 3FFF
    // eeprom@0x2100
    // erase:
    // CHIP ERASE (11111) + 15ms
    // write: LOAD_DATA_PROG (0010) + BEGIN_PROG2 (11000) + 1.2ms + END_PROGX (10111)
    // write eeprom: LOAD_DATA_DATA (0011) + BEGIN_PROG (1000) + 12ms
    // verify during write

    int err=0;
    WORD devID,config;
    int k=0,z=0,i,j,w;
    if(usb->sizeW<0x2007)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW3]);	//"Can't find CONFIG (0x2007)\r\nEnd\r\n"
        return;
    }
    if(dim2)
    {
        if(usb->sizeW<0x2100)
        {
            dim2=0;
            CommonFunctions::printMessage(GlobalVariables::strings[S_NoEEMem]);	//"Can't find EEPROM data\r\n"
        }
        else if(dim2>usb->sizeW-0x2100) dim2=usb->sizeW-0x2100;
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write16F87xA(%d,%d,%d)\n",dim,dim2,seq);
    }
    for(i=0;i<usb->sizeW;i++)
       usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=25000>>8;
    usb->bufferU[j++]=25000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    if(seq==0)
    {
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=WAIT_T3;
    }
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x06;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Config
    usb->bufferU[j++]=EN_VPP_VCC;		//exit program mode
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(40);
    if(seq==0) usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    findByID(devID);
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    config=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ConfigWord],config);	//"Config word: 0x%04X\r\n"
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=15000>>8;
    usb->bufferU[j++]=15000&0xff;
    if(usb->ICDenable||usb->programID)
    {			//erase 0x2000-2004 also
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0x3F;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
    }
    usb->bufferU[j++]=CUST_CMD;
    usb->bufferU[j++]=0x1F;					// CHIP_ERASE (11111)
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=EN_VPP_VCC;		//exit program mode
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=1200>>8;
    usb->bufferU[j++]=1200&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(60);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    // PrintStatusSetup();
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG2;			//externally timed
            usb->bufferU[j++]=WAIT_T3;				//Tprogram
            usb->bufferU[j++]=CUST_CMD;
            usb->bufferU[j++]=0x17;					//END_PROGX (10111)
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-11||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*1.5+(6-w)*0.2+2);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-6;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+4]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+5]<<8)+usb->bufferI[z+6])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+5]<<8)+usb->bufferI[z+6]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=7;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                // WriteLogIO();
            }
        }
        emit onProgressChanged(i, dim);
    }
    emit onProgressChanged(dim, dim);
    err+=i-k;
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write eeprom ********************
    if(dim2)
    {
        int err_e=0;
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Writing EEPROM ... "
        // PrintStatusSetup();
        j=1;
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T3;
        usb->bufferU[j++]=12000>>8;
        usb->bufferU[j++]=12000&0xff;
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=INC_ADDR_N;		//use only INC_ADDR_N so verification does not look at it
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=INC_ADDR_N;		//EEPROM: counter at 0x2100
        usb->bufferU[j++]=1;
        for(w=0,i=k=0x2100;i<0x2100+dim2;i++)
        {
            if(usb->memCODE_W[i]<0xff)
            {
                usb->bufferU[j++]=LOAD_DATA_DATA;
                usb->bufferU[j++]=usb->memCODE_W[i]&0xff;
                usb->bufferU[j++]=BEGIN_PROG;			//internally timed
                usb->bufferU[j++]=WAIT_T3;				//Tprogram 8ms
                usb->bufferU[j++]=READ_DATA_DATA;
                w++;
            }
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF-10||i==0x2100+dim2-1)
            {
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],(i-0x2100+dim)*100/(dim+dim2),i);	//"Writing: %d%%, add. %03X"
                emit onProgressChanged(i, dim+dim2);
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(w*12.5+5);
                w=0;
                usb->read();
                for(z=1;z<usb->DIMBUF-4;z++)
                {
                    if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0xff) k++;
                    else if(usb->bufferI[z]==LOAD_DATA_DATA&&usb->bufferI[z+3]==READ_DATA_DATA)
                    {
                        if (usb->memCODE_W[k]!=usb->bufferI[z+4])
                        {
                            CommonFunctions::printMessage("\r\n");
                            CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError3],k,usb->memCODE_W[k],usb->bufferI[z+4]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                            err_e++;
                            if(usb->max_err&&err>usb->max_err)
                            {
                                CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                                CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                                i=0x2200;
                                z=usb->DIMBUF;
                            }
                        }
                        k++;
                        z+=5;
                    }
                }
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                    // WriteLogIO();
                }
            }
            emit onProgressChanged(i, dim);
        }
        emit onProgressChanged(dim, dim);
        err_e+=i-k;
        err+=err_e;
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_e);	//"completed, %d errors\r\n"
    }
    //****************** write ID, CONFIG, CALIB ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    int ICDgoto=0x2800+(usb->ICDaddr&0x7FF);		//GOTO ICD routine (0x28xx)
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=12000>>8;
    usb->bufferU[j++]=12000&0xff;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        //main code
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;   //LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed
            usb->bufferU[j++]=WAIT_T3;				//Tprogram
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
        /*
        //signal group code
        for(i=0;i<4;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCONFIG[i*2+1];		//MSB
            usb->bufferU[j++]=usb->memCONFIG[i*2];		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed
            usb->bufferU[j++]=WAIT_T3;				//Tprogram
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }*/
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=4;
    }
    if(usb->ICDenable)
    {		//write a GOTO ICD routine (0x28xx)
        usb->bufferU[j++]=LOAD_DATA_PROG;
        usb->bufferU[j++]=ICDgoto>>8;		//MSB
        usb->bufferU[j++]=ICDgoto&0xFF;			//LSB
        usb->bufferU[j++]=BEGIN_PROG;			//internally timed
        usb->bufferU[j++]=WAIT_T3;				//Tprogram
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007

    //usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    //usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB

    //signal group
    usb->bufferU[j++]=usb->memCONFIG[1];		//MSB
    usb->bufferU[j++]=usb->memCONFIG[0];		//LSB

    usb->bufferU[j++]=BEGIN_PROG;				//internally timed
    usb->bufferU[j++]=WAIT_T3;					//Tprogram
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=INC_ADDR;
    if(usb->sizeW>0x2008&&usb->memCODE_W[0x2008]!=0x3fff)
    {
        usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word2 0x2008
        //usb->bufferU[j++]=usb->memCODE_W[0x2008]>>8;		//MSB
        //usb->bufferU[j++]=usb->memCODE_W[0x2008]&0xff;		//LSB

        //signal group
        usb->bufferU[j++]=usb->memCONFIG[3];		//MSB
        usb->bufferU[j++]=usb->memCONFIG[2];		//LSB

        usb->bufferU[j++]=BEGIN_PROG;				//internally timed
        usb->bufferU[j++]=WAIT_T3;					//Tprogram
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    if(usb->programID) usb->msDelay(50);
    if(usb->ICDenable) usb->msDelay(13);
    usb->msDelay(28);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    if(usb->ICDenable)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (ICDgoto!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage4(GlobalVariables::strings[S_ICDErr],0x2004,i,ICDgoto,(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ICD (0x%X): written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    if(usb->sizeW>0x2008&&usb->memCODE_W[0x2008]!=0x3fff)
    {
        for(z+=6;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if(~usb->memCODE_W[0x2008]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
        {	//error if written 0 and read 1 (~W&R)
            CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2008],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
            err_c++;
        }
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d \n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    //CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report


}


void ProgP16::Write16F88x(int dim,int dim2)
{
    // write 14 bit PIC
    // dim=program usb->size 	dim2=eeprom usb->size
    // vpp before vdd
    // DevID@0x2006
    // Config@0x2007
    // Config2@0x2008
    // Calib1@0x2009 (salva)
    // eeprom@0x2100
    // erase: BULK_ERASE_PROG (1001) +6ms
    // write:LOAD_DATA_PROG (0010) + BEGIN_PROG (1000) + 3ms
    // eeprom:	BULK_ERASE_DATA (1011) + 6ms
    //			LOAD_DATA_DATA (0011) + BEGIN_PROG (1000) + 6ms
    // verify during write

    int err=0;
    WORD devID=0x3fff,calib1=0x3fff;
    int k=0,z=0,i,j,w;
    if(usb->sizeW<0x2009)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW4]);	//"Can't find CONFIG (0x2008)\r\nEnd\r\n"
        return;
    }
    if(usb->load_calibword)
    {
        if(usb->sizeW>0x200A) usb->load_calibword=1;
        else{
            CommonFunctions::printMessage(GlobalVariables::strings[S_NoCalibW]);	//"Can't find calibration data\r\n"
            usb->load_calibword=0;
        }
    }
    if(dim2)
    {
        if(usb->sizeW<0x2100)
        {
            dim2=0;
            CommonFunctions::printMessage(GlobalVariables::strings[S_NoEEMem]);	//"Can't find EEPROM data\r\n"
        }
        else if(dim2>usb->sizeW-0x2100) dim2=usb->sizeW-0x2100;
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write16F88x(%d,%d)\n",dim,dim2);
    }
    for(i=0;i<0x200A&&i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x06;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Calib1
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=6000>>8;
    usb->bufferU[j++]=6000&0xff;
    usb->bufferU[j++]=WAIT_T3;			//delay after exiting prog mode
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    findByID(devID);
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    calib1=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    if(calib1<0x3fff)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_CalibWord1],calib1);	//"Calib1: 0x%04X\r\n"
    }
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VPP
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    if(usb->programID||usb->load_calibword||usb->ICDenable)
    {
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        if(usb->load_calibword)
        {
            usb->bufferU[j++]=INC_ADDR_N;
            usb->bufferU[j++]=9;
        }
    }
    usb->bufferU[j++]=BULK_ERASE_PROG;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=6ms
    usb->bufferU[j++]=EN_VPP_VCC;		//exit program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=6ms after exiting program mode
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x4;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=3000>>8;
    usb->bufferU[j++]=3000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(40);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    // PrintStatusSetup();
    for(w=i=k=0,j=1;i<dim;i++)
    {
        if(usb->memCODE_W[i]<0x3fff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 3ms
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-12||i==dim-1)
        {
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, add. %03X"
            emit onProgressChanged(i, dim);
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*3+2);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-5;z++)
            {
                if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0x3fff) k++;
                else if(usb->bufferI[z]==LOAD_DATA_PROG&&usb->bufferI[z+3]==READ_DATA_PROG)
                {
                    if (usb->memCODE_W[k]!=(usb->bufferI[z+4]<<8)+usb->bufferI[z+5])
                    {
                        CommonFunctions::printMessage("\r\n");
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+4]<<8)+usb->bufferI[z+5]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=6;
                }
            }
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d,\n"
                // WriteLogIO();
            }
            emit onProgressChanged(i, dim);
        }
    }
    emit onProgressChanged(dim, dim);
    err+=i-k;
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write eeprom ********************
    if(dim2)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Writing EEPROM ... "
        // PrintStatusSetup();
        j=1;
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T3;
        usb->bufferU[j++]=6000>>8;
        usb->bufferU[j++]=6000&0xff;
        usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=0xFF;				//fake config
        usb->bufferU[j++]=BULK_ERASE_DATA;
        usb->bufferU[j++]=WAIT_T3;			// delay T3=6ms
        usb->bufferU[j++]=INC_ADDR_N;		//use only INC_ADDR_N so verification does not look at it
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=INC_ADDR_N;		//EEPROM: counter at 0x2100
        usb->bufferU[j++]=1;
        for(w=2,i=k=0x2100;i<0x2100+dim2;i++)
        {
            if(usb->memCODE_W[i]<0xff)
            {
                usb->bufferU[j++]=LOAD_DATA_DATA;
                usb->bufferU[j++]=usb->memCODE_W[i]&0xff;
                usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=6ms min
                usb->bufferU[j++]=WAIT_T3;				//Tprogram 6ms
                usb->bufferU[j++]=READ_DATA_DATA;
                w++;
            }
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF-12||i==0x2100+dim2-1)
            {
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],(i-0x2100+dim)*100/(dim+dim2),i);	//"Writing: %d%%, add. %03X"
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(w*6.5+2);
                w=0;
                usb->read();
                for(z=1;z<usb->DIMBUF-4;z++)
                {
                    if(usb->bufferI[z]==INC_ADDR&&usb->memCODE_W[k]>=0xff) k++;
                    else if(usb->bufferI[z]==LOAD_DATA_DATA&&usb->bufferI[z+3]==READ_DATA_DATA)
                    {
                        if (usb->memCODE_W[k]!=usb->bufferI[z+4])
                        {
                            CommonFunctions::printMessage("\r\n");
                            CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError3],k,usb->memCODE_W[k],usb->bufferI[z+4]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                            err++;
                            if(usb->max_err&&err>usb->max_err)
                            {
                                CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                                CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                                i=0x2200;
                                z=usb->DIMBUF;
                            }
                        }
                        k++;
                        z+=5;
                    }
                }
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d \n"
                    // WriteLogIO();
                }
            }
        }
        err+=i-k;
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],i-k);	//"completed, %d errors\r\n"
    }
    //****************** write ID, CONFIG, CALIB ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
    int err_c=0;
    int ICDgoto=0x2800+(usb->ICDaddr&0x7FF);		//GOTO ICD routine (0x28xx)
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=3000>>8;
    usb->bufferU[j++]=3000&0xff;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;				//fake config
    usb->bufferU[j++]=0xFF;				//fake config
    if(usb->programID)
    {
        for(i=0x2000;i<0x2004;i++)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 3ms
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=INC_ADDR;
        }
    }
    else{
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=4;
    }
    if(usb->ICDenable)
    {		//write a GOTO ICD routine (0x28xx)
        usb->bufferU[j++]=LOAD_DATA_PROG;
        usb->bufferU[j++]=ICDgoto>>8;			//MSB
        usb->bufferU[j++]=ICDgoto&0xFF;			//LSB
        usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
        usb->bufferU[j++]=WAIT_T3;				//Tprogram 3ms
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x2007
    usb->bufferU[j++]=usb->memCODE_W[0x2007]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2007]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG;				//internally timed, T=3ms min
    usb->bufferU[j++]=WAIT_T3;					//Tprogram 3ms
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word2 0x2008
    usb->bufferU[j++]=usb->memCODE_W[0x2008]>>8;		//MSB
    usb->bufferU[j++]=usb->memCODE_W[0x2008]&0xff;		//LSB
    usb->bufferU[j++]=BEGIN_PROG;				//internally timed, T=3ms min
    usb->bufferU[j++]=WAIT_T3;					//Tprogram 3ms
    usb->bufferU[j++]=READ_DATA_PROG;
    usb->bufferU[j++]=INC_ADDR;
    if(usb->load_calibword)
    {
        usb->bufferU[j++]=LOAD_DATA_PROG;		//Calib word 1
        usb->bufferU[j++]=usb->memCODE_W[0x2009]>>8;	//MSB
        usb->bufferU[j++]=usb->memCODE_W[0x2009]&0xff;	//LSB
        usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=3ms min
        usb->bufferU[j++]=WAIT_T3;				//Tprogram 3ms
        usb->bufferU[j++]=READ_DATA_PROG;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(35);
    usb->read();
    for(i=0,z=0;usb->programID&&i<4;i++)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x2000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    if(usb->ICDenable)
    {
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (ICDgoto!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage4(GlobalVariables::strings[S_ICDErr],0x2004,i,ICDgoto,(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ICD (0x%X): written %04X, read %04X\r\n"
            err_c++;
        }
        z+=6;
    }
    for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
        err_c++;
    }
    for(z+=6;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(~usb->memCODE_W[0x2008]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x2008],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"\r\n"
        err_c++;
    }
    if(usb->load_calibword)
    {
        for(z+=6;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if (usb->memCODE_W[0x2009]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_Calib1Err],usb->memCODE_W[0x2009],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing Calib1: written %04X, read %04X\r\n"
            err_c++;
        }
    }
    err+=err_c;
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    if(usb->saveLog)
    {
        //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Area config. 	errors=%d \n"
        // WriteLogIO();
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    // CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report

}

void ProgP16::Write16F1xxx(int dim,int dim2,int options)
{
    // write 14 bit enhanced PIC
    // dim=program usb->size 	dim2=eeprom usb->size
    // options:
    //		bit0=0 -> vpp before vdd
    //		bit0=1 -> vdd before vpp
    //		bit1=1 -> LVP programming
    // DevREV@0x8005
    // DevID@0x8006
    // Config1@0x8007
    // Config2@0x8008
    // Calib1@0x8009
    // Calib2@0x800A
    // Calib3@0x800B
    // eeprom@0x0
    // erase: BULK_ERASE_PROG (1001) +5ms
    // write:LOAD_DATA_PROG (0010) + BEGIN_PROG (1000) + 2.5ms (8 word algorithm)
    // config write time 5ms
    // eeprom:	BULK_ERASE_DATA (1011) + 5ms
    //			LOAD_DATA_DATA (0011) + BEGIN_PROG (1000) + 2.5ms
    // verify after write

    int err=0;
    usb->load_calibword=0;
    WORD devID=0x3fff,devREV=0x3fff,calib1=0x3fff,calib2=0x3fff,calib3=0x3fff;
    int k=0,k2=0,z=0,i,j,w;
    if(! usb->CheckV33Regulator())
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_noV33reg]);	//Can't find 3.3V expansion board
        return;
    }
    if(usb->sizeW<0x8009)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW5]);	//"Can't find CONFIG (0x8007-0x8008)\r\n"
        CommonFunctions::printMessage(GlobalVariables::strings[S_End]);
        return;
    }
    if(usb->load_calibword)
    {
        if(usb->sizeW>0x800A) usb->load_calibword=1;
        else  CommonFunctions::printMessage(GlobalVariables::strings[S_NoCalibW]);	//"Can't find calibration data\r\n"
    }
    if(usb->saveLog)
    {
        // OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"Write16F1xxx(%d,%d,%d)\n",dim,dim2,options);
    }
    if(dim2>usb->sizeEE) dim2=usb->sizeEE;
    if((options&2)==0)
    {				//HV entry
        if(!usb->StartHVReg(8.5))
        {
            CommonFunctions::printMessage(GlobalVariables::strings[S_HVregErr]); //"HV regulator error\r\n"
            return;
        }
    }
    else usb->StartHVReg(-1);			//LVP mode, turn off HV
    for(i=0;i<0x800B&&i<usb->sizeW;i++) usb->memCODE_W[i]&=0x3FFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    if((options&2)==0)
    {				//HV entry
        if((options&1)==0)
        {				//VPP before VDD
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=4;				//VPP
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=0x5;			//VDD+VPP
        }
        else{							//VDD before VPP without delay
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=1;				//VDD
            usb->bufferU[j++]=EN_VPP_VCC;
            usb->bufferU[j++]=0x5;			//VDD+VPP
        }
    }
    else{			//Low voltage programming
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=4;				//VPP
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=TX16;			//0000 1010 0001 0010 1100 0010 1011 0010 = 0A12C2B2
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=0x0A;
        usb->bufferU[j++]=0x12;
        usb->bufferU[j++]=0xC2;
        usb->bufferU[j++]=0xB2;
        usb->bufferU[j++]=SET_CK_D;		//Clock pulse
        usb->bufferU[j++]=0x4;
        usb->bufferU[j++]=SET_CK_D;
        usb->bufferU[j++]=0x0;
    }
    usb->bufferU[j++]=WAIT_T2;
    usb->bufferU[j++]=WAIT_T2;
    usb->bufferU[j++]=WAIT_T2;
    usb->bufferU[j++]=LOAD_CONF;			//counter at 0x2000
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=0x05;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevREV
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//DevID
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Calib1
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Calib2
    usb->bufferU[j++]=INC_ADDR;
    usb->bufferU[j++]=READ_DATA_PROG;	//Calib3
    usb->bufferU[j++]=CUST_CMD;
    usb->bufferU[j++]=0x16;		//Reset address
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2500>>8;
    usb->bufferU[j++]=2500&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(3);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devREV=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    devID=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    CommonFunctions::printMessage1(GlobalVariables::strings[S_DevID],devID);	//"DevID: 0x%04X\r\n"
    if(devREV<0x3FFF)  CommonFunctions::printMessage1(GlobalVariables::strings[S_DevREV],devREV);	//"DevREV: 0x%04X\r\n"
    findByID(devID);
    if(usb->memCODE_W[0x8006]<0x3FFF&&devID!=usb->memCODE_W[0x8006])  CommonFunctions::printMessage(GlobalVariables::strings[S_DevMismatch]);	//"Warning: the device is different from what specified in source data"
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    calib1=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    if(calib1<0x3fff)  CommonFunctions::printMessage2(GlobalVariables::strings[S_CalibWordX],1,calib1);	//"Calibration word %d: 0x%04X\r\n"
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    calib2=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    if(calib2<0x3fff)  CommonFunctions::printMessage2(GlobalVariables::strings[S_CalibWordX],2,calib2);	//"Calibration word %d: 0x%04X\r\n"
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
    calib3=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2];
    if(calib3<0x3fff)  CommonFunctions::printMessage2(GlobalVariables::strings[S_CalibWordX],3,calib3);	//"Calibration word %d: 0x%04X\r\n"
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erasing ... "
    j=1;
    if(usb->programID)
    {
        usb->bufferU[j++]=LOAD_CONF;			//PC @ 0x8000
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=0xFF;
    }
    usb->bufferU[j++]=BULK_ERASE_PROG;
    usb->bufferU[j++]=WAIT_T3;			// wait 5ms
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=CUST_CMD;
    usb->bufferU[j++]=0x16;		//Reset address
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(8);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    // PrintStatusSetup();
    for(;dim>0&&usb->memCODE_W[dim]>=0x3fff;dim--); //skip empty space at end
    dim+=dim%8;		//grow to 8 word multiple
    int valid,inc;
    for(i=k=0,j=1;i<dim;i+=8)
    {
        valid=inc=0;
        for(;i<dim&&!valid;)
        {	//skip empty locations (8 words)
            valid=0;
            for(k=0;k<8;k++) if(usb->memCODE_W[i+k]<0x3fff) valid=1;
            if(!valid)
            {
                inc+=8;
                i+=8;
            }
            if(inc&&(valid||inc==248))
            {	//increase address to skip empty words
                usb->bufferU[j++]=INC_ADDR_N;
                usb->bufferU[j++]=k=inc;
                inc=0;
            }
            if(j>usb->DIMBUF-4||valid)
            {
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(3);
                usb->msDelay(1*k/2);	//wait for long INC_ADDR_N
                usb->read();
                j=1;
                //if(usb->saveLog)	WriteLogIO();
            }
        }
        if(valid)
        {
            k=0;
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]=usb->memCODE_W[i+k]>>8;  		//MSB
            usb->bufferU[j++]=usb->memCODE_W[i+k]&0xff;		//LSB
            for(k=1;k<8;k++)
            {
                usb->bufferU[j++]=INC_ADDR;
                usb->bufferU[j++]=LOAD_DATA_PROG;
                usb->bufferU[j++]=usb->memCODE_W[i+k]>>8;  		//MSB
                usb->bufferU[j++]=usb->memCODE_W[i+k]&0xff;		//LSB
            }
            usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=2.5ms
            usb->bufferU[j++]=WAIT_T3;
            usb->bufferU[j++]=INC_ADDR;
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(3);
            usb->read();
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,0,0);	//"i=%d, k=%d 0=%d\n"
                // WriteLogIO();
            }
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Writing: %d%%, addr. %03X"
            emit onProgressChanged(i, dim);
        }
    }
    emit onProgressChanged(dim, dim);
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** verify code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_CodeV]);	//"Verifying code ... "
    // PrintStatusSetup();
    if(usb->saveLog)	//fprintf(logfile,"VERIFY CODE\n");
        j=1;
    usb->bufferU[j++]=CUST_CMD;
    usb->bufferU[j++]=0x16;		//Reset address
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    //if(usb->saveLog)	WriteLogIO();
    j=1;
    for(i=k=0;i<dim;i++)
    {
        if(j==1)
        {				//skip empty locations (only after a write)
            for(valid=0;i<dim&&!valid;)
            {
                for(k2=0;k2<255&&!valid;k2++) if(usb->memCODE_W[i+k2]<0x3fff) valid=1;
                if(k2>16)
                {			//increase address to skip empty words, if enough is found
                    usb->bufferU[j++]=INC_ADDR_N;
                    usb->bufferU[j++]=k2;
                    i+=k2;
                    k+=k2;
                }
                if(j>usb->DIMBUF-4||(valid&&j>1))
                {		//if buffer is full or last skip
                    usb->bufferU[j++]=FLUSH;
                    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                    usb->write();
                    usb->msDelay(3);
                    usb->msDelay(1*j/2);	//wait for long INC_ADDR_N
                    usb->read();
                    j=1;
                    //if(usb->saveLog)	WriteLogIO();
                }
            }
        }
        if(usb->memCODE_W[i]<0x3FFF)  usb->bufferU[j++]=READ_DATA_PROG;
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF*2/4-2||i==dim-1)
        {		//2B cmd -> 4B data
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(5);
            usb->read();
            for(z=1;z<usb->DIMBUF-2;z++)
            {
                if(usb->bufferI[z]==INC_ADDR) k++;
                else if(usb->bufferI[z]==READ_DATA_PROG)
                {
                    if(usb->memCODE_W[k]<0x3FFF&&(usb->memCODE_W[k]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
                    {
                        CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError2],k,usb->memCODE_W[k],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing address %3X: written %04X, read %04X\r\n"
                        err++;
                    }
                    z+=2;
                }
            }
            emit PrintStatus(GlobalVariables::strings[S_CodeV2],i*100/(dim+dim2),i);	//"Verify: %d%%, addr. %04X"
            emit onProgressChanged(i, dim+dim2);
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d\n"
                // WriteLogIO();
            }
            if(err>=usb->max_err) i=dim;
        }
    }
    CommonFunctions::PrintStatusEnd();
    if(k<dim)
    {
        CommonFunctions::printMessage2(GlobalVariables::strings[S_CodeVError3],dim,k);	//"Error verifying code area, requested %d words, read %d\r\n"
    }
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    if(err>=usb->max_err)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
    }
    //****************** write eeprom ********************
    if(dim2&&err<usb->max_err)
    {
        int errEE=0;
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Writing EEPROM ... "
        // PrintStatusSetup();
        j=1;
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T3;
        usb->bufferU[j++]=5000>>8;
        usb->bufferU[j++]=5000&0xff;
        usb->bufferU[j++]=BULK_ERASE_DATA;
        usb->bufferU[j++]=WAIT_T3;			// wait 5ms
        usb->bufferU[j++]=CUST_CMD;
        usb->bufferU[j++]=0x16;		//Reset address
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(7);
        usb->read();
        j=1;
        //if(usb->saveLog)WriteLogIO();
        for(w=i=k=0;i<dim2;i++)
        {
            if(usb->memEE[i]<0xff)
            {
                usb->bufferU[j++]=LOAD_DATA_DATA;
                usb->bufferU[j++]=usb->memEE[i];
                usb->bufferU[j++]=BEGIN_PROG;			//internally timed, T=5ms max
                usb->bufferU[j++]=WAIT_T3;				//Tprogram
                usb->bufferU[j++]=READ_DATA_DATA;
                w++;
            }
            usb->bufferU[j++]=INC_ADDR;
            if(j>usb->DIMBUF-12||i==dim2-1)
            {
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],(i+dim)*100/(dim+dim2),i);	//"Writing: %d%%, add. %03X"
                emit onProgressChanged(i, dim+dim2);
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(w*5+2);
                w=0;
                usb->read();
                for(z=1;z<usb->DIMBUF-4;z++)
                {
                    if(usb->bufferI[z]==INC_ADDR&&usb->memEE[k]>=0xff) k++;
                    else if(usb->bufferI[z]==LOAD_DATA_DATA&&usb->bufferI[z+3]==READ_DATA_DATA)
                    {
                        if (usb->memEE[k]!=usb->bufferI[z+4])
                        {
                            CommonFunctions::printMessage("\r\n");
                            CommonFunctions::printMessage3(GlobalVariables::strings[S_CodeWError3],k,usb->memEE[k],usb->bufferI[z+4]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                            errEE++;
                            if(usb->max_err&&err+errEE>usb->max_err)
                            {
                                CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err+errEE);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                                CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"write interrupted"
                                i=dim2;
                                z=usb->DIMBUF;
                            }
                        }
                        k++;
                        z+=5;
                    }
                }
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,errEE);	//"i=%d, k=%d, errors=%d\n"
                    // WriteLogIO();
                }
            }
            emit onProgressChanged(i, dim);
        }
        emit onProgressChanged(dim, dim);
        errEE+=i-k;
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],errEE);	//"completed, %d errors\r\n"
        err+=errEE;
    }
    //****************** write ID, CONFIG, CALIB ********************
    if(usb->max_err&&err<usb->max_err)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigAreaW]);	//"Writing CONFIG area ... "
        int err_c=0;
        usb->bufferU[j++]=SET_PARAMETER;
        usb->bufferU[j++]=SET_T3;
        usb->bufferU[j++]=5000>>8;
        usb->bufferU[j++]=5000&0xff;
        usb->bufferU[j++]=LOAD_CONF;			//PC @ 0x8000
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=0xFF;
        if(usb->programID)
        {
            for(i=0x8000;i<0x8004;i++)
            {
                usb->bufferU[j++]=LOAD_DATA_PROG;
                usb->bufferU[j++]=usb->memCODE_W[i]>>8;		//MSB
                usb->bufferU[j++]=usb->memCODE_W[i]&0xff;		//LSB
                usb->bufferU[j++]=BEGIN_PROG;			//internally timed
                usb->bufferU[j++]=WAIT_T3;				//Tprogram 5ms
                usb->bufferU[j++]=READ_DATA_PROG;
                usb->bufferU[j++]=INC_ADDR;
            }
            usb->bufferU[j++]=INC_ADDR_N;
            usb->bufferU[j++]=3;
        }
        else{
            usb->bufferU[j++]=INC_ADDR_N;
            usb->bufferU[j++]=7;
        }
        usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 0x8007
        usb->bufferU[j++]=usb->memCODE_W[0x8007]>>8;		//MSB
        usb->bufferU[j++]=usb->memCODE_W[0x8007]&0xff;		//LSB
        usb->bufferU[j++]=BEGIN_PROG;			//internally timed
        usb->bufferU[j++]=WAIT_T3;				//Tprogram 5ms
        usb->bufferU[j++]=READ_DATA_PROG;
        usb->bufferU[j++]=INC_ADDR;
        usb->bufferU[j++]=LOAD_DATA_PROG;			//Config word 2
        usb->bufferU[j++]=usb->memCODE_W[0x8008]>>8;		//MSB
        usb->bufferU[j++]=usb->memCODE_W[0x8008]&0xff;		//LSB
        usb->bufferU[j++]=BEGIN_PROG;			//internally timed
        usb->bufferU[j++]=WAIT_T3;				//Tprogram 5ms
        usb->bufferU[j++]=READ_DATA_PROG;
        usb->bufferU[j++]=INC_ADDR;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(12);
        if(usb->programID) usb->msDelay(22);
        usb->read();
        for(i=0,z=0;usb->programID&&i<4;i++)
        {
            for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
            if (usb->memCODE_W[0x8000+i]!=(usb->bufferI[z+1]<<8)+usb->bufferI[z+2])
            {
                CommonFunctions::printMessage("\r\n");
                CommonFunctions::printMessage3(GlobalVariables::strings[S_IDErr],i,usb->memCODE_W[0x8000+i],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing ID%d: written %04X, read %04X\r\n"
                err_c++;
            }
            z+=6;
        }
        for(;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if(~usb->memCODE_W[0x8007]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
        {	//error if written 0 and read 1 (~W&R)
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x8007],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
            err_c++;
        }
        for(z+=6;z<usb->DIMBUF-2&&usb->bufferI[z]!=READ_DATA_PROG;z++);
        if(~usb->memCODE_W[0x8008]&((usb->bufferI[z+1]<<8)+usb->bufferI[z+2]))
        {	//error if written 0 and read 1 (~W&R)
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr3],usb->memCODE_W[0x8008],(usb->bufferI[z+1]<<8)+usb->bufferI[z+2]);	//"Error writing config area: written %04X, read %04X\r\n"
            err_c++;
        }
        err+=err_c;
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
        if(usb->saveLog)
        {
            //fprintf(logfile,GlobalVariables::strings[S_Log9],err);	//"Config area 	errors=%d \n"
            // WriteLogIO();
        }
    }
    //****************** exit ********************
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    //CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(usb->stop-usb->start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();			//clear status report
}

void ProgP16::initPIC16_ID()
{
    devices->setDeviceID(0x00A,"16F72",0x1F);			//00 0000 101x xxxx
    devices->setDeviceID(0x046,"12F683",0x1F);			//00 0100 011x xxxx
    devices->setDeviceID(0x04A,"16F685",0x1F);			//00 0100 101x xxxx
    devices->setDeviceID(0x04C,"16F818",0x1F);			//00 0100 110x xxxx
    devices->setDeviceID(0x04E,"16F819",0x1F);			//00 0100 111x xxxx
    devices->setDeviceID(0x056,"16F84A",0x1F); 		//00 0101 011x xxxx
    devices->setDeviceID(0x060,"16F73",0x1F);  		//00 0110 000x xxxx
    devices->setDeviceID(0x062,"16F74",0x1F); 	 		//00 0110 001x xxxx
    devices->setDeviceID(0x064,"16F76",0x1F);			//00 0110 010x xxxx
    devices->setDeviceID(0x066,"16F77",0x1F);			//00 0110 011x xxxx
    devices->setDeviceID(0x072,"16F87",0x1F);			//00 0111 001x xxxx
    devices->setDeviceID(0x076,"16F88",0x1F);			//00 0111 011x xxxx
    devices->setDeviceID(0x07A,"16F627",0x1F);			//00 0111 101x xxxx
    devices->setDeviceID(0x07C,"16F628",0x1F);			//00 0111 110x xxxx
    devices->setDeviceID(0x08E,"16F872",0x1F);			//00 1000 111x xxxx
    devices->setDeviceID(0x092,"16F874",0x1F);			//00 1001 001x xxxx
    devices->setDeviceID(0x096,"16F873",0x1F);			//00 1001 011x xxxx
    devices->setDeviceID(0x09A,"16F877",0x1F);			//00 1001 101x xxxx
    devices->setDeviceID(0x09E,"16F876",0x1F);			//00 1001 111x xxxx
    devices->setDeviceID(0x0BA,"16F737",0x1F);			//00 1011 101x xxxx
    devices->setDeviceID(0x0BE,"16F747",0x1F);			//00 1011 111x xxxx
    devices->setDeviceID(0x0D0,"16F870",0x1F);			//00 1101 000x xxxx
    devices->setDeviceID(0x0D2,"16F871",0x1F);			//00 1101 001x xxxx
    devices->setDeviceID(0x0DE,"16F777",0x1F);			//00 1101 111x xxxx
    devices->setDeviceID(0x0E0,"16F876A",0xF);			//00 1110 0000 xxxx
    devices->setDeviceID(0x0E2,"16F877A",0xF);			//00 1110 0010 xxxx
    devices->setDeviceID(0x0E4,"16F873A",0xF);			//00 1110 0100 xxxx
    devices->setDeviceID(0x0E6,"16F874A",0xF);			//00 1110 0110 xxxx
    devices->setDeviceID(0x0EA,"16F767",0x1F);			//00 1110 101x xxxx
    devices->setDeviceID(0x0F8,"12F629",0x1F);			//00 1111 100x xxxx
    devices->setDeviceID(0x0FA,"12F635",0x1F);			//00 1111 101x xxxx
    devices->setDeviceID(0x0FC,"12F675",0x1F);			//00 1111 110x xxxx
    devices->setDeviceID(0x104,"16F627A",0x1F);		//01 0000 010x xxxx
    devices->setDeviceID(0x106,"16F628A",0x1F);		//01 0000 011x xxxx
    devices->setDeviceID(0x108,"16F684",0x1F);			//01 0000 100x xxxx
    devices->setDeviceID(0x110,"16F648A",0x1F);		//01 0001 000x xxxx
    devices->setDeviceID(0x10A,"16F636-639",0x1F);		//01 0000 101x xxxx
    devices->setDeviceID(0x10C,"16F630",0x1F);			//01 0000 110x xxxx
    devices->setDeviceID(0x10E,"16F676",0x1F);			//01 0000 111x xxxx
    devices->setDeviceID(0x114,"16F716",0x1F);			//01 0001 010x xxxx
    devices->setDeviceID(0x118,"16F688",0x1F);			//01 0001 100x xxxx
    devices->setDeviceID(0x120,"16F785",0x1F);			//01 0010 000x xxxx
    devices->setDeviceID(0x122,"16HV785",0x1F);		//01 0010 001x xxxx
    devices->setDeviceID(0x124,"16F616",0x1F);			//01 0010 010x xxxx
    devices->setDeviceID(0x126,"16HV616",0x1F);		//01 0010 011x xxxx
    devices->setDeviceID(0x132,"16F687",0x1F);			//01 0011 001x xxxx
    devices->setDeviceID(0x134,"16F689",0x1F);			//01 0011 010x xxxx
    devices->setDeviceID(0x138,"16F917",0xF);			//01 0011 1000 xxxx
    devices->setDeviceID(0x13A,"16F916",0xF);			//01 0011 1010 xxxx
    devices->setDeviceID(0x13C,"16F914",0xF);			//01 0011 1100 xxxx
    devices->setDeviceID(0x13E,"16F913",0xF);			//01 0011 1110 xxxx
    devices->setDeviceID(0x140,"16F690",0x1F);			//01 0100 000x xxxx
    devices->setDeviceID(0x142,"16F631",0x1F);			//01 0100 001x xxxx
    devices->setDeviceID(0x144,"16F677",0x1F);			//01 0100 010x xxxx
    devices->setDeviceID(0x146,"16F946",0xF);			//01 0100 0110 xxxx
    devices->setDeviceID(0x148,"16F1847",0x1F);		//01 0100 100x xxxx
    devices->setDeviceID(0x14A,"16LF1847",0x1F);		//01 0100 101x xxxx
    devices->setDeviceID(0x158,"16F1526",0x1F);		//01 0101 100x xxxx
    devices->setDeviceID(0x15A,"16F1527",0x1F);		//01 0101 101x xxxx
    devices->setDeviceID(0x15C,"16LF1526",0x1F);		//01 0101 110x xxxx
    devices->setDeviceID(0x15E,"16LF1527",0x1F);		//01 0101 111x xxxx
    devices->setDeviceID(0x168,"16F1516",0x1F);		//01 0110 100x xxxx
    devices->setDeviceID(0x16A,"16F1517",0x1F);		//01 0110 101x xxxx
    devices->setDeviceID(0x16C,"16F1518",0x1F);		//01 0110 110x xxxx
    devices->setDeviceID(0x16E,"16F1519",0x1F);		//01 0110 111x xxxx
    devices->setDeviceID(0x178,"16LF1516",0x1F);		//01 0111 100x xxxx
    devices->setDeviceID(0x17A,"16LF1517",0x1F);		//01 0111 101x xxxx
    devices->setDeviceID(0x17C,"16LF1518",0x1F);		//01 0111 110x xxxx
    devices->setDeviceID(0x17E,"16LF1519",0x1F);		//01 0111 111x xxxx
    devices->setDeviceID(0x180,"16F727",0x1F);			//01 1000 000x xxxx
    devices->setDeviceID(0x182,"16F726",0x1F);			//01 1000 001x xxxx
    devices->setDeviceID(0x184,"16F724",0x1F);			//01 1000 010x xxxx
    devices->setDeviceID(0x186,"16F723",0x1F);			//01 1000 011x xxxx
    devices->setDeviceID(0x188,"16F722",0x1F);			//01 1000 100x xxxx
    devices->setDeviceID(0x190,"16LF727",0x1F);		//01 1001 000x xxxx
    devices->setDeviceID(0x192,"16LF726",0x1F);		//01 1001 001x xxxx
    devices->setDeviceID(0x194,"16LF724",0x1F);		//01 1001 010x xxxx
    devices->setDeviceID(0x196,"16LF723",0x1F);		//01 1001 011x xxxx
    devices->setDeviceID(0x198,"16LF722",0x1F);		//01 1001 100x xxxx
    devices->setDeviceID(0x1AC,"16F707",0x1F);			//01 1010 110x xxxx
    devices->setDeviceID(0x1AE,"16LF707",0x1F);		//01 1010 111x xxxx
    devices->setDeviceID(0x1B0,"16F723A",0x1F);		//01 1011 000x xxxx
    devices->setDeviceID(0x1B2,"16F722A",0x1F);		//01 1011 001x xxxx
    devices->setDeviceID(0x1B4,"16LF723A",0x1F);		//01 1011 010x xxxx
    devices->setDeviceID(0x1B6,"16LF722A",0x1F);		//01 1011 011x xxxx
    devices->setDeviceID(0x1B8,"12F1840",0x1F);		//01 1011 100x xxxx
    devices->setDeviceID(0x1BA,"12LF1840",0x1F);		//01 1011 101x xxxx
    devices->setDeviceID(0x1C0,"16F720",0x1F);			//01 1100 000x xxxx
    devices->setDeviceID(0x1C2,"16F721",0x1F);			//01 1100 001x xxxx
    devices->setDeviceID(0x1C4,"16LF720",0x1F);		//01 1100 010x xxxx
    devices->setDeviceID(0x1C6,"16LF721",0x1F);		//01 1100 011x xxxx
    devices->setDeviceID(0x200,"16F882",0x1F);			//10 0000 000x xxxx
    devices->setDeviceID(0x202,"16F883",0x1F);			//10 0000 001x xxxx
    devices->setDeviceID(0x204,"16F884",0x1F);			//10 0000 010x xxxx
    devices->setDeviceID(0x206,"16F886",0x1F);			//10 0000 011x xxxx
    devices->setDeviceID(0x208,"16F887",0x1F);			//10 0000 100x xxxx
    devices->setDeviceID(0x218,"12F615",0x1F);			//10 0001 100x xxxx
    devices->setDeviceID(0x21A,"12HV615",0x1F);		//10 0001 101x xxxx
    devices->setDeviceID(0x224,"12F609",0x1F);			//10 0010 010x xxxx
    devices->setDeviceID(0x226,"16F610",0x1F);			//10 0010 011x xxxx
    devices->setDeviceID(0x228,"12HV609",0x1F);		//10 0010 100x xxxx
    devices->setDeviceID(0x22A,"16HV610",0x1F);		//10 0010 101x xxxx
    devices->setDeviceID(0x232,"16F1933",0x1F);		//10 0011 001x xxxx
    devices->setDeviceID(0x234,"16F1934",0x1F);		//10 0011 010x xxxx
    devices->setDeviceID(0x236,"16F1936",0x1F);		//10 0011 011x xxxx
    devices->setDeviceID(0x238,"16F1937",0x1F);		//10 0011 100x xxxx
    devices->setDeviceID(0x23A,"16F1938",0x1F);		//10 0011 101x xxxx
    devices->setDeviceID(0x23C,"16F1939",0x1F);		//10 0011 110x xxxx
    devices->setDeviceID(0x242,"16LF1933",0x1F);		//10 0100 001x xxxx
    devices->setDeviceID(0x244,"16LF1934",0x1F);		//10 0100 010x xxxx
    devices->setDeviceID(0x246,"16LF1936",0x1F);		//10 0100 011x xxxx
    devices->setDeviceID(0x248,"16LF1937",0x1F);		//10 0100 100x xxxx
    devices->setDeviceID(0x24A,"16LF1938",0x1F);		//10 0100 101x xxxx
    devices->setDeviceID(0x24C,"16LF1939",0x1F);		//10 0100 110x xxxx
    devices->setDeviceID(0x250,"16F1946",0x1F);		//10 0101 000x xxxx
    devices->setDeviceID(0x252,"16F1947",0x1F);		//10 0101 001x xxxx
    devices->setDeviceID(0x258,"16LF1946",0x1F);		//10 0101 100x xxxx
    devices->setDeviceID(0x25A,"16LF1947",0x1F);		//10 0101 101x xxxx
    devices->setDeviceID(0x270,"16F1822",0x1F);		//10 0111 000x xxxx
    devices->setDeviceID(0x272,"16F1823",0x1F);		//10 0111 001x xxxx
    devices->setDeviceID(0x274,"16F1824",0x1F);		//10 0111 010x xxxx
    devices->setDeviceID(0x276,"16F1825",0x1F);		//10 0111 011x xxxx
    devices->setDeviceID(0x278,"16F1826",0x1F);		//10 0111 100x xxxx
    devices->setDeviceID(0x27A,"16F1827",0x1F);		//10 0111 101x xxxx
    devices->setDeviceID(0x280,"16LF1822",0x1F);		//10 1000 000x xxxx
    devices->setDeviceID(0x282,"16LF1823",0x1F);		//10 1000 001x xxxx
    devices->setDeviceID(0x284,"16LF1824",0x1F);		//10 1000 010x xxxx
    devices->setDeviceID(0x286,"16LF1825",0x1F);		//10 1000 011x xxxx
    devices->setDeviceID(0x288,"16LF1826",0x1F);		//10 1000 100x xxxx
    devices->setDeviceID(0x28A,"16LF1827",0x1F);		//10 1000 101x xxxx
    devices->setDeviceID(0x2A0,"16F1782",0x1F);		//10 1010 000x xxxx
    devices->setDeviceID(0x2A2,"16F1783",0x1F);		//10 1010 001x xxxx
    devices->setDeviceID(0x2A4,"16F1784",0x1F);		//10 1010 010x xxxx
    devices->setDeviceID(0x2A6,"16F1786",0x1F);		//10 1010 011x xxxx
    devices->setDeviceID(0x2A8,"16F1787",0x1F);		//10 1010 100x xxxx
    devices->setDeviceID(0x2AA,"16LF1782",0x1F);		//10 1010 101x xxxx
    devices->setDeviceID(0x2AC,"16LF1783",0x1F);		//10 1010 110x xxxx
    devices->setDeviceID(0x2AE,"16LF1784",0x1F);		//10 1010 111x xxxx
    devices->setDeviceID(0x2B0,"16LF1786",0x1F);		//10 1011 000x xxxx
    devices->setDeviceID(0x2B2,"16LF1787",0x1F);		//10 1011 001x xxxx
    devices->setDeviceID(0x2C0,"16LF1903",0x1F);		//10 1100 000x xxxx
    devices->setDeviceID(0x2C2,"16LF1902",0x1F);		//10 1100 001x xxxx
    devices->setDeviceID(0x2C4,"16LF1907",0x1F);		//10 1100 010x xxxx
    devices->setDeviceID(0x2C6,"16LF1906",0x1F);		//10 1100 011x xxxx
    devices->setDeviceID(0x2C8,"16LF1904",0x1F);		//10 1100 100x xxxx
    devices->setDeviceID(0x2CC,"12F1501",0x1F);		//10 1100 110x xxxx
    devices->setDeviceID(0x2CE,"16F1503",0x1F);		//10 1100 111x xxxx
    devices->setDeviceID(0x2D0,"16F1507",0x1F);		//10 1101 000x xxxx
    devices->setDeviceID(0x2D2,"16F1508",0x1F);		//10 1101 001x xxxx
    devices->setDeviceID(0x2D4,"16F1509",0x1F);		//10 1101 010x xxxx
    devices->setDeviceID(0x2D8,"12LF1501",0x1F);		//10 1101 100x xxxx
    devices->setDeviceID(0x2DA,"16LF1503",0x1F);		//10 1101 101x xxxx
    devices->setDeviceID(0x2DC,"16LF1507",0x1F);		//10 1101 110x xxxx
    devices->setDeviceID(0x2DE,"16LF1508",0x1F);		//10 1101 111x xxxx
    devices->setDeviceID(0x2E0,"16LF1509",0x1F);		//10 1110 000x xxxx
    devices->setDeviceID(0x3020,"16F1454",0);
    devices->setDeviceID(0x3021,"16F1455",0);
    devices->setDeviceID(0x3023,"16F1459",0);
    devices->setDeviceID(0x3024,"16LF1454",0);
    devices->setDeviceID(0x3025,"16LF1455",0);
    devices->setDeviceID(0x3027,"16LF1459",0);
    devices->setDeviceID(0x3030,"16F753",0);
    devices->setDeviceID(0x3031,"16HV753",0);
    devices->setDeviceID(0x3050,"12F1572",0);
    devices->setDeviceID(0x3051,"12F1571",0);
    devices->setDeviceID(0x3052,"12LF1572",0);
    devices->setDeviceID(0x3053,"12LF1571",0);
}
