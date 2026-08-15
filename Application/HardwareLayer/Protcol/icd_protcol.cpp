#include "icd_protcol.h"

//icd_protcol::icd_protcol(QObject *parent) :
//    QObject(parent)
//{
//}

icd_protcol::icd_protcol(USBTools *usb, QObject *parent):ProgDevice(usb, parent)
{
    initVar();
}

void icd_protcol::startICD(int Tck)
{
    usb->bufferU[0]=0;
    int j=1;
    usb->bufferU[j++]=PROG_RST;
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T1T2;
    usb->bufferU[j++]=Tck;				//T1=XXu
    usb->bufferU[j++]=100;				//T2=100u
    usb->bufferU[j++]=SET_PARAMETER;
    usb->bufferU[j++]=SET_T3;			//2ms
    usb->bufferU[j++]=2000>>8;
    usb->bufferU[j++]=2000&0xff;
    usb->bufferU[j++]=VREG_DIS;			//disable HV regulator
    usb->bufferU[j++]=EN_VPP_VCC;		// reset target
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;				//set D as input
    usb->bufferU[j++]=EN_VPP_VCC;		// power-up
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(3);
    usb->read();


}

int icd_protcol::isRunning()
{
    int z,j=1;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x0;		//D=0
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;		//set D as input
    usb->bufferU[j++]=READ_PINS;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    if(saveLog)
    {
        // fprintf(logfile,"isRunning()\n");
        //WriteLogIO();
    }
    for(z=0;z<usb->DIMBUF-1&&usb->bufferI[z]!=READ_PINS;z++);
    if(usb->bufferI[z+1]&1) running=0;
    else running=1;
    return running;
}

void icd_protcol::cont(int break_addr, int freeze)
{
    int j=1;
    //set breakpoint and freeze
    break_addr&=0x1FFF;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=0x2;
    usb->bufferU[j++]=WREG;		//write register
    usb->bufferU[j++]=(break_addr>>8)+(freeze?0x40:0);
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=0x8E;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=0x2;
    usb->bufferU[j++]=WREG;		//write register
    usb->bufferU[j++]=break_addr&0xFF;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=0x8F;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=GO;		//GO
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;		//set D as input
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1+5*Tcom);
    usb->read();
    if(saveLog)
    {
        // fprintf(logfile,"continue()\n");
        // WriteLogIO();
    }
    running=1;
}

void icd_protcol::step()
{
    int j=1;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=STEP;		//single step
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;		//set D as input
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1+Tcom);
    usb->read();
    if(saveLog)
    {
        //  fprintf(logfile,"step()\n");
        // WriteLogIO();
    }
}

void icd_protcol::stop()
{

    usb->bufferU[0]=0;
    int j=1;
    usb->bufferU[j++]=EN_VPP_VCC;		// reset target
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;				//set D as input
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(3);
    usb->read();


}

void icd_protcol::run()
{
    int j=1;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;
    usb->bufferU[j++]=EN_VPP_VCC;		//MCLR=H
    usb->bufferU[j++]=0x5;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    if(saveLog)
    {
        //   fprintf(logfile,"run()\n");
        //   WriteLogIO();
    }
    running=1;
}


int icd_protcol::version()
{
    int j=1,z;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=VER;		//version
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=RX16;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1+2*Tcom);
    usb->read();
    if(saveLog)
    {
        //fprintf(logfile,"version()\n");
        //WriteLogIO();
    }
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=RX16;z++);
    return usb->bufferI[z+3];
}

void icd_protcol::Halt()
{
    int j=1;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x6;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;		//set D as input
    usb->bufferU[j++]=FLUSH;
    for(;j< usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(2);
    usb->read();
    if(saveLog)
    {
        // fprintf(logfile,"halt()\n");
        // WriteLogIO();
    }
    running=0;
    //printf("halted\n");
}

int icd_protcol::ReadRegister(int addr)
{
    int j=1,z;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=0x2;
    usb->bufferU[j++]=RREG;		//Read register
    usb->bufferU[j++]=0x1;		//1 byte
    usb->bufferU[j++]=(addr>>8)&0xFF;
    usb->bufferU[j++]=addr&0xFF;
    usb->bufferU[j++]=RX16;
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;		//set D as input
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1+3*Tcom);
    usb->read();
    if(saveLog)
    {
        //  fprintf(logfile,"ReadRegister(0x%X)\n",addr);
        //  WriteLogIO();
    }
    for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=RX16;z++);
    return usb->bufferI[z+3];
}

int icd_protcol::ReadRegisterN(int addr, int n, int *buf)
{
    int i,j=1,z,w;
    for(i=0;i<n;i+=w)
    {
        w=i+(usb->DIMBUF-9)/2<n?(usb->DIMBUF-9)/2:n-i;
        usb->bufferU[j++]=TX16;
        usb->bufferU[j++]=0x2;
        usb->bufferU[j++]=RREG;				//Read register
        usb->bufferU[j++]=w;
        usb->bufferU[j++]=(addr+i)>>8;
        usb->bufferU[j++]=(addr+i)&0xFF;
        usb->bufferU[j++]=RX16;
        usb->bufferU[j++]=w;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->write();
        usb->msDelay(1+(w+2)*Tcom);
        usb->read();
        if(saveLog)
        {
            // fprintf(logfile,"ReadRegisterN(0x%X,%d)\n",addr,n);
            // WriteLogIO();
        }
        for(z=0;z<usb->DIMBUF-2&&usb->bufferI[z]!=RX16;z++);
        for(j=0;j<w;j++) buf[i+j]=usb->bufferI[z+3+j*2];
        j=1;
    }
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;		//set D as input
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    return i==n?0:-1;
}

void icd_protcol::WriteRegister(int addr, int data)
{
    int j=1;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=0x2;
    usb->bufferU[j++]=WREG;		//write register
    usb->bufferU[j++]=data&0xFF;
    usb->bufferU[j++]=(addr>>8)&0xFF;
    usb->bufferU[j++]=addr&0xFF;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;		//set D as input
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1+2*Tcom);
    usb->read();
    if(saveLog)
    {
        //  fprintf(logfile,"WriteRegister(0x%X,0x%X)\n",addr,data);
        //  WriteLogIO();
    }
}

int icd_protcol::ReadProgMem(int addr)
{
    int addr_temp, data_temp, eecon_temp,data;
    addr_temp=(ReadRegister(EEADRH)<<8)+ReadRegister(EEADR);
    data_temp=(ReadRegister(EEDATH)<<8)+ReadRegister(EEDATA);
    eecon_temp=ReadRegister(EECON1);
    WriteRegister(EEADRH,addr>>8);
    WriteRegister(EEADR,addr&0xFF);
    WriteRegister(EECON1,eecon_temp|0x80);	//EEPGD=1
    WriteRegister(EECON1,eecon_temp|0x81);	//EEPGD=1 + RD=1
    data=(ReadRegister(EEDATH)<<8)+ReadRegister(EEDATA);
    WriteRegister(EEADRH,addr_temp<<8);
    WriteRegister(EEADR,addr_temp&0xFF);
    WriteRegister(EEDATH,data_temp<<8);
    WriteRegister(EEDATA,data_temp&0xFF);
    WriteRegister(EECON1,eecon_temp);
    return data;
}

int icd_protcol::ReadProgMemN(int addr, int n, int *buf)
{
    int addr_temp, data_temp, eecon_temp;
    // if(saveLog) fprintf(logfile,"ReadProgMemN(0x%X,%d)\n",addr,n);
    int i,j=1,z,w,k;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=RREG;		//Read register
    usb->bufferU[j++]=4;			//4 bytes: EEDATA,EEADR,EEDATH,EEADRH
    usb->bufferU[j++]=(EEDATA>>8)&0xFF;
    usb->bufferU[j++]=EEDATA&0xFF;
    usb->bufferU[j++]=RX16;
    usb->bufferU[j++]=4;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=RREG;		//Read register
    usb->bufferU[j++]=1;			//1 byte
    usb->bufferU[j++]=(EECON1>>8)&0xFF;
    usb->bufferU[j++]=EECON1&0xFF;
    usb->bufferU[j++]=RX16;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=WREG;		//write register
    usb->bufferU[j++]=0x80;		//EEPGD=1
    usb->bufferU[j++]=(EECON1>>8)&0xFF;
    usb->bufferU[j++]=EECON1&0xFF;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1+13*Tcom);
    usb->read();
    j=1;
    // if(saveLog)	WriteLogIO();
    for(z=0;z<usb->DIMBUF-5&&usb->bufferI[z]!=RX16;z++);
    data_temp=usb->bufferI[z+3]+(usb->bufferI[z+7]<<8);
    addr_temp=usb->bufferI[z+5]+(usb->bufferI[z+9]<<8);
    for(z+=10;z<usb->DIMBUF-3&&usb->bufferI[z]!=RX16;z++);
    eecon_temp=usb->bufferI[z+3];
    w=k=0;
    for(i=0;i<n;i++)
    {
        usb->bufferU[j++]=TX16;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=WREG;			//write register
        usb->bufferU[j++]=(addr+i)&0xFF;
        usb->bufferU[j++]=(EEADR>>8)&0xFF;
        usb->bufferU[j++]=EEADR&0xFF;
        usb->bufferU[j++]=WREG;			//write register
        usb->bufferU[j++]=((addr+i)>>8)&0xFF;
        usb->bufferU[j++]=(EEADRH>>8)&0xFF;
        usb->bufferU[j++]=EEADRH&0xFF;
        usb->bufferU[j++]=WREG;			//write register
        usb->bufferU[j++]=0x81;			//RD=1
        usb->bufferU[j++]=(EECON1>>8)&0xFF;
        usb->bufferU[j++]=EECON1&0xFF;
        usb->bufferU[j++]=RREG;		//Read register
        usb->bufferU[j++]=3;			//3 bytes: EEDATA,EEADR,EEDATH
        usb->bufferU[j++]=(EEDATA>>8)&0xFF;
        usb->bufferU[j++]=EEDATA&0xFF;
        usb->bufferU[j++]=RX16;
        usb->bufferU[j++]=3;
        w++;
        if(j>usb->DIMBUF-21||i==n-1)
        {
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(2+13*Tcom*w);
            usb->read();
            j=1;
            w=0;
            //  if(saveLog)	WriteLogIO();
            for(z=0;z<usb->DIMBUF-5;z++)
            {
                if(usb->bufferI[z]==RX16)
                {
                    buf[k++]=usb->bufferI[z+3]+(usb->bufferI[z+7]<<8);
                    z+=8; //******controllare!!**********
                }
            }
        }
    }
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=10;
    usb->bufferU[j++]=WREG;			//write register
    usb->bufferU[j++]=eecon_temp;	//EEPGD=1
    usb->bufferU[j++]=(EECON1>>8)&0xFF;
    usb->bufferU[j++]=EECON1&0xFF;
    usb->bufferU[j++]=WREG;			//write register
    usb->bufferU[j++]=data_temp&0xFF;
    usb->bufferU[j++]=(EEDATA>>8)&0xFF;
    usb->bufferU[j++]=EEDATA&0xFF;
    usb->bufferU[j++]=WREG;			//write register
    usb->bufferU[j++]=data_temp>>8;
    usb->bufferU[j++]=(EEDATH>>8)&0xFF;
    usb->bufferU[j++]=EEDATH&0xFF;
    usb->bufferU[j++]=WREG;			//write register
    usb->bufferU[j++]=addr_temp&0xFF;
    usb->bufferU[j++]=(EEADR>>8)&0xFF;
    usb->bufferU[j++]=EEADR&0xFF;
    usb->bufferU[j++]=WREG;			//write register
    usb->bufferU[j++]=addr_temp>>8;
    usb->bufferU[j++]=(EEADRH>>8)&0xFF;
    usb->bufferU[j++]=EEADRH&0xFF;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;		//set D as input
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1+10*Tcom);
    usb->read();
    //if(saveLog)	WriteLogIO();
    return i;

}

int icd_protcol::ReadDataMem(int addr)
{
    int addr_temp, data_temp, eecon_temp,data;
    addr_temp=ReadRegister(EEADR);
    data_temp=ReadRegister(EEDATA);
    eecon_temp=ReadRegister(EECON1);
    WriteRegister(EEADR,addr);
    WriteRegister(EECON1,eecon_temp&0x7F);			//EEPGD=0
    WriteRegister(EECON1,(eecon_temp&0x7F)|0x1);	//EEPGD=0 + RD=1
    data=ReadRegister(EEDATA);
    WriteRegister(EEADR,addr_temp);
    WriteRegister(EEDATA,data_temp);
    WriteRegister(EECON1,eecon_temp);
}

int icd_protcol::ReadDataMemN(int addr, int n, unsigned char *buf)
{
    int addr_temp, data_temp, eecon_temp;
    //if(saveLog) fprintf(logfile,"ReadDataMemN(0x%X,%d)\n",addr,n);
    int i,j=1,z,w,k;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=RREG;		//Read register
    usb->bufferU[j++]=2;			//2 bytes: EEDATA,EEADR
    usb->bufferU[j++]=(EEDATA>>8)&0xFF;
    usb->bufferU[j++]=EEDATA&0xFF;
    usb->bufferU[j++]=RX16;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=RREG;		//Read register
    usb->bufferU[j++]=1;			//1 byte
    usb->bufferU[j++]=(EECON1>>8)&0xFF;
    usb->bufferU[j++]=EECON1&0xFF;
    usb->bufferU[j++]=RX16;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=WREG;		//write register
    usb->bufferU[j++]=0;			//EEPGD=0
    usb->bufferU[j++]=(EECON1>>8)&0xFF;
    usb->bufferU[j++]=EECON1&0xFF;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1+13*Tcom);
    usb->read();
    j=1;
    //if(saveLog)	WriteLogIO();
    for(z=0;z<usb->DIMBUF-5&&usb->bufferI[z]!=RX16;z++);
    data_temp=usb->bufferI[z+3];
    addr_temp=usb->bufferI[z+5];
    for(z+=6;z<usb->DIMBUF-3&&usb->bufferI[z]!=RX16;z++);
    eecon_temp=usb->bufferI[z+3];
    w=k=0;
    for(i=0;i<n;i++)
    {
        usb->bufferU[j++]=TX16;
        usb->bufferU[j++]=6;
        usb->bufferU[j++]=WREG;			//write register
        usb->bufferU[j++]=(addr+i)&0xFF;
        usb->bufferU[j++]=(EEADR>>8)&0xFF;
        usb->bufferU[j++]=EEADR&0xFF;
        usb->bufferU[j++]=WREG;			//write register
        usb->bufferU[j++]=0x1;			//RD=1
        usb->bufferU[j++]=(EECON1>>8)&0xFF;
        usb->bufferU[j++]=EECON1&0xFF;
        usb->bufferU[j++]=RREG;		//Read register
        usb->bufferU[j++]=1;			// EEDATA
        usb->bufferU[j++]=(EEDATA>>8)&0xFF;
        usb->bufferU[j++]=EEDATA&0xFF;
        usb->bufferU[j++]=RX16;
        usb->bufferU[j++]=1;
        w++;
        if(j>usb->DIMBUF-17||i==n-1)
        {
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
            usb->write();
            usb->msDelay(2+10*Tcom*w);
            usb->read();
            j=1;
            w=0;
            //if(saveLog)	WriteLogIO();
            for(z=0;z<usb->DIMBUF-5;z++)
            {
                if(usb->bufferI[z]==RX16)
                {
                    buf[k++]=usb->bufferI[z+3];
                    z+=4;
                }
            }
        }
    }
    usb->bufferU[j++]=TX16;
    usb->bufferU[j++]=6;
    usb->bufferU[j++]=WREG;			//write register
    usb->bufferU[j++]=eecon_temp;	//EEPGD=1
    usb->bufferU[j++]=(EECON1>>8)&0xFF;
    usb->bufferU[j++]=EECON1&0xFF;
    usb->bufferU[j++]=WREG;			//write register
    usb->bufferU[j++]=data_temp&0xFF;
    usb->bufferU[j++]=(EEDATA>>8)&0xFF;
    usb->bufferU[j++]=EEDATA&0xFF;
    usb->bufferU[j++]=WREG;			//write register
    usb->bufferU[j++]=addr_temp&0xFF;
    usb->bufferU[j++]=(EEADR>>8)&0xFF;
    usb->bufferU[j++]=EEADR&0xFF;
    usb->bufferU[j++]=SET_CK_D;
    usb->bufferU[j++]=0x2;		//set D as input
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1+10*Tcom);
    usb->read();
    // if(saveLog)	WriteLogIO();
    return i;
}

char *icd_protcol::decodeCmd(int cmd, char *str, int addrH)
{
    //Disassemble a command and return string
    //addrH is the higher (bank) address for memory (RP1-RP0)

    char ins[32],reg[32];
    if((cmd&0x3F9F)==0) sprintf(str,"nop");
    else if(cmd==0x0001) sprintf(str,"reset");
    else if(cmd==0x0008) sprintf(str,"return");
    else if(cmd==0x0009) sprintf(str,"retfie");
    else if(cmd==0x000A) sprintf(str,"callw");
    else if(cmd==0x000B) sprintf(str,"brw");
    else if(cmd==0x0062) sprintf(str,"option");
    else if(cmd==0x0063) sprintf(str,"sleep");
    else if(cmd==0x0064) sprintf(str,"clrwdt");
    else if(cmd==0x0065) sprintf(str,"trisa");
    else if(cmd==0x0066) sprintf(str,"trisb");
    else if(cmd==0x0067) sprintf(str,"trisc");
    else if((cmd>>12)==0)
    {	//byte oriented instructions
        if((cmd>>8)==0&&cmd&0x80) sprintf(str,"movwf %s",getVar(addrH+(cmd&0x7F),reg));
        else if((cmd>>8)==1)
        {
            if(cmd&0x80) sprintf(str,"clrf %s",getVar(addrH+(cmd&0x7F),reg));
            else sprintf(str,"clrf w");
        }
        else{
            switch(cmd>>8)
            {
            case 2:
                sprintf(ins,"subwf");
                break;
            case 3:
                sprintf(ins,"decf");
                break;
            case 4:
                sprintf(ins,"iorwf");
                break;
            case 5:
                sprintf(ins,"andwf");
                break;
            case 6:
                sprintf(ins,"xorwf");
                break;
            case 7:
                sprintf(ins,"addwf");
                break;
            case 8:
                sprintf(ins,"movf");
                break;
            case 9:
                sprintf(ins,"comf");
                break;
            case 10:
                sprintf(ins,"incf");
                break;
            case 11:
                sprintf(ins,"decfsz");
                break;
            case 12:
                sprintf(ins,"rrf");
                break;
            case 13:
                sprintf(ins,"rlf");
                break;
            case 14:
                sprintf(ins,"swapf");
                break;
            case 15:
                sprintf(ins,"incfsz");
                break;
            default:
                sprintf(ins,"???");
                break;
            }
            sprintf(str,"%s %s,%c",ins,getVar(addrH+(cmd&0x7F),reg),cmd&0x80?'f':'w');
        }
    }
    else if((cmd>>12)==1)
    {	//bit oriented instructions
        switch(cmd>>10)
        {
        case 4:
            sprintf(ins,"bcf");
            break;
        case 5:
            sprintf(ins,"bsf");
            break;
        case 6:
            sprintf(ins,"btfsc");
            break;
        case 7:
            sprintf(ins,"btfss");
            break;
        default:
            sprintf(ins,"??");	//(not possible)
        }
        sprintf(str,"%s %s,%d",ins,getVar(addrH+(cmd&0x7F),reg),(cmd&0x380)>>7);
    }
    else if((cmd>>12)==2) sprintf(str,"%s 0x%X",cmd&0x800?"goto":"call",cmd&0x7FF);
    else if((cmd>>10)==0xC) sprintf(str,"movlw 0x%X",cmd&0xFF);
    else if((cmd>>10)==0xD) sprintf(str,"retlw 0x%X",cmd&0xFF);
    else if((cmd>>9)==0x1E) sprintf(str,"sublw 0x%X",cmd&0xFF);
    else if((cmd>>9)==0x1F) sprintf(str,"addlw 0x%X",cmd&0xFF);
    else if((cmd>>8)==0x38) sprintf(str,"iorlw 0x%X",cmd&0xFF);
    else if((cmd>>8)==0x39) sprintf(str,"andlw 0x%X",cmd&0xFF);
    else if((cmd>>8)==0x3A) sprintf(str,"xorlw 0x%X",cmd&0xFF);
    else sprintf(str,"unknown command");
    return str;
}

char *icd_protcol::getVar(int addr, char *var)
{
    addr&=0x1FF;
    if(variables[addr].name) strcpy(var,variables[addr].name);
    else sprintf(var,"0x%03X",addr);
    return var;
}

void icd_protcol::initVar()
{
    int i;
    for(i=0;i<0x200;i++)
    {//clear variable list
        variables[i].name=0;
        variables[i].display=0;
    }
    variables[0].name="INDF";
    variables[1].name="TMR0";
    variables[2].name="PCL";
    variables[3].name="STATUS";
    variables[4].name="FSR";
    variables[5].name="PORTA";
    variables[6].name="PORTB";
    variables[7].name="PORTC";
    variables[8].name="PORTD";
    variables[9].name="PORTE";
    variables[10].name="PCLATH";
    variables[11].name="INTCON";
    variables[12].name="PIR1";
    variables[13].name="PIR2";
    variables[14].name="TMR1L";
    variables[15].name="TMR1H";
    variables[16].name="T1CON";
    variables[17].name="TMR2";
    variables[18].name="T2CON";
    variables[19].name="SSPBUF";
    variables[20].name="SSPCON";
    variables[21].name="CCPR1L";
    variables[22].name="CCPR1H";
    variables[23].name="CCP1CON";
    variables[24].name="RCSTA";
    variables[25].name="TXREG";
    variables[26].name="RCREG";
    variables[27].name="CCPR2L";
    variables[28].name="CCPR2H";
    variables[29].name="CCP2CON";
    variables[30].name="ADRESH";
    variables[31].name="ADCON0";
    variables[0x6B].name="DEBUG_VAR1";
    variables[0x6C].name="DEBUG_VAR2";
    variables[0x6D].name="DEBUG_VAR3";
    variables[0x6E].name="DEBUG_VAR4";
    variables[0x6F].name="DEBUG_VAR5";
    variables[0x70].name="DEBUG_VAR6";
    variables[0x71].name="DEBUG_VAR7";
    variables[0x72].name="DEBUG_VAR8";
    variables[0x80].name="INDF";
    variables[0x81].name="OPTION_REG";
    variables[0x82].name="PCL";
    variables[0x83].name="STATUS";
    variables[0x84].name="FSR";
    variables[0x85].name="TRISA";
    variables[0x86].name="TRISB";
    variables[0x87].name="TRISC";
    variables[0x88].name="TRISD";
    variables[0x89].name="TRISE";
    variables[0x8A].name="PCLATH";
    variables[0x8B].name="INTCON";
    variables[0x8C].name="PIE1";
    variables[0x8D].name="PIE2";
    variables[0x8E].name="PCON";
    variables[0x91].name="SSPCON2";
    variables[0x92].name="PR2";
    variables[0x93].name="SSPADD";
    variables[0x94].name="SSPSTAT";
    variables[0x98].name="TXSTA";
    variables[0x99].name="SPBRG";
    variables[0x9E].name="ADRESL";
    variables[0x9F].name="ADCON1";
    variables[0x100].name="INDF";
    variables[0x101].name="TMR0";
    variables[0x102].name="PCL";
    variables[0x103].name="STATUS";
    variables[0x104].name="FSR";
    variables[0x106].name="PORTB";
    variables[0x10A].name="PCLATH";
    variables[0x10B].name="INTCON";
    variables[0x10C].name="EEDATA";
    variables[0x10D].name="EEADR";
    variables[0x10E].name="EEDATH";
    variables[0x10F].name="EEADRH";
    variables[0x180].name="INDF";
    variables[0x181].name="OPTION_REG";
    variables[0x182].name="PCL";
    variables[0x183].name="STATUS";
    variables[0x184].name="FSR";
    variables[0x186].name="TRISB";
    variables[0x18A].name="PCLATH";
    variables[0x18B].name="INTCON";
    variables[0x18C].name="EECON1";
    variables[0x18D].name="EECON2";

}
