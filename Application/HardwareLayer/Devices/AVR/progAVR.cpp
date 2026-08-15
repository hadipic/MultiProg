#include "progAVR.h"
#include "Libs/globalVariables.h"

#define  LOCK	1
#define  FUSE	2
#define  FUSE_H 4
#define  FUSE_X	8
#define  CAL	16
#define  SLOW	256
#define  RST 0x40


#define PB0 0x10
#define PB1 0x1
#define PB2 0x2
#define PB3 0x8
/// read ATMEL AVR using HV serial programming
/// dim=FLASH size in bytes, dim2=EEPROM size
/// options: LOCK,FUSE,FUSE_H,FUSE_X,CAL


ProgAVR::ProgAVR(USBTools *usb, Devices *devices, QObject *parent):ProgDevice(usb, parent)
{
    this->devices=devices;
    initAtmelID();
    AVRlock=0x100,AVRfuse=0x100,AVRfuse_h=0x100,AVRfuse_x=0x100;
}
void ProgAVR::ReadAT(int dim, int dim2, int options)
{
    // read ATMEL AVR
    // dim=FLASH size in bytes, dim2=EEPROM size
    // options: LOCK,FUSE,FUSE_H,FUSE_X,CAL
    //			SLOW = slow communication

    int k=0,k2=0,z=0,i,j;
    BYTE signature[]={0,0,0};
    if(dim>0x20000||dim<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_CodeLim]);	//"Code size out of limits\r\n"
        return;
    }
    if(dim2>0x1000||dim2<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    // if(usb->saveLog)
    //{
    //OpenLogFile();	//"Log.txt"
    //fprintf(logfile,"ReadAT(0x%X,0x%X,0x%X)\n",dim,dim2,options);
    // }
    usb->size=dim;
    usb->sizeEE=dim2;
    if(usb->memCODE)
        free(usb->memCODE);
    usb->memCODE=(unsigned char*)malloc(dim);		//CODE
    if(usb->memEE)
        free(usb->memEE);
    usb->memEE=(unsigned char*)malloc(dim2);			//EEPROM
    for(j=0;j<usb->size;j++)
        usb->memCODE[j]=0xFF;
    for(j=0;j<usb->sizeEE;j++)
        usb->memEE[j]=0xFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=20000>>8;
    usb->bufferU[j++]=20000&0xff;
    usb->bufferU[j++]=VREG_DIS;		//Disable HV reg
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SPI_INIT;
    usb->bufferU[j++]=options&SLOW?0:1;				//0=100k, 1=200k
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=3;				//0=100k,200k,500k,1M,2M
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(i=0;i<32;i++)
    {
        j=1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=RST;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP (for 14 pin and 8pin devices)
        usb->bufferU[j++]=0x5;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=WAIT_T3;		//20ms
        usb->bufferU[j++]=SPI_WRITE;		//Programming enable
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0x53;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(25);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(usb->bufferI[z+2]==0x53) i=32;
    }
    if(i<33)
    {
        j=1;
        usb->bufferU[j++]=EN_VPP_VCC;	//VDD
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SPI_INIT;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(3);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        CommonFunctions::printMessage(GlobalVariables::strings[S_SyncErr]);	//"Synchronization error\r\n"
        //if(usb->saveLog) CloseLogFile();
        return;
    }
    j=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    if(options&LOCK)
    {			//LOCK byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x58;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }
    if(options&FUSE)
    {			//FUSE byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x50;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }
    if(options&FUSE_H)
    {			//FUSE high byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x58;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }
    if(options&FUSE_X)
    {			//extended FUSE byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x50;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(8);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[0]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[1]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[2]=usb->bufferI[z+2];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    findByID(signature, true);
    if(options&LOCK)
    {			//LOCK byte
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        AVRlock=usb->bufferI[z+2];
        CommonFunctions::printMessage1("LOCK bits:\t  0x%02X\r\n",usb->bufferI[z+2]);
    }
    if(options&FUSE)
    {			//FUSE byte
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        AVRfuse=usb->bufferI[z+2];
        CommonFunctions::printMessage1("FUSE bits:\t  0x%02X\r\n",usb->bufferI[z+2]);
    }
    if(options&FUSE_H)
    {			//FUSE high byte
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        AVRfuse_h=usb->bufferI[z+2];
        CommonFunctions::printMessage1("FUSE HIGH bits:\t  0x%02X\r\n",usb->bufferI[z+2]);
    }
    if(options&FUSE_X)
    {			//extended FUSE byte
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        AVRfuse_x=usb->bufferI[z+2];
        CommonFunctions::printMessage1("Extended FUSE bits: 0x%02X\r\n",usb->bufferI[z+2]);
    }
    if(options&CAL)
    {			//calibration byte
        j=1;
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x38;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x38;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x38;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x38;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(4);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        CommonFunctions::printMessage1("Calibration bits:\t  0x%02X",usb->bufferI[z+2]);
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        CommonFunctions::printMessage1(",0x%02X",usb->bufferI[z+2]);
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        CommonFunctions::printMessage1(",0x%02X",usb->bufferI[z+2]);
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        CommonFunctions::printMessage1(",0x%02X\r\n",usb->bufferI[z+2]);
    }
    //****************** read code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_CodeReading1]);		//read code ...
    //PrintStatusSetup();
    int c=(usb->DIMBUF-5)/2;
    for(i=0,j=1;i<dim;i+=c*2)
    {
        usb->bufferU[j++]=AT_READ_DATA;
        usb->bufferU[j++]=i<(dim-2*c)?c:(dim-i)/2;
        usb->bufferU[j++]=i>>9;
        usb->bufferU[j++]=i>>1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(options&SLOW?30:15);	//15
        usb->read();
        if(usb->bufferI[1]==AT_READ_DATA)
        {
            for(z=3;z<usb->bufferI[2]*2+3&&z<usb->DIMBUF;z++) usb->memCODE[k++]=usb->bufferI[z];
        }
        emit PrintStatus(GlobalVariables::strings[S_CodeReading],i*100/(dim+dim2),i);	//"Reading: %3d%%, add. %03X"
        emit onProgressChanged(i, dim);
        j=1;

        if(usb->saveLog)
        {
            //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
            //WriteLogIO();
        }
    }
    emit onProgressChanged(dim, dim);
    //   GlobalVariables::printStatusEnd();
    if(k!=dim)
    {
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ReadCodeErr2],dim,k);	//"Errore in lettura area programma, richiesti %d byte, letti %d\r\n"
    }
    else CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** read eeprom ********************
    if(dim2)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_ReadEE]);		//read EE ...
        //PrintStatusSetup();
        for(k2=0,i=0,j=1;i<dim2;i++)
        {
            usb->bufferU[j++]=SPI_WRITE;		//Read eeprom memory
            usb->bufferU[j++]=3;
            usb->bufferU[j++]=0xA0;
            usb->bufferU[j++]=i>>8;
            usb->bufferU[j++]=i;
            usb->bufferU[j++]=SPI_READ;
            usb->bufferU[j++]=1;
            if(j>usb->DIMBUF-9||i==dim-1)
            {
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(4);
                usb->read();
                for(z=1;z<usb->DIMBUF-2;z++)
                {
                    if(usb->bufferI[z]==SPI_READ&&usb->bufferI[z+1]==1)
                    {
                        usb->memEE[k2++]=usb->bufferI[z+2];
                        z+=3;
                    }
                }
                emit PrintStatus(GlobalVariables::strings[S_CodeReading],(i+dim)*100/(dim+dim2),i);	//"Reading: %3d%%, add. %03X"
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k2,k2);	//"i=%d(0x%X), k=%d(0x%X)\n"
                    //WriteLogIO();
                }
            }
        }
        CommonFunctions::PrintStatusEnd();
        if(k2!=dim2)
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_ReadEEErr],dim2,k2);	//"Error reading EEPROM area, requested %d bytes, read %d\r\n"
        }
        else CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    }
    //****************** exit program mode ********************
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=SPI_WRITE;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    // GlobalVariables::PintStatusClear();
    //****************** visualize ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_CodeMem]);	//"\r\nProgram memory\r\n"
    //  DisplayCODEAVR(dim);
    if(dim2)
    {
        //DisplayEE();	//visualize EE
    }
    emit onFuseRead();
    //  CommonFunctions::printMessage1(GlobalVariables::strings[S_End],((stop-start)/1000.0));	//"\r\nEnd (%.2f s)\r\n"
    //if(usb->saveLog) CloseLogFile();
}

void ProgAVR::ReadAT_Fuse(int options, bool readLock)
{
    int k=0, k2=0, z=0, i,j;
    BYTE signature[]={0,0,0};
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=20000>>8;
    usb->bufferU[j++]=20000&0xff;
    usb->bufferU[j++]=VREG_DIS;		//Disable HV reg
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SPI_INIT;
    usb->bufferU[j++]=options&SLOW?0:1;				//0=100k, 1=200k
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=3;				//0=100k,200k,500k,1M,2M
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(i=0;i<32;i++)
    {
        j=1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=RST;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP (for 14 pin and 8pin devices)
        usb->bufferU[j++]=0x5;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=WAIT_T3;		//20ms
        usb->bufferU[j++]=SPI_WRITE;		//Programming enable
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0x53;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(25);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(usb->bufferI[z+2]==0x53) i=32;
    }
    if(i<33)
    {
        j=1;
        usb->bufferU[j++]=EN_VPP_VCC;	//VDD
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SPI_INIT;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(3);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        CommonFunctions::printMessage(GlobalVariables::strings[S_SyncErr]);	//"Synchronization error\r\n"
        //if(usb->saveLog) CloseLogFile();
        return;
    }
    j=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    if(options&LOCK)
    {			//LOCK byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x58;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }
    if(options&FUSE)
    {			//FUSE byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x50;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }
    if(options&FUSE_H)
    {			//FUSE high byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x58;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }
    if(options&FUSE_X)
    {			//extended FUSE byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x50;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(8);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[0]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[1]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[2]=usb->bufferI[z+2];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    findByID(signature, true);
    if(options&LOCK)
    {			//LOCK byte
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(readLock)
        {
            AVRlock=usb->bufferI[z+2];
            CommonFunctions::printMessage1("LOCK bits:\t  0x%02X\r\n",usb->bufferI[z+2]);
        }
    }
    if(options&FUSE)
    {			//FUSE byte
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(!readLock)
        {
            AVRfuse=usb->bufferI[z+2];
            CommonFunctions::printMessage1("FUSE bits:\t  0x%02X\r\n",usb->bufferI[z+2]);
        }
    }
    if(options&FUSE_H)
    {			//FUSE high byte
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(!readLock)
        {
            AVRfuse_h=usb->bufferI[z+2];
            CommonFunctions::printMessage1("FUSE HIGH bits:\t  0x%02X\r\n",usb->bufferI[z+2]);
        }
    }
    if(options&FUSE_X)
    {			//extended FUSE byte
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(!readLock)
        {
            AVRfuse_x=usb->bufferI[z+2];
            CommonFunctions::printMessage1("Extended FUSE bits: 0x%02X\r\n",usb->bufferI[z+2]);
        }
    }
    if(options&CAL)
    {			//calibration byte
        j=1;
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x38;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x38;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x38;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x38;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(4);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        CommonFunctions::printMessage1("Calibration bits:\t  0x%02X",usb->bufferI[z+2]);
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        CommonFunctions::printMessage1(",0x%02X",usb->bufferI[z+2]);
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        CommonFunctions::printMessage1(",0x%02X",usb->bufferI[z+2]);
        for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        CommonFunctions::printMessage1(",0x%02X\r\n",usb->bufferI[z+2]);
    }
    //****************** exit program mode ********************
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=SPI_WRITE;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
}




void ProgAVR::ReadAT_HV(int dim, int dim2, int options)
{
    int k=0,z=0,i,j;
    BYTE signature[]={0,0,0};
    if(usb->FWVersion<0x900)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_FWver2old],"0.9.0");	//"This firmware is too old. Version %s is required\r\n"
        return;
    }
    if(dim>0x20000||dim<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_CodeLim]);	//"Code size out of limits\r\n"
        return;
    }
    if(dim2>0x800||dim2<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(usb->saveLog)
    {
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"ReadAT_HV(0x%X,0x%X,0x%X)\n",dim,dim2,options);
    }
    usb->size=dim;
    usb->sizeEE=dim2;
    if(usb->memCODE) free(usb->memCODE);
    usb->memCODE=(unsigned char*)malloc(dim);		//CODE
    if(usb->memEE) free(usb->memEE);
    usb->memEE=(unsigned char*)malloc(dim2);			//EEPROM
    for(j=0;j<usb->size;j++) usb->memCODE[j]=0xFF;
    for(j=0;j<usb->sizeEE;j++) usb->memEE[j]=0xFF;
    if(!usb->StartHVReg(12))
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_HVregErr]); //"HV regulator error\r\n"
        return;
    }
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PORT_DIR;
    usb->bufferU[j++]=0xFC;
    usb->bufferU[j++]=0x7;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD + VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PORT_DIR;	//RELEASE PB2
    usb->bufferU[j++]=0xFE;
    usb->bufferU[j++]=0x7;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(5);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    j=1;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x01;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x02;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    if(options&LOCK)
    {			//LOCK byte
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x78;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7C;
        usb->bufferU[j++]=0x00;
    }
    if(options&FUSE)
    {			//FUSE byte
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x68;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6C;
        usb->bufferU[j++]=0x00;
    }
    if(options&FUSE_H)
    {			//FUSE high byte
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x7A;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7E;
        usb->bufferU[j++]=0x00;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    j=1;
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[0]=usb->bufferI[z+1];
    for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[1]=usb->bufferI[z+1];
    for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[2]=usb->bufferI[z+1];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    findByID(signature, true);
    if(options&LOCK)
    {			//LOCK byte
        for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        CommonFunctions::printMessage1("LOCK byte:\t  0x%02X\r\n",usb->bufferI[z+1]);
    }
    if(options&FUSE)
    {			//FUSE byte
        for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        CommonFunctions::printMessage1("FUSE byte:\t  0x%02X\r\n",usb->bufferI[z+1]);
    }
    if(options&FUSE_H)
    {			//FUSE high byte
        for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        CommonFunctions::printMessage1("FUSE HIGH byte:\t  0x%02X\r\n",usb->bufferI[z+1]);
    }
    if(options&FUSE_X)
    {			//extended FUSE byte
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x6A;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6E;
        usb->bufferU[j++]=0x00;
    }
    if(options&CAL)
    {			//calibration byte
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x08;
        usb->bufferU[j++]=0x0C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x78;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7C;
        usb->bufferU[j++]=0x00;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    j=1;
    usb->write();
    usb->msDelay(2);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    z=1;
    if(options&FUSE_X)
    {			//extended FUSE byte
        for(;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        CommonFunctions::printMessage1("Extended FUSE byte: 0x%02X\r\n",usb->bufferI[z+1]);
        z+=2;
    }
    if(options&CAL)
    {			//calibration byte
        for(;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        CommonFunctions::printMessage1("Calibration byte: 0x%02X\r\n",usb->bufferI[z+1]);
    }
    //****************** read code ********************
    if(usb->saveLog)//fprintf(logfile,"READ CODE\n");
        CommonFunctions::printMessage(GlobalVariables::strings[S_CodeReading1]);		//read code ...
    //PrintStatusSetup();
    j=1;
    usb->bufferU[j++]=AT_HV_RTX;		//Read FLASH
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x02;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    j=1;
    //if(usb->saveLog)	WriteLogIO();
    for(i=0;i<dim;)
    {
        if((i&511)==0)
        { //change high address after 256 words
            usb->bufferU[j++]=AT_HV_RTX;
            usb->bufferU[j++]=1;
            usb->bufferU[j++]=0x1C;
            usb->bufferU[j++]=i>>9;
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(2);
            usb->read();
            j=1;
            //if(usb->saveLog)	WriteLogIO();
        }
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x0C;
        usb->bufferU[j++]=(i>>1)&0xFF;
        usb->bufferU[j++]=0x68;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=0x78;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7C;
        usb->bufferU[j++]=0x00;
        i+=2;
        if(j>usb->DIMBUF-14||i>=dim-2)
        {
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(2);
            usb->read();
            for(z=1;z<usb->DIMBUF-1;z++)
            {
                if(usb->bufferI[z]==AT_HV_RTX)
                {
                    usb->memCODE[k++]=usb->bufferI[z+1];
                    z+=1;
                }
            }
            emit PrintStatus(GlobalVariables::strings[S_CodeReading],i*100/(dim+dim2),i);	//"Read: %d%%, addr. %03X"
            j=1;
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
                //     WriteLogIO();
            }
        }
    }
    CommonFunctions::PrintStatusEnd();
    if(k!=dim)
    {
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2(GlobalVariables::strings[S_ReadCodeErr],dim,k);	//"Error reading code area, requested %d words, read %d\r\n"
    }
    else CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);
    //****************** read eeprom ********************
    if(dim2)
    {
        if(usb->saveLog)//fprintf(logfile,"READ EEPROM\n");
            CommonFunctions::printMessage(GlobalVariables::strings[S_ReadEE]);		//read EE ...
        //PrintStatusSetup();
        j=1;
        k=0;
        usb->bufferU[j++]=AT_HV_RTX;		//Read EEPROM
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x03;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(2);
        usb->read();
        j=1;
        //if(usb->saveLog)	WriteLogIO();
        for(i=0;i<dim2;i++)
        {
            if((i&255)==0)
            {
                usb->bufferU[j++]=AT_HV_RTX;
                usb->bufferU[j++]=1;
                usb->bufferU[j++]=0x1C;
                usb->bufferU[j++]=i>>8;
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(2);
                usb->read();
                j=1;
                //if(usb->saveLog)	WriteLogIO();
            }
            usb->bufferU[j++]=AT_HV_RTX;
            usb->bufferU[j++]=3;
            usb->bufferU[j++]=0x0C;
            usb->bufferU[j++]=i&0xFF;
            usb->bufferU[j++]=0x68;
            usb->bufferU[j++]=0x00;
            usb->bufferU[j++]=0x6C;
            usb->bufferU[j++]=0x00;
            if(j>usb->DIMBUF-8||i>=dim2-2)
            {
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(2);
                usb->read();
                for(z=1;z<usb->DIMBUF-1;z++)
                {
                    if(usb->bufferI[z]==AT_HV_RTX)
                    {
                        usb->memEE[k++]=usb->bufferI[z+1];
                        z+=1;
                    }
                }
                emit PrintStatus(GlobalVariables::strings[S_CodeReading],i*100/(dim+dim2),i);	//"Read: %d%%, addr. %03X"
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
                    //    WriteLogIO();
                }
            }
        }
        CommonFunctions::PrintStatusEnd();
        if(k!=dim2)
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2(GlobalVariables::strings[S_ReadEEErr],dim2,k);	//"Error reading EEPROM area, requested %d bytes, read %d\r\n"
        }
        else CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);
    }
    //****************** exit program mode ********************
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PORT_DIR; //All input
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    CommonFunctions::PrintStatusClear();
    //****************** visualize ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_CodeMem]);	//"\r\nProgram memory\r\n"
    //DisplayCODEAVR(dim);
    if(dim2)
    {
        //    DisplayEE();	//visualize EE
    }
    // CommonFunctions::printMessage1(GlobalVariables::strings[S_End],((stop-start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"
    //if(usb->saveLog) CloseLogFile();
    emit onFuseRead();
}

void ProgAVR::ReadAT_HV_Fuse(int options, bool readLock)
{
    int z=0,j;
    BYTE signature[]={0,0,0};
    if(!usb->StartHVReg(12))
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_HVregErr]); //"HV regulator error\r\n"
        return;
    }
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PORT_DIR;
    usb->bufferU[j++]=0xFC;
    usb->bufferU[j++]=0x7;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD + VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PORT_DIR;	//RELEASE PB2
    usb->bufferU[j++]=0xFE;
    usb->bufferU[j++]=0x7;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(5);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    j=1;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x01;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x02;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    if(options&LOCK)
    {			//LOCK byte
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x78;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7C;
        usb->bufferU[j++]=0x00;
    }
    if(options&FUSE)
    {			//FUSE byte
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x68;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6C;
        usb->bufferU[j++]=0x00;
    }
    if(options&FUSE_H)
    {			//FUSE high byte
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x7A;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7E;
        usb->bufferU[j++]=0x00;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    j=1;
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[0]=usb->bufferI[z+1];
    for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[1]=usb->bufferI[z+1];
    for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[2]=usb->bufferI[z+1];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    findByID(signature, true);
    if(options&LOCK)
    {			//LOCK byte
        for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        CommonFunctions::printMessage1("LOCK byte:\t  0x%02X\r\n",usb->bufferI[z+1]);
    }
    if(options&FUSE)
    {			//FUSE byte
        for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        CommonFunctions::printMessage1("FUSE byte:\t  0x%02X\r\n",usb->bufferI[z+1]);
    }
    if(options&FUSE_H)
    {			//FUSE high byte
        for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        CommonFunctions::printMessage1("FUSE HIGH byte:\t  0x%02X\r\n",usb->bufferI[z+1]);
    }
    if(options&FUSE_X)
    {			//extended FUSE byte
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x6A;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6E;
        usb->bufferU[j++]=0x00;
    }
    if(options&CAL)
    {			//calibration byte
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x08;
        usb->bufferU[j++]=0x0C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x78;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7C;
        usb->bufferU[j++]=0x00;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    j=1;
    usb->write();
    usb->msDelay(2);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    z=1;
    if(options&FUSE_X)
    {			//extended FUSE byte
        for(;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        CommonFunctions::printMessage1("Extended FUSE byte: 0x%02X\r\n",usb->bufferI[z+1]);
        z+=2;
    }
    if(options&CAL)
    {			//calibration byte
        for(;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        CommonFunctions::printMessage1("Calibration byte: 0x%02X\r\n",usb->bufferI[z+1]);
    }
    //****************** exit program mode ********************
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PORT_DIR; //All input
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
}


void ProgAVR::WriteAT(int dim, int dim2)
{
    // write ATMEL micro
    // dim=FLASH size in bytes, dim2=EEPROM size

    int k=0,z=0,i,j;
    int err=0,Rtry=0,maxTry=0;
    BYTE signature[]={0,0,0};
    if(dim>0x8000||dim<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_CodeLim]);	//"Code size out of limits\r\n"
        return;
    }
    if(dim2>0x800||dim2<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(usb->saveLog)
    {
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"WriteAT(0x%X,0x%X)\n",dim,dim2);
    }
    if(dim>usb->size) dim=usb->size;
    if(dim2>usb->sizeEE) dim2=usb->sizeEE;
    if(dim<1)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoCode]);	//"Data area is empty\r\n"
        return;
    }
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=20000>>8;
    usb->bufferU[j++]=20000&0xff;
    usb->bufferU[j++]=VREG_DIS;		//Disable HV reg
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SPI_INIT;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(i=0;i<32;i++)
    {
        j=1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=RST;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=WAIT_T3;		//20ms
        usb->bufferU[j++]=SPI_WRITE;		//Programming enable
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0x53;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(25);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        //CommonFunctions::printMessage("i=%d z=%d   rx:%02X%02X\r\n",i,z,usb->bufferI[z+2],usb->bufferI[z+3]);
        if(usb->bufferI[z+2]==0x53) i=32;
    }
    if(i<33)
    {
        j=1;
        usb->bufferU[j++]=EN_VPP_VCC;	//VDD
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SPI_INIT;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        CommonFunctions::printMessage(GlobalVariables::strings[S_SyncErr]);	//"Synchronization error\r\n"
        //if(usb->saveLog) CloseLogFile();
        return;
    }
    j=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[0]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[1]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[2]=usb->bufferI[z+2];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    findByID(signature, true);
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=SPI_WRITE;		//Chip erase
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0xAC;
    usb->bufferU[j++]=0x80;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    //PrintStatusSetup();
    for(i=0,j=1;i<dim;i++)
    {
        if(usb->memCODE[i]!=0xFF)
        {
            usb->bufferU[j++]=SPI_WRITE;		//Write program memory
            usb->bufferU[j++]=4;
            usb->bufferU[j++]=0x40+(i&1?8:0);
            usb->bufferU[j++]=i>>9;
            usb->bufferU[j++]=i>>1;
            usb->bufferU[j++]=usb->memCODE[i];
            usb->bufferU[j++]=WAIT_T3;		//6ms
            usb->bufferU[j++]=WAIT_T3;
            usb->bufferU[j++]=WAIT_T3;
            usb->bufferU[j++]=SPI_WRITE;		//Read program memory
            usb->bufferU[j++]=3;
            usb->bufferU[j++]=0x20+(i&1?8:0);
            usb->bufferU[j++]=i>>9;
            usb->bufferU[j++]=i>>1;
            usb->bufferU[j++]=SPI_READ;
            usb->bufferU[j++]=1;
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
            j=1;
            usb->write();
            usb->msDelay(9);
            usb->read();
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Write: %d%%, addr. %03X"
            emit onProgressChanged(i, dim);
            for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
            if(z==usb->DIMBUF-2||usb->memCODE[i]!=usb->bufferI[z+2])
            {
                if(Rtry<5)
                {
                    Rtry++;
                    if (Rtry>maxTry) maxTry=Rtry;
                    i--;
                }
                else{
                    err++;
                    Rtry=0;
                }
            }
            if(usb->max_err &&err>usb->max_err)
            {
                CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                i=dim;
            }
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, err=%d\n"
                //WriteLogIO();
            }
        }
    }
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write eeprom ********************
    if(dim2)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Write EEPROM ... "
        //PrintStatusSetup();
        int errEE=0;
        for(i=0,j=1;i<dim2;i++)
        {
            if(usb->memEE[i]!=0xFF)
            {
                usb->bufferU[j++]=SPI_WRITE;		//Write EEPROM memory
                usb->bufferU[j++]=4;
                usb->bufferU[j++]=0xC0;
                usb->bufferU[j++]=i>>8;
                usb->bufferU[j++]=i;
                usb->bufferU[j++]=usb->memEE[i];
                usb->bufferU[j++]=WAIT_T3;		//6ms
                usb->bufferU[j++]=WAIT_T3;
                usb->bufferU[j++]=WAIT_T3;
                usb->bufferU[j++]=SPI_WRITE;		//Read EEPROM memory
                usb->bufferU[j++]=3;
                usb->bufferU[j++]=0xA0;
                usb->bufferU[j++]=i>>8;
                usb->bufferU[j++]=i;
                usb->bufferU[j++]=SPI_READ;
                usb->bufferU[j++]=1;
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                j=1;
                usb->write();
                usb->msDelay(9);
                usb->read();
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim2,i);	//"Write: %d%%, addr. %03X"
                emit onProgressChanged(i, dim2);
                for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
                if(z==usb->DIMBUF-2||usb->memEE[i]!=usb->bufferI[z+2])
                {
                    if(Rtry<10)
                    {
                        Rtry++;
                        if (Rtry>maxTry) maxTry=Rtry;
                        i--;
                    }
                    else{
                        errEE++;
                        Rtry=0;
                    }
                }
                if(usb->max_err&&err+errEE>usb->max_err)
                {
                    CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err+errEE);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                    CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                    i=dim2;
                }
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,errEE);	//"i=%d, k=%d, err=%d\n"
                    //WriteLogIO();
                }
            }
        }
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],errEE);	//"completed, %d errors\r\n"
        err+=errEE;
    }
    if(maxTry) CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxRetry],maxTry); 	//"Max retries in writing: %d\r\n"
    //****************** write FUSE ********************
    if(AVRlock<0x100)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_FuseAreaW]);	//"Write Fuse ... "
        usb->bufferU[j++]=SPI_WRITE;		//Write lock
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0xF9+(AVRlock&0x06);
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=WAIT_T3;		//9ms
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(9);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    }
    //****************** exit program mode ********************
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    //   CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],((stop-start)/1000.0),err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    // if(usb->saveLog)	CloseLogFile();
    //  GlobalVariables::PintStatusClear();

}

void ProgAVR::WriteAT_Fuse()
{
    // write ATMEL micro
    // dim=FLASH size in bytes, dim2=EEPROM size

    int k=0,z=0,i,j;
    int err=0,Rtry=0,maxTry=0;
    BYTE signature[]={0,0,0};
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=20000>>8;
    usb->bufferU[j++]=20000&0xff;
    usb->bufferU[j++]=VREG_DIS;		//Disable HV reg
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SPI_INIT;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(i=0;i<32;i++)
    {
        j=1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=RST;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=WAIT_T3;		//20ms
        usb->bufferU[j++]=SPI_WRITE;		//Programming enable
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0x53;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(25);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        //CommonFunctions::printMessage("i=%d z=%d   rx:%02X%02X\r\n",i,z,usb->bufferI[z+2],usb->bufferI[z+3]);
        if(usb->bufferI[z+2]==0x53) i=32;
    }
    if(i<33)
    {
        j=1;
        usb->bufferU[j++]=EN_VPP_VCC;	//VDD
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SPI_INIT;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        CommonFunctions::printMessage(GlobalVariables::strings[S_SyncErr]);	//"Synchronization error\r\n"
        //if(usb->saveLog) CloseLogFile();
        return;
    }
    j=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[0]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[1]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[2]=usb->bufferI[z+2];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    findByID(signature, true);
    //****************** write FUSE ********************
    if(AVRlock<0x100)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_FuseAreaW]);	//"Write Fuse ... "
        usb->bufferU[j++]=SPI_WRITE;		//Write lock
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0xF9+(AVRlock&0x06);
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=WAIT_T3;		//9ms
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(9);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    }
    //****************** exit program mode ********************
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    //   CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],((stop-start)/1000.0),err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    // if(usb->saveLog)	CloseLogFile();
    //  GlobalVariables::PintStatusClear();

}

void ProgAVR::WriteATmega(int dim, int dim2, int page, int options)
{
    // write ATMEL micro
    // dim=FLASH size in bytes, dim2=EEPROM, page=FLASH page size in bytes
    // options: SLOW=slow communication

    int k=0,z=0,i,j;
    int err=0,Rtry=0,maxTry=0;
    BYTE signature[]={0,0,0};
    if(dim>0x20000||dim<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_CodeLim]);	//"Code size out of limits\r\n"
        return;
    }
    if(dim2>0x1000||dim2<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(usb->saveLog)
    {
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"WriteATmega(0x%X,0x%X,0x%X,0x%X)\n",dim,dim2,page,options);
    }
    if(dim>usb->size) dim=usb->size;
    else{
        usb->size=dim;
        usb->memCODE=(unsigned char*)realloc(usb->memCODE,dim);
    }
    if(usb->size%(page*2))
    {	//grow to an integer number of pages
        j=usb->size;
        dim=(j/(page*2)+1)*page*2;
        usb->memCODE=(unsigned char*)realloc(usb->memCODE,dim);
        for(;j<dim;j++) usb->memCODE[j]=0xFF;
    }
    if(dim2>usb->sizeEE) dim2=usb->sizeEE;
    if(dim<1)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoCode]);	//"Data area is empty\r\n"
        return;
    }
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=20000>>8;
    usb->bufferU[j++]=20000&0xff;
    usb->bufferU[j++]=VREG_DIS;		//Disable HV reg
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SPI_INIT;
    usb->bufferU[j++]=options&SLOW?0:1;				//0=100k, 1=200k
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(i=0;i<32;i++)
    {
        j=1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=RST;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP (for 14 pin and 8pin devices)
        usb->bufferU[j++]=0x5;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=WAIT_T3;		//20ms
        usb->bufferU[j++]=SPI_WRITE;		//Programming enable
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0x53;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(25);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        //CommonFunctions::printMessage("i=%d z=%d   rx:%02X%02X\r\n",i,z,usb->bufferI[z+2],usb->bufferI[z+3]);
        //CommonFunctions::printMessage(str);
        if(usb->bufferI[z+2]==0x53)
            i=32;
    }
    if(i<33)
    {
        j=1;
        usb->bufferU[j++]=EN_VPP_VCC;	//VDD
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SPI_INIT;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        CommonFunctions::printMessage(GlobalVariables::strings[S_SyncErr]);	//"Synchronization error\r\n"
        //if(usb->saveLog) CloseLogFile();
        return;
    }
    j=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[0]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[1]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[2]=usb->bufferI[z+2];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    findByID(signature, true);
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=SPI_WRITE;		//Chip erase
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0xAC;
    usb->bufferU[j++]=0x80;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=5000>>8;
    usb->bufferU[j++]=5000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    //PrintStatusSetup();
    int w=0,v,c;
    for(i=0;i<dim;i+=page*2)
    {
        for(z=i,v=0;z<i+page*2;z++) if(usb->memCODE[z]<0xFF)v=1;
        if(v)
        {
            for(k=0,j=1,v=0;k<page;k+=w)
            {
                w=(page-k)<(usb->DIMBUF-6)/2?(page-k):(usb->DIMBUF-6)/2;
                usb->bufferU[j++]=AT_LOAD_DATA;
                usb->bufferU[j++]=w;
                usb->bufferU[j++]=k>>8;
                usb->bufferU[j++]=k;
                for(z=0;z<w*2;z++)	usb->bufferU[j++]=usb->memCODE[i+k*2+z];
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                j=1;
                usb->write();
                usb->msDelay(options&SLOW?30:15);
                usb->read();
                //if(usb->saveLog)WriteLogIO();
            }
            usb->bufferU[j++]=SPI_WRITE;		//Write program memory page
            usb->bufferU[j++]=4;
            usb->bufferU[j++]=0x4C;
            usb->bufferU[j++]=i>>9;
            usb->bufferU[j++]=i>>1;
            usb->bufferU[j++]=0;
            usb->bufferU[j++]=WAIT_T3;		//5ms
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
            j=1;
            usb->write();
            usb->msDelay(10);
            usb->read();
            emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Write: %d%%, addr. %03X"
            emit onProgressChanged(i, dim);
            //if(usb->saveLog)WriteLogIO();
            //write verification
            c=(usb->DIMBUF-5)/2;
            for(k=0,j=1;k<page;k+=c)
            {
                for(Rtry=0;Rtry<5;Rtry++)
                {		//Try to read a few times
                    usb->bufferU[j++]=AT_READ_DATA;
                    usb->bufferU[j++]=k<(page-c)?c:page-k;
                    usb->bufferU[j++]=(i+k*2)>>9;
                    usb->bufferU[j++]=(i+k*2)>>1;
                    usb->bufferU[j++]=FLUSH;
                    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                    usb->write();
                    usb->msDelay(options&SLOW?30:15);	//25
                    usb->read();
                    //if(usb->saveLog)WriteLogIO();
                    if(usb->bufferI[1]==AT_READ_DATA)
                    {
                        for(w=0,z=3;z<usb->bufferI[2]*2+3&&z<usb->DIMBUF;z++)
                        {
                            if(usb->memCODE[i+k*2+w]!=usb->bufferI[z])
                            {
                                if(Rtry<4)	z=usb->DIMBUF;
                                else err++;
                            }
                            w++;
                        }
                        if(z<usb->DIMBUF) Rtry=100;
                    }
                    j=1;
                }
            }
            if(usb->saveLog)
            {
                //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,w,w,err);	//"i=%d, k=%d, err=%d\n"
            }
            if(usb->max_err&&err>usb->max_err)
            {
                CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                i=dim;
            }
        }
    }
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write eeprom ********************
    if(dim2)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Write EEPROM ... "
        //PrintStatusSetup();
        int errEE=0;
        for(i=0,j=1;i<dim2;i++)
        {
            if(usb->memEE[i]!=0xFF)
            {
                usb->bufferU[j++]=SPI_WRITE;		//Write EEPROM memory
                usb->bufferU[j++]=4;
                usb->bufferU[j++]=0xC0;
                usb->bufferU[j++]=i>>8;
                usb->bufferU[j++]=i;
                usb->bufferU[j++]=usb->memEE[i];
                usb->bufferU[j++]=WAIT_T3;		//5ms
                usb->bufferU[j++]=WAIT_T3;		//5ms
                usb->bufferU[j++]=SPI_WRITE;		//Read EEPROM memory
                usb->bufferU[j++]=3;
                usb->bufferU[j++]=0xA0;
                usb->bufferU[j++]=i>>8;
                usb->bufferU[j++]=i;
                usb->bufferU[j++]=SPI_READ;
                usb->bufferU[j++]=1;
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)
                    usb->bufferU[j]=0x0;
                j=1;
                usb->write();
                usb->msDelay(12);
                usb->read();
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim2,i);	//"Write: %d%%, addr. %03X"
                emit onProgressChanged(i, dim2);
                for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
                if(z==usb->DIMBUF-2||usb->memEE[i]!=usb->bufferI[z+2])
                {
                    if(Rtry<4)
                    {
                        Rtry++;
                        if (Rtry>maxTry) maxTry=Rtry;
                        i--;
                    }
                    else{
                        errEE++;
                        Rtry=0;
                    }
                }
                if(usb->max_err&&err+errEE>usb->max_err)
                {
                    CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err+errEE);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                    CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                    i=dim2;
                }
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,errEE);	//"i=%d, k=%d, err=%d\n"
                    //     WriteLogIO();
                }
            }
        }
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],errEE);	//"completed, %d errors\r\n"
        err+=errEE;
    }
    //****************** write FUSE ********************
    int err_f=0;
    if(AVRlock<0x100||AVRfuse<0x100||AVRfuse_h<0x100||AVRfuse_x<0x100)
        CommonFunctions::printMessage(GlobalVariables::strings[S_FuseAreaW]);	//"Write Fuse ... "
    if(options&FUSE && AVRfuse<0x100)
    {
        usb->bufferU[j++]=SPI_WRITE;		//Write fuse
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0xA0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=AVRfuse;
        usb->bufferU[j++]=WAIT_T3;		//9ms
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x50;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(9);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(z==usb->DIMBUF-2||AVRfuse!=usb->bufferI[z+2])
            err_f++;
    }
    if(options&FUSE_H && AVRfuse_h<0x100)
    {
        usb->bufferU[j++]=SPI_WRITE;		//Write fuse_h
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0xA8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=AVRfuse_h;
        usb->bufferU[j++]=WAIT_T3;		//9ms
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x58;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(9);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(z==usb->DIMBUF-2||AVRfuse_h!=usb->bufferI[z+2])
            err_f++;
    }
    if(options&FUSE_X && AVRfuse_x<0x100)
    {
        usb->bufferU[j++]=SPI_WRITE;		//Write ext fuse
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0xA4;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=AVRfuse_x;
        usb->bufferU[j++]=WAIT_T3;		//9ms
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x50;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(9);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(z==usb->DIMBUF-2||AVRfuse_x!=usb->bufferI[z+2])
            err_f++;
    }
    if(options&LOCK && AVRlock<0x100)
    {
        usb->bufferU[j++]=SPI_WRITE;		//Write lock
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0xE0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=AVRlock;
        usb->bufferU[j++]=WAIT_T3;		//9ms
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x58;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(9);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(z==usb->DIMBUF-2||AVRlock!=usb->bufferI[z+2]) err_f++;
    }
    err+=err_f;
    if(AVRlock<0x100||AVRfuse<0x100||AVRfuse_h<0x100||AVRfuse_x<0x100)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_f);	//"completed, %d errors\r\n"
    }
    //	if(maxTry) CommonFunctions::printMessage(GlobalVariables::strings[S_MaxRetry],maxTry); 	//"Max retries in writing: %d\r\n"
    //****************** exit program mode ********************
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=SPI_WRITE;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    // CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(stop-start)/1000.0,err,err!=1?GlobalVariables::strings[S_ErrPlur]:GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //    if(usb->saveLog)CloseLogFile();
    //GlobalVariables::PintStatusClear();
}

void ProgAVR::WriteATmega_Fuse(int options)
{
    int k=0,z=0,i,j;
    int err=0,Rtry=0,maxTry=0;
    BYTE signature[]={0,0,0};

    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=20000>>8;
    usb->bufferU[j++]=20000&0xff;
    usb->bufferU[j++]=VREG_DIS;		//Disable HV reg
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SPI_INIT;
    usb->bufferU[j++]=options&SLOW?0:1;				//0=100k, 1=200k
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(i=0;i<32;i++)
    {
        j=1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=RST;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP (for 14 pin and 8pin devices)
        usb->bufferU[j++]=0x5;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=WAIT_T3;		//20ms
        usb->bufferU[j++]=SPI_WRITE;		//Programming enable
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0x53;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(25);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        //CommonFunctions::printMessage("i=%d z=%d   rx:%02X%02X\r\n",i,z,usb->bufferI[z+2],usb->bufferI[z+3]);
        //CommonFunctions::printMessage(str);
        if(usb->bufferI[z+2]==0x53)
            i=32;
    }
    if(i<33)
    {
        j=1;
        usb->bufferU[j++]=EN_VPP_VCC;	//VDD
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SPI_INIT;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        CommonFunctions::printMessage(GlobalVariables::strings[S_SyncErr]);	//"Synchronization error\r\n"
        //if(usb->saveLog) CloseLogFile();
        return;
    }
    j=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[0]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[1]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[2]=usb->bufferI[z+2];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    findByID(signature, true);
    //****************** write FUSE ********************
    int err_f=0;
    j=1;
    if(AVRlock<0x100||AVRfuse<0x100||AVRfuse_h<0x100||AVRfuse_x<0x100)
        CommonFunctions::printMessage(GlobalVariables::strings[S_FuseAreaW]);	//"Write Fuse ... "
    if(options&FUSE && AVRfuse<0x100)
    {
        usb->bufferU[j++]=SPI_WRITE;		//Write fuse
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0xA0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=AVRfuse;
        usb->bufferU[j++]=WAIT_T3;		//9ms
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x50;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(9);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        usb->msDelay(100);
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++)
            qDebug()<<"bufferI["<<z+1<<"]"<<usb->bufferI[z+1]<<"DIMBUF"<<usb->DIMBUF<<"SPI_READ:"<<SPI_READ;
        if(z==usb->DIMBUF-2||AVRfuse!=usb->bufferI[z+2])
            err_f++;
    }
    if(options&FUSE_H && AVRfuse_h<0x100)
    {
        usb->bufferU[j++]=SPI_WRITE;		//Write fuse_h
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0xA8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=AVRfuse_h;
        usb->bufferU[j++]=WAIT_T3;		//9ms
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x58;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(9);
        usb->read();
        usb->msDelay(100);
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(z==usb->DIMBUF-2||AVRfuse_h!=usb->bufferI[z+2])
            err_f++;
    }
    if(options&FUSE_X && AVRfuse_x<0x100)
    {
        usb->bufferU[j++]=SPI_WRITE;		//Write ext fuse
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0xA4;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=AVRfuse_x;
        usb->bufferU[j++]=WAIT_T3;		//9ms
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x50;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(9);
        usb->read();
        usb->msDelay(100);
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(z==usb->DIMBUF-2||AVRfuse_x!=usb->bufferI[z+2])
            err_f++;
    }
    if(options&LOCK && AVRlock<0x100)
    {
        usb->bufferU[j++]=SPI_WRITE;		//Write lock
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0xE0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=AVRlock;
        usb->bufferU[j++]=WAIT_T3;		//9ms
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x58;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(9);
        usb->read();
        usb->msDelay(100);
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(z==usb->DIMBUF-2||AVRlock!=usb->bufferI[z+2])
            err_f++;
    }
    err+=err_f;
    if(AVRlock<0x100||AVRfuse<0x100||AVRfuse_h<0x100||AVRfuse_x<0x100)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_f);	//"completed, %d errors\r\n"
    }
    //	if(maxTry) CommonFunctions::printMessage(GlobalVariables::strings[S_MaxRetry],maxTry); 	//"Max retries in writing: %d\r\n"
    //****************** exit program mode ********************
    j=1;
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=SPI_WRITE;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    emit onFuseRead();
}


void ProgAVR::WriteAT_HV(int dim, int dim2, int page, int options)
{
    /// Write ATMEL AVR using HV serial programming
    /// dim=FLASH size in bytes, dim2=EEPROM, page=FLASH page size in words (0 if page write not supported)
    /// options: not used

    int k=0,z=0,i,j,t,sdo,err=0;
    BYTE signature[]={0,0,0};
    if(usb->FWVersion<0x900)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_FWver2old],"0.9.0");	//"This firmware is too old. Version %s is required\r\n"
        return;
    }
    if(dim>0x10000||dim<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_CodeLim]);	//"Code size out of limits\r\n"
        return;
    }
    if(dim2>0x800||dim2<0)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    //if(usb->saveLog)
    //{
    //OpenLogFile();	//"Log.txt"
    //fprintf(logfile,"WriteAT_HV(0x%X,0x%X,0x%X,0x%X)\n",dim,dim2,page,options);
    //}
    if(dim>usb->size) dim=usb->size;
    else{
        usb->size=dim;
        usb->memCODE=(unsigned char*)realloc(usb->memCODE,dim);
    }
    if(page&&(usb->size%(page*2)))
    {	//grow to an integer number of pages
        j=usb->size;
        dim=(j/(page*2)+1)*page*2;
        usb->memCODE=(unsigned char*)realloc(usb->memCODE,dim);
        for(;j<dim;j++) usb->memCODE[j]=0xFF;
    }
    if(dim2>usb->sizeEE) dim2=usb->sizeEE;
    if(dim<1)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoCode]);	//"Data area is empty\r\n"
        return;
    }
    if(!usb->StartHVReg(12))
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_HVregErr]); //"HV regulator error\r\n"
        return;
    }
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PORT_DIR;
    usb->bufferU[j++]=0xFC;
    usb->bufferU[j++]=0x7;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD + VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PORT_DIR;	//RELEASE PB2
    usb->bufferU[j++]=0xFE;
    usb->bufferU[j++]=0x7;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(5);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    j=1;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x01;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x02;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(8);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[0]=usb->bufferI[z+1];
    for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[1]=usb->bufferI[z+1];
    for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[2]=usb->bufferI[z+1];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    findByID(signature, true);
    //****************** erase memory ********************
    //if(usb->saveLog)//fprintf(logfile,"CHIP ERASE\n");
    j=1;
    usb->bufferU[j++]=AT_HV_RTX;	//Chip erase
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x80;
    usb->bufferU[j++]=0x64;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=READ_B;	//check SDO
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1.5);
    usb->read();
    j=1;
    //if(usb->saveLog)WriteLogIO();
    usb->bufferU[j++]=READ_B;	//check SDO
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    for(t=0,sdo=0;t<20&&sdo==0;t++)
    {
        usb->write();
        usb->msDelay(1.5);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
        sdo=usb->bufferI[z+1]&2;
    }
    if(sdo==0&&usb->saveLog) //fprintf(logfile,"SDO=0\r\n");
        CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    //PrintStatusSetup();
    //if(usb->saveLog)//fprintf(logfile,"WRITE CODE\n");
    int currPage=-1;
    j=1;
    if(page==0)
    {		//byte write
        for(i=0,k=0;i<dim;i+=2)
        {
            if(usb->memCODE[i]!=0xFF||usb->memCODE[i+1]!=0xFF)
            {
                usb->bufferU[j++]=AT_HV_RTX;
                usb->bufferU[j++]=6;
                usb->bufferU[j++]=0x4C;	//Write FLASH
                usb->bufferU[j++]=0x10;
                usb->bufferU[j++]=0x1C;
                usb->bufferU[j++]=i>>9;
                usb->bufferU[j++]=0x0C;
                usb->bufferU[j++]=(i/2)&0xFF;
                usb->bufferU[j++]=0x2C;
                usb->bufferU[j++]=usb->memCODE[i];
                usb->bufferU[j++]=0x64;		//write data low
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=0x6C;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=READ_B;	//check SDO
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(2);
                usb->read();
                j=1;
                //if(usb->saveLog)
                //{
                //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
                //WriteLogIO();
                //}
                usb->bufferU[j++]=READ_B;	//check SDO
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                j=1;
                for(t=0,sdo=0;t<20&&sdo==0;t++)
                {
                    usb->write();
                    usb->msDelay(1.5);
                    usb->read();
                    //if(usb->saveLog)WriteLogIO();
                    for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
                    sdo=usb->bufferI[z+1]&2;
                }
                if(usb->saveLog&&sdo==0) //fprintf(logfile,"SDO=0\r\n");
                    usb->bufferU[j++]=AT_HV_RTX;
                usb->bufferU[j++]=3;
                usb->bufferU[j++]=0x3C;
                usb->bufferU[j++]=usb->memCODE[i+1];
                usb->bufferU[j++]=0x74;		//write data high
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=0x7C;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=READ_B;	//check SDO
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(2);
                usb->read();
                j=1;
                //if(usb->saveLog)
                //{
                //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
                //WriteLogIO();
                //}
                usb->bufferU[j++]=READ_B;	//check SDO
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                j=1;
                for(t=0,sdo=0;t<20&&sdo==0;t++)
                {
                    usb->write();
                    usb->msDelay(1.5);
                    usb->read();
                    //if(usb->saveLog)WriteLogIO();
                    for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
                    sdo=usb->bufferI[z+1]&2;
                }
                if(usb->saveLog&&sdo==0) //fprintf(logfile,"SDO=0\r\n");
                    emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Write: %d%%, addr. %03X"
                emit onProgressChanged(i, dim);
                //write verification
                usb->bufferU[j++]=AT_HV_RTX;		//Read FLASH
                usb->bufferU[j++]=3;
                usb->bufferU[j++]=0x4C;
                usb->bufferU[j++]=0x02;
                usb->bufferU[j++]=0x68;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=0x6C;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=AT_HV_RTX;
                usb->bufferU[j++]=2;
                usb->bufferU[j++]=0x78;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=0x7C;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(1.5);
                usb->read();
                j=1;
                for(z=1;z<usb->DIMBUF-1;z++)
                {
                    if(usb->bufferI[z]==AT_HV_RTX)
                    {
                        if(usb->memCODE[k]!=usb->bufferI[z+1])
                        {
                            CommonFunctions::printMessage4(GlobalVariables::strings[S_CodeVError],k,k,usb->memCODE[k],usb->bufferI[z+1]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                            err++;
                        }
                        k++;
                        z++;
                    }
                }
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, err=%d\n"
                    //WriteLogIO();
                }
                if(usb->max_err&&err>usb->max_err)
                {
                    CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                    CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                    i=dim;
                }
            }
        }
    }
    else{		//page write
        for(i=0;i<dim;i+=page*2)
        {	//page in words
            for(k=0;k<page;k++)
            {
                if(usb->memCODE[i+k*2]!=0xFF||usb->memCODE[i+k*2+1]!=0xFF) k=page;
            }
            if(k>page)
            {	//only pages with data!=0xFF
                usb->bufferU[j++]=AT_HV_RTX;
                usb->bufferU[j++]=1;
                usb->bufferU[j++]=0x4C;	//Write FLASH
                usb->bufferU[j++]=0x10;
                for(k=0;k<page;k++)
                {
                    usb->bufferU[j++]=AT_HV_RTX;
                    usb->bufferU[j++]=5;
                    usb->bufferU[j++]=0x0C;
                    usb->bufferU[j++]=(i/2+k)&0xFF;
                    usb->bufferU[j++]=0x2C;
                    usb->bufferU[j++]=usb->memCODE[i+k*2];	//data low
                    usb->bufferU[j++]=0x3C;
                    usb->bufferU[j++]=usb->memCODE[i+k*2+1];	//data high
                    usb->bufferU[j++]=0x7D;
                    usb->bufferU[j++]=0x00;
                    usb->bufferU[j++]=0x7C;
                    usb->bufferU[j++]=0x00;
                    if(j>usb->DIMBUF-13||k>=page||i>=dim-2)
                    {
                        usb->bufferU[j++]=FLUSH;
                        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                        usb->write();
                        usb->msDelay(1.5);
                        usb->read();
                        j=1;
                        if(usb->saveLog)
                        {
                            //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
                            //    WriteLogIO();
                        }
                    }
                }
                if((i>>9)!=currPage)
                {	//change high address if changed
                    usb->bufferU[j++]=AT_HV_RTX;
                    usb->bufferU[j++]=1;
                    usb->bufferU[j++]=0x1C;
                    usb->bufferU[j++]=i>>9;
                    currPage=i>>9;
                }
                usb->bufferU[j++]=AT_HV_RTX;	//write page
                usb->bufferU[j++]=2;
                usb->bufferU[j++]=0x64;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=0x6C;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=READ_B;	//check SDO
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(1.5);
                usb->read();
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
                    //WriteLogIO();
                }
                usb->bufferU[j++]=READ_B;	//check SDO
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                for(t=0,sdo=0;t<20&&sdo==0;t++)
                {
                    usb->write();
                    usb->msDelay(1.5);
                    usb->read();
                    //if(usb->saveLog)WriteLogIO();
                    for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
                    sdo=usb->bufferI[z+1]&2;
                }
                if(sdo==0&&usb->saveLog) //fprintf(logfile,"SDO=0\r\n");
                    emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Write: %d%%, addr. %03X"
                emit onProgressChanged(i, dim);
                j=1;
                //write verification
                int m=0;
                for(k=0;k<page;k++)
                {
                    if(k==0)
                    {
                        usb->bufferU[j++]=AT_HV_RTX;		//Read FLASH
                        usb->bufferU[j++]=4;
                        usb->bufferU[j++]=0x4C;
                        usb->bufferU[j++]=0x02;
                    }
                    else{
                        usb->bufferU[j++]=AT_HV_RTX;
                        usb->bufferU[j++]=3;
                    }
                    usb->bufferU[j++]=0x0C;
                    usb->bufferU[j++]=(i/2+k)&0xFF;
                    usb->bufferU[j++]=0x68;
                    usb->bufferU[j++]=0x00;
                    usb->bufferU[j++]=0x6C;
                    usb->bufferU[j++]=0x00;
                    usb->bufferU[j++]=AT_HV_RTX;
                    usb->bufferU[j++]=2;
                    usb->bufferU[j++]=0x78;
                    usb->bufferU[j++]=0x00;
                    usb->bufferU[j++]=0x7C;
                    usb->bufferU[j++]=0x00;
                    if(j>usb->DIMBUF-14||k>=page||i>=dim-2)
                    {
                        usb->bufferU[j++]=FLUSH;
                        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                        usb->write();
                        usb->msDelay(1.5);
                        usb->read();
                        j=1;
                        for(z=1;z<usb->DIMBUF-1;z++)
                        {
                            if(usb->bufferI[z]==AT_HV_RTX)
                            {
                                if(usb->memCODE[i+m]!=usb->bufferI[z+1])
                                {
                                    CommonFunctions::printMessage4(GlobalVariables::strings[S_CodeVError],i+m,i+m,usb->memCODE[i+m],usb->bufferI[z+1]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                                    err++;
                                }
                                m++;
                                z++;
                            }
                        }
                        if(usb->saveLog)
                        {
                            //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,m,m,err);	//"i=%d, k=%d, err=%d\n"
                            //WriteLogIO();
                        }
                        if(usb->max_err&&err>usb->max_err)
                        {
                            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage(GlobalVariables::strings[S_IntW]);	//"Write interrupted"
                            i=dim;
                        }
                    }
                }
            }
        }
    }
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write eeprom ********************
    if(dim2)
    {
        int errEE=0;
        CommonFunctions::printMessage(GlobalVariables::strings[S_EEAreaW]);	//"Write EEPROM ... "
        //PrintStatusSetup();
        if(usb->saveLog)//fprintf(logfile,"WRITE EEPROM\n");
            j=1;
        for(i=0;i<dim2;i++)
        {
            if(usb->memEE[i]!=0xFF)
            {
                usb->bufferU[j++]=AT_HV_RTX;		//Write EEPROM
                usb->bufferU[j++]=7;
                usb->bufferU[j++]=0x4C;
                usb->bufferU[j++]=0x11;
                usb->bufferU[j++]=0x0C;
                usb->bufferU[j++]=i&0xFF;
                usb->bufferU[j++]=0x1C;
                usb->bufferU[j++]=i>>8;
                usb->bufferU[j++]=0x2C;
                usb->bufferU[j++]=usb->memEE[i];
                usb->bufferU[j++]=0x6D;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=0x64;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=0x6C;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=READ_B;	//check SDO
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                j=1;
                usb->write();
                usb->msDelay(2);
                usb->read();
                emit PrintStatus(GlobalVariables::strings[S_CodeWriting],i*100/dim2,i);	//"Write: %d%%, addr. %03X"
                emit onProgressChanged(i, dim2);
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
                    //WriteLogIO();
                }
                usb->bufferU[j++]=READ_B;	//check SDO
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                j=1;
                for(t=0,sdo=0;t<20&&sdo==0;t++)
                {
                    usb->write();
                    usb->msDelay(1.5);
                    usb->read();
                    //if(usb->saveLog)WriteLogIO();
                    for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
                    sdo=usb->bufferI[z+1]&2;
                }
                if(sdo==0&&usb->saveLog) //fprintf(logfile,"SDO=0\r\n");
                    //write verification
                    usb->bufferU[j++]=AT_HV_RTX;		//Read EEPROM
                usb->bufferU[j++]=5;
                usb->bufferU[j++]=0x4C;
                usb->bufferU[j++]=0x03;
                usb->bufferU[j++]=0x1C;
                usb->bufferU[j++]=i>>8;
                usb->bufferU[j++]=0x0C;
                usb->bufferU[j++]=i&0xFF;
                usb->bufferU[j++]=0x68;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=0x6C;
                usb->bufferU[j++]=0x00;
                usb->bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
                usb->write();
                usb->msDelay(2);
                usb->read();
                for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
                if(usb->memEE[i]!=usb->bufferI[z+1])
                {
                    CommonFunctions::printMessage4(GlobalVariables::strings[S_CodeVError],i,i,usb->memEE[i],usb->bufferI[z+1]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                    errEE++;
                }
                j=1;
                if(usb->saveLog)
                {
                    //fprintf(logfile,GlobalVariables::strings[S_Log8],i,i,k,k,errEE);	//"i=%d, k=%d, errors=%d\n"
                    //WriteLogIO();
                }
                if(err+errEE>=usb->max_err) break;
            }
        }
        CommonFunctions::PrintStatusEnd();
        err+=errEE;
        if(err>=usb->max_err)
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage1(GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
        }
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],errEE);	//"completed: %d errors\r\n"
    }
    //****************** write FUSE ********************
    int err_f=0;
    if(AVRlock<0x100||AVRfuse<0x100||AVRfuse_h<0x100||AVRfuse_x<0x100)CommonFunctions::printMessage(GlobalVariables::strings[S_FuseAreaW]);	//"Write Fuse ... "
    if(AVRfuse<0x100)
    {
        if(usb->saveLog)//fprintf(logfile,"WRITE FUSE\n");
            usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x40;
        usb->bufferU[j++]=0x2C;
        usb->bufferU[j++]=AVRfuse;
        usb->bufferU[j++]=0x64;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)	WriteLogIO();
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
        sdo=usb->bufferI[z+1]&2;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        for(i=0;i<20&&sdo==0;i++)
        {
            usb->write();
            usb->msDelay(1.5);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
            sdo=usb->bufferI[z+1]&2;
        }
        if(sdo==0&&usb->saveLog) //fprintf(logfile,"SDO=0\r\n");
            j=1;
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x68;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1.5);
        usb->read();
        j=1;
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        if(z==usb->DIMBUF-1||AVRfuse!=usb->bufferI[z+1])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_ConfigWErr4],"fuse",AVRfuse,usb->bufferI[z+1]);	//"Error writing %s: written %02X, read %02X"
            err_f++;
        }
    }
    if(AVRfuse_h<0x100)
    {
        if(usb->saveLog)//fprintf(logfile,"WRITE FUSEH\n");
            usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x40;
        usb->bufferU[j++]=0x2C;
        usb->bufferU[j++]=AVRfuse_h;
        usb->bufferU[j++]=0x74;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)	WriteLogIO();
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
        sdo=usb->bufferI[z+1]&2;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        for(i=0;i<20&&sdo==0;i++)
        {
            usb->write();
            usb->msDelay(1.5);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
            sdo=usb->bufferI[z+1]&2;
        }
        if(sdo==0&&usb->saveLog) //fprintf(logfile,"SDO=0\r\n");
            j=1;
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x7A;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7E;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1.5);
        usb->read();
        j=1;
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        if(z==usb->DIMBUF-1||AVRfuse_h!=usb->bufferI[z+1])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_ConfigWErr4],"fuseH",AVRfuse_h,usb->bufferI[z+1]);	//"Error writing %s: written %02X, read %02X"
            err_f++;
        }
    }
    if(AVRfuse_x<0x100)
    {
        //if(usb->saveLog)fprintf(logfile,"WRITE FUSEX\n");
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x40;
        usb->bufferU[j++]=0x2C;
        usb->bufferU[j++]=AVRfuse_x;
        usb->bufferU[j++]=0x66;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6E;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)	WriteLogIO();
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
        sdo=usb->bufferI[z+1]&2;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        for(i=0;i<20&&sdo==0;i++)
        {
            usb->write();
            usb->msDelay(1.5);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
            sdo=usb->bufferI[z+1]&2;
        }
        //	if(sdo==0&&usb->saveLog) fprintf(logfile,"SDO=0\r\n");
        j=1;
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x6A;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6E;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1.5);
        usb->read();
        j=1;
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        if(z==usb->DIMBUF-1||AVRfuse_x!=usb->bufferI[z+1])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_ConfigWErr4],"fuseX",AVRfuse_x,usb->bufferI[z+1]);	//"Error writing %s: written %02X, read %02X"
            err_f++;
        }
    }
    if(AVRlock<0x100)
    {
        if(usb->saveLog)//fprintf(logfile,"WRITE LOCK\n");
            usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x20;
        usb->bufferU[j++]=0x2C;
        usb->bufferU[j++]=AVRlock;
        usb->bufferU[j++]=0x64;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)	WriteLogIO();
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
        sdo=usb->bufferI[z+1]&2;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        for(i=0;i<20&&sdo==0;i++)
        {
            usb->write();
            usb->msDelay(1.5);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
            sdo=usb->bufferI[z+1]&2;
        }
        if(sdo==0&&usb->saveLog) //fprintf(logfile,"SDO=0\r\n");
            j=1;
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x78;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1.5);
        usb->read();
        j=1;
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        if(z==usb->DIMBUF-1||AVRlock!=usb->bufferI[z+1])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_ConfigWErr4],"lock",AVRlock,usb->bufferI[z+1]);	//"Error writing %s: written %02X, read %02X"
            err_f++;
        }
    }
    err+=err_f;
    if(AVRlock<0x100||AVRfuse<0x100||AVRfuse_h<0x100||AVRfuse_x<0x100)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_f);	//"completed, %d errors\r\n"
    }
    //****************** exit program mode ********************
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PORT_DIR; //All input
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    PrintMessage3(GlobalVariables::strings[S_EndErr], (usb->stop-usb->start)/1000.0, err, err!=1 ? GlobalVariables::strings[S_ErrPlur].toUtf8().data():GlobalVariables::strings[S_ErrSing].toUtf8().data());	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    // if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();

}

void ProgAVR::WriteAT_HV_Fuse(int options)
{
    /// Write ATMEL AVR using HV serial programming
    /// dim=FLASH size in bytes, dim2=EEPROM, page=FLASH page size in words (0 if page write not supported)
    /// options: not used

    int k=0,z=0,i,j,t,sdo,err=0;
    BYTE signature[]={0,0,0};
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PORT_DIR;
    usb->bufferU[j++]=0xFC;
    usb->bufferU[j++]=0x7;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD + VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PORT_DIR;	//RELEASE PB2
    usb->bufferU[j++]=0xFE;
    usb->bufferU[j++]=0x7;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(5);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    j=1;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x01;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x02;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(8);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[0]=usb->bufferI[z+1];
    for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[1]=usb->bufferI[z+1];
    for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[2]=usb->bufferI[z+1];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    findByID(signature, true);
    //****************** write FUSE ********************
    j=1;
    int err_f=0;
    if(AVRlock<0x100||AVRfuse<0x100||AVRfuse_h<0x100||AVRfuse_x<0x100)
        CommonFunctions::printMessage(GlobalVariables::strings[S_FuseAreaW]);	//"Write Fuse ... "
    if(options&FUSE && AVRfuse<0x100)
    {
        if(usb->saveLog)//fprintf(logfile,"WRITE FUSE\n");
            usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x40;
        usb->bufferU[j++]=0x2C;
        usb->bufferU[j++]=AVRfuse;
        usb->bufferU[j++]=0x64;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)	WriteLogIO();
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
        sdo=usb->bufferI[z+1]&2;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        for(i=0;i<20&&sdo==0;i++)
        {
            usb->write();
            usb->msDelay(1.5);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
            sdo=usb->bufferI[z+1]&2;
        }
        if(sdo==0&&usb->saveLog) //fprintf(logfile,"SDO=0\r\n");
            j=1;
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x68;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1.5);
        usb->read();
        j=1;
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        if(z==usb->DIMBUF-1||AVRfuse!=usb->bufferI[z+1])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_ConfigWErr4],"fuse",AVRfuse,usb->bufferI[z+1]);	//"Error writing %s: written %02X, read %02X"
            err_f++;
        }
    }
    if(options&FUSE_H && AVRfuse_h<0x100)
    {
        if(usb->saveLog)//fprintf(logfile,"WRITE FUSEH\n");
            usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x40;
        usb->bufferU[j++]=0x2C;
        usb->bufferU[j++]=AVRfuse_h;
        usb->bufferU[j++]=0x74;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)	WriteLogIO();
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
        sdo=usb->bufferI[z+1]&2;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        for(i=0;i<20&&sdo==0;i++)
        {
            usb->write();
            usb->msDelay(1.5);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
            sdo=usb->bufferI[z+1]&2;
        }
        if(sdo==0&&usb->saveLog) //fprintf(logfile,"SDO=0\r\n");
            j=1;
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x7A;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7E;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1.5);
        usb->read();
        j=1;
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        if(z==usb->DIMBUF-1||AVRfuse_h!=usb->bufferI[z+1])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_ConfigWErr4],"fuseH",AVRfuse_h,usb->bufferI[z+1]);	//"Error writing %s: written %02X, read %02X"
            err_f++;
        }
    }
    if(options&FUSE_X && AVRfuse_x<0x100)
    {
        //if(usb->saveLog)fprintf(logfile,"WRITE FUSEX\n");
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x40;
        usb->bufferU[j++]=0x2C;
        usb->bufferU[j++]=AVRfuse_x;
        usb->bufferU[j++]=0x66;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6E;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)	WriteLogIO();
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
        sdo=usb->bufferI[z+1]&2;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        for(i=0;i<20&&sdo==0;i++)
        {
            usb->write();
            usb->msDelay(1.5);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
            sdo=usb->bufferI[z+1]&2;
        }
        //	if(sdo==0&&usb->saveLog) fprintf(logfile,"SDO=0\r\n");
        j=1;
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x6A;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6E;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1.5);
        usb->read();
        j=1;
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        if(z==usb->DIMBUF-1||AVRfuse_x!=usb->bufferI[z+1])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_ConfigWErr4],"fuseX",AVRfuse_x,usb->bufferI[z+1]);	//"Error writing %s: written %02X, read %02X"
            err_f++;
        }
    }
    if(options&LOCK && AVRlock<0x100)
    {
        if(usb->saveLog)//fprintf(logfile,"WRITE LOCK\n");
            usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x20;
        usb->bufferU[j++]=0x2C;
        usb->bufferU[j++]=AVRlock;
        usb->bufferU[j++]=0x64;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x6C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        j=1;
        usb->write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)	WriteLogIO();
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
        sdo=usb->bufferI[z+1]&2;
        usb->bufferU[j++]=READ_B;	//check SDO
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        for(i=0;i<20&&sdo==0;i++)
        {
            usb->write();
            usb->msDelay(1.5);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_B;z++);
            sdo=usb->bufferI[z+1]&2;
        }
        if(sdo==0&&usb->saveLog) //fprintf(logfile,"SDO=0\r\n");
            j=1;
        usb->bufferU[j++]=AT_HV_RTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x4C;
        usb->bufferU[j++]=0x04;
        usb->bufferU[j++]=0x78;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=0x7C;
        usb->bufferU[j++]=0x00;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1.5);
        usb->read();
        j=1;
        for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
        if(z==usb->DIMBUF-1||AVRlock!=usb->bufferI[z+1])
        {
            CommonFunctions::printMessage3(GlobalVariables::strings[S_ConfigWErr4],"lock",AVRlock,usb->bufferI[z+1]);	//"Error writing %s: written %02X, read %02X"
            err_f++;
        }
    }
    err+=err_f;
    if(AVRlock<0x100||AVRfuse<0x100||AVRfuse_h<0x100||AVRfuse_x<0x100)
    {
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err_f);	//"completed, %d errors\r\n"
    }
    //****************** exit program mode ********************
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PORT_DIR; //All input
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    PrintMessage3(GlobalVariables::strings[S_EndErr], (usb->stop-usb->start)/1000.0, err, err!=1 ? GlobalVariables::strings[S_ErrPlur].toUtf8().data():GlobalVariables::strings[S_ErrSing].toUtf8().data());	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    // if(usb->saveLog)CloseLogFile();
    CommonFunctions::PrintStatusClear();
    emit onFuseRead();
}



void ProgAVR::DisplayCODEAVR(int dim)
{
    char s[256]="",t[256]="";
    int valid=0,empty=1,i,j;
    char* aux=(char*)malloc((dim/COL+1)*(16+COL*6));
    aux[0]=0;
    s[0]=0;
    for(i=0;i<dim;i+=COL*2)
    {
        valid=0;
        for(j=i;j<i+COL*2&&j<dim;j++)
        {
            sprintf(t,"%02X ",usb->memCODE[j]);
            strcat(s,t);
            if(usb->memCODE[j]<0xff) valid=1;
        }
        if(valid)
        {
            sprintf(t,"%04X: %s\r\n",i,s);
            strcat(aux,t);
            empty=0;
        }
        s[0]=0;
    }
    if(empty) CommonFunctions::printMessage(GlobalVariables::strings[S_Empty]);	//empty
    else CommonFunctions::printMessage(aux);
    free(aux);
}
DeviceSelected ProgAVR::detectDevice()
{
    DeviceSelected deviceSelected;
    deviceSelected.deviceInfo = NULL;
    deviceSelected=detectDeviceAT();
    if(!deviceSelected.deviceInfo)
        deviceSelected=detectDeviceAT_HV();
    return deviceSelected;
}

DeviceSelected ProgAVR::detectDeviceAT()
{
    DeviceSelected deviceSelected;
    deviceSelected.deviceInfo = NULL;
    int z=0, i,j;
    BYTE signature[]={0,0,0};
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=20000>>8;
    usb->bufferU[j++]=20000&0xff;
    usb->bufferU[j++]=VREG_DIS;		//Disable HV reg
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SPI_INIT;
    usb->bufferU[j++]=0;				//0=100k, 1=200k
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=3;				//0=100k,200k,500k,1M,2M
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;		//20ms
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(50);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(i=0;i<32;i++)
    {
        j=1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=RST;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP (for 14 pin and 8pin devices)
        usb->bufferU[j++]=0x5;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=WAIT_T3;		//20ms
        usb->bufferU[j++]=SPI_WRITE;		//Programming enable
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=0xAC;
        usb->bufferU[j++]=0x53;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(25);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
        if(usb->bufferI[z+2]==0x53)
            i=32;
    }
    if(i<33)
    {
        j=1;
        usb->bufferU[j++]=EN_VPP_VCC;	//VDD
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=SPI_INIT;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=CLOCK_GEN;
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(3);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        CommonFunctions::printMessage(GlobalVariables::strings[S_SyncErr]);	//"Synchronization error\r\n"
        //if(usb->saveLog) CloseLogFile();
        return deviceSelected;
    }
    j=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SPI_WRITE;		//Read signature bytes
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=0x30;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    /*if(options&LOCK)
    {			//LOCK byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x58;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }
    if(options&FUSE)
    {			//FUSE byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x50;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }
    if(options&FUSE_H)
    {			//FUSE high byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x58;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }
    if(options&FUSE_X)
    {			//extended FUSE byte
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0x50;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
    }*/
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(8);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[0]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[1]=usb->bufferI[z+2];
    for(z+=3;z<usb->DIMBUF-2&&usb->bufferI[z]!=SPI_READ;z++);
    signature[2]=usb->bufferI[z+2];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    deviceSelected=findByID(signature, true);
    //****************** exit program mode ********************
    j=1;
    usb->bufferU[j++]=CLOCK_GEN;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=SPI_WRITE;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    return deviceSelected;
}

DeviceSelected ProgAVR::detectDeviceAT_HV()
{
    DeviceSelected deviceSelected;
    deviceSelected.deviceInfo = NULL;

    int z=0,j;
    BYTE signature[]={0,0,0};
    /*if(!usb->StartHVReg(12))
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_HVregErr]); //"HV regulator error\r\n"
        return deviceSelected;
    }*/
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PORT_DIR;
    usb->bufferU[j++]=0xFC;
    usb->bufferU[j++]=0x7;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=PB3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD + VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=SET_PORT_DIR;	//RELEASE PB2
    usb->bufferU[j++]=0xFE;
    usb->bufferU[j++]=0x7;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(5);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    j=1;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x01;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=AT_HV_RTX;		//Read signature bytes
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=0x4C;
    usb->bufferU[j++]=0x08;
    usb->bufferU[j++]=0x0C;
    usb->bufferU[j++]=0x02;
    usb->bufferU[j++]=0x68;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=0x6C;
    usb->bufferU[j++]=0x00;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    j=1;
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[0]=usb->bufferI[z+1];
    for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[1]=usb->bufferI[z+1];
    for(z+=2;z<usb->DIMBUF-1&&usb->bufferI[z]!=AT_HV_RTX;z++);
    signature[2]=usb->bufferI[z+1];
    CommonFunctions::printMessage3("CHIP ID:%02X%02X%02X\r\n",signature[0],signature[1],signature[2]);
    deviceSelected=findByID(signature, true);
    //****************** exit program mode ********************
    j=1;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_PORT_DIR; //All input
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=0xFF;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    return deviceSelected;
}

DeviceSelected ProgAVR::findByID(quint8 id[], bool printMsg)
{
    DeviceSelected deviceSelected;
    deviceSelected.deviceInfo = NULL;
    char str[128]="";
    int i,idw=(id[1]<<8)+id[2];
    if(idw==0)
        return deviceSelected;
    if(id[0]==0&&id[1]==1&&id[2]==2)
    {
        if(printMsg)
            CommonFunctions::printMessage(GlobalVariables::strings[S_Protected]);		//"Device protected"
        return deviceSelected;
    }
    int max= devices->allDevices.count();
    int step=1;
    if(id[0]==0x1E)
        strcat(str,"Atmel ");
    bool finish=false;
    for (int i = 0;!finish && i < devices->deviceFamilyList.count(); ++i)
    {
        DeviceFamily *deviceFamily=devices->deviceFamilyList.at(i);
        if(deviceFamily->familyName=="AVR")
            for (int j = 0;!finish && j < deviceFamily->deviceTypesList.count(); ++j)
            {
                DeviceType *deviceType=deviceFamily->deviceTypesList.at(j);
                if(deviceType->typeName.startsWith("AT"))
                    for (int k = 0;!finish && k < deviceType->devicesList.count(); ++k)
                    {
                        emit onProgressChanged(step++, max);

                        DeviceInfo *deviceInfo=deviceType->devicesList.at(k);
                        if(deviceInfo->id==idw)
                        {
                            deviceSelected.deviceInfo = deviceInfo;
                            deviceSelected.familyIndex =i;
                            deviceSelected.typeIndex=j;
                            deviceSelected.infoIndex=k;
                            emit onProgressChanged(max, max);
                            finish=true;
                            strcat(str, deviceSelected.deviceInfo->name.toLatin1());
                        }
                    }
            }
    }

    switch(id[1])
    {
        case 0x90:
            strcat(str," 1KB Flash");
            break;
        case 0x91:
            strcat(str," 2KB Flash");
            break;
        case 0x92:
            strcat(str," 4KB Flash");
            break;
        case 0x93:
            strcat(str," 8KB Flash");
            break;
        case 0x94:
            strcat(str," 16KB Flash");
            break;
        case 0x95:
            strcat(str," 32KB Flash");
            break;
        case 0x96:
            strcat(str," 64KB Flash");
            break;
        case 0x97:
            strcat(str," 128KB Flash");
            break;
        case 0x98:
            strcat(str," 256KB Flash");
            break;
    }
    if(!deviceSelected.deviceInfo)
        strcat(str,"Unknown device\r\n"); //"Unknown device\r\n");
    else
        strcat(str,"\r\n");
    emit onProgressChanged(max, max);
    if(printMsg)
        CommonFunctions::printMessage(str);
    return deviceSelected;
}

void ProgAVR::initAtmelID()
{
    int i=0; // 149 30
    devices->setDeviceID(0x9001,"AT90S1200");
    devices->setDeviceID(0x9004,"ATtiny11");
    devices->setDeviceID(0x9004,"ATtiny12");
    devices->setDeviceID(0x9007,"ATtiny13");
    //2K
    devices->setDeviceID(0x9101,"AT90S2313");
    devices->setDeviceID(0x9109,"ATtiny26");
    devices->setDeviceID(0x910A,"ATtiny2313");
    devices->setDeviceID(0x910B,"ATtiny24");
    devices->setDeviceID(0x910C,"ATtiny261");
    //4K
    devices->setDeviceID(0x9205,"ATmega48");
    devices->setDeviceID(0x920A,"ATmega48PA");
    devices->setDeviceID(0x9207,"ATtiny44");
    devices->setDeviceID(0x9208,"ATtiny461");
    devices->setDeviceID(0x9209,"ATtiny48");
    devices->setDeviceID(0x920D,"ATtiny4313");
    //8K
    devices->setDeviceID(0x9301,"AT90S8515");
    devices->setDeviceID(0x9303,"AT90S8535");
    devices->setDeviceID(0x9306,"ATmega8515");
    devices->setDeviceID(0x9307,"ATmega8");
    devices->setDeviceID(0x9308,"ATmega8535");
    devices->setDeviceID(0x930A,"ATmega88");
    devices->setDeviceID(0x930C,"ATtiny84");
    devices->setDeviceID(0x930D,"ATtiny861");
    devices->setDeviceID(0x930F,"ATmega88PA");
    devices->setDeviceID(0x9311,"ATtiny88");
    //16K
    devices->setDeviceID(0x9403,"ATmega16");
    devices->setDeviceID(0x9406,"ATmega168");
    devices->setDeviceID(0x940A,"ATmega164PA");
    devices->setDeviceID(0x940B,"ATmega168PA");
    devices->setDeviceID(0x940F,"ATmega164A");
    //32K
    devices->setDeviceID(0x9502,"ATmega32");
    devices->setDeviceID(0x950F,"ATmega328P");
    devices->setDeviceID(0x9511,"ATmega324PA");
    devices->setDeviceID(0x9514,"ATmega328");
    devices->setDeviceID(0x9515,"ATmega324A");
    //64K
    devices->setDeviceID(0x9602,"ATmega64");
    devices->setDeviceID(0x9609,"ATmega644A");
    devices->setDeviceID(0x960A,"ATmega644PA");
    //128K
    devices->setDeviceID(0x9705,"ATmega1284P");
    devices->setDeviceID(0x9706,"ATmega1284");
}



