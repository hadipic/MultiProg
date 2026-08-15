#include "progP12.h"
#include "Libs/globalVariables.h"
ProgP12::ProgP12(USBTools *usb, QObject *parent):ProgDevice(usb, parent)
{
    use_osccal=1;
    use_BKosccal=0;
    saveLog=0,programID=1,MinDly=1,load_osccal=0,load_BKosccal=0;
    load_calibword=0,max_err=200;
}

void ProgP12::detectDevice()
{
    int k=0,z=0,i,j;
    char s[256],t[256];
    usb->sizeW=0x1000;
    if( usb->memCODE_W)
        free( usb->memCODE_W);
    usb->memCODE_W=(WORD*)malloc(sizeof(WORD)*usb->sizeW);
    unsigned int start=CommonFunctions::GetTickCount();
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
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=READ_DATA_PROG;	//configuration word
    usb->bufferU[j++]=INC_ADDR;			// 7FF->000
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    //   if(saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&& usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(z<usb->DIMBUF-2)
    {
        usb->memCODE_W[0xfff]=( usb->bufferI[z+1]<<8)+ usb->bufferI[z+2];
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ConfigWord], usb->memCODE_W[0xfff]);	//"\r\nConfiguration word: 0x%03X\r\n"
        switch( usb->memCODE_W[0xfff]&0x03)
        {
        case 0:
            CommonFunctions::printMessage(GlobalVariables::strings[S_LPOsc]);	//"LP oscillator\r\n"
            break;
        case 1:
            CommonFunctions::printMessage(GlobalVariables::strings[S_XTOsc]);	//"XT oscillator\r\n"
            break;
        case 2:
            CommonFunctions::printMessage(GlobalVariables::strings[S_IntOsc]);	//"Internal osc.\r\n"
            break;
        case 3:
            CommonFunctions::printMessage(GlobalVariables::strings[S_RCOsc]);	//"RC oscillator\r\n"
            break;
        }
        if( usb->memCODE_W[0xfff]&0x04)
            CommonFunctions::printMessage(GlobalVariables::strings[S_WDTON]);	//"WDT ON\r\n"
        else
            CommonFunctions::printMessage(GlobalVariables::strings[S_WDTOFF]);	//"WDT OFF\r\n"
        if( usb->memCODE_W[0xfff]&0x08)
            CommonFunctions::printMessage(GlobalVariables::strings[S_CPOFF]);	//"Code protection OFF\r\n"
        else
            CommonFunctions::printMessage(GlobalVariables::strings[S_CPON]);	//"Code protection ON\r\n"
        if( usb->memCODE_W[0xfff]&0x10)
            CommonFunctions::printMessage(GlobalVariables::strings[S_MCLRON]);	//"Master clear ON\r\n"
        else
            CommonFunctions::printMessage(GlobalVariables::strings[S_MCLROFF]);	//"Master clear OFF\r\n"
    }
    else
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW]);	//"Impossible to read config word\r\n"
    j=1;
    // PrintStatusEnd();
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
}

void ProgP12::Read12F5xx(int dim, int dim2)
{
    qDebug()<<"Read12F5xx"<<dim<<dim2;


    int k=0,z=0,i,j;
    char s[256],t[256];
    if(dim2<4) dim2=4;
    usb->sizeW=0x1000;
    if( usb->memCODE_W) free( usb->memCODE_W);
    usb->memCODE_W=(WORD*)malloc(sizeof(WORD)*usb->sizeW);
    /*
    if(saveLog)
    {
        OpenLogFile();	//"Log.txt"
        fprintf(logfile,"Read12F5xx(%d,%d)\n",dim,dim2);
    }*/
    unsigned int start=CommonFunctions::GetTickCount();
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
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=READ_DATA_PROG;	//configuration word
    usb->bufferU[j++]=INC_ADDR;			// 7FF->000
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    //   if(saveLog)WriteLogIO();
    for(z=0;z<usb->DIMBUF-2&& usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(z<usb->DIMBUF-2)
    {
        usb->memCODE_W[0xfff]=( usb->bufferI[z+1]<<8)+ usb->bufferI[z+2];
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage1(GlobalVariables::strings[S_ConfigWord], usb->memCODE_W[0xfff]);	//"\r\nConfiguration word: 0x%03X\r\n"
        switch( usb->memCODE_W[0xfff]&0x03)
        {
        case 0:
            CommonFunctions::printMessage(GlobalVariables::strings[S_LPOsc]);	//"LP oscillator\r\n"
            break;
        case 1:
            CommonFunctions::printMessage(GlobalVariables::strings[S_XTOsc]);	//"XT oscillator\r\n"
            break;
        case 2:
            CommonFunctions::printMessage(GlobalVariables::strings[S_IntOsc]);	//"Internal osc.\r\n"
            break;
        case 3:
            CommonFunctions::printMessage(GlobalVariables::strings[S_RCOsc]);	//"RC oscillator\r\n"
            break;
        }
        if( usb->memCODE_W[0xfff]&0x04)
            CommonFunctions::printMessage(GlobalVariables::strings[S_WDTON]);	//"WDT ON\r\n"
        else
            CommonFunctions::printMessage(GlobalVariables::strings[S_WDTOFF]);	//"WDT OFF\r\n"
        if( usb->memCODE_W[0xfff]&0x08)
            CommonFunctions::printMessage(GlobalVariables::strings[S_CPOFF]);	//"Code protection OFF\r\n"
        else
            CommonFunctions::printMessage(GlobalVariables::strings[S_CPON]);	//"Code protection ON\r\n"
        if( usb->memCODE_W[0xfff]&0x10)
            CommonFunctions::printMessage(GlobalVariables::strings[S_MCLRON]);	//"Master clear ON\r\n"
        else
            CommonFunctions::printMessage(GlobalVariables::strings[S_MCLROFF]);	//"Master clear OFF\r\n"
    }
    else CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW]);	//"Impossible to read config word\r\n"
    //****************** read code ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_CodeReading1]);		//reading code ...
    //  PrintStatusSetup();
    for(i=0,j=1;i<dim+dim2;i++)
    {
        usb->bufferU[j++]=READ_DATA_PROG;
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF*2/4-2||i==dim+dim2-1)
        {		//2 ins -> 4 ans
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)
                usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(5);
            usb->read();
            for(z=1;z<usb->DIMBUF-2;z++)
            {
                if( usb->bufferI[z]==READ_DATA_PROG)
                {
                    usb->memCODE_W[k++]=( usb->bufferI[z+1]<<8)+ usb->bufferI[z+2];
                    z+=2;
                }
            }
            // CommonFunctions::PrintStatus(GlobalVariables::strings[S_CodeReading],i*100/(dim+dim2),i);	//"Read: %d%%, addr. %03X"
            j=1;
            /*
            if(saveLog)
            {
                fprintf(logfile,strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
                WriteLogIO();
            }
            */
        }
    }
    // PrintStatusEnd();
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    unsigned int stop=CommonFunctions::GetTickCount();
    // if(saveLog)CloseLogFile();
    for(i=k;i<0xfff;i++)
        usb->memCODE_W[i]=0xfff;
    if(k!=dim+dim2)
    {
        CommonFunctions::printMessage("\r\n");
        // CommonFunctions::printMessage2(GlobalVariables::strings[S_ReadErr],dim+dim2,k);	//"Error reading, requested %d words, read %d\r\n"
    }
    else CommonFunctions::printMessage(GlobalVariables::strings[S_Compl]);
    //****************** visualize ********************
    for(i=0;i<4;i+=2)
    {
        //CommonFunctions::printMessage4(GlobalVariables::strings[S_ChipID],i, usb->memCODE_W[dim+i],i+1, usb->memCODE_W[dim+i+1]);	//"ID%d: 0x%03X   ID%d: 0x%03X\r\n"
    }
    if(dim2>4)
    {
        // CommonFunctions::printMessage1(GlobalVariables::strings[S_BKOsccal], usb->memCODE_W[dim+4]);	//"Backup OSCCAL: 0x%03X\r\n"
    }
    CommonFunctions::printMessage(GlobalVariables::strings[S_CodeMem]);	//"\r\nCode memory\r\n"
    s[0]=0;
    int valid=0,empty=1;
    char* aux=(char*)malloc((dim/COL+1)*(16+COL*5));
    aux[0]=0;
    for(i=0;i<dim;i+=COL)
    {
        valid=0;
        for(j=i;j<i+COL&&j<dim;j++)
        {
            //   sprintf(t,"%03X ", usb->memCODE_W[j]);
            //  strcat(s,t);
            if( usb->memCODE_W[j]<0xfff) valid=1;
        }
        if(valid)
        {
            // sprintf(t,"%04X: %s\r\n",i,s);
            // empty=0;
            strcat(aux,t);
        }
        s[0]=0;
    }
    if(empty) CommonFunctions::printMessage(GlobalVariables::strings[S_Empty]);	//empty
    else CommonFunctions::printMessage(aux);
    free(aux);
    if(dim2>5)
    {
        aux=(char*)malloc((dim2/COL+1)*(16+COL*5));
        aux[0]=0;
        s[0]=0;
        CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigResMem]);	//"\r\nConfig and reserved memory:\r\n"
        empty=1;
        for(i=dim;i<dim+dim2;i+=COL)
        {
            valid=0;
            for(j=i;j<i+COL&&j<dim+64;j++)
            {
                sprintf(t,"%03X ", usb->memCODE_W[j]);
                strcat(s,t);
                if( usb->memCODE_W[j]<0xfff) valid=1;
            }
            if(valid)
            {
                sprintf(t,"%04X: %s\r\n",i,s);
                empty=0;
                strcat(aux,t);
            }
            s[0]=0;
        }
        if(empty) CommonFunctions::printMessage(GlobalVariables::strings[S_Empty]);	//empty
        else CommonFunctions::printMessage(aux);
        free(aux);
    }
    // PrintStatusClear();			//clear status report
    CommonFunctions::printMessage("\r\n");
    //CommonFunctions::printMessage1(GlobalVariables::strings[S_End],(stop-start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"







}

void ProgP12::Write12F5xx(int dim, int OscAddr)
{
    qDebug()<<"Write12F5xx"<<dim<<OscAddr;

    // write 12 bit PIC
    // dim=program size     max~4300=10CC
    // OscAddr=OSCCAL address (saved at the beginning), -1 not to use it
    // vdd before vpp
    // CONFIG @ 0x7FF upon entering program mode
    // BACKUP OSCCAL @ dim+5 (saved at the beginning)
    // erase: BULK_ERASE_PROG (1001) +10ms
    // write: BEGIN_PROG (1000) + Tprogram 2ms + END_PROG2 (1110);
    int k=0,z=0,i,j,w;
    int err=0;
    WORD osccal=-1,BKosccal=-1;
    if(OscAddr>dim) OscAddr=dim-1;


    if(OscAddr==-1)
        use_BKosccal=use_osccal=0;

    if(usb->sizeW<0x1000)
    {
        CommonFunctions::printMessage(GlobalVariables::strings[S_NoConfigW2]);	//"Can't find CONFIG (0xFFF)\r\n"
        return;
    }
    /*
        if(saveLog)
{
            OpenLogFile();	//"Log.txt"
            fprintf(logfile,"Write12F5xx(%d,%d)\n",dim,OscAddr);
        }*/
    for(i=0;i<usb->sizeW;i++) usb->memCODE_W[i]&=0xFFF;
    unsigned int start=CommonFunctions::GetTickCount();
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
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    if(OscAddr!=-1)
    {
        for(i=-1;i<OscAddr-0xff;i+=0xff)
        {
            usb->bufferU[j++]=INC_ADDR_N;
            usb->bufferU[j++]=0xff;
        }
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=OscAddr-i;
        usb->bufferU[j++]=READ_DATA_PROG;	// OSCCAL
        if(OscAddr<dim)
        {
            usb->bufferU[j++]=INC_ADDR_N;
            usb->bufferU[j++]=dim-OscAddr;
        }
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=0x4;				// 400->404
        usb->bufferU[j++]=READ_DATA_PROG;	// backup OSCCAL
    }
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
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    // if(saveLog)WriteLogIO();
    if(OscAddr!=-1)
    {
        for(z=4;z<usb->DIMBUF-2&& usb->bufferI[z]!=READ_DATA_PROG;z++);
        if(z<usb->DIMBUF-2) osccal=( usb->bufferI[z+1]<<8)+ usb->bufferI[z+2];
        for(z+=3;z<usb->DIMBUF-2&& usb->bufferI[z]!=READ_DATA_PROG;z++);
        if(z<usb->DIMBUF-2) BKosccal=( usb->bufferI[z+1]<<8)+ usb->bufferI[z+2];
        if(osccal==-1||BKosccal==-1)
        {
            CommonFunctions::printMessage(GlobalVariables::strings[S_ErrOsccal]);	//"Error reading OSCCAL and BKOSCCAL"
            CommonFunctions::printMessage("\r\n");
            return;
        }
        // CommonFunctions::printMessage1(GlobalVariables::strings[S_Osccal],osccal);	//"OSCCAL: 0x%03X\r\n"
        // CommonFunctions::printMessage1(GlobalVariables::strings[S_BKOsccal],BKosccal);	//"Backup OSCCAL: 0x%03X\r\n"
    }
    //****************** erase memory ********************
    CommonFunctions::printMessage(GlobalVariables::strings[S_StartErase]);	//"Erase ... "
    j=1;
    usb->bufferU[j++]=EN_VPP_VCC;			// enter program mode
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x5;
    if(dim>OscAddr+1)
    {				//12F519 (Flash+EEPROM)
        usb->bufferU[j++]=BULK_ERASE_PROG;	// Bulk erase
        usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
        for(i=-1;i<dim-0xff;i+=0xff)
        {	// 0x43F
            usb->bufferU[j++]=INC_ADDR_N;
            usb->bufferU[j++]=0xff;
        }
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=dim-i-1;
        usb->bufferU[j++]=BULK_ERASE_PROG;	// Bulk erase EEPROM
        usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
        if(programID)
        {
            usb->bufferU[j++]=INC_ADDR;
            usb->bufferU[j++]=BULK_ERASE_PROG;	// Bulk erase
            usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
        }
    }
    else{							//12Fxxx
        if(programID)
        {
            for(i=-1;i<dim-0xff;i+=0xff)
            {
                usb->bufferU[j++]=INC_ADDR_N;
                usb->bufferU[j++]=0xff;
            }
            usb->bufferU[j++]=INC_ADDR_N;
            usb->bufferU[j++]=dim-i;
            usb->bufferU[j++]=BULK_ERASE_PROG;	// Bulk erase
            usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
        }
        else{
            usb->bufferU[j++]=BULK_ERASE_PROG;	// Bulk erase
            usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms
        }
    }
    usb->bufferU[j++]=EN_VPP_VCC;		// exit program mode
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			// delay T3=10ms before entering program mode
    usb->bufferU[j++]=EN_VPP_VCC;		// enter program mode
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=INC_ADDR;				// 7FF->000
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;
    usb->bufferU[j++]=2000>>8;				//T3=2ms
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(30);
    if(dim>OscAddr+1)  usb->msDelay(20);
    usb->read();
    CommonFunctions::printMessage( GlobalVariables::strings[S_Compl]);	//"completed\r\n"
    // if(saveLog)WriteLogIO();
    //****************** write code ********************
    CommonFunctions::printMessage( GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    //PrintStatusSetup();
    int dim1=dim;
    if(programID) dim1=dim+5;
    if( usb->memCODE_W[dim+4]>=0xFFF)  usb->memCODE_W[dim+4]=BKosccal;  //reload BKosccal if not present
    if(use_BKosccal)  usb->memCODE_W[OscAddr]=BKosccal;
    else if(use_osccal)  usb->memCODE_W[OscAddr]=osccal;
    for(i=k=w=0,j=1;i<dim1;i++)
    {
        if( usb->memCODE_W[i]<0xfff)
        {
            usb->bufferU[j++]=LOAD_DATA_PROG;
            usb->bufferU[j++]= usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]= usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=BEGIN_PROG;
            usb->bufferU[j++]=WAIT_T3;				//Tprogram 2ms
            usb->bufferU[j++]=END_PROG2;
            usb->bufferU[j++]=WAIT_T2;				//Tdischarge
            usb->bufferU[j++]=READ_DATA_PROG;
            w++;
        }
        usb->bufferU[j++]=INC_ADDR;
        if(j>usb->DIMBUF-10||i==dim1-1)
        {
            //PrintStatus( GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Write: %d%%, ind. %03X"
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(w*3+3);
            w=0;
            usb->read();
            for(z=1;z<usb->DIMBUF-7;z++)
            {
                if( usb->bufferI[z]==INC_ADDR&& usb->memCODE_W[k]>=0xfff) k++;
                else if( usb->bufferI[z]==LOAD_DATA_PROG&& usb->bufferI[z+5]==READ_DATA_PROG)
                {
                    if ( usb->memCODE_W[k]!=( usb->bufferI[z+6]<<8)+ usb->bufferI[z+7])
                    {
                        CommonFunctions::printMessage("\r\n");
                        //CommonFunctions::printMessage3( GlobalVariables::strings[S_CodeWError],k, usb->memCODE_W[k],( usb->bufferI[z+6]<<8)+ usb->bufferI[z+7]);	//"Error writing address %3X: written %03X, read %03X"
                        err++;
                        if(max_err&&err>max_err)
                        {
                            //    CommonFunctions::printMessage1( GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                            CommonFunctions::printMessage( GlobalVariables::strings[S_IntW]);	//"write interrupted"
                            i=dim1;
                            z=usb->DIMBUF;
                        }
                    }
                    k++;
                    z+=8;
                }
            }
            j=1;
            if(saveLog)
            {
                // fprintf(logfile, GlobalVariables::strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errori=%d\n"
                //  WriteLogIO();
            }
        }
    }
    // PrintStatusEnd();
    err+=i-k;
    //  CommonFunctions::printMessage1( GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** write CONFIG ********************
    CommonFunctions::printMessage( GlobalVariables::strings[S_ConfigW]);	//"Write CONFIG ... "
    int err_c=0;
    usb->bufferU[j++]=NOP;				//exit program mode
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;			//10 ms tra uscita e rientro prog. mode
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=LOAD_DATA_PROG;	//config word
    usb->bufferU[j++]= usb->memCODE_W[0xfff]>>8;			//MSB
    usb->bufferU[j++]= usb->memCODE_W[0xfff]&0xff;			//LSB
    usb->bufferU[j++]=BEGIN_PROG;
    usb->bufferU[j++]=WAIT_T3;			//Tprogram 2ms
    usb->bufferU[j++]=END_PROG2;
    usb->bufferU[j++]=WAIT_T2;			//Tdischarge
    usb->bufferU[j++]=READ_DATA_PROG;
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
    usb->msDelay(20);
    usb->read();
    unsigned int stop=CommonFunctions::GetTickCount();
    for(z=10;z<usb->DIMBUF-2&& usb->bufferI[z]!=READ_DATA_PROG;z++);
    if (~ usb->memCODE_W[0xfff]&(( usb->bufferI[z+1]<<8)+ usb->bufferI[z+2]))
    {	//error if written 0 and read 1 (~W&R)
        CommonFunctions::printMessage("\r\n");
        //  CommonFunctions::printMessage2( GlobalVariables::strings[S_ConfigWErr], usb->memCODE_W[0xfff],( usb->bufferI[z+1]<<8)+ usb->bufferI[z+2]);	//"Error writing CONFIG:\r\nwritten %03X, read %03X\r\n"
        err_c++;
    }
    err+=err_c;
    if (z>usb->DIMBUF-2)
    {
        CommonFunctions::printMessage("\r\n");
        CommonFunctions::printMessage( GlobalVariables::strings[S_ConfigWErr2]);	//"Error writing CONFIG"
    }
    // CommonFunctions::printMessage1( GlobalVariables::strings[S_ComplErr],err_c);	//"completed, %d errors\r\n"
    /*
        if(saveLog)
{
            fprintf(logfile,strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d\n"
            WriteLogIO();
            CloseLogFile();
        }*/
    //CommonFunctions::printMessage3( GlobalVariables::strings[S_EndErr],(stop-start)/1000.0,err,err!=1?strings[S_ErrPlur]:strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //PrintStatusClear();			//clear status report




}

void ProgP12::Write12C5xx(int dim, int dummy)
{
    qDebug()<<"Write12C5xx"<<dim<<dummy;
    // write 12 bit PIC with OTP
    // dim=program size     max~4300=10CC
    // vdd before vpp
    // CONFIG @ 0x7FF upon entering program mode
    // write: BEGIN_PROG (1000) + Tprogram 100us + END_PROG2 (1110);
    // 8 pulses + 11N overpulses
    int k=0,z=0,i,j;
    int err=0;
    WORD osccal=-1;
    int OscAddr=dim-1;
    //if(FWVersion<0x800)    {
    //CommonFunctions::printMessage1( GlobalVariables::strings[S_FWver2old],"0.8.0");	//"This firmware is too old. Version %s is required\r\n"
    //  return;
    //}
    if(usb->sizeW<0x1000)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_NoConfigW2]);	//"Can't find CONFIG (0xFFF)\r\n"
        return;
    }
    if(saveLog)
    {
        // OpenLogFile();	//"Log.txt"
        // fprintf(logfile,"Write12C5xx(%d)\n",dim);
    }
    for(i=0;i<usb->sizeW;i++) usb->memCODE_W[i]&=0xFFF;
    unsigned int start=CommonFunctions::GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=1;						//T1=1u
    usb->bufferU[j++]=100;					//T2=100u
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_MN;
    usb->bufferU[j++]=8;						//M=8 pulses
    usb->bufferU[j++]=11;					//N=11 overpulses
    usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD+VPP
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=NOP;
    for(i=-1;i<OscAddr-0xff;i+=0xff)
    {
        usb->bufferU[j++]=INC_ADDR_N;
        usb->bufferU[j++]=0xff;
    }
    usb->bufferU[j++]=INC_ADDR_N;
    usb->bufferU[j++]=OscAddr-i;
    usb->bufferU[j++]=READ_DATA_PROG;	// OSCCAL
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
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(15);
    usb->read();
    j=1;
    // if(saveLog)WriteLogIO();
    for(z=4;z<usb->DIMBUF-2&& usb->bufferI[z]!=READ_DATA_PROG;z++);
    if(z<usb->DIMBUF-2) osccal=( usb->bufferI[z+1]<<8)+ usb->bufferI[z+2];
    if(osccal==-1)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_ErrOsccal]);	//"Error reading OSCCAL and BKOSCCAL"
        CommonFunctions::printMessage("\r\n");
        return;
    }
    //  CommonFunctions::printMessage1( GlobalVariables::strings[S_Osccal],osccal);	//"OSCCAL: 0x%03X\r\n"
    usb->bufferU[j++]=EN_VPP_VCC;		// enter program mode
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=NOP;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=INC_ADDR;				// 7FF->000
    usb->bufferU[j++]=READ_ADC;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    j=1;
    //if(saveLog)WriteLogIO();
    //****************** write code ********************
    CommonFunctions::printMessage( GlobalVariables::strings[S_StartCodeProg]);	//"Write code ... "
    //PrintStatusSetup();
    int N,Nt=0,Nmin=255,Nmax=0,xN=0;
    int dim1=dim;
    if(programID) dim1=dim+5;
    if(use_osccal) usb->memCODE_W[OscAddr]=osccal;
    for(i=k=0,j=1;i<dim1;i++)
    {
        if( usb->memCODE_W[i]<0xfff)
        {
            usb->bufferU[j++]=PROG_C;				//prog&verify with 8 pulses and 11N overpulses
            usb->bufferU[j++]= usb->memCODE_W[i]>>8;		//MSB
            usb->bufferU[j++]= usb->memCODE_W[i]&0xff;		//LSB
            usb->bufferU[j++]=READ_DATA_PROG;
            usb->bufferU[j++]=READ_ADC;
            usb->bufferU[j++]=INC_ADDR;
        }
        else{
            for(; usb->memCODE_W[i]>=0xfff&&j<usb->DIMBUF-1&&i<dim1;i++)  usb->bufferU[j++]=INC_ADDR;
            i--;
        }
        //  PrintStatus( GlobalVariables::strings[S_CodeWriting],i*100/dim,i);	//"Write: %d%%, ind. %03X"
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(10+3);			//Tprogram max 100u*96 ~ 10ms
        usb->read();
        j=1;
        //      if(saveLog)WriteLogIO();
        if( usb->bufferI[1]==PROG_C&& usb->bufferI[3]==READ_DATA_PROG)
        {
            N= usb->bufferI[2];
            if(N<0xF0)
            {
                Nt+=N;
                xN++;
                if(N<Nmin)Nmin=N;
                if(N>Nmax)Nmax=N;
            }
            if( usb->memCODE_W[k]!=( usb->bufferI[4]<<8)+ usb->bufferI[5])
            {
                CommonFunctions::printMessage("\r\n");
                // CommonFunctions::printMessage3( GlobalVariables::strings[S_CodeWError],k, usb->memCODE_W[k],( usb->bufferI[4]<<8)+ usb->bufferI[5]);	//"Error writing address %3X: written %03X, read %03X"
                err++;

                if(max_err&&err>max_err)
                {
                    //    CommonFunctions::printMessage1( GlobalVariables::strings[S_MaxErr],err);	//"Exceeded maximum number of errors (%d), write interrupted\r\n"
                    CommonFunctions::printMessage( GlobalVariables::strings[S_IntW]);	//"write interrupted"
                    i=dim1;
                    z=usb->DIMBUF;
                }
            }
            k++;
        }
        else for(z=1;z<usb->DIMBUF;z++)
        {
            if( usb->bufferI[z]==INC_ADDR&& usb->memCODE_W[k]>=0xfff) k++;
        }
        if(saveLog)
        {
            //     fprintf(logfile,strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d\n"
        }
    }
    // PrintStatusEnd();
    err+=i-k;
    // CommonFunctions::printMessage1( GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //  if(saveLog && xN) fprintf(logfile,"Programming pulses: avg %.1f, min %d, max %d\r\n",(double)Nt/xN,Nmin,Nmax);

    //****************** write CONFIG ********************
    if( usb->memCODE_W[0xfff]<0xfff)
    {
        CommonFunctions::printMessage( GlobalVariables::strings[S_ConfigW]);	//"Write CONFIG ... "
        int err_c=0;
        usb->bufferU[j++]=EN_VPP_VCC;		//exit program mode
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=WAIT_T3;			//10 ms
        usb->bufferU[j++]=EN_VPP_VCC;		//enter program mode
        usb->bufferU[j++]=0x1;
        usb->bufferU[j++]=EN_VPP_VCC;
        usb->bufferU[j++]=0x5;
        usb->bufferU[j++]=LOAD_DATA_PROG;	//config word
        usb->bufferU[j++]= usb->memCODE_W[0xfff]>>8;			//MSB
        usb->bufferU[j++]= usb->memCODE_W[0xfff]&0xff;			//LSB
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(12);
        usb->read();
        j=1;
        //   if(saveLog)WriteLogIO();
        for(i=0;i<20;i++)
        {		//20 pulses
            usb->bufferU[j++]=BEGIN_PROG;
            usb->bufferU[j++]=WAIT_T2;			//Tprogram 100us
            usb->bufferU[j++]=END_PROG2;
        }
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
        j=1;
        for(i=0;i<5;i++)
        {		//20*5=100 pulses
            usb->write();
            usb->msDelay(3);
            usb->read();
            //     if(saveLog)WriteLogIO();
        }
        usb->bufferU[j++]=READ_DATA_PROG;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)  usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(2);
        usb->read();
        j=1;
        //   if(saveLog)WriteLogIO();
        if (~ usb->memCODE_W[0xfff]&(( usb->bufferI[2]<<8)+ usb->bufferI[3]))
        {	//error if written 0 and read 1 (~W&R)
            CommonFunctions::printMessage("\r\n");
            //  CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWErr], usb->memCODE_W[0xfff],( usb->bufferI[2]<<8)+ usb->bufferI[3]);	//"Error writing CONFIG:\r\nwritten %03X, read %03X\r\n"
            err_c++;
        }
        err+=err_c;
    }
    // CommonFunctions::printMessage1(GlobalVariables::strings[S_ComplErr],err);	//"completed, %d errors\r\n"
    //****************** exit ********************
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    j=1;
    //  if(saveLog)WriteLogIO();
    unsigned int stop=CommonFunctions::GetTickCount();

    /*
        if(saveLog)
        {
            fprintf(logfile,strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, errors=%d\n"
            WriteLogIO();
            CloseLogFile();
        }*/
    // CommonFunctions::printMessage3(GlobalVariables::strings[S_EndErr],(stop-start)/1000.0,err,err!=1?strings[S_ErrPlur]:strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //PrintStatusClear();			//clear status report
}
