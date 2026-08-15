#include "ioCheck.h"
#include "ui_ioCheck.h"

IOCheck::IOCheck(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IOCheck)
{
    ui->setupUi(this);
    connect(GlobalVariables::usb, SIGNAL(onIoChanged()), this, SLOT(onIoChanged1()));
}

IOCheck::~IOCheck()
{
    delete ui;
}



void IOCheck::on_radioButton_111_clicked()
{
    ui->grbRC7->setTitle("&RC7=1");
    GlobalVariables::usb->trisc &= 0x7f;
    GlobalVariables::usb->latac |= 0x80;
    GlobalVariables::usb->IoChange();



}

void IOCheck::on_radioButton_110_clicked()
{
    ui->grbRC7->setTitle("&RC7=0");
    GlobalVariables::usb->trisc &= 0x7f;
    GlobalVariables::usb->latac &= 0x7f;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_117_clicked()
{
    ui->grbRC6->setTitle("&RC6=1");
    GlobalVariables::usb->trisc &= 0xbf;
    GlobalVariables::usb->latac |= 0x40;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_116_clicked()
{
    ui->grbRC6->setTitle("&RC6=0");
    GlobalVariables::usb->trisc &= 0xbf;
    GlobalVariables::usb->latac &= 0xbf;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_120_clicked()
{
    ui->grbRA3->setTitle("&RA3=1");
    GlobalVariables::usb->trisa &= 0xf7;
    GlobalVariables::usb->latac |= 0x08;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_123_clicked()
{
    ui->grbRA4->setTitle("&RA4=1");
    GlobalVariables::usb->trisa &=0xef;
    GlobalVariables::usb->latac |= 0x10;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_162_clicked()
{
    ui->grbRA5->setTitle("&RA5=1");
    GlobalVariables::usb->trisa &= 0xdf; // pin a5 0utput
    GlobalVariables::usb->latac |= 0x20;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_119_clicked()
{
    ui->grbRA3->setTitle("&RA3=0");
    GlobalVariables::usb->trisa &= 0xf7;
    GlobalVariables::usb->latac &= 0xf7;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_122_clicked()
{
    ui->grbRA4->setTitle("&RA4=0");
    GlobalVariables::usb->trisa &=0xef;
    GlobalVariables::usb->latac &= 0xef;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_161_clicked()
{
    ui->grbRA5->setTitle("&RA5=0");
    GlobalVariables::usb->trisa &= 0xdf; // pin a5 0utput
    GlobalVariables::usb->latac &= 0xdf;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_160_clicked()
{
    ui->grbRA5->setTitle("&RA5 IN=>");
    GlobalVariables::usb->trisa |= 0x20;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_121_clicked()
{
    ui->grbRA4->setTitle("&RA4 IN=>");
    GlobalVariables::usb->trisa |= 0x10;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_118_clicked()
{
    ui->grbRA3->setTitle("&RA3 IN=>");
    GlobalVariables::usb->trisa |= 0x08;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_109_clicked()
{
    ui->grbRC7->setTitle("&RC7 IN=>");
    GlobalVariables::usb->trisc |= 0x80;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_115_clicked()
{
    ui->grbRC6->setTitle("&RC6 IN=>");
    GlobalVariables::usb->trisc |= 0x40;
    GlobalVariables::usb->IoChange();
}










void IOCheck::on_radioButton_92_clicked()
{

    ui->grbRB1->setTitle("&RB1=0");
    GlobalVariables::usb->trisb&=0xfd; //0b11111110
    GlobalVariables::usb->latb&=0xfd;
    GlobalVariables::usb->IoChange();
}



void IOCheck::on_radioButton_95_clicked()
{
    ui->grbRB2->setTitle("&RB2=0");
    GlobalVariables::usb->trisb&=0xfb;
    GlobalVariables::usb->latb&=0xfb;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_98_clicked()
{
    ui->grbRB3->setTitle("&RB3=0");
    GlobalVariables::usb->trisb&=0xf7;
    GlobalVariables::usb->latb&=0xf7;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_101_clicked()
{
    ui->grbRB4->setTitle("&RB4=0");
    GlobalVariables::usb->trisb&=0xef;
    GlobalVariables::usb->latb&=0xef;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_104_clicked()
{
    ui->grbRB5->setTitle("&RB5=0");
    GlobalVariables::usb->trisb&=0xdf;
    GlobalVariables::usb->latb&=0xdf;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_107_clicked()
{
    ui->grbRB6->setTitle("&RB6=0");
    GlobalVariables::usb->trisb&=0xbf;
    GlobalVariables::usb->latb&=0xbf;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_113_clicked()
{
    ui->grbRB7->setTitle("&RB7=0");
    GlobalVariables::usb->trisb&=0x7f;
    GlobalVariables::usb->latb&=0x7f;
    GlobalVariables::usb->IoChange();
}




void IOCheck::on_radioButton_89_clicked()
{
    ui->grbRB0->setTitle("&RB0=0");
    GlobalVariables::usb->trisb&=0b11111110;
    GlobalVariables::usb->latb&=0b11111110;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_90_clicked()
{
    ui->grbRB0->setTitle("&RB0=1");
    GlobalVariables::usb->trisb&=0b11111110;
    GlobalVariables::usb->latb|=0b00000001;
    GlobalVariables::usb->IoChange();

}

void IOCheck::on_radioButton_88_clicked()
{
    ui->grbRB0->setTitle("&RB0 IN=>");
    GlobalVariables::usb->trisb |= 0b00000001;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_93_clicked()
{
    ui->grbRB1->setTitle("&RB1=1");
    GlobalVariables::usb->trisb&=0xfd;
    GlobalVariables::usb->latb |= 0x02;
    GlobalVariables::usb->IoChange();
}





void IOCheck::on_radioButton_96_clicked()
{
    ui->grbRB2->setTitle("&RB2=1");
    GlobalVariables::usb->trisb&=0xfb;
    GlobalVariables::usb->latb |= 0x04;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_99_clicked()
{
    ui->grbRB3->setTitle("&RB3=1");
    GlobalVariables::usb->trisb&=0xf7;
    GlobalVariables::usb->latb |= 0x08;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_102_clicked()
{
    ui->grbRB4->setTitle("&RB4=1");
    GlobalVariables::usb->trisb&=0xef;
    GlobalVariables::usb->latb |= 0x10;
    GlobalVariables::usb->IoChange();

}

void IOCheck::on_radioButton_105_clicked()
{
    ui->grbRB5->setTitle("&RB5=1");
    GlobalVariables::usb->trisb&=0xdf;
    GlobalVariables::usb->latb |= 0x20;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_108_clicked()
{
    ui->grbRB6->setTitle("&RB6=1");
    GlobalVariables::usb->trisb&=0xbf;
    GlobalVariables::usb->latb |= 0x40;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_114_clicked()
{
    ui->grbRB7->setTitle("&RB7=1");
    GlobalVariables::usb->trisb&=0x7f;
    GlobalVariables::usb->latb |= 0x80;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_112_clicked()
{
    ui->grbRB7->setTitle("&RB7 IN=>");
    GlobalVariables::usb->trisb |= 0x80;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_106_clicked()
{
    ui->grbRB6->setTitle("&RB6 IN=>");
    GlobalVariables::usb->trisb |= 0x40;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_103_clicked()
{
    ui->grbRB5->setTitle("&RB5 IN=>");
    GlobalVariables::usb->trisb |= 0x20;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_100_clicked()
{
    ui->grbRB4->setTitle("&RB4 IN=>");
    GlobalVariables::usb->trisb |= 0x10;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_97_clicked()
{
    ui->grbRB3->setTitle("&RB3 IN=>");
    GlobalVariables::usb->trisb |= 0x08;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_94_clicked()
{
    ui->grbRB2->setTitle("&RB2 IN=>");
    GlobalVariables::usb->trisb |= 0x04;
    GlobalVariables::usb->IoChange();
}

void IOCheck::on_radioButton_91_clicked()
{
    ui->grbRB1->setTitle("&RB1 IN=>");
    GlobalVariables::usb->trisb |= 0x02;
    GlobalVariables::usb->IoChange();
}

void IOCheck::onIoChanged1()
{
    // qDebug()<<"port A:"<<QString::number(GlobalVariables::usb->portA.byte, 2)<<"port B:"<< QString::number(GlobalVariables::usb->portB.byte, 2)<<"port C:"<< QString::number(GlobalVariables::usb->portC.byte, 2);
    if(ui->radioButton_88->isChecked() ) ui->grbRB0->setTitle(QString("&RB0 IN=>")+ QString::number(GlobalVariables::usb->portB.bits.Bit0));
    if(ui->radioButton_91->isChecked() ) ui->grbRB1->setTitle(QString("&RB1 IN=>")+ QString::number(GlobalVariables::usb->portB.bits.Bit1));
    if(ui->radioButton_94->isChecked() ) ui->grbRB2->setTitle(QString("&RB2 IN=>")+ QString::number(GlobalVariables::usb->portB.bits.Bit2));
    if(ui->radioButton_97->isChecked() ) ui->grbRB3->setTitle(QString("&RB3 IN=>")+ QString::number(GlobalVariables::usb->portB.bits.Bit3));
    if(ui->radioButton_100->isChecked() ) ui->grbRB4->setTitle(QString("&RB4 IN=>")+ QString::number(GlobalVariables::usb->portB.bits.Bit4));
    if(ui->radioButton_113->isChecked() ) ui->grbRB5->setTitle(QString("&RB5 IN=>")+ QString::number(GlobalVariables::usb->portB.bits.Bit5));
    if(ui->radioButton_106->isChecked() ) ui->grbRB6->setTitle(QString("&RB6 IN=>")+ QString::number(GlobalVariables::usb->portB.bits.Bit6));
    if(ui->radioButton_112->isChecked() )  ui->grbRB7->setTitle(QString("&RB7 IN=>")+ QString::number(GlobalVariables::usb->portB.bits.Bit7));

    if(ui->radioButton_115->isChecked() ) ui->grbRC6->setTitle(QString("&RC6 IN=>")+ QString::number(GlobalVariables::usb->portC.bits.Bit6));
    if(ui->radioButton_109->isChecked() ) ui->grbRC7->setTitle(QString("&RC7 IN=>")+ QString::number(GlobalVariables::usb->portC.bits.Bit7));


    if(ui->radioButton_118->isChecked() ) ui->grbRA3->setTitle(QString("&RA3 IN=>")+ QString::number(GlobalVariables::usb->portA.bits.Bit3));
    if(ui->radioButton_121->isChecked() ) ui->grbRA4->setTitle(QString("&RA4 IN=>")+ QString::number(GlobalVariables::usb->portA.bits.Bit4));
    if(ui->radioButton_160->isChecked() ) ui->grbRA5->setTitle(QString("&RA5 IN=>")+ QString::number(GlobalVariables::usb->portA.bits.Bit5));


    // qDebug()<<"adc:"<<QString::number(GlobalVariables::usb->StartHVReg(ui->horizontalSlider->value())/72, 10) ;


    if(ui->chb_dctodc->checkState()) qDebug()<<"slider:"<<QString::number(ui->horizontalSlider->value(), 10) ;
    GlobalVariables::usb->DCDCactive(ui->chb_dctodc->checkState(),ui->horizontalSlider->value());

    float vpp_v = GlobalVariables::usb->StartHVReg1(ui->horizontalSlider->value())/72.28;

    QString show = QString::number(vpp_v, 'g', 3); // show foloting number
    ui->label_adc->setText(QString("VPP Volt=")+show);

    GlobalVariables::usb->vdd_vpp(ui->ch_B_vpp->isChecked(),ui->ch_B_vcc->isChecked());



}



void IOCheck::on_grbEnbableIo_clicked(bool checked)
{
    GlobalVariables::usb->changeEnableIo(checked);
}

void IOCheck::on_io_chek_currentChanged(int index)
{
    GlobalVariables::usb->changeEnableIo(index==3 && ui->grbEnbableIo->isChecked());
}

