#include "readWriteInfo.h"
#include "Libs/globalVariables.h"

ReadWriteInfo::ReadWriteInfo(USBTools *usb)  //(USBTools *usb, QObject *parent):ProgDevice(usb, parent)
{
    this->usb=usb;
}

ReadWriteInfo::ReadWriteInfo(USBTools *usb, int family, double HV, int V33, int readFunctionIndex, qint64 memoryFlashSize, qint64 memoryEESize, QList<int> readP, int resArea, int writeFunctionIndex, QList<int> writeP, double writeParamD)
{
//    this->progEEPROM =  GlobalVariables::progEEPROM;
//    this->progAVR = GlobalVariables::GlobalVariables::progAVR;
//    this->progP12 = GlobalVariables::GlobalVariables::progP12;
//    this->progP16 = GlobalVariables::GlobalVariables::progP16;
//    this->progP18 = GlobalVariables::GlobalVariables::progP18;
//    this->progP24 = GlobalVariables::GlobalVariables::progP24;

    this->usb=usb;
    this->family=family;
    this->readFunctionIndex=readFunctionIndex;
    this->writeFunctionIndex = writeFunctionIndex;
    this->HV = HV;
    this->V33=V33;
    this->memoryFlashSize = memoryFlashSize;
    this->memoryEESize=memoryEESize;
    for (int i = 0; i < readP.length(); ++i)
        this->readParam[i]=readP[i];
    this->resArea=resArea;
    for (int i = 0; i < writeP.length(); ++i)
        this->writeParam[i]=writeP[i];
    this->writeParamD = writeParamD;

    this->addresConfig=0;
    this->addresEE=0;
    this->hasCode=true;
    this->hasEE= memoryEESize>0 ;
    this->eeInCode=false;

    switch (readFunctionIndex)
    {
        case ReadFunctions::Read12F5xx:
        case ReadFunctions::Read16Fxxx:
            this->addresConfig=0x2000;
            this->addresEE=0x2100;
            this->eeInCode=true;
            break;

        case ReadFunctions::Read16F1xxx:
            this->addresConfig=0x8000;
            break;



        case ReadFunctions::Read11xx:
            this->hasCode=false;
            break;

        case ReadFunctions::ReadI2C:
            this->hasCode=false;
            break;

        default:
            break;
    }


}

void ReadWriteInfo::read(int ee, int r)
{
    qint64 params[5];
    params[0]=memoryFlashSize;
    params[1]=memoryEESize;
    for(int i=0;i<2;i++)
        params[i+2]=readParam[i];


    if (V33>0)  //3.3V required
    {
        if(!(usb->CheckV33Regulator()))
        {
            CommonFunctions::printMessage("S_noV33reg");	//Can't find 3.3V expansion board
            return;
        }
    }

    if (HV>7)
        usb->StartHVReg(HV);
    // usb->msDelay(20);


    // vdd=1;

    if(this->family==Family::PIC12)
    {
        if(r)
            params[1]=resArea;
        switch (this->readFunctionIndex)
        {
            case ReadFunctions::Read12F5xx:
                GlobalVariables::progP12->Read12F5xx(params[0], params[1]);
                break;
        }
    }

    else if(this->family==Family::PIC16)
    {
        if(r)
            params[2]=resArea;
        switch (this->readFunctionIndex)
        {
            case ReadFunctions::Read16Fxxx:
                GlobalVariables::progP16->Read16Fxxx(params[0],params[1],params[2],params[3]);
                qDebug()<< params[3];

                break;
            case ReadFunctions::Read16F1xxx:
                GlobalVariables::progP16->Read16F1xxx(params[0],params[1],params[2],params[3]);
                break;
        }
    }
    else if(this->family==Family::PIC18)
    {
        if(!ee)
            params[1]=0;					//clear eeprom parameter
        switch (this->readFunctionIndex)
        {
            case ReadFunctions::Read18Fx:
                GlobalVariables::progP18->Read18Fx(params[0],params[1], params[2]);
                break;
        }
    }
    else if(this->family==Family::PIC24)
    {
        if(!ee)
            params[1]=0;					//clear eeprom parameter
        qint64 res=r?resArea:0;
        switch (this->readFunctionIndex)
        {
            case ReadFunctions::Read24Ex:
                GlobalVariables::progP24->Read24Ex(params[0],params[1],params[2], params[3], res);
                break;
            case ReadFunctions::Read24Fx:
                GlobalVariables::progP24->Read24Fx(params[0],params[1],params[2], params[3], res);
                break;
        }
    }
    else if(this->family==Family::AVR)
    {
        if(!ee)
            params[1]=0;					//clear eeprom parameter
        switch (this->readFunctionIndex)
        {
            case ReadFunctions::ReadAT:
                GlobalVariables::progAVR->ReadAT(params[0],params[1],params[2]);
                break;
            case ReadFunctions::ReadAT_HV:
                GlobalVariables::progAVR->ReadAT_HV(params[0],params[1],params[2]);
                break;
        }
    }
    else if(this->family==Family::I2CEE)
    {
        switch (this->readFunctionIndex)
        {
            case ReadFunctions::ReadI2C:
                progEEPROM->ReadI2C(params[0],params[1]);
                break;
        }
    }
    else if(this->family==Family::UWEE)
    {
        switch (this->readFunctionIndex)
        {
            case ReadFunctions::Read93x:
                progEEPROM->Read93x(params[0], params[1], params[2]);
                break;
        }
    }
    else if(this->family==Family::SPIEE)
    {
        switch (this->readFunctionIndex)
        {
            case ReadFunctions::Read25xx:
                progEEPROM->Read25xx(params[0]);
                break;
        }
    }
    else if(this->family==Family::OWEE)
    {
        switch (this->readFunctionIndex)
        {
            case ReadFunctions::ReadOneWireMem:
                progEEPROM->ReadOneWireMem(params[0], params[1]);
                break;
            case ReadFunctions::ReadDS1820:
                progEEPROM->ReadDS1820();
                break;
        }
    }
    else if(this->family==Family::UNIOEE)
    {
        switch (this->readFunctionIndex)
        {
            case ReadFunctions::Read11xx:
                progEEPROM->Read11xx(params[0]);
                break;
        }
    }
    emit readFinished();
}

void ReadWriteInfo::write(int ee)
{
    qint64 params[6];
    for(int i=0;i<6;i++)
        params[i]=writeParam[i];


    if (V33>0)  //3.3V required
    {

        if(!(usb->CheckV33Regulator()))
        {
            CommonFunctions::printMessage("S_noV33reg");	//Can't find 3.3V expansion board
            return;
        }
    }

    usb->StartHVReg(HV);

    if(this->family==Family::PIC12)
    {
        switch (this->writeFunctionIndex)
        {
            case WriteFunctions::Write12F5xx:
                GlobalVariables::progP12->Write12F5xx(params[0], params[1]);
                break;
            case WriteFunctions::Write12C5xx:
                GlobalVariables::progP12->Write12C5xx(params[0], params[1]);
                break;
        }
    }
    else if(this->family==Family::PIC16)
    {
        if(!ee)
            params[1]=0;					//clear eeprom parameter
        switch (this->writeFunctionIndex)
        {
            case WriteFunctions::Write16F1xxx:
                GlobalVariables::progP16->Write16F1xxx(params[0],params[1],params[2]);
                break;
            case WriteFunctions::Write16F81x:
                GlobalVariables::progP16->Write16F81x(params[0],params[1]);
                break;
            case WriteFunctions::Write16F87x:
                GlobalVariables::progP16->Write16F87x(params[0],params[1]);
                break;
            case WriteFunctions::Write16F8x:
                GlobalVariables::progP16->Write16F8x (params[0],params[1]);
                break;
            case WriteFunctions::Write12F61x:
                GlobalVariables::progP16->Write12F61x (params[0],params[1],params[2]);
                break;
            case WriteFunctions::Write12F6xx:
                GlobalVariables::progP16->Write12F6xx (params[0],params[1]);
                break;
            case WriteFunctions::Write16F62x:
                GlobalVariables::progP16->Write16F62x (params[0],params[1]);
                break;
            case WriteFunctions::Write16F71x:
                GlobalVariables::progP16->Write16F71x(params[0],params[1]);
                break;
            case WriteFunctions::Write16F72x:
                GlobalVariables::progP16->Write16F72x(params[0],params[1],params[2]);
                break;
            case WriteFunctions::Write16F7x:
                GlobalVariables::progP16->Write16F7x( params[0],params[1]);
                break;
            case WriteFunctions::Write16F87xA:
                GlobalVariables::progP16->Write16F87xA(params[0],params[1],params[2]);
                break;
            case WriteFunctions::Write16F88x:
                GlobalVariables::progP16->Write16F88x(params[0],params[1]);
                break;
        }
    }
    else if(this->family==Family::PIC18)
    {
        if(!ee)
            params[1]=0;					//clear eeprom parameter       switch (this->writeFunctionIndex)
        switch (this->writeFunctionIndex)
        {
            case WriteFunctions::Write18Fx:
                GlobalVariables::progP18->Write18Fx(params[0],params[1],params[2],params[3],params[4],params[5]);
                break;
        }
    }
    else if(this->family==Family::PIC24)
    {
        if(!ee)
            params[1]=0;					//clear eeprom parameter
        switch (this->writeFunctionIndex)
        {
            case WriteFunctions::Write24Ex:
                GlobalVariables::progP24->Write24Ex(params[0],params[1],params[2],params[3],params[4], writeParamD);
                break;
            case WriteFunctions::Write24Fx:
                GlobalVariables::progP24->Write24Fx(params[0],params[1],params[2],params[3],params[4], writeParamD);
                break;
        }
    }
    else if(this->family==Family::AVR)
    {
        if(!ee)
            params[1]=0;					//clear eeprom parameter
        switch (this->writeFunctionIndex)
        {
            case WriteFunctions::WriteAT:
                GlobalVariables::progAVR->WriteAT(params[0],params[1]);
                break;
            case WriteFunctions::WriteATmega:
                GlobalVariables::progAVR->WriteATmega(params[0],params[1],params[2],params[3]);
                break;
            case WriteFunctions::WriteAT_HV:
                GlobalVariables::progAVR->WriteAT_HV(params[0],params[1],params[2],params[3]);
                break;
        }
    }
    else if(this->family==Family::I2CEE)
    {
        switch (this->writeFunctionIndex)
        {
            case WriteFunctions::WriteI2C:
                progEEPROM->WriteI2C(params[0],params[1],params[2]);
                break;
        }
    }
    else if(this->family==Family::UWEE)
    {
        switch (this->writeFunctionIndex)
        {
            case WriteFunctions::Write93Cx:
                progEEPROM->Write93Cx(params[0], params[1], params[2]);
                break;
            case WriteFunctions::Write93Sx:
                progEEPROM->Write93Sx(params[0], params[1], params[2]);
                break;
        }
    }
    else if(this->family==Family::SPIEE)
    {
        switch (this->writeFunctionIndex)
        {
            case WriteFunctions::Write25xx:
                progEEPROM->Write25xx(params[0],params[1]);
                break;
        }
    }
    else if(this->family==Family::OWEE)
    {
        switch (this->writeFunctionIndex)
        {
            case WriteFunctions::WriteOneWireMem:
                progEEPROM->WriteOneWireMem(params[0], params[1]);
                break;
        }
    }
    else if(this->family==Family::UNIOEE)
    {
        switch (this->writeFunctionIndex)
        {
            case WriteFunctions::Write11xx:
                progEEPROM->Write11xx(params[0],params[1]);
                break;
        }
    }
}

void ReadWriteInfo::setProgrammersInterface(ProgEEPROM *progEEPROM, ProgAVR *progAVR, ProgP12 *progP12, ProgP16 *progP16, ProgP18 *progP18, ProgP24 *progP24)
{
//    this->progEEPROM = progEEPROM;
//    this->progAVR = progAVR;
//    this->progP12 = progP12;
//    this->progP16 = progP16;
//    this->progP18 = progP18;
//    this->progP24 = progP24;
}
