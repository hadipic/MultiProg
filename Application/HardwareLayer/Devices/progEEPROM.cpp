#include "progeeprom.h"
#include "Libs/commonFunctions.h"



ProgEEPROM::ProgEEPROM(USBTools *usb, QObject *parent) :
    ProgDevice(usb, parent)
{

}
void ProgEEPROM::read(int dim, int dim2, int c, int d)
{
    qDebug()<<"ProgEEPROM Read"<<dim<<dim2;
}

void ProgEEPROM::write(int dim, int OscAddr, int c, int d, int e, int f)
{
    qDebug()<<"ProgEEPROM Write"<<dim<<OscAddr;
}


/*
 * progEEPROM.c - algorithms to program various EEPROM types
 * Copyright (C) 2009-2013 Alberto Maccioni
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, usb->Write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 * or see <http://www.gnu.org/licenses/>
 */

//This cannot be executed conditionally on MSVC
//#include "stdafx.h"


//configure for GUI or command-line


void ProgEEPROM::ReadI2C(int dim,int addr)
// usb->read I2C memories
// dim=size in bytes
// addr:
//      [3:0]  =0: 1 byte address        =1: 2 byte address
//      [7:4]  A2:A0 value
//      [11:8] 17th address bit location (added to control byte)
{

    int k=0,z=0,i,j;
    int AX=(addr>>4)&7;//addr=0, >>4 0000, & 0111, AX=0
    int addr17=(addr>>8)&0xF;//, >>8 : 0, & 1111
    addr&=1; // 00000001
    if(dim>0x30000||dim<0)
{
        ////PrintMessage(strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    /*if(usb->saveLog)
    {
        OpenLogFile();	//"Log.txt"
        fprintf(logfile,"ReadI2C(%d,%d)    (0x%X,0x%X)\n",dim,addr,dim,addr);
    }*/
    sizeEE=dim;
    if(usb->memEE)
        free(usb->memEE);
    usb->memEE=(unsigned char*)malloc(dim);			//EEPROM
    unsigned int start=commonFunctions::GetTickCount();
    hvreg=0;
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=I2C_INIT;
    usb->bufferU[j++]=AX;			//100k
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++)
        usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(5);
    usb->read();
    //if(usb->saveLog)
    //    WriteLogIO();
    //****************** usb->read ********************
    //PrintMessage(strings[S_ReadEE]);		//usb->read EEPROM ...
    //////PrintStatusSetup();
    int inc=0;
    for(i=0,j=1;i<dim;i+=inc)
    {
        if(i<0x10000&&i>0x10000-(usb->DIMBUF-4))
            inc=0x10000-i;	//do not cross 64KB boundary
        else
            inc=i<dim-(usb->DIMBUF-4)?usb->DIMBUF-4:dim-i;
        if(!addr)
        {									//1 byte address
            usb->bufferU[j++]=I2C_READ;
            usb->bufferU[j++]=inc;
            usb->bufferU[j++]=0xA0+(i>>7&0x0E);
            usb->bufferU[j++]=i&0xFF;
        }
        else
        {										//2 byte address
            usb->bufferU[j++]=I2C_READ2;
            usb->bufferU[j++]=inc;
            usb->bufferU[j++]=0xA0+(i>0xFFFF?addr17:0); //17th bit if>64K
            usb->bufferU[j++]=(i>>8)&0xFF;
            usb->bufferU[j++]=i&0xFF;
        }
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++)
            usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(8);
        usb->read();
        if((usb->bufferI[1]==I2C_READ||usb->bufferI[1]==I2C_READ2)&&usb->bufferI[2]<0xFA)
        {
            for(z=3;z<usb->bufferI[2]+3&&z<usb->DIMBUF;z++)
                usb->memEE[k++]=usb->bufferI[z];
            emit onProgressChanged(i, dim);
        }
        ////PrintStatus(strings[S_CodeReading2],i*100/(dim),i);	//"usb->read: %d%%, addr. %05X"
        j=1;
        /*if(usb->saveLog)
{
            fprintf(logfile,strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X) \n"
            WriteLogIO();
        }*/
    }
    ////PrintStatusEnd();
    if(k!=dim)
{
        //PrintMessage("\r\n");
        //PrintMessage2(strings[S_ReadEEErr],dim,k);	//"Error reading EEPROM area, requested %d bytes, usb->read %d\r\n"
        sizeEE=k;
    }
    emit onProgressChanged(dim, dim);

    //else //PrintMessage(strings[S_Compl]);
    //****************** exit ********************
    usb->bufferU[j++]=EN_VPP_VCC;		//0
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(2);
    usb->read();
    unsigned int stop=GetTickCount();
    ////PrintStatusClear();
    //DisplayEE();	//visualize
    int sum=0;

    QString str;
    for(i=0;i<sizeEE;i++)
    {
        sum+=usb->memEE[i];
        QChar ch=usb->memEE[i];
        str.append(ch);
    }
   // qDebug()<<str;
    //qDebug()<<*usb->memEE;
    //PrintMessage1("Checksum: 0x%X\r\n",sum&0xFFFF);
    //PrintMessage1(strings[S_End],(stop-start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"
    ////if(usb->saveLog) CloseLogFile();
}

void ProgEEPROM::WriteI2C(int dim,int addr,int page)

// usb->Write I2C memories
// dim=size in bytes
// addr:
//      [3:0]  =0: 1 byte address        =1: 2 byte address
//      [7:4]  A2:A0 value
//      [11:8] 17th address bit location (added to control byte)
// page=page size
{
    int k=0,z=0,i,j;
    int err=0;
    int AX=(addr>>4)&7;
    int addr17=(addr>>8)&0xF;
    addr&=1;
    hvreg=0;
    if(dim>0x30000||dim<0)
{
        //PrintMessage(strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    /*if(usb->saveLog)
{
        OpenLogFile();	//"Log.txt"
        fprintf(logfile,"WriteI2C(%d,%d,%d)    (0x%X,0x%X)\n",dim,addr,page,dim,addr);
    }*/
    /*if(dim>sizeEE)
{
        i=sizeEE;
        usb->memEE=(unsigned char*)realloc(usb->memEE,dim);
        for(;i<dim;i++)
            usb->memEE[i]=0xFF;
        sizeEE=dim;
    }*/
    if(dim<1)
{
        //PrintMessage(strings[S_NoCode]);	//"Data area is empty\r\n"
        return;
    }
    unsigned int start=GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=I2C_INIT;
    usb->bufferU[j++]=AX;			//100k
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(5);
    usb->read();
    ////if(usb->saveLog)WriteLogIO();
    //****************** usb->Write ********************
    //PrintMessage(strings[S_EEAreaW]);	//"usb->Write EEPROM ... "
    ////PrintStatusSetup();
    for(;page>=usb->DIMBUF-6;page>>=1);
    for(i=0,j=1;i<dim;i+=page)
    {

        usb->bufferU[j++]=I2C_WRITE;
        if(!addr)
{									//1 byte address
            usb->bufferU[j++]=page;
            usb->bufferU[j++]=0xA0+(i>>7&0x0E);
            usb->bufferU[j++]=i&0xFF;
        }
        else{										//2 byte address
            usb->bufferU[j++]=page+1;
            usb->bufferU[j++]=0xA0+(i>0xFFFF?addr17:0); //17th bit if>64K
            usb->bufferU[j++]=(i>>8)&0xFF;
            usb->bufferU[j++]=i&0xFF;
        }
        for(k=0;k<page;k++) usb->bufferU[j++]=usb->memEE[i+k];
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(3);
        usb->read();
        if(usb->bufferI[1]!=I2C_WRITE||usb->bufferI[2]>=0xFA) i=dim+10;
        //PrintStatus(strings[S_CodeWriting2],i*100/(dim),i);	//"usb->Write: %d%%, addr. %04X"
        j=1;
        /*if(usb->saveLog)
{
            fprintf(logfile,strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
            WriteLogIO();
        }*/
        usb->bufferU[j++]=I2C_WRITE;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0xA0; 	//ACK polling
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        int ack=0xFD;
        for(j=0;ack==0xFD&&j<20;j++)
{	//ACK polling until usb->Write complete
            usb->Write();
            usb->msDelay(2);
            usb->read();
            ack=usb->bufferI[2];
            //if(usb->saveLog)
            //    WriteLogIO();
        }
        j=1;
    }
    //PrintStatusEnd();
    //PrintMessage(strings[S_Compl]);	//"completed\r\n"
    //****************** verify EEPROM ********************
    //PrintMessage(strings[S_EEV]);	//"Verify EEPROM ... "
    ////PrintStatusSetup();
    k=0;
    int inc;
    for(i=0,j=1;i<dim;i+=inc)
{
        if(i<0x10000&&i>0x10000-(usb->DIMBUF-4)) inc=0x10000-i;	//do not cross 64KB boundary
        else inc=i<dim-(usb->DIMBUF-4)?usb->DIMBUF-4:dim-i;
        if(!addr)
{									//1 byte address
            usb->bufferU[j++]=I2C_READ;
            usb->bufferU[j++]=inc;
            usb->bufferU[j++]=0xA0+(i>>7&0x0E);
            usb->bufferU[j++]=i&0xFF;
        }
        else{										//2 byte address
            usb->bufferU[j++]=I2C_READ2;
            usb->bufferU[j++]=inc;
            usb->bufferU[j++]=0xA0+(i>0xFFFF?addr17:0); //17th bit if>64K
            usb->bufferU[j++]=(i>>8)&0xFF;
            usb->bufferU[j++]=i&0xFF;
        }
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(8);
        usb->read();
        if((usb->bufferI[1]==I2C_READ||usb->bufferI[1]==I2C_READ2)&&usb->bufferI[2]<0xFA)
{
            for(z=3;z<usb->bufferI[2]+3&&z<usb->DIMBUF;z++)
{
                if(usb->memEE[k++]!=usb->bufferI[z])
{
                    //PrintMessage("\r\n");
                    //PrintMessage4(strings[S_CodeVError],i+z-3,i+z-3,usb->memEE[k-1],usb->bufferI[z]);	//"Error verifying address %04X (%d), written %02X, usb->read %02X\r\n"
                    err++;
                }
            }
        }
        //PrintStatus(strings[S_CodeV2],i*100/(dim),i);	//"Verify: %d%%, addr. %04X"
        j=1;
        /*if(usb->saveLog)
{
            fprintf(logfile,strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, err=%d\n"
            WriteLogIO();
        }*/
        if(err>=usb->max_err) break;
    }
    //PrintStatusEnd();
    if(k!=dim)
{
        //PrintMessage("\r\n");
        //PrintMessage2(strings[S_ReadEEErr],dim,k);	//"Error reading EEPROM area, requested %d bytes, usb->read %d\r\n"
    }
    //PrintMessage1(strings[S_ComplErr],err);	//"completed: %d errors\r\n"
    //****************** exit ********************
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(2);
    usb->read();
    unsigned int stop=GetTickCount();
    ////PrintStatusClear();
    //PrintMessage3(strings[S_EndErr],(stop-start)/1000.0,err,err!=1?strings[S_ErrPlur]:strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //if(usb->saveLog)
    //  CloseLogFile();
}

#define PRE 0x08	//RB3
#define S 0x10		//RB4
#define W 0x20		//RB5
#define ORG 0x20	//RB5

void ProgEEPROM::Read93x(int dim,int na,int options)

// usb->read 93Sx6 uW memories
// dim=size in bytes
// na=address bits
// options=0: x16 organization     =1: x8 organization
{
    int k=0,z=0,i,j,x8;
    hvreg=0;
    if(dim>0x3000||dim<0)
{
        //PrintMessage(strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(na>13) na=13;
    /*if(usb->saveLog)
{
        OpenLogFile();	//"Log.txt"
        fprintf(logfile,"Read93x(%d,%d,%d)    (0x%X,0x%X)\n",dim,na,options,dim,na);
    }*/
    x8=options&1;
    sizeEE=dim;
    if(usb->memEE) free(usb->memEE);
    usb->memEE=(unsigned char*)malloc(dim);			//EEPROM
    unsigned int start=GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=uW_INIT;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=x8?S:S+ORG;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(5);
    usb->read();
    //if(usb->saveLog)
    //WriteLogIO();
    //****************** usb->read ********************
    //PrintMessage(strings[S_ReadEE]);		//usb->read EEPROM ...
    ////PrintStatusSetup();
    int dim2=x8?dim:dim/2;
    for(i=0;i<dim2;)
{
        for(j=1;j<usb->DIMBUF-14&&i<dim2;)
{
            usb->bufferU[j++]=uWTX;
            usb->bufferU[j++]=na+3;				//usb->read
            usb->bufferU[j++]=0xC0+((i>>(na-5))&0x1F);				//110aaaaa aaax0000
            usb->bufferU[j++]=(i<<(13-na))&0xFF;
            usb->bufferU[j++]=uWRX;
            usb->bufferU[j++]=x8?8:16;
            usb->bufferU[j++]=EXT_PORT;
            usb->bufferU[j++]=x8?0:ORG;
            usb->bufferU[j++]=0;
            usb->bufferU[j++]=EXT_PORT;
            usb->bufferU[j++]=x8?S:S+ORG;
            usb->bufferU[j++]=0;
            i++;
        }
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(2);
        usb->read();
        for(z=1;z<usb->DIMBUF-3;z++)
{
            for(;usb->bufferI[z]!=uWRX&&z<usb->DIMBUF-3;z++);
            if(usb->bufferI[z]==uWRX)
{
                if(x8) usb->memEE[k++]=usb->bufferI[z+2];
                else{
                    usb->memEE[k+1]=usb->bufferI[z+2];
                    usb->memEE[k]=usb->bufferI[z+3];
                    k+=2;
                }
                z+=3;
            }
        }
        //PrintStatus(strings[S_CodeReading2],i*100/dim2,i);	//"usb->read: %d%%, addr. %05X"
        j=1;
        /*if(usb->saveLog)
{
            fprintf(logfile,strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X) \n"
            WriteLogIO();
        }*/
    }
    //PrintStatusEnd();
    if(k!=dim)
{
        //PrintMessage("\r\n");
        //PrintMessage2(strings[S_ReadEEErr],dim,k);	//"Error reading EEPROM area, requested %d bytes, usb->read %d\r\n"
        sizeEE=k;
    }
    //else //PrintMessage(strings[S_Compl]);
    //****************** exit ********************
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;		//0
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(2);
    usb->read();
    unsigned int stop=GetTickCount();
    ////PrintStatusClear();
    //DisplayEE();	//visualize
    int sum=0;
    for(i=0;i<sizeEE;i++)
        sum+=usb->memEE[i];
    //PrintMessage1("Checksum: 0x%X\r\n",sum&0xFFFF);
    //PrintMessage1(strings[S_End],(stop-start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"
    ////if(usb->saveLog) CloseLogFile();
}


void ProgEEPROM::Write93Sx(int dim,int na,int page)

// usb->Write 93Sx6 uW memories
// dim=size in bytes
// na=address bits
// page=page size (bytes)
// automatic usb->Write delay
{
    int k=0,z=0,i,j;
    int err=0;
    hvreg=0;
    if(dim>0x1000||dim<0)
{
        //PrintMessage(strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(na>13) na=13;
    if(page>48) page=48;
    if(usb->saveLog)
{
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"Write93Sx(%d,%d,%d)    (0x%X,0x%X)\n",dim,na,page,dim,na);
    }
    if(dim>sizeEE)
{
        i=sizeEE;
        usb->memEE=(unsigned char*)realloc(usb->memEE,dim);
        for(;i<dim;i++) usb->memEE[i]=0xFF;
        sizeEE=dim;
    }
    if(dim<1)
{
        //PrintMessage(strings[S_NoCode]);	//"Data area is empty\r\n"
        return;
    }
    unsigned int start=GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uW_INIT;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=S+W;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uWTX;
    usb->bufferU[j++]=na+3;
    usb->bufferU[j++]=0x98;				//100 11xxx usb->Write enable
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=W;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=S+W+PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uWTX;
    usb->bufferU[j++]=na+3;
    usb->bufferU[j++]=0x98;				//100 11xxx Prot. reg. enable
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=W+PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=S+W+PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uWTX;
    usb->bufferU[j++]=na+3;
    usb->bufferU[j++]=0xFF;				//111 11111111 Prot. reg. clear
    usb->bufferU[j++]=0xF0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=W+PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=S+W;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uWTX;
    usb->bufferU[j++]=na+3;
    usb->bufferU[j++]=0x98;				//100 11xxx usb->Write enable
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=W;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=S+W+PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uWTX;
    usb->bufferU[j++]=na+3;
    usb->bufferU[j++]=0xC0;				//110 xxxxx Prot. reg. usb->read
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uWRX;
    usb->bufferU[j++]=10;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=W+PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(5);
    usb->read();
    ////if(usb->saveLog)WriteLogIO();
    //****************** usb->Write ********************
    //PrintMessage(strings[S_EEAreaW]);	//"usb->Write EEPROM ... "
    ////PrintStatusSetup();
    int addr=0;
    for(i=0,j=1;i<dim;i+=page,addr+=(0x10000>>na)*page/2)
{
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=W;				//make sure to start with S=0
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=S+W;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=uWTX;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0xE0;			//111aaaaa aaa(a) D page usb->Write
        usb->bufferU[j++]=uWTX;
        usb->bufferU[j++]=na;
        usb->bufferU[j++]=addr>>8;
        if(na>8) usb->bufferU[j++]=addr&0xFF;
        usb->bufferU[j++]=uWTX;
        usb->bufferU[j++]=8*page;
        for(k=0;k<page;k+=2)
{
            usb->bufferU[j++]=usb->memEE[i+k+1];
            usb->bufferU[j++]=usb->memEE[i+k];
        }
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=W;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=EXT_PORT;
        usb->bufferU[j++]=S+W;		//S=1 to check status
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(1.5);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        j=1;
        if(usb->bufferI[3]!=uWTX||usb->bufferI[4]>=0xFA) i=dim+10;
        usb->bufferU[j++]=uWRX;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        for(z=0,k=0;z<30&&!k;z++)
{		//Wait until ready
            usb->Write();
            usb->msDelay(1.5);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            k=usb->bufferI[3];
        }
        j=1;
        //PrintStatus(strings[S_CodeWriting2],i*100/(dim),i);	//"usb->Write: %d%%, addr. %04X"
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
        }
    }
    //PrintStatusEnd();
    //PrintMessage(strings[S_Compl]);	//"completed\r\n"
    //****************** verify EEPROM ********************
    //PrintMessage(strings[S_EEV]);	//"Verify EEPROM ... "
    ////PrintStatusSetup();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=S;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uWTX;
    usb->bufferU[j++]=na+3;				//usb->read (16bit)
    usb->bufferU[j++]=0xC0;				//110aaaaa aaax0000
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(2);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    k=0;
    int n=(usb->DIMBUF-2);
    if(n>30) n=30;	//max 240 bit = 30 Byte
    for(i=0,j=1;i<dim;i+=n)
{
        usb->bufferU[j++]=uWRX;
        usb->bufferU[j++]=i<(dim-n)?n*8:(dim-i)*8;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(2);
        usb->read();
        if(usb->bufferI[1]==uWRX&&usb->bufferI[2]<0xFA)
{
            for(z=3;z<usb->bufferI[2]/8+3&&z<usb->DIMBUF;z+=2,k+=2)
{
                if(usb->memEE[k+1]!=usb->bufferI[z])
{
                    //PrintMessage("\r\n");
                    //PrintMessage4(strings[S_CodeVError],i+z-3,i+z-3,usb->memEE[k+1],usb->bufferI[z]);	//"Error verifying address %04X (%d), written %02X, usb->read %02X\r\n"
                    err++;
                }
                if(usb->memEE[k]!=usb->bufferI[z+1])
{
                    //PrintMessage("\r\n");
                    //PrintMessage4(strings[S_CodeVError],i+z-3,i+z-3,usb->memEE[k],usb->bufferI[z+1]);	//"Error verifying address %04X (%d), written %02X, usb->read %02X\r\n"
                    err++;
                }
            }
        }
        //PrintStatus(strings[S_CodeV2],i*100/(dim),i);	//"Verify: %d%%, addr. %04X"
        j=1;
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, err=%d\n"
            //WriteLogIO();
        }
        if(err>=usb->max_err) break;
    }
    //PrintStatusEnd();
    if(k!=dim)
{
        //PrintMessage("\r\n");
        //PrintMessage2(strings[S_ReadEEErr],dim,k);	//"Error reading EEPROM area, requested %d bytes, usb->read %d\r\n"
    }
    //PrintMessage1(strings[S_ComplErr],err);	//"completed: %d errors\r\n"
    //****************** exit ********************
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(2);
    usb->read();
    unsigned int stop=GetTickCount();
    ////PrintStatusClear();
    //PrintMessage3(strings[S_EndErr],(stop-start)/1000.0,err,err!=1?strings[S_ErrPlur]:strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    ////if(usb->saveLog) CloseLogFile();
}


void ProgEEPROM::Write93Cx(int dim,int na, int options)

// usb->Write 93Cx6 uW memories
// dim=size in bytes
// na=address bits
// options=0: x16 organization     =1: x8 organization
{
    int k=0,z=0,i,j;
    int err=0;
    hvreg=0;
    if(dim>0x1000||dim<0)
{
        //PrintMessage(strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(na>13) na=13;
    if(usb->saveLog)
{
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"Write93Cx(%d,%d,%d)    (0x%X,0x%X)\n",dim,na,options,dim,na);
    }
    if(dim>sizeEE)
{
        i=sizeEE;
        usb->memEE=(unsigned char*)realloc(usb->memEE,dim);
        for(;i<dim;i++) usb->memEE[i]=0xFF;
        sizeEE=dim;
    }
    if(dim<1)
{
        //PrintMessage(strings[S_NoCode]);	//"Data area is empty\r\n"
        return;
    }
    unsigned int start=GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uW_INIT;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=options==0?S+ORG+PRE:S+PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uWTX;
    usb->bufferU[j++]=na+3;
    usb->bufferU[j++]=0x98;				//100 11xxx EWEN
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=options==0?ORG+PRE:PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=options==0?S+ORG+PRE:S+PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uWTX;
    usb->bufferU[j++]=na+3;
    usb->bufferU[j++]=0x90;				//100 10xxx ERAL
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=options==0?ORG+PRE:PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=options==0?S+ORG+PRE:S+PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=uWRX;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(5);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    j=1;
    usb->bufferU[j++]=uWRX;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    for(i=0,k=0;i<30&&!k;i++)
{		//Wait until ready
        usb->Write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        k=usb->bufferI[3];
    }
    //****************** usb->Write ********************
    //PrintMessage(strings[S_EEAreaW]);	//"usb->Write EEPROM ... "
    ////PrintStatusSetup();
    int addr=0;
    j=1;
    for(i=0;i<dim;i+=options==0?2:1,addr+=0x10000>>na)
{
        if(usb->memEE[i]<0xFF||(options==0&&usb->memEE[i+1]<0xFF))
{
            usb->bufferU[j++]=EXT_PORT;
            usb->bufferU[j++]=options==0?ORG+PRE:PRE;
            usb->bufferU[j++]=0;
            usb->bufferU[j++]=EXT_PORT;
            usb->bufferU[j++]=options==0?S+ORG+PRE:S+PRE;
            usb->bufferU[j++]=0;
            usb->bufferU[j++]=uWTX;
            usb->bufferU[j++]=3;
            usb->bufferU[j++]=0xA0;			//101aaaaa aaa(a) usb->Write
            usb->bufferU[j++]=uWTX;
            usb->bufferU[j++]=na;
            usb->bufferU[j++]=addr>>8;
            if(na>8) usb->bufferU[j++]=addr&0xFF;
            usb->bufferU[j++]=uWTX;
            if(options==0)
{		//x16
                usb->bufferU[j++]=16;
                usb->bufferU[j++]=usb->memEE[i+1];
                usb->bufferU[j++]=usb->memEE[i];
            }
            else{				//x8
                usb->bufferU[j++]=8;
                usb->bufferU[j++]=usb->memEE[i];
            }
            usb->bufferU[j++]=EXT_PORT;
            usb->bufferU[j++]=options==0?ORG+PRE:PRE;
            usb->bufferU[j++]=0;
            usb->bufferU[j++]=EXT_PORT;
            usb->bufferU[j++]=options==0?S+ORG+PRE:S+PRE;
            usb->bufferU[j++]=0;
            usb->bufferU[j++]=uWRX;
            usb->bufferU[j++]=1;
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
            usb->Write();
            usb->msDelay(1.5);
            usb->read();
            //PrintStatus(strings[S_CodeWriting2],i*100/(dim),i);	//"usb->Write: %d%%, addr. %04X"
            j=1;
            if(usb->saveLog)
{
                //fprintf(logfile,strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
                //WriteLogIO();
            }
            usb->bufferU[j++]=uWRX;
            usb->bufferU[j++]=1;
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
            for(z=0,k=0;z<30&&!k;z++)
{		//Wait until ready
                usb->Write();
                usb->msDelay(1.5);
                usb->read();
                //if(usb->saveLog)WriteLogIO();
                k=usb->bufferI[3];
            }
            j=1;
        }
    }
    usb->msDelay(1);
    //PrintStatusEnd();
    if(i!=dim)
{
        //PrintMessage2(strings[S_CodeWError4],i,dim);	//"Error writing code area, requested %d bytes, usb->read %d\r\n"
    }
    //else //PrintMessage(strings[S_Compl]);	//"completed\r\n"
    //****************** verify EEPROM ********************
    //PrintMessage(strings[S_EEV]);	//"Verify EEPROM ... "
    ////PrintStatusSetup();
    j=1;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=options==0?ORG+PRE:PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=options==0?S+ORG+PRE:S+PRE;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=FLUSH;
    usb->Write();
    usb->msDelay(1.5);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    k=0;
    int dim2=options==0?dim/2:dim;
    for(i=0;i<dim2;)
{
        for(j=1;j<usb->DIMBUF-14&&i<dim2;)
{
            usb->bufferU[j++]=uWTX;
            usb->bufferU[j++]=na+3;				//usb->read
            usb->bufferU[j++]=0xC0+((i>>(na-5))&0x1F);				//110aaaaa aaax0000
            usb->bufferU[j++]=(i<<(13-na))&0xFF;
            usb->bufferU[j++]=uWRX;
            usb->bufferU[j++]=options==0?16:8;
            usb->bufferU[j++]=EXT_PORT;
            usb->bufferU[j++]=options==0?ORG:0;
            usb->bufferU[j++]=0;
            usb->bufferU[j++]=EXT_PORT;
            usb->bufferU[j++]=options==0?S+ORG:S;
            usb->bufferU[j++]=0;
            i++;
        }
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(2);
        usb->read();
        for(z=1;z<usb->DIMBUF-3;z++)
{
            for(;usb->bufferI[z]!=uWRX&&z<usb->DIMBUF-3;z++);
            if(usb->bufferI[z]==uWRX)
{
                if(options==1)
{		//x8
                    if(usb->memEE[k]!=usb->bufferI[z+2])
{
                        //PrintMessage("\r\n");
                        //PrintMessage4(strings[S_CodeVError],k,k,usb->memEE[k],usb->bufferI[z+2]);	//"Error verifying address %04X (%d), written %02X, usb->read %02X\r\n"
                        err++;
                    }
                    k++;
                }
                else{				//x16
                    if(usb->memEE[k]!=usb->bufferI[z+3])
{
                        //PrintMessage("\r\n");
                        //PrintMessage4(strings[S_CodeVError],k,k,usb->memEE[k],usb->bufferI[z+3]);	//"Error verifying address %04X (%d), written %02X, usb->read %02X\r\n"
                        err++;
                    }
                    if(usb->memEE[k+1]!=usb->bufferI[z+2])
{
                        //PrintMessage("\r\n");
                        //PrintMessage4(strings[S_CodeVError],k+1,k+1,usb->memEE[k+1],usb->bufferI[z+2]);	//"Error verifying address %04X (%d), written %02X, usb->read %02X\r\n"
                        err++;
                    }
                    k+=2;
                }
                z+=3;
            }
        }
        //PrintStatus(strings[S_CodeV2],i*100/dim2,i);	//"Verify: %d%%, addr. %04X"
        j=1;
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, err=%d\n"
            //WriteLogIO();
        }
        if(err>=usb->max_err) break;
    }
    //PrintStatusEnd();
    if(k!=dim)
{
        //PrintMessage("\r\n");
        //PrintMessage2(strings[S_ReadEEErr],dim,k);	//"Error reading EEPROM area, requested %d bytes, usb->read %d\r\n"
    }
    //PrintMessage1(strings[S_ComplErr],err);	//"completed: %d errors\r\n"
    //****************** exit ********************
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(2);
    usb->read();
    unsigned int stop=GetTickCount();
    ////PrintStatusClear();
    //PrintMessage3(strings[S_EndErr],(stop-start)/1000.0,err,err!=1?strings[S_ErrPlur]:strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //if(usb->saveLog) CloseLogFile();
}

#define CS 8
#define HLD 16		//Hold
#define WP 0x40		//usb->Write protect



void ProgEEPROM::Read25xx(int dim)

// usb->read 25xx SPI memories
// dim=size in bytes
{
    int k=0,z=0,i,j,ID;
    hvreg=0;
    if(dim>0x1000000||dim<0)
{
        //PrintMessage(strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(usb->saveLog)
{
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"Read25xx(%d)    (0x%X)\n",dim,dim);
    }
    sizeEE=dim;
    if(usb->memEE) free(usb->memEE);
    usb->memEE=(unsigned char*)malloc(dim);			//EEPROM
    unsigned int start=GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=SPI_INIT;
    usb->bufferU[j++]=3;				//0=100k, 1=200k, 2=300k, 3=500k
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=0
    usb->bufferU[j++]=CS+HLD;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
    usb->bufferU[j++]=HLD;
    usb->bufferU[j++]=WP;
    usb->bufferU[j++]=SPI_WRITE;		//usb->read ID
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=0x9F;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
    usb->bufferU[j++]=CS+HLD;
    usb->bufferU[j++]=WP;
    usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=0
    usb->bufferU[j++]=HLD;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SPI_WRITE;		//usb->read
    if(dim>0x10000)
{				//24 bit address
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
    }
    else if(dim>0x200)
{				//16 bit address
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
    }
    else{						//8 bit address
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(5);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    for(z=1;z<usb->DIMBUF-4&&usb->bufferI[z]!=SPI_READ;z++);
    ID=(usb->bufferI[z+2]<<16)+(usb->bufferI[z+3]<<8)+usb->bufferI[z+4];
    //if(ID>0&&ID!=0xFFFFFF)
    //PrintMessage1("DEVICE ID=0x%06X\r\n",ID);
    //****************** usb->read ********************
    //PrintMessage(strings[S_ReadEE]);		//usb->read EEPROM ...
    ////PrintStatusSetup();
    for(i=0,j=1;i<dim;i+=usb->DIMBUF-4)
{
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=i<dim-(usb->DIMBUF-4)?usb->DIMBUF-4:dim-i;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(4);
        usb->read();
        if(usb->bufferI[1]==SPI_READ&&usb->bufferI[2]<0xFA)
{
            for(z=3;z<usb->bufferI[2]+3&&z<usb->DIMBUF;z++) usb->memEE[k++]=usb->bufferI[z];
        }
        //PrintStatus(strings[S_CodeReading2],i*100/(dim),i);	//"usb->read: %d%%, addr. %05X"
        j=1;
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X) \n"
            //WriteLogIO();
        }
    }
    //PrintStatusEnd();
    if(k!=dim)
{
        //PrintMessage("\r\n");
        //PrintMessage2(strings[S_ReadEEErr],dim,k);	//"Error reading EEPROM area, requested %d bytes, usb->read %d\r\n"
        sizeEE=k;
    }
    //else //PrintMessage(strings[S_Compl]);
    //****************** exit ********************
    usb->bufferU[j++]=EXT_PORT;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EN_VPP_VCC;		//0
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(2);
    usb->read();
    unsigned int stop=GetTickCount();
    ////PrintStatusClear();
    //DisplayEE();	//visualize
    int sum=0;
    for(i=0;i<sizeEE;i++)
        sum+=usb->memEE[i];
    //PrintMessage1("Checksum: 0x%X\r\n",sum&0xFFFF);
    //PrintMessage1(strings[S_End],(stop-start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"
    //if(usb->saveLog) CloseLogFile();
}


void ProgEEPROM::Write25xx(int dim,int options)

// usb->Write SPI memories
// dim=size in bytes
// options:
//        [11:0]=page size
//        [12]=erase before usb->Write
// automatic usb->Write delay
{
    int k=0,z=0,i,j,ID;
    int err=0;
    hvreg=0;
    int page=options&0xFFF;
    if(dim>0x1000000||dim<0)
{
        //PrintMessage(strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(usb->saveLog)
{
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"Write25xx(%d,%d)    (0x%X,0x%X)\n",dim,options,dim,options);
    }
    if(dim>sizeEE)
{
        i=sizeEE;
        usb->memEE=(unsigned char*)realloc(usb->memEE,dim);
        for(;i<dim;i++) usb->memEE[i]=0xFF;
        sizeEE=dim;
    }
    if(dim<1)
{
        //PrintMessage(strings[S_NoCode]);	//"Data area is empty\r\n"
        return;
    }
    unsigned int start=GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=SPI_INIT;
    usb->bufferU[j++]=3;				//0=100k, 1=200k, 2=300k, 3=500k
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
    usb->bufferU[j++]=CS+HLD;
    usb->bufferU[j++]=WP;
    usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
    usb->bufferU[j++]=HLD;
    usb->bufferU[j++]=WP;
    usb->bufferU[j++]=SPI_WRITE;		//usb->read ID
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=0x9F;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=3;
    usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
    usb->bufferU[j++]=CS+HLD;
    usb->bufferU[j++]=WP;
    usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
    usb->bufferU[j++]=HLD;
    usb->bufferU[j++]=WP;
    usb->bufferU[j++]=SPI_WRITE;		//usb->Write ENABLE
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=6;
    usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
    usb->bufferU[j++]=CS+HLD;
    usb->bufferU[j++]=WP;
    usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
    usb->bufferU[j++]=HLD;
    usb->bufferU[j++]=WP;
    usb->bufferU[j++]=SPI_WRITE;		//usb->Write STATUS
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
    usb->bufferU[j++]=CS+HLD;
    usb->bufferU[j++]=WP;
    usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
    usb->bufferU[j++]=HLD;
    usb->bufferU[j++]=WP;
    usb->bufferU[j++]=SPI_WRITE;		//usb->read STATUS
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=5;
    usb->bufferU[j++]=SPI_READ;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
    usb->bufferU[j++]=CS+HLD;
    usb->bufferU[j++]=WP;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(25);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    j=1;
    for(z=1;z<usb->DIMBUF-4&&usb->bufferI[z]!=SPI_READ;z++);
    ID=(usb->bufferI[z+2]<<16)+(usb->bufferI[z+3]<<8)+usb->bufferI[z+4];
    //if(ID>0&&ID!=0xFFFFFF) 	//PrintMessage1("DEVICE ID=0x%06X\r\n",ID);
    if(options&0x1000)
{		//erase before usb->Write
        //PrintMessage(strings[S_StartErase]);	//"Erasing ... "
        usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
        usb->bufferU[j++]=HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=SPI_WRITE;		//usb->Write ENABLE
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=6;
        usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
        usb->bufferU[j++]=CS+HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
        usb->bufferU[j++]=HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=SPI_WRITE;		//CHIP ERASE
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=0xC7;
        usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
        usb->bufferU[j++]=CS+HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(2);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        j=1;
        usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
        usb->bufferU[j++]=HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=SPI_WRITE;		//usb->read STATUS
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=5;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
        usb->bufferU[j++]=CS+HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        int pp;
        for(pp=1,j=0;pp&&j<400;j++)
{	//wait for usb->Write completion
            usb->Write();
            usb->msDelay(100);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=SPI_READ;z++);
            pp=usb->bufferI[z+2]&1;	//WIP bit
        }
        //if(usb->saveLog) fprintf(logfile,"Erase time %d ms\n",j*100);
        //PrintMessage(strings[S_Compl]);	//"completed"
        usb->msDelay(100);
    }
    //****************** usb->Write ********************
    //PrintMessage(strings[S_EEAreaW]);	//"usb->Write EEPROM ... "
    int pp;
    for(i=0,j=1;i<dim;i+=page)
{
        if(options&0x1000)
{		//if chip erase skip empty pages
            for(k=page;k==page&&i<dim-page;)
{
                for(k=0;k<page;k++)
{ if(usb->memEE[i+k]<0xFF) k=page;}
                if(k==page) i+=page;
            }
        }
        usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
        usb->bufferU[j++]=HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=SPI_WRITE;		//usb->Write ENABLE
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=6;
        usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
        usb->bufferU[j++]=CS+HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
        usb->bufferU[j++]=HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=SPI_WRITE;		//usb->Write
        if(dim>0x10000)
{				//24 bit address
            usb->bufferU[j++]=4;
            usb->bufferU[j++]=2;
            usb->bufferU[j++]=i>>16;
            usb->bufferU[j++]=(i>>8)&0xFF;
            usb->bufferU[j++]=i&0xFF;
        }
        else if(dim>0x200)
{				//16 bit address
            usb->bufferU[j++]=3;
            usb->bufferU[j++]=2;
            usb->bufferU[j++]=i>>8;
            usb->bufferU[j++]=i&0xFF;
        }
        else{						//8 bit address
            usb->bufferU[j++]=2;
            usb->bufferU[j++]=2+(i&0x100?8:0);
            usb->bufferU[j++]=i&0xFF;
        }
        pp=page<usb->DIMBUF-j-4?page:usb->DIMBUF-j-4;
        for(k=0;k<page;)
{
            usb->bufferU[j++]=SPI_WRITE;
            usb->bufferU[j++]=pp;
            for(;k<page&&pp;k++,pp--) usb->bufferU[j++]=usb->memEE[i+k];
            usb->bufferU[j++]=FLUSH;
            for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
            usb->Write();
            usb->msDelay(1.5);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            for(;z<usb->DIMBUF-1&&usb->bufferI[z]!=SPI_WRITE;z++);
            if(usb->bufferI[z+1]>=0xFA) k=i=dim+10;
            pp=(page-k)<usb->DIMBUF-4?page-k:usb->DIMBUF-4;
            j=1;
        }
        usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
        usb->bufferU[j++]=CS+HLD;
        usb->bufferU[j++]=WP;

        usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
        usb->bufferU[j++]=HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=SPI_WRITE;		//usb->read STATUS
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=5;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
        usb->bufferU[j++]=CS+HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(1.5);
        usb->read();
        //if(usb->saveLog)WriteLogIO();
        j=1;
        usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=1
        usb->bufferU[j++]=HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=SPI_WRITE;		//usb->read STATUS
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=5;
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=EXT_PORT;	//CS=1, HLD=1, WP=1
        usb->bufferU[j++]=CS+HLD;
        usb->bufferU[j++]=WP;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        for(pp=1,j=0;pp&&j<50;j++)
{	//wait for usb->Write completion
            usb->Write();
            usb->msDelay(1.5);
            usb->read();
            //if(usb->saveLog)WriteLogIO();
            for(z=1;z<usb->DIMBUF-1&&usb->bufferI[z]!=SPI_READ;z++);
            pp=usb->bufferI[z+2]&1;	//WIP bit
        }
        //PrintStatus(strings[S_CodeWriting2],i*100/(dim),i);	//"usb->Write: %d%%, addr. %04X"
        j=1;
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log7],i,i,0,0);	//"i=%d(0x%X), k=%d(0x%X)\n"
        }
    }
    //PrintStatusEnd();
    //PrintMessage(strings[S_Compl]);	//"completed\r\n"
    //****************** verify EEPROM ********************
    //PrintMessage(strings[S_EEV]);	//"Verify EEPROM ... "
    ////PrintStatusSetup();
    j=1;
    usb->bufferU[j++]=EXT_PORT;	//CS=0, HLD=1, WP=0
    usb->bufferU[j++]=HLD;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=SPI_WRITE;		//usb->read
    if(dim>0x10000)
{				//24 bit address
        usb->bufferU[j++]=4;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
    }
    else if(dim>0x200)
{				//16 bit address
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=0;
    }
    else{						//8 bit address
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=0;
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(2);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    k=0;
    for(i=0,j=1;i<dim;i+=usb->DIMBUF-4)
{
        usb->bufferU[j++]=SPI_READ;
        usb->bufferU[j++]=i<dim-(usb->DIMBUF-4)?usb->DIMBUF-4:dim-i;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(4);
        usb->read();
        if(usb->bufferI[1]==SPI_READ&&usb->bufferI[2]<0xFA)
{
            for(z=3;z<usb->bufferI[2]+3&&z<usb->DIMBUF;z++)
{
                if(usb->memEE[k++]!=usb->bufferI[z])
{
                    //PrintMessage("\r\n");
                    //PrintMessage4(strings[S_CodeVError],i+z-3,i+z-3,usb->memEE[k-1],usb->bufferI[z]);	//"Error verifying address %04X (%d), written %02X, usb->read %02X\r\n"
                    err++;
                }
            }
        }
        //PrintStatus(strings[S_CodeV2],i*100/(dim),i);	//"Verify: %d%%, addr. %04X"
        j=1;
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, err=%d\n"
            //WriteLogIO();
        }
        if(err>=usb->max_err) break;
    }
    //PrintStatusEnd();
    if(k!=dim)
{
        //PrintMessage("\r\n");
        //PrintMessage2(strings[S_ReadEEErr],dim,k);	//"Error reading EEPROM area, requested %d bytes, usb->read %d\r\n"
        sizeEE=k;
    }
    //PrintMessage1(strings[S_ComplErr],err);	//"completed: %d errors\r\n"
    //****************** exit ********************
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(2);
    usb->read();
    unsigned int stop=GetTickCount();
    ////PrintStatusClear();
    //PrintMessage3(strings[S_EndErr],(stop-start)/1000.0,err,err!=1?strings[S_ErrPlur]:strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //if(usb->saveLog) CloseLogFile();
}

struct ID_OW{
    int id;
    char *device;
} OW_LIST[]={
{0x10,"DS1820\r\n"},
{0x14,"DS2430\r\n"},
{0x23,"DS2433\r\n"},
{0x28,"DS18B20\r\n"},
{0x2D,"DS2431\r\n"},
{0x43,"DS28EC20\r\n"},
};


void ProgEEPROM::OW_ID(int id)
{
    char s[64];
    int i;
    for(i=0;i<sizeof(OW_LIST)/sizeof(OW_LIST[0]);i++)
{
        if(id==OW_LIST[i].id)
{
            sprintf(s,OW_LIST[i].device,id);
            //PrintMessage(s);
            return;
        }
    }
    //sprintf(s,"%s",strings[S_nodev]); //"Unknown device\r\n");
    //PrintMessage(s);
}

#define READ_ROM 0x33
#define MATCH_ROM 0x55
#define SKIP_ROM 0xCC
#define SEARCH_ROM 0xF0
#define Write_SCRATCHPAD 0x0F
#define READ_SCRATCHPAD 0xAA
#define COPY_SCRATCHPAD 0x55
#define READ_MEMORY 0xF0
#define Write_APP_REGISTER 0x99
#define READ_STAT_REGISTER 0x66
#define READ_APP_REGISTER 0xC3
#define COPY_LOCK_APP_REGISTER 0x5A



void ProgEEPROM::ReadOneWireMem(int dim,int options)

// usb->read OneWire memories
// dim=size in bytes
// options:
//          1=status register + application register
//          2=protection bytes + ID after memory area
{
    int k=0,z=0,i,j;
    hvreg=0;
    if(usb->FWVersion<0x800)
    {
        //PrintMessage1(strings[S_FWver2old],"0.8.0");	//"This firmware is too old. Version %s is required\r\n"
        return;
    }
    if(dim>0x10000||dim<0)
{
        //PrintMessage(strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(usb->saveLog)
{
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"ReadOneWireMem(%d)    (0x%X)\n",dim,dim);
    }
    sizeEE=dim;
    if(usb->memEE) free(usb->memEE);
    usb->memEE=(unsigned char*)malloc(dim);			//EEPROM
    unsigned int start=GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=uW_INIT;	//set RB1=0 to use as GND terminal beside RB0
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=OW_RESET;
    usb->bufferU[j++]=OW_WRITE;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=READ_ROM;
    usb->bufferU[j++]=OW_READ;
    usb->bufferU[j++]=8;
    if(dim<=32)
{	//1 byte address
        usb->bufferU[j++]=OW_WRITE;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=READ_MEMORY;
        usb->bufferU[j++]=0;	//address
    }
    else{			//2 byte address
        usb->bufferU[j++]=OW_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=READ_MEMORY;
        usb->bufferU[j++]=0;	//address
        usb->bufferU[j++]=0;	//address
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(12);
    usb->read();
    j=1;
    //if(usb->saveLog)WriteLogIO();
    if(usb->bufferI[5]==OW_RESET&&usb->bufferI[6]==0)
{	//no presence pulse
        //PrintMessage(strings[S_ComErr]);		//communication error
        usb->bufferU[j++]=EN_VPP_VCC;		//turn off
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(1);
        usb->read();
        //if(usb->saveLog) CloseLogFile();
        return;
    }
    for(z=1;usb->bufferI[z]!=OW_READ&&z<usb->DIMBUF;z++);
    if(z<usb->DIMBUF-9)
{
        //PrintMessage1("Family code: 0x%02X ",usb->bufferI[z+2]);
        OW_ID(usb->bufferI[z+2]);
        //PrintMessage3("Serial ID: 0x%02X%02X%02X",usb->bufferI[z+3],usb->bufferI[z+4],usb->bufferI[z+5]);
        //PrintMessage3("%02X%02X%02X",usb->bufferI[z+6],usb->bufferI[z+7],usb->bufferI[z+8]);
        //PrintMessage1("\r\nCRC: 0x%02X\r\n",usb->bufferI[z+9]);
    }
    //****************** usb->read ********************
    ////PrintStatusSetup();
    for(i=0,j=1;i<dim;i+=usb->DIMBUF-4)
{
        usb->bufferU[j++]=OW_READ;
        usb->bufferU[j++]=i<dim-(usb->DIMBUF-4)?usb->DIMBUF-4:dim-i;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(37);
        usb->read();
        if(usb->bufferI[1]==OW_READ&&usb->bufferI[2]<0xFA)
{
            for(z=3;z<usb->bufferI[2]+3&&z<usb->DIMBUF;z++) usb->memEE[k++]=usb->bufferI[z];
        }
        //PrintStatus(strings[S_CodeReading2],i*100/(dim),i);	//"usb->read: %d%%, addr. %05X"
        j=1;
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X) \n"
            //WriteLogIO();
        }
    }
    if(options==1)
{		//usb->read status register + application register
        usb->bufferU[j++]=OW_RESET;
        usb->bufferU[j++]=OW_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=SKIP_ROM;
        usb->bufferU[j++]=READ_STAT_REGISTER;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=OW_READ;
        usb->bufferU[j++]=1;
        usb->bufferU[j++]=OW_RESET;
        usb->bufferU[j++]=OW_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=SKIP_ROM;
        usb->bufferU[j++]=READ_APP_REGISTER;
        usb->bufferU[j++]=0;
        usb->bufferU[j++]=OW_READ;
        usb->bufferU[j++]=8;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(15);
        usb->read();
        j=1;
        for(z=1;usb->bufferI[z]!=OW_READ&&z<usb->DIMBUF-2;z++);
        //if(z<usb->DIMBUF-2) //PrintMessage1("Status register: 0x%02X\r\n",usb->bufferI[z+2]);
        for(z+=2;usb->bufferI[z]!=OW_READ&&z<usb->DIMBUF-10;z++);
        //PrintMessage("Application register: 0x");
        //for(i=z+2;i<z+10&&i<usb->DIMBUF;i++)
        //PrintMessage1("%02X",usb->bufferI[i]);
        //PrintMessage("\r\n");
    }
    else if(options==2)
{		//usb->read protection & ID bytes
        usb->bufferU[j++]=OW_READ;
        if(dim==0xA00) usb->bufferU[j++]=0x24;
        else usb->bufferU[j++]=8;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(25);
        usb->read();
        j=1;
        if(usb->bufferI[1]==OW_READ&&usb->bufferI[2]<0xFA)
{
            if(dim==0xA00)
            {
                //for(i=0;i<10;i++) //PrintMessage2("Protection Control Byte Block %d: 0x%02X\r\n",i,usb->bufferI[i+3]);
                //PrintMessage("User EEPROM:\r\n");
                //for(i=0;i<10;i++) //PrintMessage1("%02X",usb->bufferI[i+13]);
                //PrintMessage("\r\n");
                //for(i=0;i<10;i++) //PrintMessage1("%02X",usb->bufferI[i+23]);
                //PrintMessage1("\r\nMemory Block Lock: 0x%02X\r\n",usb->bufferI[33]);
                //PrintMessage1("Register Page Lock: 0x%02X\r\n",usb->bufferI[34]);
                //PrintMessage1("Factory Byte: 0x%02X\r\n",usb->bufferI[35]);
                //PrintMessage2("Factory Trim Bytes: 0x%02X%02X\r\n",usb->bufferI[36],usb->bufferI[37]);
                //PrintMessage2("Manufacturer ID: 0x%02X%02X\r\n",usb->bufferI[38],usb->bufferI[39]);
            }
            else{
                //PrintMessage1("Protection Control Byte Page 0: 0x%02X\r\n",usb->bufferI[3]);
                //PrintMessage1("Protection Control Byte Page 1: 0x%02X\r\n",usb->bufferI[4]);
                //PrintMessage1("Protection Control Byte Page 2: 0x%02X\r\n",usb->bufferI[5]);
                //PrintMessage1("Protection Control Byte Page 3: 0x%02X\r\n",usb->bufferI[6]);
                //PrintMessage1("Copy Protection Byte: 0x%02X\r\n",usb->bufferI[7]);
                //PrintMessage1("Factory Byte: 0x%02X\r\n",usb->bufferI[8]);
                //PrintMessage2("User Bytes/Manufacturer ID: 0x%02X%02X\r\n",usb->bufferI[9],usb->bufferI[10]);
            }
        }
    }
    //PrintStatusEnd();
    if(k!=dim)
{
        //PrintMessage("\r\n");
        //PrintMessage2(strings[S_ReadEEErr],dim,k);	//"Error reading EEPROM area, requested %d bytes, usb->read %d\r\n"
        sizeEE=k;
    }
    //****************** exit ********************
    usb->bufferU[j++]=EN_VPP_VCC;		//turn off
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(1);
    usb->read();
    unsigned int stop=GetTickCount();
    ////PrintStatusClear();
    //DisplayEE();	//visualize
    int sum=0;
    for(i=0;i<sizeEE;i++)
        sum+=usb->memEE[i];
    //PrintMessage1("Checksum: 0x%X\r\n",sum&0xFFFF);
    //PrintMessage1(strings[S_End],(stop-start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"
    //if(usb->saveLog) CloseLogFile();
}



void ProgEEPROM::WriteOneWireMem(int dim,int options)

// usb->Write OneWire memories
// dim=size in bytes
// options:
//          0 = 8 byte scratchpad
//          1 = 32 byte scratchpad
{
    int k=0,z=0,i,j;
    int err=0;
    hvreg=0;
    if(usb->FWVersion<0x800)
{
        //PrintMessage1(strings[S_FWver2old],"0.8.0");	//"This firmware is too old. Version %s is required\r\n"
        return;
    }
    if(dim>0x10000||dim<0)
{
        //PrintMessage(strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(usb->saveLog)
{
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"WriteOneWireMem(%d)    (0x%X)\n",dim,dim);
    }
    if(dim>sizeEE)
{
        i=sizeEE;
        usb->memEE=(unsigned char*)realloc(usb->memEE,dim);
        for(;i<dim;i++) usb->memEE[i]=0xFF;
        sizeEE=dim;
    }
    if(dim<1)
{
        //PrintMessage(strings[S_NoCode]);	//"Data area is empty\r\n"
        return;
    }
    unsigned int start=GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=uW_INIT;	//set RB1=0 to use as GND terminal beside RB0
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=OW_RESET;
    usb->bufferU[j++]=OW_WRITE;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=READ_ROM;
    usb->bufferU[j++]=OW_READ;
    usb->bufferU[j++]=8;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(12);
    usb->read();
    j=1;
    //if(usb->saveLog)WriteLogIO();
    if(usb->bufferI[5]==OW_RESET&&usb->bufferI[6]==0)
{	//no presence pulse
        //PrintMessage(strings[S_ComErr]);		//communication error
        usb->bufferU[j++]=EN_VPP_VCC;		//turn off
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(1);
        usb->read();
        j=1;
        //if(usb->saveLog) CloseLogFile();
        return;
    }
    for(z=1;usb->bufferI[z]!=OW_READ&&z<usb->DIMBUF;z++);
    if(z<usb->DIMBUF-9)
{
        //PrintMessage1("Family code: 0x%02X ",usb->bufferI[z+2]);
        OW_ID(usb->bufferI[z+2]);
        //PrintMessage3("Serial ID: 0x%02X%02X%02X",usb->bufferI[z+3],usb->bufferI[z+4],usb->bufferI[z+5]);
        //PrintMessage3("%02X%02X%02X",usb->bufferI[z+6],usb->bufferI[z+7],usb->bufferI[z+8]);
        //PrintMessage1("\r\nCRC: 0x%02X\r\n",usb->bufferI[z+9]);
    }
    //****************** usb->Write ********************
    //PrintMessage(strings[S_EEAreaW]);	//"usb->Write EEPROM ... "
    int page=options==0?8:32;
    for(i=0,j=1;i<dim;i+=page)
{
        usb->bufferU[j++]=OW_RESET;
        usb->bufferU[j++]=OW_WRITE;
        usb->bufferU[j++]=page+(dim<=32?3:4);
        usb->bufferU[j++]=SKIP_ROM;
        usb->bufferU[j++]=Write_SCRATCHPAD;
        usb->bufferU[j++]=i&0xFF;
        if(dim>32) usb->bufferU[j++]=i>>8;
        for(k=0;k<page;k++) usb->bufferU[j++]=usb->memEE[i+k];
        usb->bufferU[j++]=OW_RESET;
        usb->bufferU[j++]=OW_WRITE;
        usb->bufferU[j++]=dim<=32?3:5;
        usb->bufferU[j++]=SKIP_ROM;
        usb->bufferU[j++]=COPY_SCRATCHPAD;
        if(dim<=32) usb->bufferU[j++]=0xA5;
        else{
            usb->bufferU[j++]=i&0xFF;
            usb->bufferU[j++]=i>>8;
            usb->bufferU[j++]=page-1;
        }
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(6+0.65*page+10);
        usb->read();
        j=1;
        //PrintStatus(strings[S_CodeWriting2],i*100/(dim),i);	//"usb->Write: %d%%, addr. %04X"
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log7],i,i,0,0);	//"i=%d(0x%X), k=%d(0x%X)\n"
            //WriteLogIO();
        }
    }
    //PrintStatusEnd();
    //PrintMessage(strings[S_Compl]);	//"completed\r\n"
    //****************** verify EEPROM ********************
    //PrintMessage(strings[S_EEV]);	//"Verify EEPROM ... "44
    ////PrintStatusSetup();
    k=0;
    usb->bufferU[j++]=OW_RESET;
    usb->bufferU[j++]=OW_WRITE;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=SKIP_ROM;
    if(dim<=32)
{	//1 byte address
        usb->bufferU[j++]=OW_WRITE;
        usb->bufferU[j++]=2;
        usb->bufferU[j++]=READ_MEMORY;
        usb->bufferU[j++]=0;	//address
    }
    else{			//2 byte address
        usb->bufferU[j++]=OW_WRITE;
        usb->bufferU[j++]=3;
        usb->bufferU[j++]=READ_MEMORY;
        usb->bufferU[j++]=0;	//address
        usb->bufferU[j++]=0;	//address
    }
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(10);
    usb->read();
    j=1;
    if(usb->saveLog)
{
        //fprintf(logfile,strings[S_EEV]);	//"Verify EEPROM ... "
        //WriteLogIO();
    }
    for(i=0,j=1;i<dim;i+=usb->DIMBUF-4)
{
        usb->bufferU[j++]=OW_READ;
        usb->bufferU[j++]=i<dim-(usb->DIMBUF-4)?usb->DIMBUF-4:dim-i;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(37);
        usb->read();
        if(usb->bufferI[1]==OW_READ&&usb->bufferI[2]<0xFA)
{
            for(z=3;z<usb->bufferI[2]+3&&z<usb->DIMBUF;z++)
{
                if(usb->memEE[k++]!=usb->bufferI[z])
{
                    //PrintMessage("\r\n");
                    //PrintMessage4(strings[S_CodeVError],i+z-3,i+z-3,usb->memEE[k-1],usb->bufferI[z]);	//"Error verifying address %04X (%d), written %02X, usb->read %02X\r\n"
                    err++;
                }
            }
        }
        //PrintStatus(strings[S_CodeV2],i*100/(dim),i);	//"Verify: %d%%, addr. %04X"
        j=1;
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, err=%d\n"
            //WriteLogIO();
        }
        if(err>=usb->max_err) break;
    }
    //PrintStatusEnd();
    if(k!=dim)
{
        //PrintMessage("\r\n");
        //PrintMessage2(strings[S_ReadEEErr],dim,k);	//"Error reading EEPROM area, requested %d bytes, usb->read %d\r\n"
        sizeEE=k;
    }
    //PrintMessage1(strings[S_ComplErr],err);	//"completed: %d errors\r\n"
    //****************** exit ********************
    usb->bufferU[j++]=EN_VPP_VCC;		//turn off
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(1);
    usb->read();
    unsigned int stop=GetTickCount();
    ////PrintStatusClear();
    //PrintMessage1(strings[S_End],(stop-start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"
    //if(usb->saveLog) CloseLogFile();
}

#define READ_SCRATCHPAD2 0xBE
#define CONVERT_TEMP 0x44
#define RECALL_EE 0xB8
#define READ_PWSUP 0xB4

void ProgEEPROM::ReadDS1820()

// usb->read DS1820 one-wire digital thermometer
{
    int z=0,j;
    double TLSB=0.5;
    hvreg=0;
    if(usb->FWVersion<0x800)
{
        //PrintMessage1(strings[S_FWver2old],"0.8.0");	//"This firmware is too old. Version %s is required\r\n"
        return;
    }
    if(usb->saveLog)
{
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"ReadDS1820()\n");
    }
    unsigned int start=GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=uW_INIT;	//set RB1=0 to use as GND terminal beside RB0
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=OW_RESET;
    usb->bufferU[j++]=OW_WRITE;
    usb->bufferU[j++]=1;
    usb->bufferU[j++]=READ_ROM;
    usb->bufferU[j++]=OW_READ;
    usb->bufferU[j++]=8;
    usb->bufferU[j++]=OW_RESET;
    usb->bufferU[j++]=OW_WRITE;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=SKIP_ROM;
    usb->bufferU[j++]=CONVERT_TEMP;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(16);
    usb->read();
    j=1;
    //if(usb->saveLog)WriteLogIO();
    for(z=1;usb->bufferI[z]!=OW_RESET&&z<usb->DIMBUF;z++);
    if(usb->bufferI[z]==OW_RESET&&usb->bufferI[z+1]==0)
{	//no presence pulse
        //PrintMessage(strings[S_ComErr]);		//communication error
        usb->bufferU[j++]=EN_VPP_VCC;		//turn off
        usb->bufferU[j++]=0x0;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(1);
        usb->read();
        //if(usb->saveLog) CloseLogFile();
        return;
    }
    for(z=1;usb->bufferI[z]!=OW_READ&&z<usb->DIMBUF;z++);
    if(z<usb->DIMBUF-9)
{
        //PrintMessage1("Family code: 0x%02X ",usb->bufferI[z+2]);
        OW_ID(usb->bufferI[z+2]);
        if(usb->bufferI[z+2]==0x10) TLSB=0.5;			//DS1820
        else if(usb->bufferI[z+2]==0x28) TLSB=0.0625;	//DS18B20
        //PrintMessage3("Serial ID: 0x%02X%02X%02X",usb->bufferI[z+3],usb->bufferI[z+4],usb->bufferI[z+5]);
        //PrintMessage3("%02X%02X%02X",usb->bufferI[z+6],usb->bufferI[z+7],usb->bufferI[z+8]);
        //PrintMessage1("\r\nCRC: 0x%02X\r\n",usb->bufferI[z+9]);
    }
    //****************** usb->read ********************
    usb->msDelay(800);
    usb->bufferU[j++]=OW_RESET;
    usb->bufferU[j++]=OW_WRITE;
    usb->bufferU[j++]=2;
    usb->bufferU[j++]=SKIP_ROM;
    usb->bufferU[j++]=READ_SCRATCHPAD2;
    usb->bufferU[j++]=OW_READ;
    usb->bufferU[j++]=8;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(10);
    usb->read();
    j=1;
    //if(usb->saveLog)WriteLogIO();
    for(z=2;usb->bufferI[z]!=OW_READ&&z<usb->DIMBUF-2;z++);
    ////PrintMessage("0x");
    //for(i=z+2;i<z+10&&i<usb->DIMBUF;i++) //PrintMessage1("%02X",usb->bufferI[i]);
    ////PrintMessage("\r\n");
    int T=usb->bufferI[z+2]+(usb->bufferI[z+3]<<8);
    if(T>0xF000) T|=0xFFFF0000; //adjust negative value
#ifdef _MSC_VER
    //PrintMessage2("T=%.4f°C  (0x%04X)\r\n",T*TLSB,T);
#else
    //PrintMessage2("T=%.4fÂ°C  (0x%04X)\r\n",T*TLSB,T);
#endif
    //****************** exit ********************
    usb->bufferU[j++]=EN_VPP_VCC;		//turn off
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(1);
    usb->read();
    unsigned int stop=GetTickCount();
    //PrintMessage1(strings[S_End],(stop-start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"
    //if(usb->saveLog) CloseLogFile();
}
#define UNIO_READ 0x03
#define UNIO_CRRD 0x06
#define UNIO_Write 0x6C
#define UNIO_WREN 0x96
#define UNIO_WRDI 0x91
#define UNIO_RDSR 0x05
#define UNIO_WRSR 0x6E
#define UNIO_ERAL 0x6D
#define UNIO_SETAL 0x67


void ProgEEPROM::Read11xx(int dim)
// usb->read 11xx UNIO memories
// dim=size in bytes
{
    int k=0,z=0,i,j;
    hvreg=0;
    if(usb->FWVersion<0x800)
{
        //PrintMessage1(strings[S_FWver2old],"0.8.0");	//"This firmware is too old. Version %s is required\r\n"
        return;
    }
    if(dim>=0x10000||dim<0)
{
        //PrintMessage(strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(usb->saveLog)
{
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"Read11xx(%d)    (0x%X)\n",dim,dim);
    }
    sizeEE=dim;
    if(usb->memEE) free(usb->memEE);
    usb->memEE=(unsigned char*)malloc(dim);			//EEPROM
    unsigned int start=GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=UNIO_COM;
    usb->bufferU[j++]=2;		//usb->Write x bytes
    usb->bufferU[j++]=1;		//usb->read x bytes
    usb->bufferU[j++]=0xA0;
    usb->bufferU[j++]=UNIO_RDSR;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(5);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    //****************** usb->read ********************
    //PrintMessage(strings[S_ReadEE]);		//usb->read EEPROM ...
    ////PrintStatusSetup();
    for(i=0,j=1;i<dim;i+=usb->DIMBUF-5)
{
        usb->bufferU[j++]=UNIO_COM;
        usb->bufferU[j++]=4;		//usb->Write x bytes
        usb->bufferU[j++]=i<dim-(usb->DIMBUF-5)?usb->DIMBUF-5:dim-i;		//usb->read x bytes
        usb->bufferU[j++]=0xA0;
        usb->bufferU[j++]=UNIO_READ;
        usb->bufferU[j++]=i>>8;
        usb->bufferU[j++]=i&0xFF;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(14);
        usb->read();
        if(usb->bufferI[1]==UNIO_COM&&usb->bufferI[2]<0xFA)
{
            for(z=3;z<usb->bufferI[2]+3&&z<usb->DIMBUF;z++) usb->memEE[k++]=usb->bufferI[z];
        }
        //PrintStatus(strings[S_CodeReading2],i*100/(dim),i);	//"usb->read: %d%%, addr. %05X"
        j=1;
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X) \n"
            //WriteLogIO();
        }
    }
    //PrintStatusEnd();
    if(k!=dim)
{
        //PrintMessage("\r\n");
        //PrintMessage2(strings[S_ReadEEErr],dim,k);	//"Error reading EEPROM area, requested %d bytes, usb->read %d\r\n"
        sizeEE=k;
    }
    else //PrintMessage(strings[S_Compl]);
        //****************** exit ********************
        usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(2);
    usb->read();
    unsigned int stop=GetTickCount();
    ////PrintStatusClear();
    //DisplayEE();	//visualize
    int sum=0;
    for(i=0;i<sizeEE;i++) sum+=usb->memEE[i];
    //PrintMessage1("Checksum: 0x%X\r\n",sum&0xFFFF);
    //PrintMessage1(strings[S_End],(stop-start)/1000.0);	//"\r\nEnd (%.2f s)\r\n"
    //if(usb->saveLog) CloseLogFile();
}


void ProgEEPROM::Write11xx(int dim,int page)
// usb->Write 11xx UNIO memories
// dim=size in bytes
// page=page size
{
    int k=0,z=0,i,j;
    int err=0;
    if(usb->FWVersion<0x800)
{
        //PrintMessage1(strings[S_FWver2old],"0.8.0");	//"This firmware is too old. Version %s is required\r\n"
        return;
    }
    hvreg=0;
    if(dim>=0x10000||dim<0)
{
        //PrintMessage(strings[S_EELim]);	//"EEPROM size out of limits\r\n"
        return;
    }
    if(usb->saveLog)
{
        //OpenLogFile();	//"Log.txt"
        //fprintf(logfile,"Write11xx(%d,%d)    (0x%X,0x%X)\n",dim,page,dim,page);
    }
    if(dim>sizeEE)
{
        i=sizeEE;
        usb->memEE=(unsigned char*)realloc(usb->memEE,dim);
        for(;i<dim;i++) usb->memEE[i]=0xFF;
        sizeEE=dim;
    }
    if(dim<1)
{
        //PrintMessage(strings[S_NoCode]);	//"Data area is empty\r\n"
        return;
    }
    unsigned int start=GetTickCount();
    usb->bufferU[0]=0;
    j=1;
    usb->bufferU[j++]=VREG_DIS;
    usb->bufferU[j++]=EN_VPP_VCC;		//VDD
    usb->bufferU[j++]=0x1;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=UNIO_STBY;
    usb->bufferU[j++]=UNIO_COM;
    usb->bufferU[j++]=2;		//usb->Write x bytes
    usb->bufferU[j++]=0;		//usb->read x bytes
    usb->bufferU[j++]=0xA0;
    usb->bufferU[j++]=UNIO_WREN;
    usb->bufferU[j++]=UNIO_COM;
    usb->bufferU[j++]=2;		//usb->Write x bytes
    usb->bufferU[j++]=1;		//usb->read x bytes
    usb->bufferU[j++]=0xA0;
    usb->bufferU[j++]=UNIO_RDSR;
    usb->bufferU[j++]=UNIO_COM;
    usb->bufferU[j++]=3;		//usb->Write x bytes
    usb->bufferU[j++]=0;		//usb->read x bytes
    usb->bufferU[j++]=0xA0;
    usb->bufferU[j++]=UNIO_WRSR;
    usb->bufferU[j++]=0;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=WAIT_T3;
    usb->bufferU[j++]=UNIO_COM;
    usb->bufferU[j++]=2;		//usb->Write x bytes
    usb->bufferU[j++]=1;		//usb->read x bytes
    usb->bufferU[j++]=0xA0;
    usb->bufferU[j++]=UNIO_RDSR;
    usb->bufferU[j++]=UNIO_COM;
    usb->bufferU[j++]=2;		//usb->Write x bytes
    usb->bufferU[j++]=0;		//usb->read x bytes
    usb->bufferU[j++]=0xA0;
    usb->bufferU[j++]=UNIO_WREN;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(25);
    usb->read();
    //if(usb->saveLog)WriteLogIO();
    //****************** usb->Write ********************
    //PrintMessage(strings[S_EEAreaW]);	//"usb->Write EEPROM ... "
    ////PrintStatusSetup();
    for(;page>=usb->DIMBUF-8;page>>=1);
    for(i=0,j=1;i<dim;i+=page)
{
        usb->bufferU[j++]=UNIO_COM;
        usb->bufferU[j++]=4+page;	//usb->Write x bytes
        usb->bufferU[j++]=0;			//usb->read x bytes
        usb->bufferU[j++]=0xA0;
        usb->bufferU[j++]=UNIO_Write;
        usb->bufferU[j++]=i>>8;
        usb->bufferU[j++]=i&0xFF;
        for(k=0;k<page;k++) usb->bufferU[j++]=usb->memEE[i+k];
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay((5+page)*0.2+2);
        usb->read();
        if(usb->bufferI[1]!=UNIO_COM||usb->bufferI[2]>=0xFA) i=dim+10;
        //PrintStatus(strings[S_CodeWriting2],i*100/(dim),i);	//"usb->Write: %d%%, addr. %04X"
        j=1;
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log7],i,i,k,k);	//"i=%d(0x%X), k=%d(0x%X)\n"
            //WriteLogIO();
        }
        usb->bufferU[j++]=UNIO_COM;
        usb->bufferU[j++]=2;		//usb->Write x bytes
        usb->bufferU[j++]=1;		//usb->read x bytes
        usb->bufferU[j++]=0xA0;
        usb->bufferU[j++]=UNIO_RDSR;
        usb->bufferU[j++]=UNIO_COM;
        usb->bufferU[j++]=2;		//usb->Write x bytes
        usb->bufferU[j++]=0;		//usb->read x bytes
        usb->bufferU[j++]=0xA0;
        usb->bufferU[j++]=UNIO_WREN;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        int status=1;
        for(j=0;status&&j<20;j++)
{	//status polling until usb->Write complete
            usb->Write();
            usb->msDelay(2);
            usb->read();
            status=usb->bufferI[3];
            //if(usb->saveLog)WriteLogIO();
        }
        j=1;
    }
    //PrintStatusEnd();
    //PrintMessage(strings[S_Compl]);	//"completed\r\n"
    //****************** verify EEPROM ********************
    //PrintMessage(strings[S_EEV]);	//"Verify EEPROM ... "
    ////PrintStatusSetup();
    k=0;
    for(i=0,j=1;i<dim;i+=usb->DIMBUF-4)
{
        usb->bufferU[j++]=UNIO_COM;
        usb->bufferU[j++]=4;		//usb->Write x bytes
        usb->bufferU[j++]=i<dim-(usb->DIMBUF-4)?usb->DIMBUF-4:dim-i;		//usb->read x bytes
        usb->bufferU[j++]=0xA0;
        usb->bufferU[j++]=UNIO_READ;
        usb->bufferU[j++]=i>>8;
        usb->bufferU[j++]=i&0xFF;
        usb->bufferU[j++]=FLUSH;
        for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
        usb->Write();
        usb->msDelay(14);
        usb->read();
        if(usb->bufferI[1]==UNIO_COM&&usb->bufferI[2]<0xFA)
{
            for(z=3;z<usb->bufferI[2]+3&&z<usb->DIMBUF;z++)
{
                if(usb->memEE[k++]!=usb->bufferI[z])
{
                    //PrintMessage("\r\n");
                    //PrintMessage4(strings[S_CodeVError],i+z-3,i+z-3,usb->memEE[k-1],usb->bufferI[z]);	//"Error verifying address %04X (%d), written %02X, usb->read %02X\r\n"
                    err++;
                }
            }
        }
        //PrintStatus(strings[S_CodeV2],i*100/(dim),i);	//"Verify: %d%%, addr. %04X"
        j=1;
        if(usb->saveLog)
{
            //fprintf(logfile,strings[S_Log8],i,i,k,k,err);	//"i=%d, k=%d, err=%d\n"
            //WriteLogIO();
        }
        if(err>=usb->max_err) break;
    }
    //PrintStatusEnd();
    if(k!=dim)
{
        //PrintMessage("\r\n");
        //PrintMessage2(strings[S_ReadEEErr],dim,k);	//"Error reading EEPROM area, requested %d bytes, usb->read %d\r\n"
    }
    //PrintMessage1(strings[S_ComplErr],err);	//"completed: %d errors\r\n"
    //****************** exit ********************
    usb->bufferU[j++]=EN_VPP_VCC;
    usb->bufferU[j++]=0x0;
    usb->bufferU[j++]=FLUSH;
    for(;j<usb->DIMBUF;j++) usb->bufferU[j]=0x0;
    usb->Write();
    usb->msDelay(2);
    usb->read();
    unsigned int stop=GetTickCount();
    ////PrintStatusClear();
    //PrintMessage3(strings[S_EndErr],(stop-start)/1000.0,err,err!=1?strings[S_ErrPlur]:strings[S_ErrSing]);	//"\r\nEnd (%.2f s) %d %s\r\n\r\n"
    //if(usb->saveLog) CloseLogFile();
}
