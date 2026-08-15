#include "fileIO.h"
#include "Libs/globalVariables.h"
#include "QByteArray"
using namespace std;
FileIO::FileIO(QObject *parent) :
   QObject(parent)
{
   usb=GlobalVariables::usb;
}

int FileIO::htoi(const char *hex, int length)
{
   int i;
   unsigned int v = 0;
   for (i = 0; i < length; i++)
   {
      v <<= 4;
      if (hex[i] >= '0' && hex[i] <= '9') v += hex[i] - '0';
      else if (hex[i] >= 'a' && hex[i] <= 'f') v += hex[i] - 'a' + 10;
      else if (hex[i] >= 'A' && hex[i] <= 'F') v += hex[i] - 'A' + 10;
      else
         CommonFunctions::printMessage1(GlobalVariables::strings[S_Inohex],hex);	//"Error: '%.4s' doesn't look very hexadecimal, right?\n"
   }
   return v;
}

int FileIO::load(DeviceInfo *dev, QString loadfile)
{
   QByteArray ba1 = dev->name.toLatin1();
   char *c_dev = ba1.data();

   QByteArray ba2 = loadfile.toLatin1();
   char *c_loadfile = ba2.data();

   load(dev, c_dev, c_loadfile);
}

int FileIO::load(DeviceInfo *deviceInfo, char* dev, char*loadfile)
{

   int i,input_address=0,ext_addr=0,sum,valid,empty;
   char s[256]="",line[256];
   FILE* f=fopen(loadfile,"r");
   if(!f) return -1;
   CommonFunctions::printMessage1("%s :\r\n\r\n",loadfile);
   //**************** 10-16F *******************************************
   if(!strncmp(dev,"10",2)||!strncmp(dev,"12",2)||!strncmp(dev,"16",2))
   {
      unsigned char buffer[0x20000],bufferEE[0x1000];
      //unsigned char buffer[usb->sizeW],bufferEE[usb->sizeEE];
      int sizeM=0;
      usb->sizeW=0;
      memset(buffer,0xFF,sizeof(buffer));
      memset(bufferEE,0xFF,sizeof(bufferEE));
      //usb->sizeEE=0;
      for(;fgets(line,256,f);)
      {
         if(strlen(line)>9&&line[0]==':')
         {
            int hex_count = htoi(line+1, 2);
            if((int)strlen(line)-11<hex_count*2)
            {
               CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
            }
            else
            {
               input_address=htoi(line+3,4);
               sum=0;
               for (i=1;i<=hex_count*2+9;i+=2)
                  sum += htoi(line+i,2);
               if ((sum & 0xff)!=0)
               {
                  CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
               }
               else
               {
                  switch(htoi(line+7,2))
                  {
                     case 0:		//Data record
                        if(ext_addr<=0x01&&input_address<0xE000)
                        {		//Code
                           sizeM=(ext_addr<<16)+input_address+hex_count;
                           if(sizeM>usb->sizeW)
                              usb->sizeW=sizeM;
                           for (i=0;i<hex_count;i++)
                           {
                              buffer[(ext_addr<<16)+input_address+i]=htoi(line+9+i*2,2);
                           }
                        }
                        else if(ext_addr==0x0&&input_address>=0xE000&&input_address<0xF000)
                        {	//EEPROM
                           sizeM=(input_address-0xE000+hex_count)/2;
                           if(sizeM>usb->sizeEE) usb->sizeEE=sizeM;
                           for (i=0;i<hex_count;i+=2)
                           {
                              bufferEE[(input_address-0xE000)/2+i/2]=htoi(line+9+i*2,2);
                           }
                        }
                        break;
                     case 4:		//extended linear address record
                        if(strlen(line)>14)
                           ext_addr=htoi(line+9,4);
                        break;
                     default:
                        break;
                  }
               }
            }
         }
      }

      int sizeW=usb->sizeW/2;
      usb->sizeW/=2;
      //int sizeW=((ext_addr<<16)+input_address+i)/2;
      if(usb->memCODE_W)
         free(usb->memCODE_W);
      usb->memCODE_W=(WORD*)malloc(sizeof(WORD)*sizeW);
      for(i=0;i<sizeW;i++)
         //Swap bytes
         usb->memCODE_W[i]=(buffer[i*2+1]<<8)+buffer[i*2];

      if(deviceInfo->picInfo->memoriesInfo.config)
         deviceInfo->picInfo->memoriesInfo.config->loadFromMemCODE_W(usb->memCONFIG);
      if(deviceInfo->picInfo->memoriesInfo.userIDs)
         deviceInfo->picInfo->memoriesInfo.userIDs->loadFromMemCODE_W(usb->memID);

      if(usb->memEE)
         free(usb->memEE);
      if(usb->sizeEE)
      {
         usb->memEE=(unsigned char*)malloc(usb->sizeEE);
         memcpy(usb->memEE,bufferEE,usb->sizeEE);
      }
      else
         usb->memEE=0;
      CommonFunctions::printMessage(GlobalVariables::strings[S_CodeMem]);	//"\r\nCode memory:\r\n"
      s[0]=0;
      empty=1;
      int imax=sizeW>0x8000?0x8500:0x2100;
      ////DisplayCODE16F(imax);
      if(sizeW>=0x2100&&sizeW<0x3000)
      {	//EEPROM@0x2100
         CommonFunctions::printMessage(GlobalVariables::strings[S_EEMem]);	//"\r\nEEPROM memory:\r\n"
         ////DisplayEE16F(0x700);
      }
      //else if(usb->sizeEE) DisplayEE();
      CommonFunctions::printMessage("\r\n");
   }
   //**************** 18F *******************************************
   else if(!strncmp(dev,"18F",3))
   {
      unsigned char buffer[0x30000],bufferEE[0x1000];
      int sizeM;
      memset(buffer,0xFF,sizeof(buffer));
      memset(bufferEE,0xFF,sizeof(bufferEE));
      memset(usb->memID,0xFF,sizeof(usb->memID));
      memset(usb->memCONFIG,0xFF,sizeof(usb->memCONFIG));
      for(;fgets(line,256,f);)
      {
         if(strlen(line)>9&&line[0]==':')
         {
            int hex_count = htoi(line+1, 2);
            if((int)strlen(line) - 11 < hex_count * 2)
            {
               CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
            }
            else
            {
               input_address=htoi(line+3,4);
               sum=0;
               for (i=1;i<=hex_count*2+9;i+=2)
                  sum += htoi(line+i,2);
               if ((sum & 0xff)!=0)
               {
                  CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
               }
               else
               {
                  switch(htoi(line+7,2))
                  {
                     case 0:		//Data record
                        if(ext_addr<0x20)
                        {		//Code
                           sizeM=(ext_addr<<16)+input_address+hex_count;
                           if(sizeM > usb->size)
                              usb->size=sizeM;
                           for (i=0;i<hex_count;i++)
                           {
                              buffer[(ext_addr<<16)+input_address+i]=htoi(line+9+i*2,2);
                           }
                        }
                        else if(ext_addr==0x20&&input_address<8)
                        {	//ID
                           for (i=0;i<hex_count;i++)
                           {
                              usb->memID[input_address+i]=htoi(line+9+i*2,2);
                           }
                        }
                        else if(ext_addr==0x30&&input_address<14)
                        {	//CONFIG
                           for (i=0;i<hex_count;i++)
                           {
                              usb->memCONFIG[input_address+i]=htoi(line+9+i*2,2);
                           }
                        }
                        else if(ext_addr==0xF0&&input_address<0x1000)
                        {	//EEPROM
                           for (i=0;i<hex_count;i++)
                           {
                              bufferEE[input_address+i]=htoi(line+9+i*2,2);
                           }
                           sizeM=input_address+hex_count;
                           if(sizeM>usb->sizeEE) usb->sizeEE=sizeM;

                        }
                        break;
                     case 4:		//extended linear address record
                        if(strlen(line)>14)
                           ext_addr=htoi(line+9,4);
                        break;
                     default:
                        break;
                  }
               }
            }
         }
      }
      if(usb->memCODE)
         free(usb->memCODE);
      usb->memCODE=(unsigned char*)malloc(usb->size);
      memcpy(usb->memCODE,buffer,usb->size);
      if(usb->memEE)
         free(usb->memEE);
      usb->memEE=(unsigned char*)malloc(usb->sizeEE);
      memcpy(usb->memEE,bufferEE,usb->sizeEE);
      CommonFunctions::printMessage(GlobalVariables::strings[S_IDMem]);	//"ID memory:\r\n"
      for(i=0;i<8;i+=2)
         CommonFunctions::printMessage4("ID%d: 0x%02X   ID%d: 0x%02X\r\n",i,usb->memID[i],i+1,usb->memID[i+1]);
      CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigMem]);	//"CONFIG memory:\r\n"
      for(i=0;i<7;i++)
      {
         CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWordH],i+1,usb->memCONFIG[i*2+1]);	//"CONFIG%dH: 0x%02X\t"
         CommonFunctions::printMessage2(GlobalVariables::strings[S_ConfigWordL],i+1,usb->memCONFIG[i*2]);	//"CONFIG%dL: 0x%02X\r\n"
      }
      CommonFunctions::printMessage(GlobalVariables::strings[S_CodeMem]);	//"\r\nCODE memory:\r\n"
      //DisplayCODE18F(usb->size);
      if(usb->sizeEE) //DisplayEE();
         CommonFunctions::printMessage("\r\n");
   }
   //**************** 24F *******************************************
   else if(!strncmp(dev,"24F",3)||!strncmp(dev,"24H",3)||!strncmp(dev,"24E",3)||!strncmp(dev,"30F",3)||!strncmp(dev,"33F",3)||!strncmp(dev,"33E",3))
   {
      unsigned char *buffer,bufferEE[0x2000];
      int d;
      buffer=(unsigned char*)malloc(0x100000);
      memset(buffer,0xFF,0x100000);
      memset(bufferEE,0xFF,sizeof(bufferEE));
      memset(usb->memCONFIG,0xFF,sizeof(usb->memCONFIG));
      memset(usb->memUSERID,0xFF,sizeof(usb->memUSERID));
      usb->sizeUSERID=0;
      for(;fgets(line,256,f);)
      {
         if(strlen(line)>9&&line[0]==':')
         {
            int hex_count = htoi(line+1, 2);
            if((int)strlen(line) - 11 < hex_count * 2)
            {
               CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
            }
            else
            {
               input_address=htoi(line+3,4);
               sum=0;
               for (i=1;i<=hex_count*2+9;i+=2)
                  sum += htoi(line+i,2);
               if ((sum & 0xff)!=0)
               {
                  CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
               }
               else
               {
                  switch(htoi(line+7,2))
                  {
                     case 0:		//Data record
                        if(ext_addr<0x20)
                        {		//Code
                           int end1=(ext_addr<<16)+input_address+hex_count;
                           if(usb->size<end1) usb->size=end1;
                           for (i=0;i<hex_count;i++)
                           {
                              buffer[(ext_addr<<16)+input_address+i]=htoi(line+9+i*2,2);
                           }
                        }
                        else if(ext_addr==0x1F0&&input_address<48)
                        {	//CONFIG
                           usb->sizeCONFIG=input_address+hex_count;
                           for (i=0;i<hex_count;i++)
                           {
                              usb->memCONFIG[input_address+i]=htoi(line+9+i*2,2);
                           }
                        }
                        else if(ext_addr==0xFF&&input_address>=0xE000)
                        {	//EEPROM
                           for (i=0;i<hex_count;i++)
                           {
                              bufferEE[input_address-0xE000+i]=htoi(line+9+i*2,2);
                           }
                           usb->sizeEE=input_address-0xE000+hex_count;
                        }
                        else if(ext_addr==0x100&&input_address<8)
                        {	//USER ID
                           usb->sizeUSERID=input_address+hex_count;
                           for (i=0;i<hex_count&&(i+input_address)<8;i++)
                           {
                              usb->memUSERID[input_address+i]=htoi(line+9+i*2,2);
                           }
                        }
                        break;
                     case 4:		//extended linear address record
                        if(strlen(line)>14)	ext_addr=htoi(line+9,4);
                        break;
                     default:
                        break;
                  }
               }
            }
         }
      }
      if(usb->memCODE)
         free(usb->memCODE);
      usb->memCODE=(unsigned char*)malloc(usb->size);
      memcpy(usb->memCODE,buffer,usb->size);
      free(buffer);
      usb->sizeEE=usb->sizeEE?0x1000:0;
      if(usb->memEE)
         free(usb->memEE);
      usb->memEE=(unsigned char*)malloc(usb->sizeEE);
      for(i=0;i<usb->sizeEE;i+=2)
      {		//skip voids in the hex file organization
         usb->memEE[i]=bufferEE[i*2]; 	//0 1 4 5 8 9 12 13 ...
         usb->memEE[i+1]=bufferEE[i*2+1];
      }
      for(i=valid=0;i<48;i++) if(usb->memCONFIG[i]<0xFF) valid=1;
      if(valid)
      {
         CommonFunctions::printMessage(GlobalVariables::strings[S_ConfigMem]);				//"\r\nCONFIG memory:\r\n"
         for(i=0;i<48;i+=4)
         {
            d=(usb->memCONFIG[i+1]<<8)+usb->memCONFIG[i];
            if(i<36||d<0xFFFF)CommonFunctions::printMessage2("0xF800%02X: 0x%04X\r\n",i/2,d);
         }
      }
      if(usb->size) CommonFunctions::printMessage(GlobalVariables::strings[S_CodeMem]);	//"\r\nCODE memory:\r\n"
      //DisplayCODE24F(usb->size);
      if(usb->sizeEE)
      {			//show eeprom with address offset by 0x7FF000
         CommonFunctions::printMessage(GlobalVariables::strings[S_EEMem]);	//"\r\nEEPROM memory:\r\n"
         //DisplayEE24F();
      }
      CommonFunctions::printMessage("\r\n");
   }
   //**************** ATxxxx *******************************************
   else if(!strncmp(dev,"AT",2))
   {
      unsigned char buffer[0x30000];
      memset(buffer,0xFF,sizeof(buffer));
      for(;fgets(line,256,f);)
      {
         if(strlen(line)>9&&line[0]==':')
         {
            int hex_count = htoi(line+1, 2);
            if((int)strlen(line) - 11 < hex_count * 2)
            {
               CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
            }
            else
            {
               input_address=htoi(line+3,4);
               sum=0;
               for (i=1;i<=hex_count*2+9;i+=2)
                  sum += htoi(line+i,2);
               if ((sum & 0xff)!=0)
               {
                  CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
               }
               else
               {
                  switch(htoi(line+7,2))
                  {
                     case 0:		//Data record
                        if(ext_addr<0x20)
                        {		//Code
                           usb->size=input_address+hex_count;
                           for (i=0;i<hex_count;i++)
                           {
                              buffer[(ext_addr<<16)+input_address+i]=htoi(line+9+i*2,2);
                           }
                        }
                        break;
                     case 4:		//extended linear address record
                        if(strlen(line)>14)	ext_addr=htoi(line+9,4);
                        break;
                     default:
                        break;
                  }
               }
            }
         }
      }
      if(usb->memCODE)
         free(usb->memCODE);
      usb->memCODE=(unsigned char*)malloc(usb->size);
      memcpy(usb->memCODE,buffer,usb->size);
      if(usb->size)
         CommonFunctions::printMessage(GlobalVariables::strings[S_CodeMem]);	//"\r\nmemoria CODICE:\r\n"
      //DisplayCODEAVR(usb->size);
      CommonFunctions::printMessage("\r\n");
   }
   //**************** 24xxx / 93xxx / 25xxx / 95xxx / DSxxxx /11xxx *******************************
   else if(!strncmp(dev,"24",2)||!strncmp(dev,"93",2)||!strncmp(dev,"25",2)||!strncmp(dev,"95",2)||\
           !strncmp(dev,"DS",2)||!strncmp(dev,"11",2))
   {
      if(strstr(loadfile,".bin")||strstr(loadfile,".BIN"))
      {
#ifdef _WIN32
         //brain-damaged op. systems need this to avoid messing with some bytes
         f=freopen(loadfile,"rb",f);
         if(!f) return -1;
#endif
         fseek(f, 0L, SEEK_END);
         usb->sizeEE=ftell(f);
         fseek(f, 0L, SEEK_SET);
         if(usb->sizeEE>0x1000000) usb->sizeEE=0x1000000;	//max 16MB
         if(usb->memEE)
            free(usb->memEE);
         usb->memEE=(unsigned char*)malloc(usb->sizeEE);
         usb->sizeEE=fread(usb->memEE,1,usb->sizeEE,f);
      }
      else
      {			//Hex file
         int bufSize=0x40000;	//256K
         unsigned char *bufferEE=(unsigned char*)malloc(bufSize);
         memset(bufferEE,0xFF,bufSize);
         for(;fgets(line,256,f);)
         {
            if(strlen(line)>9&&line[0]==':')
            {
               int hex_count = htoi(line+1, 2);
               if((int)strlen(line) - 11 < hex_count * 2)
               {
                  CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
               }
               else
               {
                  input_address=htoi(line+3,4);
                  sum=0;
                  int end1;
                  for (i=1;i<=hex_count*2+9;i+=2) sum+=htoi(line+i,2);
                  if ((sum & 0xff)!=0)
                  {
                     CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
                  }
                  else
                  {
                     switch(htoi(line+7,2))
                     {
                        case 0:		//Data record
                           end1=(ext_addr<<16)+input_address+hex_count;
                           if(end1>=0x1000000) break; //max 16MB
                           if(usb->sizeEE<end1)
                           {			//grow array
                              usb->sizeEE=end1;
                           }
                           if(bufSize<=end1)
                           {			//grow buffer
                              int newsize=(end1&0xFFFC0000)+0x40000;
                              bufferEE=(unsigned char*)realloc(bufferEE,newsize);
                              memset(bufferEE+bufSize,0xFF,newsize-bufSize);
                              bufSize=newsize;
                           }
                           for (i=0;i<hex_count;i++)
                           {
                              bufferEE[(ext_addr<<16)+input_address+i]=htoi(line+9+i*2,2);
                           }
                           break;
                        case 4:		//extended linear address record
                           if(strlen(line)>14)	ext_addr=htoi(line+9,4);
                           break;
                        default:
                           break;
                     }
                  }
               }
            }
         }
         if(usb->memEE)
            free(usb->memEE);
         usb->memEE=(unsigned char*)malloc(usb->sizeEE);
         memcpy(usb->memEE,bufferEE,usb->sizeEE);
         free(bufferEE);
      }
      //DisplayEE();	//visualize
      int sum=0;
      for(i=0;i<usb->sizeEE;i++) sum+=usb->memEE[i];
      CommonFunctions::printMessage1("Checksum: 0x%X\r\n",sum&0xFFFF);
      CommonFunctions::printMessage("\r\n");
   }
   fclose(f);
   return 0;
}


void FileIO::saveEE(char* dev,char* savefile)
{
   FILE* f=fopen(savefile,"w");
   if(!f)
      return;
   //**************** ATMEL *******************************************
   if(!strncmp(dev,"AT",2))
   {
      char str[512],str1[512]="";
      int i,base;
      fprintf(f,":020000040000FA\n");			//extended address=0
      int sum=0,count=0,s;
      for(i=0,count=sum=0;i<usb->sizeEE;i++)
      {
         sum+=usb->memEE[i];
         sprintf(str,"%02X",usb->memEE[i]&0xff);
         strcat(str1,str);
         count++;
         if(count==16||i==usb->sizeEE-1)
         {
            base=i-count+1;
            for(s=i;s>i-count&&usb->memEE[s]>=0xff;s--)
            {	//remove trailing 0xFF
               sum-=usb->memEE[s]&0xff;
               str1[strlen(str1)-2]=0;
            }
            count-=i-s;
            sum+=count+(base&0xff)+((base>>8)&0xff);
            if(count)
            {
               fprintf(f,":%02X%04X00%s%02X\n",count,base&0xFFFF,str1,(-sum)&0xff);
            }
            str1[0]=0;
            count=sum=0;
         }
      }
      fprintf(f,":00000001FF\n");
   }
   if(f)
      fclose(f);
}

int FileIO::save(DeviceInfo *deviceInfo, QString loadfile)
{
   QByteArray ba1 = deviceInfo->name.toLatin1();
   char *c_dev = ba1.data();

   QByteArray ba2=loadfile.toLatin1();
   char *c_loadfile = ba2.data();

   save(deviceInfo, c_dev, c_loadfile);
}



void FileIO::saveToMemCODE_W(const Memory &memory, unsigned char *mem)
{
   if(memory.isValid)
   {
      for(quint16 i=memory.start;i<=memory.end;i++)
      {
         quint16 index =i-memory.start;
         usb->memCODE_W[i] = (mem[index*2+1]<<8) + mem[index*2];
      }
   }
}

void FileIO::loadFromMemCODE_W(const Memory &memory, unsigned char *mem)
{
   if(memory.isValid)
   {
      for(quint16 i=memory.start;i<=memory.end;i++)
      {
         quint16 index=i-memory.start;
         mem[index*2]   =  usb->memCODE_W[i]     & 0x00FF;
         mem[index*2+1] = (usb->memCODE_W[i]>>8) & 0x00FF;
      }
   }
}

void FileIO::save(DeviceInfo *deviceInfo, char* dev, char* savefile)
{
   FILE* f=fopen(savefile,"w");
   if(!f)
      return;
   char str[512],str1[512]="";
   int i,sum=0,count=0,ext=0,s,base;
   //**************** 10-16F *******************************************
   if(!strncmp(dev,"10",2)||!strncmp(dev,"12",2)||!strncmp(dev,"16",2))
   {
      int x=0xfff,addr;
      if(!strncmp(dev,"16",2)||!strncmp(dev,"12F6",4))
         x=0x3fff;
      fprintf(f,":020000040000FA\n");			//extended address=0
      quint16 start=0x2007;
      quint16 end=0x2007;

      if(deviceInfo->picInfo->memoriesInfo.config)
         deviceInfo->picInfo->memoriesInfo.config->saveToMemCODE_W(usb->memCONFIG);
      if(deviceInfo->picInfo->memoriesInfo.userIDs)
         deviceInfo->picInfo->memoriesInfo.config->saveToMemCODE_W(usb->memID);
      //saveToMemCODE_W(deviceInfo->picInfo->memoriesInfo.config, usb->memCONFIG);
      //saveToMemCODE_W(deviceInfo->picInfo->memoriesInfo.userIDs, usb->memID);

      for(i=0;i<usb->sizeW;i++)
         usb->memCODE_W[i]&=x;



      for(i=0;i<usb->sizeW && usb->memCODE_W[i]>=x;i++); //remove leading 0xFFF
      for(;i<usb->sizeW;i++)
      {
         sum+=(usb->memCODE_W[i]>>8)+(usb->memCODE_W[i]&0xff);
         sprintf(str,"%02X%02X",usb->memCODE_W[i]&0xff,usb->memCODE_W[i]>>8);
         strcat(str1,str);
         count++;
         if(count==8||i==usb->sizeW-1)
         {
            base=i-count+1;
            for(s=i;s>=base&&usb->memCODE_W[s]>=x;s--)
            {	//remove trailing 0xFFF
               sum-=(usb->memCODE_W[s]>>8)+(usb->memCODE_W[s]&0xff);
               str1[strlen(str1)-4]=0;
            }
            count-=i-s;
            addr=(s-count+1)*2;
            sum+=count*2+(addr&0xff)+(addr>>8);
            if(base>>15>ext)
            {
               ext=base>>15;
               fprintf(f,":02000004%04X%02X\n",ext,(-6-ext)&0xff);
            }
            if(count)
               fprintf(f,":%02X%04X00%s%02X\n",count*2,addr&0xFFFF,str1,(-sum)&0xff);
            str1[0]=0;
            count=sum=0;
         }
      }
      if(usb->sizeEE)
      {		//this is only for 16F1xxx
         if(ext!=0x01) fprintf(f,":020000040001F9\n");
         for(i=0,count=sum=0;i<usb->sizeEE;i++)
         {
            sum+=usb->memEE[i];
            sprintf(str,"%02X00",usb->memEE[i]&0xff);
            strcat(str1,str);
            count++;
            if(count==8||i==usb->sizeEE-1)
            {
               for(s=i;s>i-count&&usb->memEE[s]>=0xff;s--)
               {	//remove trailing 0xFF
                  sum-=usb->memEE[s]&0xff;
                  str1[strlen(str1)-4]=0;
               }
               count-=i-s;
               addr=(s-count+1)*2+0xE000;
               sum+=count*2+(addr&0xff)+(addr>>8);
               if(count)
               {
                  fprintf(f,":%02X%04X00%s%02X\n",count*2,addr,str1,(-sum)&0xff);
               }
               str1[0]=0;
               count=sum=0;
            }
         }
      }
      fprintf(f,":00000001FF\n");
   }
   //**************** 18F *******************************************
   else if(!strncmp(dev,"18F",3))
   {
      fprintf(f,":020000040000FA\n");			//extended address=0
      for(i=0;i<usb->size&&usb->memCODE[i]==0xff;i++); //remove leading 0xFF
      for(;i<usb->size;i++)
      {
         sum+=usb->memCODE[i];
         sprintf(str,"%02X",usb->memCODE[i]);
         strcat(str1,str);
         count++;
         if(count==16||i==usb->size-1)
         {
            base=i-count+1;
            for(s=i;s>=base&&usb->memCODE[s]==0xff;s--)
            {	//remove trailing 0xFF
               sum-=usb->memCODE[s];
               str1[strlen(str1)-2]=0;
            }
            count-=i-s;
            sum+=count+(base&0xff)+((base>>8)&0xff);
            if(base>>16>ext)
            {
               ext=base>>16;
               fprintf(f,":02000004%04X%02X\n",ext,(-6-ext)&0xff);
            }
            if(count)
            {
               fprintf(f,":%02X%04X00%s%02X\n",count,base&0xFFFF,str1,(-sum)&0xff);
            }
            str1[0]=0;
            count=sum=0;
         }
      }
      for(i=0,count=sum=0;i<8;i++)
      {
         sum+=usb->memID[i];
         sprintf(str,"%02X",usb->memID[i]&0xff);
         strcat(str1,str);
         count++;
         if(count==8)
         {
            fprintf(f,":020000040020DA\n");
            base=i-count+1;
            for(s=i;s>i-count&&usb->memID[s]>=0xff;s--)
            {	//remove trailing 0xFF
               sum-=usb->memID[s]&0xff;
               str1[strlen(str1)-2]=0;
            }
            count-=i-s;
            sum+=count+(base&0xff)+((base>>8)&0xff);
            if(count)
            {
               fprintf(f,":%02X%04X00%s%02X\n",count,base&0xFFFF,str1,(-sum)&0xff);
            }
            str1[0]=0;
            count=sum=0;
         }
      }
      for(i=0,count=sum=0;i<14;i++)
      {
         sum+=usb->memCONFIG[i];
         sprintf(str,"%02X",usb->memCONFIG[i]&0xff);
         strcat(str1,str);
         count++;
         if(count==14)
         {
            fprintf(f,":020000040030CA\n");
            base=i-count+1;
            for(s=i;s>i-count&&usb->memCONFIG[s]>=0xff;s--)
            {	//remove trailing 0xFF
               sum-=usb->memCONFIG[s]&0xff;
               str1[strlen(str1)-2]=0;
            }
            count-=i-s;
            sum+=count+(base&0xff)+((base>>8)&0xff);
            if(count)
            {
               fprintf(f,":%02X%04X00%s%02X\n",count,base&0xFFFF,str1,(-sum)&0xff);
            }
            str1[0]=0;
            count=sum=0;
         }
      }
      if(usb->sizeEE)
      {
         fprintf(f,":0200000400F00A\n");
         for(i=0,count=sum=0;i<usb->sizeEE;i++)
         {
            sum+=usb->memEE[i];
            sprintf(str,"%02X",usb->memEE[i]&0xff);
            strcat(str1,str);
            count++;
            if(count==16||i==usb->sizeEE-1)
            {
               base=i-count+1;
               for(s=i;s>i-count&&usb->memEE[s]>=0xff;s--)
               {	//remove trailing 0xFF
                  sum-=usb->memEE[s]&0xff;
                  str1[strlen(str1)-2]=0;
               }
               count-=i-s;
               sum+=count+(base&0xff)+((base>>8)&0xff);
               if(count)
               {
                  fprintf(f,":%02X%04X00%s%02X\n",count,base&0xFFFF,str1,(-sum)&0xff);
               }
               str1[0]=0;
               count=sum=0;
            }
         }
      }
      fprintf(f,":00000001FF\n");
   }
   //**************** 24F *******************************************
   else if((!strncmp(dev,"24F",3)||!strncmp(dev,"24H",3)||!strncmp(dev,"24E",3)||!strncmp(dev,"30F",3)||!strncmp(dev,"33F",3)||!strncmp(dev,"33E",3)))
   {
      int valid;
      fprintf(f,":020000040000FA\n");			//extended address=0
      int sum=0,count=0,s,word;
      word=usb->memCODE[0]+(usb->memCODE[1]<<8)+(usb->memCODE[2]<<16)+(usb->memCODE[3]<<24);
      for(i=0;i<usb->size&&word==0xffffffff;i+=4) //remove leading 0xFFFFFFFF
         word=usb->memCODE[i]+(usb->memCODE[i+1]<<8)+(usb->memCODE[i+2]<<16)+(usb->memCODE[i+3]<<24);
      for(;i<usb->size;i++)
      {
         sum+=usb->memCODE[i];
         sprintf(str,"%02X",usb->memCODE[i]);
         strcat(str1,str);
         count++;
         if(count==16||i==usb->size-1)
         {
            base=i-count+1;
            for(s=base,valid=0;s<=i&&!valid;s+=4)
            {	//remove empty lines
               if(usb->memCODE[s]<0xFF||usb->memCODE[s+1]<0xFF||+usb->memCODE[s+2]<0xFF) valid=1;
            }
            sum+=count+(base&0xff)+((base>>8)&0xff);
            if(base>>16>ext)
            {
               ext=base>>16;
               fprintf(f,":02000004%04X%02X\n",ext,(-6-ext)&0xff);
            }
            if(count&&valid)
            {
               fprintf(f,":%02X%04X00%s%02X\n",count,base&0xFFFF,str1,(-sum)&0xff);
            }
            str1[0]=0;
            count=sum=0;
         }
      }
      if(usb->sizeCONFIG)
      {
         fprintf(f,":0200000401F009\n");
         for(i=0,count=sum=0;i<usb->sizeCONFIG&&i<48;i++)
         {
            sum+=usb->memCONFIG[i];
            sprintf(str,"%02X",usb->memCONFIG[i]);
            strcat(str1,str);
            count++;
            if(count==4||i==usb->sizeCONFIG-1)
            {
               base=i-count+1;
               sum+=count+(base&0xff)+((base>>8)&0xff);
               if(count)
               {
                  fprintf(f,":%02X%04X00%s%02X\n",count,base&0xFFFF,str1,(-sum)&0xff);
               }
               str1[0]=0;
               count=sum=0;
            }
         }
      }
      if(usb->sizeEE)
      {
         fprintf(f,":0200000400FFFB\n");
         str1[0]=0;
         for(i=0,count=sum=0;i<usb->sizeEE;i+=2)
         {		//append 0000 every 2 bytes
            sum+=usb->memEE[i]+usb->memEE[i+1];
            sprintf(str,"%02X%02X0000",usb->memEE[i]&0xff,usb->memEE[i+1]&0xff);
            strcat(str1,str);
            count+=4;
            if(count==16||i==usb->sizeEE-2)
            {
               base=2*i-count+4;
               for(s=base/2,valid=0;s<=i&&!valid;s+=2)
               {	//remove empty lines
                  if(usb->memEE[s]<0xFF||usb->memEE[s+1]<0xFF) valid=1;
               }
               sum+=0xE0+count+(base&0xff)+(base>>8);
               if(count&&valid)
               {
                  fprintf(f,":%02X%04X00%s%02X\n",count,base+0xE000,str1,(-sum)&0xff);
               }
               str1[0]=0;
               count=sum=0;
            }
         }
      }
      fprintf(f,":00000001FF\n");
   }
   //**************** ATxxxx *******************************************
   else if(!strncmp(dev,"AT",2))
   {
      fprintf(f,":020000040000FA\n");			//extended address=0
      for(i=0;i<usb->size&&usb->memCODE[i]==0xff;i++); //remove leading 0xFF
      for(;i<usb->size;i++)
      {
         sum+=usb->memCODE[i];
         sprintf(str,"%02X",usb->memCODE[i]);
         strcat(str1,str);
         count++;
         if(count==16||i==usb->size-1)
         {
            base=i-count+1;
            for(s=i;s>=base&&usb->memCODE[s]==0xff;s--)
            {	//remove trailing 0xFF
               sum-=usb->memCODE[s];
               str1[strlen(str1)-2]=0;
            }
            count-=i-s;
            sum+=count+(base&0xff)+((base>>8)&0xff);
            if(base>>16>ext)
            {
               ext=base>>16;
               fprintf(f,":02000004%04X%02X\n",ext,(-6-ext)&0xff);
            }
            if(count)
            {
               fprintf(f,":%02X%04X00%s%02X\n",count,base&0xFFFF,str1,(-sum)&0xff);
            }
            str1[0]=0;
            count=sum=0;
         }
      }
      fprintf(f,":00000001FF\n");
   }
   //**************** 24xxx / 93xxx / 25xxx / 95xxx / DSxxxx *******************************************
   else if(!strncmp(dev,"24",2)||!strncmp(dev,"93",2)||!strncmp(dev,"25",2)||!strncmp(dev,"95",2)||\
           !strncmp(dev,"DS",2)||!strncmp(dev,"11",2))
   {
      if(strstr(savefile,".bin")||strstr(savefile,".BIN"))
      {
#ifdef _WIN32
         //brain-damaged op. systems need this to avoid messing with some bytes
         f=freopen(savefile,"wb",f);
         if(!f) return;
#endif
         fwrite(usb->memEE,1,usb->sizeEE,f);
      }
      else
      {			//HEX
         int valid;
         fprintf(f,":020000040000FA\n");			//extended address=0
         for(i=0;i<usb->sizeEE;i++)
         {
            sum+=usb->memEE[i];
            sprintf(str,"%02X",usb->memEE[i]);
            strcat(str1,str);
            count++;
            if(count==16||i==usb->sizeEE-1)
            {
               for(s=valid=0;str1[s]&&!valid;s++) if(str1[s]!='F') valid=1;
               if(valid)
               {
                  base=i-count+1;
                  sum+=count+(base&0xff)+((base>>8)&0xff);
                  if(base>>16>ext)
                  {
                     ext=base>>16;
                     fprintf(f,":02000004%04X%02X\n",ext,(-6-ext)&0xff);
                  }
                  if(count)
                  {
                     fprintf(f,":%02X%04X00%s%02X\n",count,base&0xFFFF,str1,(-sum)&0xff);
                  }
               }
               str1[0]=0;
               count=sum=0;
            }
         }
         fprintf(f,":00000001FF\n");
      }
   }
   if(f) fclose(f);
}

int FileIO::loadEE(QString dev, QString loadfile)
{
   QByteArray ba1 = dev.toLatin1();
   char *c_dev = ba1.data();

   QByteArray ba2=loadfile.toLatin1();
   char *c_loadfile = ba2.data();

   loadEE(c_dev, c_loadfile);
}


void FileIO::loadEE(char*dev, char*loadfile)
{
   FILE* f=fopen(loadfile,"r");
   if(!f) return;
   int i;
   //**************** ATMEL *******************************************
   if(!strncmp(dev,"AT",2))
   {
      char line[256];
      int input_address=0,ext_addr=0;
      unsigned char bufferEE[0x1000];
      CommonFunctions::printMessage1("%s :\r\n\r\n",loadfile);
      memset(bufferEE,0xFF,sizeof(bufferEE));
      for(;fgets(line,256,f);)
      {
         if(strlen(line)>9&&line[0]==':')
         {
            int hex_count = htoi(line+1, 2);
            if((int)strlen(line) - 11 < hex_count * 2)
            {
               CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
            }
            else
            {
               input_address=htoi(line+3,4);
               int sum = 0;
               for (i=1;i<=hex_count*2+9;i+=2)	sum+=htoi(line+i,2);
               if ((sum & 0xff)!=0)
               {
                  CommonFunctions::printMessage1(GlobalVariables::strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
               }
               else
               {
                  switch(htoi(line+7,2))
                  {
                     case 0:		//Data record
                        if(ext_addr==0&&input_address<0x1000)
                        {	//EEPROM
                           for (i=0;i<hex_count;i++)
                           {
                              bufferEE[input_address+i]=htoi(line+9+i*2,2);
                           }
                           usb->sizeEE=input_address+hex_count;
                        }
                        break;
                     case 4:		//extended linear address record
                        if(strlen(line)>14)	ext_addr=htoi(line+9,4);
                        break;
                     default:
                        break;
                  }
               }
            }
         }
      }
      if(usb->memEE)
         free(usb->memEE);
      usb->memEE=(unsigned char*)malloc(usb->sizeEE);
      memcpy(usb->memEE,bufferEE,usb->sizeEE);
      //if(usb->sizeEE)
      //    usb->DisplayEE();	//visualize
      CommonFunctions::printMessage("\r\n");
      fclose(f);
   }
}

int FileIO::saveEE(QString dev, QString loadfile)
{
   QByteArray ba1 = dev.toLatin1();
   char *c_dev = ba1.data();

   QByteArray ba2=loadfile.toLatin1();
   char *c_loadfile = ba2.data();

   saveEE(c_dev, c_loadfile);
}

