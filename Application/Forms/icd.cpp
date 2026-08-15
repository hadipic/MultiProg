#include "icd.h"
#include "ui_icd.h"
#include "QString"
#include <QPixmap>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableView>
#include <QApplication>


Icd::Icd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Icd)
{
    ui->setupUi(this);
    createActions();
    createToolbars();
}

Icd::~Icd()
{
    delete ui;
}

void Icd::createActions()
{


    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::New);
    openAct->setStatusTip(tr("Open and load coff file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionLoad_Conf, SIGNAL(triggered()), this, SLOT(open()));


    icd_run= new QAction (QIcon(":/images/go.png"), tr("&Run "), this);
    icd_run->setShortcuts(QKeySequence::New);
    icd_run->setStatusTip(tr("icd run "));

    icd_halt= new QAction (QIcon(":/images/halt.png"), tr("&Halt "), this);
    icd_halt->setShortcuts(QKeySequence::New);
    icd_halt->setStatusTip(tr(" halt "));

    icd_step= new QAction (QIcon(":/images/step.png"), tr("&Step "), this);
    icd_step->setShortcuts(QKeySequence::New);
    icd_step->setStatusTip(tr("step "));

    icd_step_over= new QAction (QIcon(":/images/stepover"), tr("&Step over "), this);
    icd_step_over->setShortcuts(QKeySequence::New);
    icd_step_over->setStatusTip(tr(" step over "));

    icd_stop= new QAction (QIcon(":/images/stop.png"), tr("&Stop "), this);
    icd_stop->setShortcuts(QKeySequence::New);
    icd_stop->setStatusTip(tr("icd "));

    icd_refresh= new QAction (QIcon(":/images/refresh.png"), tr("&Refresh file "), this);
    icd_refresh->setShortcuts(QKeySequence::New);
    icd_refresh->setStatusTip(tr(" refreshing "));

}

void Icd::createToolbars()
{


    ui->toolBar->clear();
    ui->toolBar->addAction(openAct);
    ui->toolBar->addAction(icd_refresh);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(icd_run);
    ui->toolBar->addAction(icd_halt);
    ui->toolBar->addAction( icd_step);
    ui->toolBar->addAction( icd_step_over);
    ui->toolBar->addAction(icd_stop);

    //ui->toolBar->addAction(pasteAct);

}

void Icd::run_icd_slot()
{
}

void Icd::run_halt_slot()
{
}

void Icd::run_step_slot()
{
}

void Icd::run_step_over_slot()
{
}

void Icd::run_stop_slot()
{
}

void Icd::run_refresh_slot()
{
}

void Icd::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {



    }
}

