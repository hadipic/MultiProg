#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include <QPixmap>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableView>
#include <QApplication>
#include <QCombobox>
#include <QtMath>
#include <QThread>
#ifdef MY_ANDROID
#include <QAndroidJniObject>
#endif



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->deviceInfo=NULL;

    createActions();
    createToolbars();


    usb = GlobalVariables::usb;

    connect(usb, SIGNAL(onIoChanged()), this, SLOT(onIoChanged()));
    connect( GlobalVariables::progEEPROM, SIGNAL(onProgressChanged(int, int)),this, SLOT(on_Progress_Changed(int, int)));
    connect( GlobalVariables::progP16, SIGNAL(onProgressChanged(int, int )),this, SLOT(on_Progress_Changed(int, int )));
    connect( GlobalVariables::progP18, SIGNAL(onProgressChanged(int, int)),this, SLOT(on_Progress_Changed(int, int)));
    connect( GlobalVariables::progP24, SIGNAL(onProgressChanged(int, int)),this, SLOT(on_Progress_Changed(int, int)));
    connect( GlobalVariables::progAVR, SIGNAL(onProgressChanged(int, int)),this, SLOT(on_Progress_Changed(int, int)));
    connect( GlobalVariables::progEEPROM, SIGNAL(PrintStatus(QString  ,int ,int )),this, SLOT(Print_Status(QString  ,int ,int )));
    connect( GlobalVariables::progP16, SIGNAL(PrintStatus(QString  ,int ,int )),this, SLOT(Print_Status(QString  ,int ,int )));
    connect( GlobalVariables::progP18, SIGNAL(PrintStatus(QString  ,int ,int )),this, SLOT(Print_Status(QString  ,int ,int )));
    connect( GlobalVariables::progP24, SIGNAL(PrintStatus(QString  ,int ,int )),this, SLOT(Print_Status(QString  ,int ,int )));
    connect( GlobalVariables::progAVR, SIGNAL(PrintStatus(QString  ,int ,int )),this, SLOT(Print_Status(QString  ,int ,int )));

    connect(ui->deviceListWidget, SIGNAL(onDeviceSelected(DeviceFamily*,DeviceType*,DeviceInfo*)), this, SLOT(on_DeviceSelected(DeviceFamily*,DeviceType*,DeviceInfo*)));



    CommonFunctions::txtLog=ui->txtLog;
#ifndef MY_ANDROID
    detectHardware();
#endif

    ui->wdgHexEditorFlash->setBitLength(16);
    ui->wdgHexEditorFlash->setSize(64*1024);



    settings=new QSettings("Settings.ini", QSettings::IniFormat);
    loadSettings();
    usb->sizeW=4800;
    #ifdef MY_ANDROID
    //ui->twdgShowBuffers->hide();
    //ui->gridLayout_2->removeWidget(ui->twdgShowBuffers);

   #endif
   save_falg=0;


}

void MainWindow::loadSettings()
{
    if(settings->allKeys().count()>=3)
    {
        int familyIndex = settings->value("DeviceFamilyIndex", 0).toInt();
        int typeIndex=settings->value("DeviceTypeIndex", 0).toInt();
        int infoIndex=settings->value("DeviceNameIndex", 0).toInt();
        ui->deviceListWidget->setComboIndex(familyIndex, typeIndex, infoIndex);
    }
    else
    {
        ui->deviceListWidget->setComboIndex(0, 0, 1);
        ui->deviceListWidget->setComboIndex(0, 0, 0);
    }
    fileName=settings->value("FileName", "").toString();
}

void MainWindow::saveSettings()
{
    settings->setValue("DeviceFamilyIndex", ui->deviceListWidget->familyIndex());
    settings->setValue("DeviceTypeIndex"  , ui->deviceListWidget->typeIndex());
    settings->setValue("DeviceNameIndex"  , ui->deviceListWidget->infoIndex());
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->mnuOpen, SIGNAL(triggered()), this, SLOT(open()));

    openEEAct = new QAction(QIcon(":/images/open.png"), tr("&Open EEProm..."), this);
    openEEAct->setShortcuts(QKeySequence::Open);
    openEEAct->setStatusTip(tr("Open an existing file"));
    connect(openEEAct, SIGNAL(triggered()), this, SLOT(openEE()));
    connect(ui->mnuOpenEEProm, SIGNAL(triggered()), this, SLOT(openEE()));


    mnuOpen = new QAction(tr("About &Qt"), this);
    mnuOpen->setStatusTip(tr("Show the Qt library's About box"));
    connect( mnuOpen, SIGNAL(triggered()),this,  SLOT(open()));
    //! [22]


    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(saveAs()));

    //! [0]
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
    //! [0]

    //#ifndef QT_NO_CLIPBOARD
    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));


    readchip= new QAction (QIcon(":/images/read.png"), tr("&خواندن"), this);
    readchip->setStatusTip(tr("After Reading chip  plese shure correct chip"));
    connect(readchip, SIGNAL(triggered()),this,  SLOT(readChip()));

    writechip= new QAction (QIcon(":/images/write.png"), tr("&نوشتن"), this);
    writechip->setStatusTip(tr("  After Writing chip  plese shure correct chip"));
    connect(writechip, SIGNAL(triggered()), this, SLOT(writeChip()));

    _none =new QAction (this);
    _none->setEnabled(false);

    _detectHardware= new QAction (QIcon(":/images/DetectDevice.png"), tr("&تشخيص پروگرمر"), this);
    _detectHardware->setStatusTip(tr("Detect Hardware"));
    connect(_detectHardware, SIGNAL(triggered()),this,  SLOT(detectHardware()));

    connect(ui->actionHardware_Chek, SIGNAL(triggered()), this, SLOT(io_show()));
    connect(ui->actionConverter, SIGNAL(triggered()), this, SLOT( con_show()));
    connect(ui->actionDis_Asmbler, SIGNAL(triggered()), this, SLOT( Dis_asm_show() ));
    connect(ui->actionIcd, SIGNAL(triggered()), this, SLOT( Icd_show() ));
}

void MainWindow::createToolbars()
{
    ui->mainToolBar->clear();
    ui->mainToolBar->addAction( openAct);
    ui->mainToolBar->addAction(newAct);
    ui->mainToolBar->addAction( saveAct);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(cutAct);
    ui->mainToolBar->addAction(copyAct);
    ui->mainToolBar->addAction(pasteAct);

    ui->toolBar->addAction( readchip);
    ui->toolBar->addAction( writechip);
    ui->toolBar->addAction(_detectHardware);
    ui->toolBar_2->addAction(_none);
    ui->deviceListWidget->setParent(ui->toolBar_2);
    //ui->toolBar_2->setSizePolicy();


    //fileMenu = menuBar()->addMenu(tr("&File"));
    // fileMenu->addAction(newAct);

}



void MainWindow::readChip()
{

    QString str="Reding\r\n";

    save_falg=1; // if cheng reding save flag

    ui->wdgHexEditorFlash->setBitLength(16);
    ui->wdgHexEditorFlash->setSize(deviceInfo->readWriteInfo->memoryFlashSize);

    ui->wdgHexEditorEEProm->setColumnCount(8);
    ui->wdgHexEditorEEProm->setBitLength(8);
    ui->wdgHexEditorFlash->setSize(deviceInfo->readWriteInfo->memoryEESize);


    //ui->txtLog->clear();
    ui->txtLog->appendPlainText(deviceInfo->name+" size=0x"+QString::number(deviceInfo->readWriteInfo->memoryFlashSize,16));

    //labtr:

    // if( usb->CheckS1()==1)
    /*if(deviceFamily->familyName=="AVR")
        ui->wdgFuseSettings->loadAVRFuseSettings();
    else if(deviceFamily->familyName=="PIC")
        ui->wdgFuseSettings->loadPICFuseSettings();
    */

    deviceInfo->readWriteInfo->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, deviceInfo->readWriteInfo, &QObject::deleteLater);
    connect(this, &MainWindow::read, deviceInfo->readWriteInfo, &ReadWriteInfo::read);
    connect(deviceInfo->readWriteInfo, SIGNAL(readFinished()), this, SLOT(readDataReady()));

    workerThread.start();
    emit read(deviceInfo->readWriteInfo->memoryFlashSize, 2);

    //deviceInfo->readWriteInfo->read(deviceInfo->readWriteInfo->memoryFlashSize, 2);


}

void MainWindow::readDataReady()
{
    loadFromBuffer(MemoryType::Flash_EEProm);
    //qDebug()<<str<<deviceInfo->readWriteInfo->memoryEESize;
}
template <typename T>
void MainWindow::clearMemory(T *&mem, quint32 &destSize, quint32 size, quint32 value)
{
    destSize=sizeof(T)*size;
    if(mem)
        free(mem);
    mem=(T*)malloc(sizeof(T)*size);
    memset(mem, (T)value, size*sizeof(T));
}

void MainWindow::loadFromBuffer(MemoryType::MemoryType memoryType, bool forEmpty)
{

    ui->wdgHexEditorEEProm->setColumnCount(8);
    ui->wdgHexEditorFlash->setColumnCount(8);
    int bitLength=16;
    int a=1;
    //QString devType=deviceInfo->name.mid(0,2);
    bitLength = deviceInfo->bitLength();

    int mode=2;
    if(bitLength==12 || bitLength==14)
        mode=1;
    if(bitLength==8)
        a=2;

    ui->wdgHexEditorFlash->setBitLength(bitLength);
    ui->wdgHexEditorFlash->setColumnCount(ui->wdgHexEditorFlash->byteLength()*a*8);
    ui->wdgHexEditorFlash->setSize(deviceInfo->readWriteInfo->memoryEESize);

    bool loadFlash=memoryType==MemoryType::Flash || memoryType==MemoryType::Flash_EEProm;
    bool loadEEProm=memoryType==MemoryType::EEProm || memoryType==MemoryType::Flash_EEProm;


    quint32 fillValue=qPow(2, deviceInfo->bitLength())-1;

    if(deviceFamily->familyName=="Memory"||deviceFamily->familyName=="One Wire")
    {
        if(forEmpty)
            clearMemory(usb->memEE, usb->sizeEE, deviceInfo->readWriteInfo->memoryFlashSize, fillValue);
        ui->wdgHexEditorEEProm->loadFromDevice(usb->memEE, usb->sizeEE, deviceInfo->readWriteInfo->memoryFlashSize);
        ui->twdgShowBuffers->setCurrentIndex(1);
    }
    else
    {
        if(deviceInfo->readWriteInfo->hasCode)
        {
            if(loadFlash)
            {
                if(mode==1)
                {
                    if(forEmpty)
                        clearMemory(usb->memCODE_W, usb->sizeW, deviceInfo->readWriteInfo->memoryFlashSize, fillValue);

                    ui->wdgHexEditorFlash->loadFromDevice(usb->memCODE_W, usb->sizeW, deviceInfo->readWriteInfo->memoryFlashSize);
                }
                else
                {
                    if(forEmpty)
                        clearMemory(usb->memCODE, usb->size, deviceInfo->readWriteInfo->memoryFlashSize, fillValue);
                    ui->wdgHexEditorFlash->loadFromDevice(usb->memCODE, usb->size, deviceInfo->readWriteInfo->memoryFlashSize);
                }
            }
            if(deviceInfo->readWriteInfo->hasEE)
            {
                if(deviceInfo->readWriteInfo->eeInCode)
                {
                    if(loadFlash)
                    {
                        if(mode==1)
                        {
                            //if(forEmpty)
                            // clearMemory(usb->memCODE_W, usb->sizeW, deviceInfo->readWriteInfo->memoryEESize, fillValue);
                            if(forEmpty)
                                memset(usb->memCODE_W + deviceInfo->readWriteInfo->addresEE, 0xFF, deviceInfo->readWriteInfo->memoryEESize);

                            ui->wdgHexEditorEEProm->loadFromDevice( usb->memCODE_W + deviceInfo->readWriteInfo->addresEE, usb->sizeW-deviceInfo->readWriteInfo->addresEE, deviceInfo->readWriteInfo->memoryEESize, 1);
                        }
                        else
                        {
                            if(forEmpty)
                                memset(usb->memCODE + deviceInfo->readWriteInfo->addresEE, 0xFF, deviceInfo->readWriteInfo->memoryEESize);

                            ui->wdgHexEditorEEProm->loadFromDevice( usb->memCODE + deviceInfo->readWriteInfo->addresEE, usb->size-deviceInfo->readWriteInfo->addresEE, deviceInfo->readWriteInfo->memoryEESize);
                        }
                    }
                }
                else
                {
                    if(loadEEProm)
                    {
                        if(forEmpty)
                            clearMemory(usb->memEE, usb->sizeEE, deviceInfo->readWriteInfo->memoryEESize, fillValue);

                        ui->wdgHexEditorEEProm->loadFromDevice(usb->memEE, usb->sizeEE,  deviceInfo->readWriteInfo->memoryEESize);
                    }
                }
            }
        }
        else
            if(deviceInfo->readWriteInfo->hasEE && loadEEProm)
            {
                if(forEmpty)
                    clearMemory(usb->memEE, usb->sizeEE, deviceInfo->readWriteInfo->memoryFlashSize, fillValue);

                ui->wdgHexEditorEEProm->loadFromDevice(usb->memEE, usb->sizeEE, deviceInfo->readWriteInfo->memoryFlashSize);
            }
        if(loadFlash)
            ui->twdgShowBuffers->setCurrentIndex(0);
        else
            ui->twdgShowBuffers->setCurrentIndex(1);
    }
    if(deviceFamily->familyName=="PIC")
        ui->wdgFuseSettings->loadPicFuseSettingsFromUSB();

}

void MainWindow::writeChip()
{
    usb->ProgID();

    //QString av="Writing";
    // statusBar()->showMessage(av);


    //ui->txtLog->clear();
    ui->txtLog->appendPlainText(deviceInfo->name+" size=0x"+QString::number(deviceInfo->readWriteInfo->memoryFlashSize,16));


    //if( usb->CheckS1()==1)
    //  else goto labtr;
    deviceInfo->readWriteInfo->write(2);
}

void MainWindow::detectHardware()
{
    usb->findDevice();
    usb->ProgID();
}

void MainWindow::io_show()
{
    ioCheck.showNormal();
}

void MainWindow::con_show()
{
    conv.showNormal();
}

void MainWindow::Icd_show()
{
    icd.showNormal();
}

void MainWindow::Dis_asm_show()
{
    dis_asm.showNormal();
}


void MainWindow::newFile()
{
    qDebug()<<"new";
}

void MainWindow::open()
{
    qDebug()<<"open";
    // fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"", tr("Files (*.hex)"));

    if (maybeSave())
    {
        fileName=settings->value("FileName", "").toString();
        fileName = QFileDialog::getOpenFileName(this, "", fileName,  tr("HEX (*hex);; BIN (*.bin);;HEX & bin (*hex *bin);; All files (*.*)") );

        settings->setValue("FileName",fileName);
        if (!fileName.isEmpty() && this->deviceInfo)
        {
            //ui->wdgHexEditor->setBitLength(8);
            //ui->wdgHexEditorFlash->loadFile(fileName);
            //usb->memEE = ui->wdgHexEditorFlash->buffer;

            fileIO.load(this->deviceInfo, fileName);
            if(this->deviceInfo->name.mid(0, 2)=="AT")
                loadFromBuffer(MemoryType::Flash);
            else
                loadFromBuffer(MemoryType::Flash_EEProm);
        }
    }
}

void MainWindow::openEE()
{
    if (maybeSave())
    {
        fileName=settings->value("FileName", "").toString();
        fileName = QFileDialog::getOpenFileName(this, "", fileName,  tr("HEX (*hex);; BIN (*.bin);;HEX & bin (*hex *bin);; All files (*.*)") );
        settings->setValue("FileName",fileName);
        if (!fileName.isEmpty())
        {
            //ui->wdgHexEditor->setBitLength(8);
            //ui->wdgHexEditorFlash->loadFile(fileName);
            //usb->memEE = ui->wdgHexEditorFlash->buffer;

            fileIO.loadEE(this->deviceInfo->name, fileName);
            loadFromBuffer(MemoryType::EEProm);
        }
    }
}



bool MainWindow::maybeSave()
//! [40] //! [41]
{
    if (save_falg)
    {
        save_falg=0;
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                                   tr("The Hexeditor has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}


bool MainWindow::save()
{
    qDebug()<<"save";
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly )) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << ui->txtLog->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif


    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}




bool MainWindow::saveAs()
{
    qDebug()<<"saveAs";
    fileName = QFileDialog::getSaveFileName(this, tr("Save as File"),"",  tr("HEX (*hex);; BIN (*.bin);;HEX & bin (*hex *bin);; All files (*.*)"));
    if(fileName!="")
        fileIO.save(this->deviceInfo, fileName);
    // return save();
    return true;
}



void MainWindow::on_Progress_Changed(int step, int max)
{
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(step);
    // qDebug()<<step<<max;
}

void MainWindow::Print_Status(QString msg11, int j1, int i1)
{
    QString str;


    str.sprintf(msg11.toUtf8(),j1,i1 );
    statusBar()->showMessage(str);
    //mainWindow->statusBar()->showMessage
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    ui->txtLog->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}


void MainWindow::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
    curFile = fileName;
    ui->txtLog->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.hex";
    setWindowFilePath(shownName);
}


MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::on_MainWindow_toolButtonStyleChanged(const Qt::ToolButtonStyle &toolButtonStyle)
{

}


void MainWindow::on_DeviceSelected(DeviceFamily *family, DeviceType *type, DeviceInfo *deviceInfo)
{
    this->deviceFamily=family;
    this->deviceType=type;
    this->deviceInfo=deviceInfo;
    ui->wdgFuseSettings->setDeviceInfo(deviceInfo, usb);


    ui->mnuOpenEEProm->setEnabled(this->deviceInfo->name.startsWith("AT"));
    if(deviceFamily->familyName=="Memory"||deviceFamily->familyName=="One Wire")
    {
        //ui->twdgShowBuffers->widget();
        ui->twdgShowBuffers->setTabEnabled(0, false);
        ui->twdgShowBuffers->setTabEnabled(2, false);
    }
    else
    {
        ui->twdgShowBuffers->setTabEnabled(0, true);
        ui->twdgShowBuffers->setTabEnabled(2, true);
    }
    if(deviceFamily->familyName=="AVR")
    {
        ui->wdgFuseSettings->ui->stackedWidget->setCurrentIndex(7);
        ui->wdgFuseSettings->loadAVRFuseSettings();
    }
    else if(deviceFamily->familyName=="PIC")
    {
        ui->wdgFuseSettings->ui->stackedWidget->setCurrentIndex(6);
        ui->wdgFuseSettings->loadPICFuseSettings();
        if(deviceInfo->picInfo)
        {
            if(deviceInfo->picInfo->memoriesInfo.code)
            {
                qDebug()<<"code.size:"<< deviceInfo->picInfo->memoriesInfo.code->size() << "memoryFlashSize:"<<deviceInfo->readWriteInfo->memoryFlashSize;
            }
        }
    }
    else
        ui->wdgFuseSettings->ui->stackedWidget->setCurrentIndex(0);


    //usb->sizeW=deviceInfo->readWriteInfo->memoryFlashSize;
    //usb->sizeEE=deviceInfo->readWriteInfo->memoryEESize;

    ui->wdgHexEditorFlash->setBitLength(16);
    // ui->wdgHexEditorFlash->setSize( deviceInfo->readWriteInfo->memoryFlashSize);

    ui->wdgHexEditorEEProm->setBitLength(8);
    //ui->wdgHexEditorEEProm->setSize(deviceInfo->readWriteInfo->memoryEESize);


    //ui->txtLog->clear();
    ui->txtLog->appendPlainText(deviceInfo->name+" size=0x"+QString::number(deviceInfo->readWriteInfo->memoryFlashSize,16));

    //qDebug()<<family->familyName<<type->typeName<<deviceInfo->name<<deviceInfo->readWriteInfo->memoryFlashSize<<deviceInfo->readWriteInfo->memoryEESize;
    //+QString::number(deviceInfo->readWriteInfo->memoryEESize,16)
    saveSettings();

    loadFromBuffer(MemoryType::Flash_EEProm, true);


    // ui->wdgFuseSettings->getUi().



}
