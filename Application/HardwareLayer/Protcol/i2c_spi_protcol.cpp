#include "i2c_spi_protcol.h"

i2c_spi_protcol::i2c_spi_protcol(USBTools *usb, QObject *parent):ProgDevice(usb, parent)
{
    txtLog=NULL;
}

void i2c_spi_protcol::I2CReceive(int mode, int speed, int N, BYTE *buffer)
{
    // I2C/SPI receive
    // mode:
    // 0 = I2C 8bit
    // 1 = I2C 16bit
    // 2 = SPI 00
    // 3 = SPI 01
    // 4 = SPI 10
    // 5 = SPI 11
    // speed:
    // 0 = 100 kbps
    // 1 = 200 kbps
    // 2 = 300/400 kbps (SPI/I2C)
    // 3 = 500/800 kbps (SPI/I2C)

    int j=1;
    if(N<0) N=0;
    if(N>60) N=60;
    if(mode<0) mode=0;
    if(mode>5) mode=5;
    if(speed<0) speed=0;
    if(speed>3) speed=3;
    //save log
    usb->bufferU[0]=0;
    usb->bufferU[j++]=VREG_DIS;		//Disable HV reg
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    if(mode<2)
    {					//I2C mode
        usb->bufferU[j++]=I2C_INIT;
        usb->bufferU[j++]=(speed<<3)+(speed>0?0x40:0);	//enable slew control if >100k
    }
    else{						//SPI mode
        usb->bufferU[j++]=EXT_PORT;	//CS=1
        usb->bufferU[j++]=CS;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=EXT_PORT;	//CS=0
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_INIT;
        usb->bufferU[j++]=speed+((mode-2)<<2);
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(1);
    usb->read();
    // if(saveLog)WriteLogIO();
    j=1;
    if(mode==0)
    {					//I2C read
        usb->bufferU[j++]=I2C_READ;
        usb->bufferU[j++]=N>(usb->DIMBUF-4)?usb->DIMBUF-4:N;
        usb->bufferU[j++]=buffer[0];		//Control byte
        usb->bufferU[j++]=buffer[1];		//Address;
    }
    else if(mode==1)
    {				//I2C read 16bit
        usb->bufferU[j++]=I2C_READ2;
        usb->bufferU[j++]=N>(usb->DIMBUF-4)?usb->DIMBUF-4:N;
        usb->bufferU[j++]=buffer[0];		//Control byte
        usb->bufferU[j++]=buffer[1];		//Address H;
        usb->bufferU[j++]=buffer[2];		//Address L;
    }
    else if(mode>=2)
    {					//SPI read
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=N>(usb->DIMBUF-5)?usb->DIMBUF-5:N;
        usb->bufferU[j++]=EXT_PORT;		//CS=1
        usb->bufferU[j++]=CS;
        usb->bufferU[j++]=0;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)usb->bufferU[j]=0x0;
    usb->write();
    usb->msDelay(10);
    usb->read();
    // save log
    if(usb->bufferI[1]==I2C_READ||usb->bufferI[1]==I2C_READ2||usb->bufferI[1]==SPI_READ)
    {
        if(usb->bufferI[2]==0xFD)
        {
            printM(strings[S_I2CAckErr]); //"acknowledge error"
        }
        else if(usb->bufferI[2]>0xFA)
        {
            printM(strings[S_InsErr]); //"unknown instruction"
        }
        else{
            char str[1024];
            char t[16];
            int i;
            if(mode==0)	sprintf(str,"> %02X %02X\r\n",usb->bufferU[3],usb->bufferU[4]);
            if(mode==1)	sprintf(str,"> %02X %02X %02X\r\n",usb->bufferU[3],usb->bufferU[4],usb->bufferU[5]);
            strcat(str,"< ");
            for(i=0;i<usb->bufferI[2];i++)
            {
                sprintf(t,"%02X ",(BYTE)usb->bufferI[i+3]);
                strcat(str,t);
                if(i&&i%16==15)
                {
                    strcat(str,"\r\n");
                }
            }
            strcat(str,"\r\n");
            printM(str);
            fflush(stdout);
        }
    }
    else printM(strings[S_ComErr]);
}

void i2c_spi_protcol::I2CSend(int mode, int speed, int N, BYTE *buffer)
{
    // I2C/SPI send
    // mode:
    // 0 = I2C 8bit
    // 1 = I2C 16bit
    // 2 = SPI 00
    // 3 = SPI 01
    // 4 = SPI 10
    // 5 = SPI 11
    // speed:
    // 0 = 100 kbps
    // 1 = 200 kbps
    // 2 = 300/400 kbps (SPI/I2C)
    // 3 = 500/800 kbps (SPI/I2C)

    int i,j=1;
    if(N<0) N=0;
    if(N>57) N=57;
    if(mode<0) mode=0;
    if(mode>5) mode=5;
    if(speed<0) speed=0;
    if(speed>3) speed=3;
    // save log
    usb->bufferU[0]=0;
    usb->bufferU[j++]=VREG_DIS;		//Disable HV reg
    usb->bufferU[j++]=EN_VPP_VCC;	//VDD
    usb->bufferU[j++]=0x1;
    if(mode<2)
    {					//I2C mode
        usb->bufferU[j++]=I2C_INIT;
        usb->bufferU[j++]=(speed<<3)+(speed>0?0x40:0);	//enable slew control if >100k
    }
    else{						//SPI mode
        usb->bufferU[j++]=EXT_PORT;	//CS=1
        usb->bufferU[j++]=CS;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=EXT_PORT;	//CS=0
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=SPI_INIT;
        usb->bufferU[j++]=speed+((mode-2)<<2);
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)usb->bufferU[j]=0x0;
    usb->write();
    usb-> msDelay(1);
    usb->read();
    // if(saveLog)WriteLogIO();
    j=1;
    if(mode==0)
    {					//I2C write
        usb->bufferU[j++]=I2C_WRITE;
        usb->bufferU[j++]=N>(usb->DIMBUF-5)?usb->DIMBUF-5:N;
        usb->bufferU[j++]=buffer[0];		//Control byte
        usb->bufferU[j++]=buffer[1];		//Address
        for(i=0;i<usb->bufferU[2];i++)usb->bufferU[j++]=buffer[i+2];
    }
    else if(mode==1)
    {				//I2C write 16bit
        usb->bufferU[j++]=I2C_WRITE;
        usb->bufferU[j++]=N+1>(usb->DIMBUF-5)?usb->DIMBUF-5:N+1;
        usb->bufferU[j++]=buffer[0];		//Control byte
        usb->bufferU[j++]=buffer[1];		//Address
        usb->bufferU[j++]=buffer[2];		//Address L
        for(i=0;i<usb->bufferU[2]-1;i++)usb->bufferU[j++]=buffer[i+3];
    }
    if(mode==2)
    {					//SPI write
        usb->bufferU[j++]=SPI_WRITE;
        usb->bufferU[j++]=N>(usb->DIMBUF-5)?usb->DIMBUF-5:N;
        for(i=0;i<usb->bufferU[2];i++)usb->bufferU[j++]=buffer[i];
        usb->bufferU[j++]=EXT_PORT;	//CS=1
        usb->bufferU[j++]=CS;
        usb->bufferU[j++]=0;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)usb->bufferU[j]=0x0;
    usb->write();
    usb-> msDelay(20);
    usb->read();
    // save log
    if(usb->bufferI[1]==I2C_WRITE||usb->bufferI[1]==SPI_WRITE)
    {
        if(usb->bufferI[2]==0xFD)
        {
            printM(strings[S_I2CAckErr]); //"acknowledge error"
        }
        else if(usb->bufferI[2]>0xFA)
        {
            printM(strings[S_InsErr]); //"unknown instruction"
        }
        else{
            char str[1024];
            char t[16];
            int n=3;
            int i;
            sprintf(str,"> ");
            if(mode<2) n=5;
            for(i=3;i<usb->bufferU[2]+n;i++)
            {
                sprintf(t,"%02X ",(BYTE)usb->bufferU[i]);
                strcat(str,t);
                if(i&&i%16==15)
                {
                    strcat(str,"\r\n");
                }
            }
            strcat(str,"\r\n");
            printM(str);
        }
    }
    else printM(strings[S_ComErr]);	//"communication error \r\n"
}




void i2c_spi_protcol::printM(QString msg)
{
    qDebug()<<msg;
    if(txtLog!=NULL)
    {
        txtLog->appendPlainText(msg);
    }
}


void i2c_spi_protcol::printM(char *msg)
{

    QString str;
    str.sprintf("%s",msg);
    printM(str);
}
