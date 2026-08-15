#include "dis_asmble.h"
#include "ui_dis_asmble.h"

Dis_asmble::Dis_asmble(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dis_asmble)
{
    ui->setupUi(this);

    QColor lineColor = QColor(Qt::yellow).lighter(160);




}

Dis_asmble::~Dis_asmble()
{
    delete ui;
}

