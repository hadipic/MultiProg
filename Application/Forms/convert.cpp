#include "convert.h"
#include "ui_convert.h"

convert::convert(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::convert)
{
    ui->setupUi(this);
}

convert::~convert()
{
    delete ui;
}

void convert::on_convert_iconSizeChanged(const QSize &iconSize)
{

}

void convert::on_textBrowser_textChanged()
{

}

void convert::on_textEdit_textChanged()
{
    // QCoreApplication a(argc, argv);
    bool ok;

    // ui->cob_type_in

    QString str = ui->textEdit->toPlainText();


    int iVal = str.toInt(&ok,16);
    QString binnumber = str.setNum(iVal, 2);
    QString hexnumber = str.setNum(iVal, 16);
    QString asci1 =  QChar(iVal);//.toAscii();


    ui->lab_HEX_2->setText("0x"+hexnumber);
    ui->label_des_2->setText(QString::number(iVal));
    ui->label_bin2->setText("0b"+binnumber);
    ui->label_accii_2->setText(asci1);
}
