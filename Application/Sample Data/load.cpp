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
            GlobalVariables::printMessage1(GlobalVariables::strings[S_Inohex],hex);	//"Error: '%.4s' doesn't look very hexadecimal, right?\n"
    }
    return v;
}
int FileIO::load(char*dev, char*loadfile)
{

    int i,input_address=0,ext_addr=0,sum,valid,empty;
    char s[256]="",line[256];
    FILE* f=fopen(loadfile,"r");
    if(!f) return -1;
    GlobalVariables::printMessage1("%s :\r\n\r\n",loadfile);
    //**************** 10-16F *******************************************
    if(!strncmp(dev,"10",2)||!strncmp(dev,"12",2)||!strncmp(dev,"16",2))
    {
        unsigned char buffer[0x20000],bufferEE[0x1000];
        int sizeM=0;
        memset(buffer,0xFF,sizeof(buffer));
        memset(bufferEE,0xFF,sizeof(bufferEE));
        usb->sizeEE=0;
        for(;fgets(line,256,f);)
        {
            if(strlen(line)>9&&line[0]==':')
            {
                int hex_count = htoi(line+1, 2);
                if((int)strlen(line)-11<hex_count*2)
                {
                    GlobalVariables::printMessage1(GlobalVariables::strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
                }
                else
                {
                    input_address=htoi(line+3,4);
                    sum=0;
                    for (i=1;i<=hex_count*2+9;i+=2) sum += htoi(line+i,2);
                    if ((sum & 0xff)!=0)
                    {
                        GlobalVariables::printMessage1(GlobalVariables::strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
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
                            else if(ext_addr==0x1&&input_address>=0xE000&&input_address<0xF000)
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
                            if(strlen(line)>14)	ext_addr=htoi(line+9,4);
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
        usb->sizeW/=2;
        if(usb->memCODE_W)
            free(usb->memCODE_W);
        usb->memCODE_W=(WORD*)malloc(sizeof(WORD)*usb->sizeW);
        for(i=0;i<usb->sizeW;i++)
        {		//Swap bytes
            usb->memCODE_W[i]=(buffer[i*2+1]<<8)+buffer[i*2];
        }
        if(usb->memEE) free(usb->memEE);
        if(usb->sizeEE)
        {
            usb->memEE=(unsigned char*)malloc(usb->sizeEE);
            memcpy(usb->memEE,bufferEE,usb->sizeEE);
        }
        else usb->memEE=0;
        GlobalVariables::printMessage(GlobalVariables::strings[S_CodeMem]);	//"\r\nCode memory:\r\n"
        s[0]=0;
        empty=1;
        int imax=usb->sizeW>0x8000?0x8500:0x2100;
        ////DisplayCODE16F(imax);
        if(usb->sizeW>=0x2100&&usb->sizeW<0x3000)
        {	//EEPROM@0x2100
            GlobalVariables::printMessage(GlobalVariables::strings[S_EEMem]);	//"\r\nEEPROM memory:\r\n"
            ////DisplayEE16F(0x700);
        }
        else if(usb->sizeEE) //DisplayEE();
            GlobalVariables::printMessage("\r\n");
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
                    GlobalVariables::printMessage1(GlobalVariables::strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
                }
                else
                {
                    input_address=htoi(line+3,4);
                    sum=0;
                    for (i=1;i<=hex_count*2+9;i+=2)
                        sum += htoi(line+i,2);
                    if ((sum & 0xff)!=0)
                    {
                        GlobalVariables::printMessage1(GlobalVariables::strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
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
                            if(strlen(line)>14)	ext_addr=htoi(line+9,4);
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
        if(usb->memCODE) free(usb->memCODE);
        usb->memCODE=(unsigned char*)malloc(usb->size);
        memcpy(usb->memCODE,buffer,usb->size);
        if(usb->memEE) free(usb->memEE);
        usb->memEE=(unsigned char*)malloc(usb->sizeEE);
        memcpy(usb->memEE,bufferEE,usb->sizeEE);
        GlobalVariables::printMessage(GlobalVariables::strings[S_IDMem]);	//"ID memory:\r\n"
        for(i=0;i<8;i+=2)	GlobalVariables::printMessage4("ID%d: 0x%02X   ID%d: 0x%02X\r\n",i,usb->memID[i],i+1,usb->memID[i+1]);
        GlobalVariables::printMessage(GlobalVariables::strings[S_ConfigMem]);	//"CONFIG memory:\r\n"
        for(i=0;i<7;i++)
        {
            GlobalVariables::printMessage2(GlobalVariables::strings[S_ConfigWordH],i+1,usb->memCONFIG[i*2+1]);	//"CONFIG%dH: 0x%02X\t"
            GlobalVariables::printMessage2(GlobalVariables::strings[S_ConfigWordL],i+1,usb->memCONFIG[i*2]);	//"CONFIG%dL: 0x%02X\r\n"
        }
        GlobalVariables::printMessage(GlobalVariables::strings[S_CodeMem]);	//"\r\nCODE memory:\r\n"
        //DisplayCODE18F(usb->size);
        if(usb->sizeEE) //DisplayEE();
            GlobalVariables::printMessage("\r\n");
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
                    GlobalVariables::printMessage1(GlobalVariables::strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
                }
                else
                {
                    input_address=htoi(line+3,4);
                    sum=0;
                    for (i=1;i<=hex_count*2+9;i+=2)
                        sum += htoi(line+i,2);
                    if ((sum & 0xff)!=0)
                    {
                        GlobalVariables::printMessage1(GlobalVariables::strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
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
        if(usb->memCODE) free(usb->memCODE);
        usb->memCODE=(unsigned char*)malloc(usb->size);
        memcpy(usb->memCODE,buffer,usb->size);
        free(buffer);
        usb->sizeEE=usb->sizeEE?0x1000:0;
        if(usb->memEE) free(usb->memEE);
        usb->memEE=(unsigned char*)malloc(usb->sizeEE);
        for(i=0;i<usb->sizeEE;i+=2)
        {		//skip voids in the hex file organization
            usb->memEE[i]=bufferEE[i*2]; 	//0 1 4 5 8 9 12 13 ...
            usb->memEE[i+1]=bufferEE[i*2+1];
        }
        for(i=valid=0;i<48;i++) if(usb->memCONFIG[i]<0xFF) valid=1;
        if(valid)
        {
            GlobalVariables::printMessage(GlobalVariables::strings[S_ConfigMem]);				//"\r\nCONFIG memory:\r\n"
            for(i=0;i<48;i+=4)
            {
                d=(usb->memCONFIG[i+1]<<8)+usb->memCONFIG[i];
                if(i<36||d<0xFFFF)GlobalVariables::printMessage2("0xF800%02X: 0x%04X\r\n",i/2,d);
            }
        }
        if(usb->size) GlobalVariables::printMessage(GlobalVariables::strings[S_CodeMem]);	//"\r\nCODE memory:\r\n"
        //DisplayCODE24F(usb->size);
        if(usb->sizeEE)
        {			//show eeprom with address offset by 0x7FF000
            GlobalVariables::printMessage(GlobalVariables::strings[S_EEMem]);	//"\r\nEEPROM memory:\r\n"
            //DisplayEE24F();
        }
        GlobalVariables::printMessage("\r\n");
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
                    GlobalVariables::printMessage1(GlobalVariables::strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
                }
                else
                {
                    input_address=htoi(line+3,4);
                    sum=0;
                    for (i=1;i<=hex_count*2+9;i+=2)
                        sum += htoi(line+i,2);
                    if ((sum & 0xff)!=0)
                    {
                        GlobalVariables::printMessage1(GlobalVariables::strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
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
        if(usb->memCODE) free(usb->memCODE);
        usb->memCODE=(unsigned char*)malloc(usb->size);
        memcpy(usb->memCODE,buffer,usb->size);
        if(usb->size) GlobalVariables::printMessage(GlobalVariables::strings[S_CodeMem]);	//"\r\nmemoria CODICE:\r\n"
        //DisplayCODEAVR(usb->size);
        GlobalVariables::printMessage("\r\n");
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
            if(usb->memEE) free(usb->memEE);
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
                        GlobalVariables::printMessage1(GlobalVariables::strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
                    }
                    else
                    {
                        input_address=htoi(line+3,4);
                        sum=0;
                        int end1;
                        for (i=1;i<=hex_count*2+9;i+=2) sum+=htoi(line+i,2);
                        if ((sum & 0xff)!=0)
                        {
                            GlobalVariables::printMessage1(GlobalVariables::strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
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
            if(usb->memEE) free(usb->memEE);
            usb->memEE=(unsigned char*)malloc(usb->sizeEE);
            memcpy(usb->memEE,bufferEE,usb->sizeEE);
            free(bufferEE);
        }
        //DisplayEE();	//visualize
        int sum=0;
        for(i=0;i<usb->sizeEE;i++) sum+=usb->memEE[i];
        GlobalVariables::printMessage1("Checksum: 0x%X\r\n",sum&0xFFFF);
        GlobalVariables::printMessage("\r\n");
    }
    fclose(f);
    return 0;
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
            GlobalVariables::printMessage1(GlobalVariables::strings[S_Inohex],hex);	//"Error: '%.4s' doesn't look very hexadecimal, right?\n"
    }
    return v;
}


void LoadEE(char*dev,char*loadfile)
{
    FILE* f=fopen(loadfile,"r");
    if(!f) return;
    int i;
//**************** ATMEL *******************************************
    if(!strncmp(dev,"AT",2)){
        char line[256];
        int input_address=0,ext_addr=0;
        unsigned char bufferEE[0x1000];
        PrintMessage1("%s :\r\n\r\n",loadfile);
        memset(bufferEE,0xFF,sizeof(bufferEE));
        for(;fgets(line,256,f);){
            if(strlen(line)>9&&line[0]==':'){
                int hex_count = htoi(line+1, 2);
                if((int)strlen(line) - 11 < hex_count * 2) {
                    PrintMessage1(strings[S_IhexShort],line);	//"Intel hex8 line too short:\r\n%s\r\n"
                }
                else{
                    input_address=htoi(line+3,4);
                    int sum = 0;
                    for (i=1;i<=hex_count*2+9;i+=2)	sum+=htoi(line+i,2);
                    if ((sum & 0xff)!=0) {
                        PrintMessage1(strings[S_IhexChecksum],line);	//"Intel hex8 checksum error in line:\r\n%s\r\n"
                    }
                    else{
                        switch(htoi(line+7,2)){
                            case 0:		//Data record
                                if(ext_addr==0&&input_address<0x1000){	//EEPROM
                                    for (i=0;i<hex_count;i++){
                                        bufferEE[input_address+i]=htoi(line+9+i*2,2);
                                    }
                                    sizeEE=input_address+hex_count;
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
        if(memEE) free(memEE);
        memEE=(unsigned char*)malloc(sizeEE);
        memcpy(memEE,bufferEE,sizeEE);
        if(sizeEE) DisplayEE();	//visualize
        PrintMessage("\r\n");
        fclose(f);
    }
}
