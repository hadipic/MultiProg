#include "progP18.h"
#include "Libs/globalVariables.h"


ProgP18::ProgP18(USBTools *usb, Devices *devices, QObject *parent):ProgDevice(usb, parent)
{
    this->devices=devices;
    initPIC18_ID();
}
void ProgP18::Read18Fx(int dim,int dim2,int options)
{
    // read 16 bit PIC 18Fxxxx
    // dim=program size 	dim2=eeprom size
    // options:
    //   0 = vdd before vpp (12V)
    //   1 = vdd before vpp (9V)
    //   2 = low voltage entry with 32 bit key
    int k=0,k2=0,z=0,i,j;
    int entry=options&0xF;
    if(dim>0x1FFFFF||dim<0)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_CodeLim]);	//"Code size out of limits\r\n"
        return;
    }
    if(dim2>0x800||dim2<0)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(entry>0&&!usb->CheckV33Regulator())
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_noV33reg]);	//Can't find 3.3V expansion board
        return;
    }
    double vpp=entry<2?(entry==0?12:8.5):-1;
    if(!usb->StartHVReg(vpp))
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_HVregErr]); //"HV regulator error\r\n"
        return;
    }
    if( usb->saveLog)
    {
        //OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Read18F(%d,%d,%d)    (0x%X,0x%X,0x%X)\n",dim,dim2,options,dim,dim2,options);
    }
    usb->size=dim;
    usb->sizeEE=dim2;
    if(usb->memCODE)
       free(usb->memCODE);
    usb->memCODE=(unsigned char*)malloc(usb->size);		//CODE
    if(usb->memEE)
       free(usb->memEE);
    usb->memEE=(unsigned char*)malloc(usb->sizeEE);			//EEPROM
    for(j=0;j<8;j++)
       usb->memID[j]=0xFF;
    for(j=0;j<14;j++)
       usb->memCONFIG[j]=0xFF;
    usb->start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb-> bufferU[j++]=SET_PARAMETER;
    usb-> bufferU[j++]=SET_T1T2;
    usb-> bufferU[j++]=1;						//T1=1u
    usb-> bufferU[j++]=100;					//T2=100u
    usb-> bufferU[j++]=SET_PARAMETER;
    usb-> bufferU[j++]=SET_T3;
    usb-> bufferU[j++]=2000>>8;
    usb-> bufferU[j++]=2000&0xff;
    usb-> bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb-> bufferU[j++]=0x0;
    usb-> bufferU[j++]=SET_CK_D;
    usb-> bufferU[j++]=0x0;
    usb-> bufferU[j++]=EN_VPP_VCC;		//VDD
    usb-> bufferU[j++]=0x1;
    usb-> bufferU[j++]=NOP;
    usb-> bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb-> bufferU[j++]=0x5;
    if(entry==2)
    {					//LV entry with key
        usb-> bufferU[j++]=EN_VPP_VCC;		//VDD
        usb-> bufferU[j++]=0x1;
        usb-> bufferU[j++]=WAIT_T3;
        usb-> bufferU[j++]=WAIT_T3;
        usb-> bufferU[j++]=TX16;
        usb-> bufferU[j++]=2;
        usb-> bufferU[j++]=0x4D;
        usb-> bufferU[j++]=0x43;
        usb-> bufferU[j++]=0x48;
        usb-> bufferU[j++]=0x50;
        usb-> bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
        usb-> bufferU[j++]=0x5;
        usb-> bufferU[j++]=WAIT_T3;
    }
    usb-> bufferU[j++]=WAIT_T3;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x0E;			//3F
    usb-> bufferU[j++]=0x3F;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6E;			//-> TBLPTRU
    usb-> bufferU[j++]=0xF8;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x0E;			//FF
    usb-> bufferU[j++]=0xFF;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6E;			//-> TBLPTRH
    usb-> bufferU[j++]=0xF7;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x0E;			//FE
    usb-> bufferU[j++]=0xFE;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6E;			//-> TBLPTRL
    usb-> bufferU[j++]=0xF6;
    usb-> bufferU[j++]=TBLR_INC_N;		//DevID1-2	0x3FFFFE-F
    usb-> bufferU[j++]=2;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRU
    usb-> bufferU[j++]=0xF8;			//TBLPTRU
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRH
    usb-> bufferU[j++]=0xF7;			//TBLPTRH
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRL
    usb-> bufferU[j++]=0xF6;			//TBLPTRL
    usb-> bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb-> bufferU[j]=0x0;
    usb->write();
    usb->msDelay(4);
    if(entry==2)  usb->msDelay(7);
    usb->read();
    //if( usb->saveLog)WriteLogIO();
    for(z=1;usb->bufferI[z]!=TBLR_INC_N&&z<usb->DIMBUF;z++);
    if(z<usb->DIMBUF-3)
    {
        CommonFunctions::printMessage2( GlobalVariables::strings[S_DevID2],usb->bufferI[z+3],usb->bufferI[z+2]);	//"DevID: 0x%02X%02X\r\n"
        findByID(usb->bufferI[z+2]+(usb->bufferI[z+3]<<8), true);
    }
    //****************** read code ********************
    CommonFunctions::printMessage( GlobalVariables::strings[S_CodeReading1]);		//code read ...
    CommonFunctions::PrintStatusSetup();
    for(i=0,j=1;i<dim;i+=usb->DIMBUF-4)
    {
        usb-> bufferU[j++]=TBLR_INC_N;
        usb-> bufferU[j++]=i<dim-(usb->DIMBUF-4)?usb->DIMBUF-4:dim-i;
        usb-> bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
        usb->write();
        usb->msDelay(2);
        usb->read();
        if(usb->bufferI[1]==TBLR_INC_N)
        {
            for(z=3;z<usb->bufferI[2]+3&&z<usb->DIMBUF;z++) usb->memCODE[k++]=usb->bufferI[z];
        }
        PrintStatus( GlobalVariables::strings[S_CodeReading2],i*100/(dim+dim2),i);	//"Read: %d%%, addr. %05X"
        emit onProgressChanged(i, dim);

        j=1;
        if( usb->saveLog)
        {
            //fprintf(logfile, GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
            //WriteLogIO();
        }
    }
    CommonFunctions::PrintStatusEnd();
    if(k!=dim)
    {
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage2( GlobalVariables::strings[S_ReadCodeErr2],dim,k);	//"Error reading code area, requested %d bytes, read %d\r\n"
    }
    else CommonFunctions::printMessage( GlobalVariables::strings[S_Compl]);
    //****************** read config area ********************
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x0E;			//TBLPTRU	ID 0x200000
    usb-> bufferU[j++]=0x20;			//TBLPTRU	ID 0x200000
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6E;			//TBLPTRU
    usb-> bufferU[j++]=0xF8;			//TBLPTRU
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRH
    usb-> bufferU[j++]=0xF7;			//TBLPTRH
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRL
    usb-> bufferU[j++]=0xF6;			//TBLPTRL
    usb-> bufferU[j++]=TBLR_INC_N;
    usb-> bufferU[j++]=8;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x0E;			//TBLPTRU	CONFIG 0x300000
    usb-> bufferU[j++]=0x30;			//TBLPTRU	CONFIG 0x300000
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6E;			//TBLPTRU
    usb-> bufferU[j++]=0xF8;			//TBLPTRU
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRH
    usb-> bufferU[j++]=0xF7;			//TBLPTRH
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRL
    usb-> bufferU[j++]=0xF6;			//TBLPTRL
    usb-> bufferU[j++]=TBLR_INC_N;
    usb-> bufferU[j++]=14;
    usb-> bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
       usb-> bufferU[j]=0x0;
    usb->write();
    usb->msDelay(8);
    usb->read();
    for(z=1;usb->bufferI[z]!=TBLR_INC_N&&z<usb->DIMBUF-28;z++);
    if(z<usb->DIMBUF-28)
    {
        for(i=0;i<8;i++)
           usb->memID[k2++]=usb->bufferI[z+i+2];
        for(;i<14+8;i++)
           usb->memCONFIG[-8+k2++]=usb->bufferI[z+i+8];
    }
    j=1;
    if( usb->saveLog)
    {
        //fprintf(logfile, GlobalVariables::strings[S_Log7],i,i,k2,k2);	//"i=%d(0x%X), k=%d(0x%X)\n"
        // WriteLogIO();
    }
    if(k2!=22)
    {
        CommonFunctions::printMessage2( GlobalVariables::strings[S_ReadConfigErr],22,k2);	//"Error reading config area, requested %d bytes, read %d\r\n"
    }
    //****************** read eeprom ********************
    if(dim2)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_ReadEE]);		//read eeprom ...
        CommonFunctions::PrintStatusSetup();
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x9E;				//EEPGD=0
        usb-> bufferU[j++]=0xA6;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x9C;				//CFGS=0
        usb-> bufferU[j++]=0xA6;
        for(k2=0,i=0;i<dim2;i++)
        {
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x0E;
            usb-> bufferU[j++]=i&0xFF;
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x6E;			//ADDR
            usb-> bufferU[j++]=0xA9;			//ADDR
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x0E;
            usb-> bufferU[j++]=(i>>8)&0xFF;
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x6E;			//ADDRH
            usb-> bufferU[j++]=0xAA;			//ADDRH
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x80;			//RD=1 :Read
            usb-> bufferU[j++]=0xA6;
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x50;			//MOVF EEDATA,W
            usb-> bufferU[j++]=0xA8;
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x6E;			//MOVWF TABLAT
            usb-> bufferU[j++]=0xF5;
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x00;			//NOP
            usb-> bufferU[j++]=0x00;
            usb-> bufferU[j++]=SHIFT_TABLAT;
            if(j>usb->DIMBUF-26||i==dim2-1)
            {
                usb-> bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
                usb->write();
                usb->msDelay(10);
                usb->read();
                for(z=1;z<usb->DIMBUF-1;z++)
                {
                    if(usb->bufferI[z]==SHIFT_TABLAT)
                    {
                        usb->memEE[k2++]=usb->bufferI[z+1];
                        z+=8;
                    }
                }
                PrintStatus( GlobalVariables::strings[S_CodeReading],(i+dim)*100/(dim+dim2),i);	//"Read: %d%%, addr. %03X"
                emit onProgressChanged(i, dim2);

                j=1;
                if( usb->saveLog)
                {
                    //fprintf(logfile, GlobalVariables::strings[S_Log7],i,i,k2,k2);	//"i=%d(0x%X), k=%d(0x%X)\n"
                    // WriteLogIO();
                }
            }
        }
        emit onProgressChanged(dim2, dim2);
        CommonFunctions::PrintStatusEnd();
        if(k2!=dim2)
        {
            CommonFunctions::printMessage("\r\n");
            CommonFunctions::printMessage2( GlobalVariables::strings[S_ReadEEErr],dim2,k2);	//"Error reading EEPROM area, requested %d bytes, read %d\r\n"
        }
        else CommonFunctions::printMessage( GlobalVariables::strings[S_Compl]);
    }
    CommonFunctions::printMessage("\r\n");
    //****************** exit ********************
    usb-> bufferU[j++]=EN_VPP_VCC;		//VDD
    usb-> bufferU[j++]=1;
    usb-> bufferU[j++]=EN_VPP_VCC;		//0
    usb-> bufferU[j++]=0x0;
    usb-> bufferU[j++]=SET_CK_D;
    usb-> bufferU[j++]=0x0;
    usb-> bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    CommonFunctions::PrintStatusClear();
    //****************** visualize ********************
    for(i=0;i<8;i+=2)
    {
        CommonFunctions::printMessage4( GlobalVariables::strings[S_ChipID2],i,usb->memID[i],i+1,usb->memID[i+1]);	//"ID%d: 0x%02X   ID%d: 0x%02X\r\n"
    }
    for(i=0;i<7;i++)
    {
        CommonFunctions::printMessage2( GlobalVariables::strings[S_ConfigWordH],i+1,usb->memCONFIG[i*2+1]);	//"CONFIG%dH: 0x%02X\t"
        CommonFunctions::printMessage2( GlobalVariables::strings[S_ConfigWordL],i+1,usb->memCONFIG[i*2]);	//"CONFIG%dL: 0x%02X\r\n"
    }
    CommonFunctions::printMessage( GlobalVariables::strings[S_CodeMem]);	//"\r\nCode memory:\r\n"
    // DisplayCODE18F(dim);
    if(dim2)
    {
        //  usb->DisplayEE();	//visualize
    }
    //CommonFunctions::PrintMessage1( GlobalVariables::strings[S_End],(stop-start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"
    // if( usb->saveLog) CloseLogFile();
}
void ProgP18::Write18Fx(int dim,int dim2,int wbuf,int eraseW1,int eraseW2,int options)
{
    // write 16 bit PIC 18Fxxxx
    // dim=program size 	dim2=eeprom size	wbuf=write buffer size {<=64}
    // eraseW1=erase word @3C0005	(not used if >= 0x10000)
    // eraseW2=erase word @3C0004	(not used if >= 0x10000)
    // options:
    //	bit [3:0]
    //     0 = vdd before vpp (12V)
    //     1 = vdd before vpp (9V)
    //     2 = low voltage entry with 32 bit key
    //	bit [7:4]
    //     0 = normal eeprom write algoritm
    //     1 = with unlock sequence 55 AA
    //	bit [11:8]
    //     0 = 15ms erase delay, 1ms code write time, 5ms EE write delay, 5ms config write time
    //     1 = 550ms erase delay, 1.2ms code write time, no config or EEPROM
    //     2 = 550ms erase delay, 3.4ms code write time, no config or EEPROM
    int k=0,k2,z=0,i,j;
    int err=0;
    int EEalgo=(options>>4)&0xF,entry=options&0xF,optWrite=(options>>8)&0xF;
    if(dim>0x1FFFFF||dim<0)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_CodeLim]);	//"Code size out of limits\r\n"
        return;
    }
    if(dim2>0x800||dim2<0)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(wbuf>64)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_WbufLim]);	//"Write buffer size out of limits\r\n"
        return;
    }
    if(entry>0&&!usb->CheckV33Regulator())
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_noV33reg]);	//Can't find 3.3V expansion board
        return;
    }
    double vpp=entry<2?(entry==0?12:8.5):-1;
    if(!usb->StartHVReg(vpp))
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_HVregErr]); //"HV regulator error\r\n"
        return;
    }
    if( usb->saveLog)
    {
        //OpenLogFile();	//"log.txt"
        //fprintf(logfile,"Write18F(%d,%d,%d,%d)    (0x%X,0x%X,0x%X,0x%X)\n",dim,dim2,wbuf,options,dim,dim2,wbuf,options);
    }
    if(dim>usb->size) dim=usb->size;
    if(dim%wbuf)
    {			//grow to an integer number of rows
        dim+=wbuf-dim%wbuf;
        j=usb->size;
        if(j<dim)
        {
            usb->size=dim;
            usb->memCODE=(unsigned char*)realloc(usb->memCODE,usb->size);
            for(;j<dim;j++) usb->memCODE[j]=0xFF;
        }
    }
    if(dim2>usb->sizeEE) dim2=usb->sizeEE;
    if(dim<1)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_NoCode]);	//"Empty data area\r\n"
        return;
    }

    usb->start=CommonFunctions::GetTickCount();
    usb-> bufferU[0]=0;
    j=1;
    usb-> bufferU[j++]=SET_PARAMETER;
    usb-> bufferU[j++]=SET_T1T2;
    usb-> bufferU[j++]=1;						//T1=1u
    usb-> bufferU[j++]=100;					//T2=100u
    usb-> bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb-> bufferU[j++]=0x0;
    usb-> bufferU[j++]=SET_CK_D;
    usb-> bufferU[j++]=0x0;
    usb-> bufferU[j++]=EN_VPP_VCC;		//VDD
    usb-> bufferU[j++]=0x1;
    usb-> bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb-> bufferU[j++]=0x5;
    if(entry==2)
    {					//LV entry with key
        usb-> bufferU[j++]=EN_VPP_VCC;		//VDD
        usb-> bufferU[j++]=0x1;
        usb-> bufferU[j++]=WAIT_T3;
        usb-> bufferU[j++]=WAIT_T3;
        usb-> bufferU[j++]=TX16;
        usb-> bufferU[j++]=2;
        usb-> bufferU[j++]=0x4D;
        usb-> bufferU[j++]=0x43;
        usb-> bufferU[j++]=0x48;
        usb-> bufferU[j++]=0x50;
        usb-> bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
        usb-> bufferU[j++]=0x5;
        usb-> bufferU[j++]=WAIT_T3;
    }
    usb-> bufferU[j++]=WAIT_T3;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x0E;			//3F
    usb-> bufferU[j++]=0x3F;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6E;			//-> TBLPTRU
    usb-> bufferU[j++]=0xF8;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x0E;			//FF
    usb-> bufferU[j++]=0xFF;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6E;			//-> TBLPTRH
    usb-> bufferU[j++]=0xF7;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x0E;			//FE
    usb-> bufferU[j++]=0xFE;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6E;			//-> TBLPTRL
    usb-> bufferU[j++]=0xF6;
    usb-> bufferU[j++]=TBLR_INC_N;		//DevID1-2	0x3FFFFE-F
    usb-> bufferU[j++]=2;
    usb-> bufferU[j++]=SET_PARAMETER;
    usb-> bufferU[j++]=SET_T3;
    usb-> bufferU[j++]=5100>>8;
    usb-> bufferU[j++]=5100&0xff;
    usb-> bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
    usb->write();
    usb->msDelay(3);
    if(entry==2)  usb->msDelay(7);
    usb->read();
    //if( usb->saveLog)WriteLogIO();
    for(z=1;usb->bufferI[z]!=TBLR_INC_N&&z<usb->DIMBUF;z++);
    if(z<usb->DIMBUF-3)
    {
        CommonFunctions::printMessage2( GlobalVariables::strings[S_DevID2],usb->bufferI[z+3],usb->bufferI[z+2]);	//"DevID: 0x%02X%02X\r\n"
        findByID(usb->bufferI[z+2]+(usb->bufferI[z+3]<<8), true);
    }
    j=1;
    //****************** erase memory ********************
    CommonFunctions::printMessage( GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x0E;			//3C
    usb-> bufferU[j++]=0x3C;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6E;			//-> TBLPTRU
    usb-> bufferU[j++]=0xF8;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRH=0
    usb-> bufferU[j++]=0xF7;
    if(eraseW1<0x10000)
    {
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x0E;			//05
        usb-> bufferU[j++]=0x05;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6E;			//-> TBLPTRL
        usb-> bufferU[j++]=0xF6;
        usb-> bufferU[j++]=TABLE_WRITE;		// eraseW1@3C0005
        usb-> bufferU[j++]=(eraseW1>>8)&0xFF; 	//0x3F;
        usb-> bufferU[j++]=eraseW1&0xFF; 		//0x3F;
    }
    if(eraseW2<0x10000)
    {
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x0E;			//04
        usb-> bufferU[j++]=0x04;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6E;			//-> TBLPTRL
        usb-> bufferU[j++]=0xF6;
        usb-> bufferU[j++]=TABLE_WRITE;		// eraseW2@3C0004
        usb-> bufferU[j++]=(eraseW2>>8)&0xFF; 	//0x8F;
        usb-> bufferU[j++]=eraseW2&0xFF; 		//0x8F;
    }
    usb-> bufferU[j++]=CORE_INS;		//NOP
    usb-> bufferU[j++]=0x00;
    usb-> bufferU[j++]=0x00;
    usb-> bufferU[j++]=CORE_INS;		//NOP
    usb-> bufferU[j++]=0x00;
    usb-> bufferU[j++]=0x00;
    usb-> bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
    usb->write();
    if(optWrite==0)  usb->msDelay(16);	//bulk erase delay
    else  usb->msDelay(550);
    usb->read();
    j=1;
    //if( usb->saveLog)WriteLogIO();
    //****************** prepare write ********************
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x8E;			//EEPGD=1
    usb-> bufferU[j++]=0xA6;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x9C;			//CFCGS=0
    usb-> bufferU[j++]=0xA6;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRU
    usb-> bufferU[j++]=0xF8;			//TBLPTRU
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRH
    usb-> bufferU[j++]=0xF7;			//TBLPTRH
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRL
    usb-> bufferU[j++]=0xF6;			//TBLPTRL
    usb-> bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    j=1;
    //if( usb->saveLog)WriteLogIO();
    CommonFunctions::printMessage( GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write code ********************
    CommonFunctions::printMessage( GlobalVariables::strings[S_StartCodeProg]);	//"code write ... "
    CommonFunctions::PrintStatusSetup();
    int ww;
    double wdly=1.0;
    if(optWrite==1) wdly=1.2;
    if(optWrite==2) wdly=3.4;
    if( usb->saveLog)
    {
        //fprintf(logfile,"WRITE CODE\ndim=%d(0x%X)\n",dim,dim);	//
    }
    int valid,i0;
    j=1;
    onProgressChanged(0, dim);
    for(i=k=0;i<dim;)
    {		//write xx instruction words
        if(k==0)
        {				//skip row if empty
            i0=i;
            for(valid=0;!valid&&i<dim;i+=valid?0:wbuf)
            {
                for(k2=0;k2<wbuf&&!valid;k2++) if(usb->memCODE[i+k2]<0xFF) valid=1;
            }
            if(i>=dim) break;
            if(i>i0)
            {				//some rows were skipped; update current address
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x0E;
                usb-> bufferU[j++]=(i>>16)&0xFF;
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x6E;			//MOVWF TBLPTRU
                usb-> bufferU[j++]=0xF8;
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x0E;
                usb-> bufferU[j++]=(i>>8)&0xFF;
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x6E;			//MOVWF TBLPTRH
                usb-> bufferU[j++]=0xF7;
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x0E;
                usb-> bufferU[j++]=i&0xFF;
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x6E;			//MOVWF TBLPTRL
                usb-> bufferU[j++]=0xF6;
                usb-> bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
                usb->write();
                usb->msDelay(2);
                usb->read();
                j=1;
                // if( usb->saveLog)//// WriteLogIO();
            }
        }
        if(usb->DIMBUF-4-j<wbuf-2-k)	ww=(usb->DIMBUF-4-j)/2;	//split data for a total of wbuf-2
        else ww=(wbuf-2-k)/2;
        usb-> bufferU[j++]=TBLW_INC_N;
        usb-> bufferU[j++]=ww;
        for(z=0;z<ww;z++)
        {
            usb-> bufferU[j++]=usb->memCODE[i+1];
            usb-> bufferU[j++]=usb->memCODE[i];
            i+=2;
        }
        k+=ww*2;
        if(k==wbuf-2)
        {	//Write row
            if(j>usb->DIMBUF-8)
            {
                usb-> bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
                usb->write();
                usb->msDelay(3);
                usb->read();
                j=1;
                // if( usb->saveLog)// WriteLogIO();
            }
            usb-> bufferU[j++]=TBLW_PROG_INC;
            usb-> bufferU[j++]=usb->memCODE[i+1];
            usb-> bufferU[j++]=usb->memCODE[i];
            if(optWrite==1)
            {	//1.2ms
                usb-> bufferU[j++]=1200>>8;
                usb-> bufferU[j++]=1200&0xFF;
            }
            else if(optWrite==2)
            {	//3.4ms
                usb-> bufferU[j++]=3400>>8;
                usb-> bufferU[j++]=3400&0xFF;
            }
            else{				//1ms
                usb-> bufferU[j++]=1000>>8;
                usb-> bufferU[j++]=1000&0xFF;
            }
            i+=2;
            k=0;
        }
        usb-> bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb-> bufferU[j]=0x0;
        usb->write();
        usb->msDelay(2);
        if(k==0)  usb->msDelay(wdly);
        usb->read();
        j=1;
        PrintStatus( GlobalVariables::strings[S_CodeWriting2],i*100/(dim+dim2),i/2);	//"Write: %d%%,addr. %04X"
        if( usb->saveLog)
        {
            //fprintf(logfile, GlobalVariables::strings[S_Log7],i,i,k,k);	//"i=%d, k=%d 0=%d\n"
            // WriteLogIO();
        }
        onProgressChanged(i, dim);
    }
    CommonFunctions::PrintStatusEnd();
    CommonFunctions::printMessage( GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    //****************** write ID ********************
    if(optWrite==0)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_IDW]);	//"Write ID ... "
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x8E;
        usb-> bufferU[j++]=0xA6;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x9C;
        usb-> bufferU[j++]=0xA6;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x0E;			//TBLPTRU	ID 0x200000
        usb-> bufferU[j++]=0x20;			//TBLPTRU	ID 0x200000
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6E;			//TBLPTRU
        usb-> bufferU[j++]=0xF8;			//TBLPTRU
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6A;			//TBLPTRH
        usb-> bufferU[j++]=0xF7;			//TBLPTRH
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6A;			//TBLPTRL
        usb-> bufferU[j++]=0xF6;			//TBLPTRL
        usb-> bufferU[j++]=TBLW_INC_N;
        usb-> bufferU[j++]=3;
        for(i=0;i<3;i++)
        {
            usb-> bufferU[j++]=usb->memID[i*2+1];
            usb-> bufferU[j++]=usb->memID[i*2];
        }
        usb-> bufferU[j++]=TBLW_PROG;
        usb-> bufferU[j++]=usb->memID[i*2+1];
        usb-> bufferU[j++]=usb->memID[i*2];
        usb-> bufferU[j++]=1000>>8;
        usb-> bufferU[j++]=1000&0xFF;
        usb-> bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
           usb-> bufferU[j]=0x0;
        usb->write();
        usb->msDelay(5);
        usb->read();
        j=1;
        if( usb->saveLog)
        {
            //fprintf(logfile, GlobalVariables::strings[S_Log7],i,i,0,0);	//"i=%d, k=%d 0=%d\n"
            // WriteLogIO();
        }
        CommonFunctions::printMessage( GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    }
    onProgressChanged(0, dim2);
    //****************** write and verify EEPROM ********************
    if(dim2&&optWrite==0)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_EEAreaW]);	//"Write EEPROM ... "
        CommonFunctions::PrintStatusSetup();
        int errEE=0;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x9E;			//EEPGD=0
        usb-> bufferU[j++]=0xA6;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x9C;			//CFGS=0
        usb-> bufferU[j++]=0xA6;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x84;			//WREN=1
        usb-> bufferU[j++]=0xA6;
        for(i=0;i<dim2&&err<=usb->max_err;i++)
        {
            if(usb->memEE[i]!=0xFF)
            {
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x0E;
                usb-> bufferU[j++]=i&0xFF;
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x6E;
                usb-> bufferU[j++]=0xA9;			//ADDR
                if(EEalgo==0)
                {
                    usb-> bufferU[j++]=CORE_INS;
                    usb-> bufferU[j++]=0x0E;
                    usb-> bufferU[j++]=(i>>8)&0xFF;
                    usb-> bufferU[j++]=CORE_INS;
                    usb-> bufferU[j++]=0x6E;
                    usb-> bufferU[j++]=0xAA;		//ADDRH
                }
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x0E;
                usb-> bufferU[j++]=usb->memEE[i];
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x6E;
                usb-> bufferU[j++]=0xA8;			//EEDATA
                if(EEalgo==1)
                {				//memory unlock
                    usb-> bufferU[j++]=CORE_INS;
                    usb-> bufferU[j++]=0x0E;
                    usb-> bufferU[j++]=0x55;
                    usb-> bufferU[j++]=CORE_INS;
                    usb-> bufferU[j++]=0x6E;
                    usb-> bufferU[j++]=0xA7;			//EECON2
                    usb-> bufferU[j++]=CORE_INS;
                    usb-> bufferU[j++]=0x0E;
                    usb-> bufferU[j++]=0xAA;
                    usb-> bufferU[j++]=CORE_INS;
                    usb-> bufferU[j++]=0x6E;
                    usb-> bufferU[j++]=0xA7;			//EECON2
                }
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x82;
                usb-> bufferU[j++]=0xA6;			//WR=1
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x00;
                usb-> bufferU[j++]=0x00;			//NOP
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x00;
                usb-> bufferU[j++]=0x00;			//NOP
                usb-> bufferU[j++]=WAIT_T3;		//write delay
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x80;			//RD=1
                usb-> bufferU[j++]=0xA6;
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x50;			//MOVF EEDATA,w
                usb-> bufferU[j++]=0xA8;
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x6E;			//MOVWF TABLAT
                usb-> bufferU[j++]=0xF5;
                usb-> bufferU[j++]=CORE_INS;
                usb-> bufferU[j++]=0x00;			//NOP
                usb-> bufferU[j++]=0x00;
                usb-> bufferU[j++]=SHIFT_TABLAT;
                usb-> bufferU[j++]=FLUSH;
                for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
                usb->write();
                usb->msDelay(8);
                usb->read();
                PrintStatus( GlobalVariables::strings[S_CodeWriting],(i+dim)*100/(dim+dim2),i);	//"Scrittura: %d%%, ind. %03X"
                j=1;
                for(z=usb->DIMBUF-1;z&&usb->bufferI[z]!=SHIFT_TABLAT;z--);
                if(z&&usb->memEE[i]!=usb->bufferI[z+1]) errEE++;
                if( usb->saveLog)
                {
                    //fprintf(logfile, GlobalVariables::strings[S_Log8],i,i,k,k,errEE);	//"i=%d, k=%d, errors=%d\n"
                    // WriteLogIO();
                }
            }
            onProgressChanged(i, dim2);
        }
        CommonFunctions::PrintStatusEnd();
        CommonFunctions::printMessage1( GlobalVariables::strings[S_ComplErr],errEE);	//"completed: %d errors \r\n"
        err+=errEE;
        onProgressChanged(dim2, dim2);
    }
    //****************** verify code ********************
    CommonFunctions::printMessage( GlobalVariables::strings[S_CodeV]);	//"Verify code ... "
    CommonFunctions::PrintStatusSetup();
    if( usb->saveLog)//fprintf(logfile,"VERIFY CODE\n");
        usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x8E;			//EEPGD=1
    usb-> bufferU[j++]=0xA6;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x9C;			//CFCGS=0
    usb-> bufferU[j++]=0xA6;
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRU
    usb-> bufferU[j++]=0xF8;			//TBLPTRU
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRH
    usb-> bufferU[j++]=0xF7;			//TBLPTRH
    usb-> bufferU[j++]=CORE_INS;
    usb-> bufferU[j++]=0x6A;			//TBLPTRL
    usb-> bufferU[j++]=0xF6;			//TBLPTRL
    usb-> bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    if( usb->saveLog)
    {
        // WriteLogIO();
        //fprintf(logfile,"\n\n");
    }

    onProgressChanged(0, dim);
    for(i=0,j=1,k=0;i<dim;i+=usb->DIMBUF-4)
    {
        i0=i;
        for(valid=0;!valid&&i<dim;i+=valid?0:usb->DIMBUF-4)
        {		//skip verification if 0xFF
            for(k2=0;k2<usb->DIMBUF-4&&!valid&&i+k2<dim;k2++) if(usb->memCODE[i+k2]<0xFF) valid=1;
        }
        if(i>=dim)
            break;
        if(i>i0)
        {				//some data was skipped; update current address
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x0E;
            usb-> bufferU[j++]=(i>>16)&0xFF;
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x6E;			//MOVWF TBLPTRU
            usb-> bufferU[j++]=0xF8;
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x0E;
            usb-> bufferU[j++]=(i>>8)&0xFF;
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x6E;			//MOVWF TBLPTRH
            usb-> bufferU[j++]=0xF7;
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x0E;
            usb-> bufferU[j++]=i&0xFF;
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x6E;			//MOVWF TBLPTRL
            usb-> bufferU[j++]=0xF6;
            usb-> bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
            usb->write();
            usb->msDelay(2);
            usb->read();
            j=1;
            // if( usb->saveLog)// WriteLogIO();
        }
        usb-> bufferU[j++]=TBLR_INC_N;
        usb-> bufferU[j++]=i<dim-(usb->DIMBUF-4)?usb->DIMBUF-4:dim-i;
        usb-> bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
        usb->write();
        usb->msDelay(2);
        usb->read();
        if(usb->bufferI[1]==TBLR_INC_N)
        {
            for(z=0;z<usb->bufferI[2]&&z<usb->DIMBUF;z++)
            {
                if(usb->memCODE[i+z]!=usb->bufferI[z+3])
                {
                    CommonFunctions::printMessage4( GlobalVariables::strings[S_CodeVError],i+z,i+z,usb->memCODE[i+z],usb->bufferI[z+3]);	//"Error writing address %4X: written %02X, read %02X\r\n"
                    err++;
                }
                //k++;
            }
        }
        PrintStatus( GlobalVariables::strings[S_CodeV2],i*100/(dim+dim2),i);	//"Verifica: %d%%, ind. %04X"
        j=1;
        if( usb->saveLog)
        {
            //fprintf(logfile, GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d\n"
            // WriteLogIO();
        }
        if(err>=usb->max_err)
            break;
        onProgressChanged(i, dim);
    }
    onProgressChanged(dim, dim);

    CommonFunctions::PrintStatusEnd();
    if(i<dim)
    {
        CommonFunctions::printMessage2( GlobalVariables::strings[S_CodeVError2],dim,i);	//"Error verifying code area, requested %d bytes, read %d\r\n"
    }
    CommonFunctions::printMessage1( GlobalVariables::strings[S_ComplErr],err);	//"completed: %d errors\r\n"
    if(err>=usb->max_err)
    {
        CommonFunctions::printMessage1( GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
    }
    //****************** verify ID ********************
    if(usb->programID&&err<usb->max_err&&optWrite==0)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_IDV]);	//"Verify ID ... "
        int errID=0;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x0E;			//TBLPTRU	ID 0x200000
        usb-> bufferU[j++]=0x20;			//TBLPTRU	ID 0x200000
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6E;			//TBLPTRU
        usb-> bufferU[j++]=0xF8;			//TBLPTRU
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6A;			//TBLPTRH
        usb-> bufferU[j++]=0xF7;			//TBLPTRH
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6A;			//TBLPTRL
        usb-> bufferU[j++]=0xF6;			//TBLPTRL
        usb-> bufferU[j++]=TBLR_INC_N;
        usb-> bufferU[j++]=8;
        usb-> bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
        usb->write();
        usb->msDelay(2);
        usb->read();
        for(z=0;usb->bufferI[z]!=TBLR_INC_N&&z<usb->DIMBUF;z++);
        for(i=0;i<8;i++) if(usb->memID[i]!=0xFF&&usb->memID[i]!=usb->bufferI[z+i+2]) errID++;
        CommonFunctions::printMessage1( GlobalVariables::strings[S_ComplErr],errID);	//"completed: %d errors\r\n"
        err+=errID;
        if(err>=usb->max_err)
        {
            CommonFunctions::printMessage1( GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
        }
        j=1;
        if( usb->saveLog)
        {
            //fprintf(logfile, GlobalVariables::strings[S_Log8],i,i,0,0,err);	//"i=%d, k=%d, errors=%d\n"
            // WriteLogIO();
        }
    }
    //****************** write CONFIG ********************
    if(err<usb->max_err&&optWrite==0)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_ConfigW]);	//"Write CONFIG ..."
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x8E;
        usb-> bufferU[j++]=0xA6;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x8C;
        usb-> bufferU[j++]=0xA6;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x0E;			//CONFIG 0x300000
        usb-> bufferU[j++]=0x30;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6E;			//TBLPTRU
        usb-> bufferU[j++]=0xF8;			//TBLPTRU
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6A;			//TBLPTRH
        usb-> bufferU[j++]=0xF7;			//TBLPTRH
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6A;			//TBLPTRL
        usb-> bufferU[j++]=0xF6;			//TBLPTRL
        for(i=0;i<14;i++)
        {
            if(usb->memCONFIG[i]<0xFF)
            {
                usb-> bufferU[j++]=TBLW_PROG;
                usb-> bufferU[j++]=0;
                usb-> bufferU[j++]=usb->memCONFIG[i];
                usb-> bufferU[j++]=5000>>8;
                usb-> bufferU[j++]=5000&0xFF;
            }
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x2A;			//INCF
            usb-> bufferU[j++]=0xF6;			//TBLPTRL
            i++;
            if(usb->memCONFIG[i]<0xFF)
            {
                usb-> bufferU[j++]=TBLW_PROG;
                usb-> bufferU[j++]=usb->memCONFIG[i];
                usb-> bufferU[j++]=0;
                usb-> bufferU[j++]=5000>>8;
                usb-> bufferU[j++]=5000&0xFF;
            }
            usb-> bufferU[j++]=CORE_INS;
            usb-> bufferU[j++]=0x2A;			//INCF
            usb-> bufferU[j++]=0xF6;			//TBLPTRL
            usb-> bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
            usb->write();
            usb->msDelay(12);
            usb->read();
            j=1;
            if( usb->saveLog)
            {
                //fprintf(logfile, GlobalVariables::strings[S_Log7],i,i,0,0);	//"i=%d, k=%d\n"
                // WriteLogIO();
            }
        }
        CommonFunctions::printMessage( GlobalVariables::strings[S_Compl]);	//"completed\r\n"
        //****************** verify CONFIG ********************
        CommonFunctions::printMessage( GlobalVariables::strings[S_ConfigV]);	//"Verify CONFIG ... "
        int errC=0;
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x0E;			//TBLPTRU	CONFIG 0x300000
        usb-> bufferU[j++]=0x30;			//TBLPTRU	CONFIG 0x300000
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6E;			//TBLPTRU
        usb-> bufferU[j++]=0xF8;			//TBLPTRU
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6A;			//TBLPTRH
        usb-> bufferU[j++]=0xF7;			//TBLPTRH
        usb-> bufferU[j++]=CORE_INS;
        usb-> bufferU[j++]=0x6A;			//TBLPTRL
        usb-> bufferU[j++]=0xF6;			//TBLPTRL
        usb-> bufferU[j++]=TBLR_INC_N;
        usb-> bufferU[j++]=14;
        usb-> bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
        usb->write();
        usb->msDelay(2);
        usb->read();
        for(z=1;usb->bufferI[z]!=TBLR_INC_N&&z<usb->DIMBUF-16;z++);
        if(z<usb->DIMBUF-16)
        {
            for(i=0;i<14;i++) if(~usb->memCONFIG[i]&usb->bufferI[z+i+2]) errC++;	//error if written 0 and read 1 (~W&R)
        }
        CommonFunctions::printMessage1( GlobalVariables::strings[S_ComplErr],errC);	//"completed: %d errors\r\n"
        err+=errC;
        if(err>=usb->max_err)
        {
            CommonFunctions::printMessage1( GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
        }
        j=1;
        if( usb->saveLog)
        {
            //fprintf(logfile, GlobalVariables::strings[S_Log8],i,i,0,0,err);	//"i=%d, k=%d, errors=%d\n"
            // WriteLogIO();
        }
    }
    usb-> bufferU[j++]=SET_PARAMETER;
    usb-> bufferU[j++]=SET_T3;
    usb-> bufferU[j++]=2000>>8;
    usb-> bufferU[j++]=2000&0xff;
    usb-> bufferU[j++]=EN_VPP_VCC;		//VDD
    usb-> bufferU[j++]=1;
    usb-> bufferU[j++]=EN_VPP_VCC;		//0
    usb-> bufferU[j++]=0x0;
    usb-> bufferU[j++]=SET_CK_D;
    usb-> bufferU[j++]=0x0;
    usb-> bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)usb-> bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    usb->stop=CommonFunctions::GetTickCount();
    CommonFunctions::PrintStatusClear();
    //CommonFunctions::printMessage3( GlobalVariables::strings[S_EndErr],(stop-start)/1000.0,err,err!=1? GlobalVariables::strings[S_ErrPlur]: GlobalVariables::strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    // if( usb->saveLog) CloseLogFile();

}



void ProgP18::DisplayCODE18F(int dim)
{
    // display  PIC18F CODE memory
    char s[256]="",t[256]="";
    char* aux=(char*)malloc((dim/COL+1)*(16+COL*6));
    aux[0]=0;
    int valid=0,empty=1,i,j,lines=0;
    for(i=0;i<dim&&i<usb->size;i+=COL*2)
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
            lines++;
            if(lines>500)
            {	//limit number of lines printed
                strcat(aux,"(...)\r\n");
                i=usb->size;
            }
        }
        s[0]=0;
    }
    if(empty) CommonFunctions::printMessage(GlobalVariables::GlobalVariables::strings[S_Empty]);	//empty
    else CommonFunctions::printMessage(aux);
    free(aux);
}

DeviceSelected ProgP18::detectDevice()
{
    DeviceSelected deviceSelected;
    deviceSelected.deviceInfo=NULL;
    int optionsList[3]={0, 1, 2};
    for(int optionsMode=0; optionsMode<3;optionsMode++)
    {
        int z=0,j;
        int options= optionsList[optionsMode];
        int entry=options&0xF;
        if(entry>0&&!usb->CheckV33Regulator())
        {
            //CommonFunctions::printMessage( GlobalVariables::strings[S_noV33reg]);	//Can't find 3.3V expansion board
            continue;
        }
        double vpp=entry<2?(entry==0?12:8.5):-1;
        if(!usb->StartHVReg(vpp))
        {
            //CommonFunctions::printMessage( GlobalVariables::strings[S_HVregErr]); //"HV regulator error\r\n"
            continue;
        }
        for(j=0;j<8;j++)
            usb->memID[j]=0xFF;
        for(j=0;j<14;j++)
            usb->memCONFIG[j]=0xFF;
        usb->start=CommonFunctions::GetTickCount();
        usb-> bufferU[0]=0;
        j=1;
        usb-> bufferU[j++]=SET_PARAMETER;
        usb-> bufferU[j++]=SET_T1T2;
        usb-> bufferU[j++]=1;						//T1=1u
        usb-> bufferU[j++]=100;					//T2=100u
        usb-> bufferU[j++]=SET_PARAMETER;
        usb-> bufferU[j++]=SET_T3;
        usb-> bufferU[j++]=2000>>8;
        usb-> bufferU[j++]=2000&0xff;
        usb-> bufferU[j++]=EN_VPP_VCC;		//enter program mode
        usb-> bufferU[j++]=0x0;
        usb-> bufferU[j++]=SET_CK_D;
        usb-> bufferU[j++]=0x0;
        usb-> bufferU[j++]=EN_VPP_VCC;		//VDD
        usb-> bufferU[j++]=0x1;
        usb-> bufferU[j++]=NOP;
        usb-> bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
        usb-> bufferU[j++]=0x5;
        if(entry==2)
        {					//LV entry with key
            usb-> bufferU[j++]=EN_VPP_VCC;		//VDD
            usb-> bufferU[j++]=0x1;
            usb-> bufferU[j++]=WAIT_T3;
            usb-> bufferU[j++]=WAIT_T3;
            usb-> bufferU[j++]=TX16;
            usb-> bufferU[j++]=2;
            usb-> bufferU[j++]=0x4D;
            usb-> bufferU[j++]=0x43;
            usb-> bufferU[j++]=0x48;
            usb-> bufferU[j++]=0x50;
            usb-> bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
            usb-> bufferU[j++]=0x5;
            usb-> bufferU[j++]=WAIT_T3;
        }
        usb->bufferU[j++]=WAIT_T3;
        usb->bufferU[j++]=CORE_INS;
        usb->bufferU[j++]=0x0E;			//3F
        usb->bufferU[j++]=0x3F;
        usb->bufferU[j++]=CORE_INS;
        usb->bufferU[j++]=0x6E;			//-> TBLPTRU
        usb->bufferU[j++]=0xF8;
        usb->bufferU[j++]=CORE_INS;
        usb->bufferU[j++]=0x0E;			//FF
        usb->bufferU[j++]=0xFF;
        usb->bufferU[j++]=CORE_INS;
        usb->bufferU[j++]=0x6E;			//-> TBLPTRH
        usb->bufferU[j++]=0xF7;
        usb->bufferU[j++]=CORE_INS;
        usb->bufferU[j++]=0x0E;			//FE
        usb->bufferU[j++]=0xFE;
        usb->bufferU[j++]=CORE_INS;
        usb->bufferU[j++]=0x6E;			//-> TBLPTRL
        usb->bufferU[j++]=0xF6;
        usb->bufferU[j++]=TBLR_INC_N;		//DevID1-2	0x3FFFFE-F
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=CORE_INS;
        usb->bufferU[j++]=0x6A;			//TBLPTRU
        usb->bufferU[j++]=0xF8;			//TBLPTRU
        usb->bufferU[j++]=CORE_INS;
        usb->bufferU[j++]=0x6A;			//TBLPTRH
        usb->bufferU[j++]=0xF7;			//TBLPTRH
        usb->bufferU[j++]=CORE_INS;
        usb->bufferU[j++]=0x6A;			//TBLPTRL
        usb->bufferU[j++]=0xF6;			//TBLPTRL
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb-> bufferU[j]=0x0;
        usb->write();
        usb->msDelay(4);
        if(entry==2)
            usb->msDelay(7);
        usb->read();
        //if( usb->saveLog)WriteLogIO();
        for(z=1;usb->bufferI[z]!=TBLR_INC_N&&z<usb->DIMBUF;z++);
        if(z<usb->DIMBUF-3)
        {
            CommonFunctions::printMessage2( GlobalVariables::strings[S_DevID2],usb->bufferI[z+3],usb->bufferI[z+2]);	//"DevID: 0x%02X%02X\r\n"
            deviceSelected = findByID(usb->bufferI[z+2]+(usb->bufferI[z+3]<<8));
        }
        //****************** exit ********************
        j=1;
        usb->bufferU[j++]=EN_VPP_VCC;		//VDD
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=EN_VPP_VCC;		//0
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
    }
    return deviceSelected;
}

void ProgP18::initPIC18_ID()
{
    devices->setDeviceID(0x0400,"18F252",0x1F);
    devices->setDeviceID(0x0400,"18F2539",0x1F);
    devices->setDeviceID(0x0420,"18F452",0x1F);
    devices->setDeviceID(0x0420,"18F4539",0x1F);
    devices->setDeviceID(0x0480,"18F242",0x1F);
    devices->setDeviceID(0x0480,"18F2439",0x1F);
    devices->setDeviceID(0x04A0,"18F442",0x1F);
    devices->setDeviceID(0x04A0,"18F4439",0x1F);
    devices->setDeviceID(0x0500,"18F2320",0x1F);
    devices->setDeviceID(0x0520,"18F4320",0x1F);
    devices->setDeviceID(0x0580,"18F2220",0x1F);
    devices->setDeviceID(0x05A0,"18F4220",0x1F);
    devices->setDeviceID(0x0600,"18F8720",0x1F);
    devices->setDeviceID(0x0620,"18F6720",0x1F);
    devices->setDeviceID(0x0640,"18F8620",0x1F);
    devices->setDeviceID(0x0660,"18F6620",0x1F);
    devices->setDeviceID(0x07C0,"18F1320",0x1F);
    devices->setDeviceID(0x07E0,"18F1220",0x1F);
    devices->setDeviceID(0x0800,"18F248",0x1F);
    devices->setDeviceID(0x0820,"18F448",0x1F);
    devices->setDeviceID(0x0840,"18F258",0x1F);
    devices->setDeviceID(0x0860,"18F458",0x1F);
    devices->setDeviceID(0x0880,"18F4431",0x1F);
    devices->setDeviceID(0x08A0,"18F4331",0x1F);
    devices->setDeviceID(0x08C0,"18F2431",0x1F);
    devices->setDeviceID(0x08E0,"18F2331",0x1F);
    devices->setDeviceID(0x0B20,"18F6520",0x1F);
    devices->setDeviceID(0x0B00,"18F8520",0x1F);
    devices->setDeviceID(0x0C00,"18F4620",0x1F);
    devices->setDeviceID(0x0C20,"18F4610",0x1F);
    devices->setDeviceID(0x0C40,"18F4525",0x1F);
    devices->setDeviceID(0x0C60,"18F4515",0x1F);
    devices->setDeviceID(0x0C80,"18F2620",0x1F);
    devices->setDeviceID(0x0CA0,"18F2610",0x1F);
    devices->setDeviceID(0x0CC0,"18F2525",0x1F);
    devices->setDeviceID(0x0CE0,"18F2515",0x1F);
    devices->setDeviceID(0x0E80,"18F4680",0x1F);
    devices->setDeviceID(0x0EA0,"18F4585",0x1F);
    devices->setDeviceID(0x0EC0,"18F2680",0x1F);
    devices->setDeviceID(0x0EE0,"18F2585",0x1F);
    devices->setDeviceID(0x1081,"18F4523",0x0F);
    devices->setDeviceID(0x1080,"18F4520",0x0F);
    devices->setDeviceID(0x10A0,"18F4510",0x1F);
    devices->setDeviceID(0x10C1,"18F4423",0x0F);
    devices->setDeviceID(0x10C0,"18F4420",0x0F);
    devices->setDeviceID(0x10E0,"18F4410",0x1F);
    devices->setDeviceID(0x1100,"18F2523",0x0F);
    devices->setDeviceID(0x1100,"18F2520",0x0F);
    devices->setDeviceID(0x1120,"18F2510",0x1F);
    devices->setDeviceID(0x1140,"18F2423",0x0F);
    devices->setDeviceID(0x1140,"18F2420",0x0F);
    devices->setDeviceID(0x1160,"18F2410",0x1F);
    devices->setDeviceID(0x1200,"18F4550",0x1F);
    devices->setDeviceID(0x1220,"18F4455",0x1F);
    devices->setDeviceID(0x1240,"18F2550",0x1F);
    devices->setDeviceID(0x1260,"18F2455",0x1F);
    devices->setDeviceID(0x1340,"18F6527",0x1F);
    devices->setDeviceID(0x1360,"18F8527",0x1F);
    devices->setDeviceID(0x1380,"18F6622",0x1F);
    devices->setDeviceID(0x13A0,"18F8622",0x1F);
    devices->setDeviceID(0x13C0,"18F6627",0x1F);
    devices->setDeviceID(0x13E0,"18F8627",0x1F);
    devices->setDeviceID(0x1400,"18F6722",0x1F);
    devices->setDeviceID(0x1420,"18F8722",0x1F);
    devices->setDeviceID(0x1800,"18F66J60",0x1F);
    devices->setDeviceID(0x1820,"18F86J60",0x1F);
    devices->setDeviceID(0x1840,"18F96J60",0x1F);
    devices->setDeviceID(0x1A80,"18F4580",0x1F);
    devices->setDeviceID(0x1AA0,"18F4480",0x1F);
    devices->setDeviceID(0x1AC0,"18F2580",0x1F);
    devices->setDeviceID(0x1AE0,"18F2480",0x1F);
    devices->setDeviceID(0x1C00,"18F25J10",0x1F);
    devices->setDeviceID(0x1C20,"18F45J10",0x1F);
    devices->setDeviceID(0x1C40,"18LF25J10",0x1F);
    devices->setDeviceID(0x1C60,"18LF45J10",0x1F);
    devices->setDeviceID(0x1D00,"18F24J10",0x1F);
    devices->setDeviceID(0x1D20,"18F44J10",0x1F);
    devices->setDeviceID(0x1D40,"18LF24J10",0x1F);
    devices->setDeviceID(0x1D60,"18LF44J10",0x1F);
    devices->setDeviceID(0x1E00,"18F1230",0x1F);
    devices->setDeviceID(0x1E20,"18F1330",0x1F);
    devices->setDeviceID(0x1F00,"18F66J65",0x1F);
    devices->setDeviceID(0x1F20,"18F67J60",0x1F);
    devices->setDeviceID(0x1F40,"18F86J65",0x1F);
    devices->setDeviceID(0x1F60,"18F87J60",0x1F);
    devices->setDeviceID(0x1F80,"18F96J65",0x1F);
    devices->setDeviceID(0x1FA0,"18F97J60",0x1F);
    devices->setDeviceID(0x1FE0,"18F1330-ICD",0x1F);
    devices->setDeviceID(0x2000,"18F46K20",0x1F);
    devices->setDeviceID(0x2020,"18F26K20",0x1F);
    devices->setDeviceID(0x2040,"18F45K20",0x1F);
    devices->setDeviceID(0x2060,"18F25K20",0x1F);
    devices->setDeviceID(0x2080,"18F44K20",0x1F);
    devices->setDeviceID(0x20A0,"18F24K20",0x1F);
    devices->setDeviceID(0x20C0,"18F43K20",0x1F);
    devices->setDeviceID(0x20E0,"18F23K20",0x1F);
    devices->setDeviceID(0x2100,"18F4321",0x1F);
    devices->setDeviceID(0x2120,"18F2321",0x1F);
    devices->setDeviceID(0x2140,"18F4221",0x1F);
    devices->setDeviceID(0x2160,"18F2221",0x1F);
    devices->setDeviceID(0x2400,"18F4450",0x1F);
    devices->setDeviceID(0x2420,"18F2450",0x1F);
    devices->setDeviceID(0x2700,"18F2682",0x1F);
    devices->setDeviceID(0x2720,"18F2685",0x1F);
    devices->setDeviceID(0x2740,"18F4682",0x1F);
    devices->setDeviceID(0x2760,"18F4685",0x1F);
    devices->setDeviceID(0x2A00,"18F4553",0x1F);
    devices->setDeviceID(0x2A20,"18F4458",0x1F);
    devices->setDeviceID(0x2A40,"18F2553",0x1F);
    devices->setDeviceID(0x2A60,"18F2458",0x1F);
    devices->setDeviceID(0x4700,"18LF13K50",0x1F);
    devices->setDeviceID(0x4720,"18LF14K50",0x1F);
    devices->setDeviceID(0x4740,"18F13K50",0x1F);
    devices->setDeviceID(0x4760,"18F14K50",0x1F);
    devices->setDeviceID(0x49C0,"18F6628",0x1F);
    devices->setDeviceID(0x49E0,"18F8628",0x1F);
    devices->setDeviceID(0x4A00,"18F6723",0x1F);
    devices->setDeviceID(0x4A20,"18F8723",0x1F);
    devices->setDeviceID(0x4C00,"18F24J50",0x1F);
    devices->setDeviceID(0x4C20,"18F25J50",0x1F);
    devices->setDeviceID(0x4C40,"18F26J50",0x1F);
    devices->setDeviceID(0x4C60,"18F44J50",0x1F);
    devices->setDeviceID(0x4C80,"18F45J50",0x1F);
    devices->setDeviceID(0x4D80,"18F24J11",0x1F);
    devices->setDeviceID(0x4DA0,"18F25J11",0x1F);
    devices->setDeviceID(0x4DC0,"18F26J11",0x1F);
    devices->setDeviceID(0x4DE0,"18F44J11",0x1F);
    devices->setDeviceID(0x4E00,"18F45J11",0x1F);
    devices->setDeviceID(0x4E20,"18F46J11",0x1F);
    devices->setDeviceID(0x4E60,"18LF25J11",0x1F);
    devices->setDeviceID(0x4E80,"18LF26J11",0x1F);
    devices->setDeviceID(0x4EA0,"18LF44J11",0x1F);
    devices->setDeviceID(0x4EC0,"18LF45J11",0x1F);
    devices->setDeviceID(0x4EE0,"18LF46J11",0x1F);
    devices->setDeviceID(0x4F20,"18F14K22",0x1F);
    devices->setDeviceID(0x4F40,"18F13K22",0x1F);
    devices->setDeviceID(0x4F60,"18LF14K22",0x1F);
    devices->setDeviceID(0x4F80,"18LF13K22",0x1F);
    devices->setDeviceID(0x5400,"18F46K22",0x1F);
    devices->setDeviceID(0x5420,"18LF46K22",0x1F);
    devices->setDeviceID(0x5440,"18F26K22",0x1F);
    devices->setDeviceID(0x5460,"18LF26K22",0x1F);
    devices->setDeviceID(0x5500,"18F45K22",0x1F);
    devices->setDeviceID(0x5520,"18LF45K22",0x1F);
    devices->setDeviceID(0x5540,"18F25K22",0x1F);
    devices->setDeviceID(0x5560,"18LF25K22",0x1F);
    devices->setDeviceID(0x5600,"18F44K22",0x1F);
    devices->setDeviceID(0x5620,"18LF44K22",0x1F);
    devices->setDeviceID(0x5640,"18F24K22",0x1F);
    devices->setDeviceID(0x5660,"18LF24K22",0x1F);
    devices->setDeviceID(0x5700,"18F43K22",0x1F);
    devices->setDeviceID(0x5720,"18LF43K22",0x1F);
    devices->setDeviceID(0x5740,"18F23K22",0x1F);
    devices->setDeviceID(0x5760,"18LF23K22",0x1F);
    devices->setDeviceID(0x5820,"18F26J53",0x1F);
    devices->setDeviceID(0x5860,"18F27J53",0x1F);
    devices->setDeviceID(0x58A0,"18F46J53",0x1F);
    devices->setDeviceID(0x58E0,"18F47J53",0x1F);
    devices->setDeviceID(0x5920,"18F26J13",0x1F);
    devices->setDeviceID(0x5960,"18F27J13",0x1F);
    devices->setDeviceID(0x59A0,"18F46J13",0x1F);
    devices->setDeviceID(0x59E0,"18F47J13",0x1F);
    devices->setDeviceID(0x5A20,"18LF26J53",0x1F);
    devices->setDeviceID(0x5A60,"18LF27J53",0x1F);
    devices->setDeviceID(0x5AA0,"18LF46J53",0x1F);
    devices->setDeviceID(0x5AE0,"18LF47J53",0x1F);
    devices->setDeviceID(0x5B20,"18LF26J13",0x1F);
    devices->setDeviceID(0x5B60,"18LF27J13",0x1F);
    devices->setDeviceID(0x5BA0,"18LF46J13",0x1F);
    devices->setDeviceID(0x5BE0,"18LF47J13",0x1F);

}

DeviceSelected ProgP18::findByID(quint16 id, bool printMsg)
{
    int max= devices->allDevices.count();
    int step=1;
    DeviceSelected deviceSelected;
    deviceSelected.deviceInfo = NULL;
    if(!id)
        return deviceSelected;
    for (int i = 0; i < devices->deviceFamilyList.count(); ++i)
    {
        DeviceFamily *deviceFamily=devices->deviceFamilyList.at(i);
        if(deviceFamily->familyName=="PIC")
            for (int j = 0; j < deviceFamily->deviceTypesList.count(); ++j)
            {
                DeviceType *deviceType=deviceFamily->deviceTypesList.at(j);
                if(deviceType->typeName=="PIC 18F")
                    for (int k = 0; k < deviceType->devicesList.count(); ++k)
                    {
                        emit onProgressChanged(step++, max);
                        DeviceInfo *deviceInfo=deviceType->devicesList.at(k);
                        if(deviceInfo->id == (id&(~deviceInfo->revMask)))
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
        CommonFunctions::printMessage("Unknown device id:0x"+QString::number(id, 16).toUpper()+"\r\n");
    return deviceSelected;
}






