/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <widgets/deviceListWidget.h>
#include "widgets/HexEditor/hexEditor.h"
#include "widgets/fuseSettings/fuseSettings.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *mnuOpen;
    QAction *actionmnuMost_recent_file;
    QAction *actionUpdate_file;
    QAction *action_2;
    QAction *mnuSave;
    QAction *actionSave_as;
    QAction *actionExit;
    QAction *actionEnglesh;
    QAction *action_3;
    QAction *actionRead_All;
    QAction *actionProgram_All;
    QAction *actionVerify_All;
    QAction *actionErase_All;
    QAction *actionDetect_hipC;
    QAction *actionBlanck_Check;
    QAction *actionHardware_Chek;
    QAction *actionIo_check;
    QAction *actionSoftware;
    QAction *actionRead_fuse_bit;
    QAction *actionWrite_Fuse_bit;
    QAction *actionFill_Bufer;
    QAction *actionClaer_Bufer;
    QAction *actionCopy_bUfeer;
    QAction *actionPaste_to_Buffer;
    QAction *actionActive_Buffer;
    QAction *actionCompaer_Buffer;
    QAction *actionPic;
    QAction *actionAvr;
    QAction *actionHadijon;
    QAction *actionInformation_of_the_Device;
    QAction *actionDIP_ZIF_Socket_Device_position;
    QAction *actionIcd;
    QAction *actionConverter;
    QAction *actionDis_Asmbler;
    QAction *mnuOpenEEProm;
    QAction *actionAbout_MultiProgrammer;
    QAction *actionAbout_SignalGroup;
    QAction *actionAbout_BSP;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    DeviceListWidget *deviceListWidget;
    QTabWidget *twdgShowBuffers;
    QWidget *tab;
    QGridLayout *gridLayout_4;
    HexEditor *wdgHexEditorFlash;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    HexEditor *wdgHexEditorEEProm;
    QWidget *AvrFuseBit;
    QGridLayout *gridLayout_5;
    FuseSettings *wdgFuseSettings;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QProgressBar *progressBar;
    QPlainTextEdit *txtLog;
    QMenuBar *menuBar;
    QMenu *mnuFile;
    QMenu *mnuFile_2;
    QMenu *mnuSeting;
    QMenu *mnuLanguge;
    QMenu *mnuHelp;
    QMenu *mnuCommand;
    QMenu *menuUtilites;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;
    QToolBar *toolBar_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(740, 670);
        MainWindow->setMinimumSize(QSize(740, 0));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/CHIP.ICO"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setWindowOpacity(1.000000000000000);
        MainWindow->setInputMethodHints(Qt::ImhTime);
        MainWindow->setIconSize(QSize(32, 32));
        MainWindow->setDocumentMode(true);
        MainWindow->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::VerticalTabs);
        mnuOpen = new QAction(MainWindow);
        mnuOpen->setObjectName(QString::fromUtf8("mnuOpen"));
        mnuOpen->setCheckable(false);
        mnuOpen->setChecked(false);
        mnuOpen->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        mnuOpen->setIcon(icon1);
        actionmnuMost_recent_file = new QAction(MainWindow);
        actionmnuMost_recent_file->setObjectName(QString::fromUtf8("actionmnuMost_recent_file"));
        actionUpdate_file = new QAction(MainWindow);
        actionUpdate_file->setObjectName(QString::fromUtf8("actionUpdate_file"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icon/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUpdate_file->setIcon(icon2);
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        mnuSave = new QAction(MainWindow);
        mnuSave->setObjectName(QString::fromUtf8("mnuSave"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        mnuSave->setIcon(icon3);
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        actionSave_as->setIcon(icon3);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icon/Exit.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon4);
        actionEnglesh = new QAction(MainWindow);
        actionEnglesh->setObjectName(QString::fromUtf8("actionEnglesh"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icon/eng.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionEnglesh->setIcon(icon5);
        action_3 = new QAction(MainWindow);
        action_3->setObjectName(QString::fromUtf8("action_3"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icon/ir.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        action_3->setIcon(icon6);
        actionRead_All = new QAction(MainWindow);
        actionRead_All->setObjectName(QString::fromUtf8("actionRead_All"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icon/read.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRead_All->setIcon(icon7);
        actionProgram_All = new QAction(MainWindow);
        actionProgram_All->setObjectName(QString::fromUtf8("actionProgram_All"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/write.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionProgram_All->setIcon(icon8);
        actionVerify_All = new QAction(MainWindow);
        actionVerify_All->setObjectName(QString::fromUtf8("actionVerify_All"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icon/ICOON.ICO"), QSize(), QIcon::Normal, QIcon::Off);
        actionVerify_All->setIcon(icon9);
        actionErase_All = new QAction(MainWindow);
        actionErase_All->setObjectName(QString::fromUtf8("actionErase_All"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icon/ICOOFF.ICO"), QSize(), QIcon::Normal, QIcon::Off);
        actionErase_All->setIcon(icon10);
        actionDetect_hipC = new QAction(MainWindow);
        actionDetect_hipC->setObjectName(QString::fromUtf8("actionDetect_hipC"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icon/ArtWork Icon 16.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionDetect_hipC->setIcon(icon11);
        actionBlanck_Check = new QAction(MainWindow);
        actionBlanck_Check->setObjectName(QString::fromUtf8("actionBlanck_Check"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icon/Pld_cupl.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionBlanck_Check->setIcon(icon12);
        actionHardware_Chek = new QAction(MainWindow);
        actionHardware_Chek->setObjectName(QString::fromUtf8("actionHardware_Chek"));
        actionHardware_Chek->setCheckable(false);
        actionHardware_Chek->setChecked(false);
        actionHardware_Chek->setEnabled(true);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icon/pcb3d.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionHardware_Chek->setIcon(icon13);
        actionIo_check = new QAction(MainWindow);
        actionIo_check->setObjectName(QString::fromUtf8("actionIo_check"));
        actionSoftware = new QAction(MainWindow);
        actionSoftware->setObjectName(QString::fromUtf8("actionSoftware"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/icon/ASCIITable.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionSoftware->setIcon(icon14);
        actionRead_fuse_bit = new QAction(MainWindow);
        actionRead_fuse_bit->setObjectName(QString::fromUtf8("actionRead_fuse_bit"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/icon/readtqfp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRead_fuse_bit->setIcon(icon15);
        actionWrite_Fuse_bit = new QAction(MainWindow);
        actionWrite_Fuse_bit->setObjectName(QString::fromUtf8("actionWrite_Fuse_bit"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/icon/writetqfp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWrite_Fuse_bit->setIcon(icon16);
        actionFill_Bufer = new QAction(MainWindow);
        actionFill_Bufer->setObjectName(QString::fromUtf8("actionFill_Bufer"));
        actionFill_Bufer->setCheckable(false);
        actionFill_Bufer->setEnabled(true);
        QIcon icon17;
        icon17.addFile(QString::fromUtf8("icon/ManufacturedItem.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionFill_Bufer->setIcon(icon17);
        actionClaer_Bufer = new QAction(MainWindow);
        actionClaer_Bufer->setObjectName(QString::fromUtf8("actionClaer_Bufer"));
        actionClaer_Bufer->setIcon(icon10);
        actionCopy_bUfeer = new QAction(MainWindow);
        actionCopy_bUfeer->setObjectName(QString::fromUtf8("actionCopy_bUfeer"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/icon/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy_bUfeer->setIcon(icon18);
        actionPaste_to_Buffer = new QAction(MainWindow);
        actionPaste_to_Buffer->setObjectName(QString::fromUtf8("actionPaste_to_Buffer"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/images/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste_to_Buffer->setIcon(icon19);
        actionActive_Buffer = new QAction(MainWindow);
        actionActive_Buffer->setObjectName(QString::fromUtf8("actionActive_Buffer"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/icon/Chip_fp2.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionActive_Buffer->setIcon(icon20);
        actionCompaer_Buffer = new QAction(MainWindow);
        actionCompaer_Buffer->setObjectName(QString::fromUtf8("actionCompaer_Buffer"));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/icon/activate.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCompaer_Buffer->setIcon(icon21);
        actionPic = new QAction(MainWindow);
        actionPic->setObjectName(QString::fromUtf8("actionPic"));
        actionPic->setCheckable(true);
        actionPic->setChecked(true);
        QIcon icon22;
        icon22.addFile(QString::fromUtf8("icon/Microchip.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionPic->setIcon(icon22);
        actionPic->setIconVisibleInMenu(false);
        actionAvr = new QAction(MainWindow);
        actionAvr->setObjectName(QString::fromUtf8("actionAvr"));
        actionHadijon = new QAction(MainWindow);
        actionHadijon->setObjectName(QString::fromUtf8("actionHadijon"));
        actionInformation_of_the_Device = new QAction(MainWindow);
        actionInformation_of_the_Device->setObjectName(QString::fromUtf8("actionInformation_of_the_Device"));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/icon/Crayin Icon 35.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionInformation_of_the_Device->setIcon(icon23);
        actionDIP_ZIF_Socket_Device_position = new QAction(MainWindow);
        actionDIP_ZIF_Socket_Device_position->setObjectName(QString::fromUtf8("actionDIP_ZIF_Socket_Device_position"));
        QIcon icon24;
        icon24.addFile(QString::fromUtf8(":/icon/critical.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionDIP_ZIF_Socket_Device_position->setIcon(icon24);
        actionIcd = new QAction(MainWindow);
        actionIcd->setObjectName(QString::fromUtf8("actionIcd"));
        QIcon icon25;
        icon25.addFile(QString::fromUtf8(":/icon/ArtWork Icon 38.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionIcd->setIcon(icon25);
        actionConverter = new QAction(MainWindow);
        actionConverter->setObjectName(QString::fromUtf8("actionConverter"));
        QIcon icon26;
        icon26.addFile(QString::fromUtf8(":/icon/Tracer Icon 16.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionConverter->setIcon(icon26);
        actionDis_Asmbler = new QAction(MainWindow);
        actionDis_Asmbler->setObjectName(QString::fromUtf8("actionDis_Asmbler"));
        QIcon icon27;
        icon27.addFile(QString::fromUtf8(":/icon/ArtWork Icon 72.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionDis_Asmbler->setIcon(icon27);
        mnuOpenEEProm = new QAction(MainWindow);
        mnuOpenEEProm->setObjectName(QString::fromUtf8("mnuOpenEEProm"));
        mnuOpenEEProm->setIcon(icon1);
        actionAbout_MultiProgrammer = new QAction(MainWindow);
        actionAbout_MultiProgrammer->setObjectName(QString::fromUtf8("actionAbout_MultiProgrammer"));
        actionAbout_SignalGroup = new QAction(MainWindow);
        actionAbout_SignalGroup->setObjectName(QString::fromUtf8("actionAbout_SignalGroup"));
        actionAbout_BSP = new QAction(MainWindow);
        actionAbout_BSP->setObjectName(QString::fromUtf8("actionAbout_BSP"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        deviceListWidget = new DeviceListWidget(centralWidget);
        deviceListWidget->setObjectName(QString::fromUtf8("deviceListWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(deviceListWidget->sizePolicy().hasHeightForWidth());
        deviceListWidget->setSizePolicy(sizePolicy);
        deviceListWidget->setMinimumSize(QSize(500, 40));
        deviceListWidget->setMaximumSize(QSize(2200, 50));
        deviceListWidget->setLayoutDirection(Qt::LeftToRight);

        gridLayout_2->addWidget(deviceListWidget, 0, 0, 1, 1);

        twdgShowBuffers = new QTabWidget(centralWidget);
        twdgShowBuffers->setObjectName(QString::fromUtf8("twdgShowBuffers"));
        twdgShowBuffers->setMinimumSize(QSize(0, 350));
        twdgShowBuffers->setTabPosition(QTabWidget::South);
        twdgShowBuffers->setTabShape(QTabWidget::Triangular);
        twdgShowBuffers->setMovable(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_4 = new QGridLayout(tab);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        wdgHexEditorFlash = new HexEditor(tab);
        wdgHexEditorFlash->setObjectName(QString::fromUtf8("wdgHexEditorFlash"));

        gridLayout_4->addWidget(wdgHexEditorFlash, 0, 0, 1, 1);

        twdgShowBuffers->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        wdgHexEditorEEProm = new HexEditor(tab_2);
        wdgHexEditorEEProm->setObjectName(QString::fromUtf8("wdgHexEditorEEProm"));

        gridLayout_3->addWidget(wdgHexEditorEEProm, 0, 0, 1, 1);

        twdgShowBuffers->addTab(tab_2, QString());
        AvrFuseBit = new QWidget();
        AvrFuseBit->setObjectName(QString::fromUtf8("AvrFuseBit"));
        gridLayout_5 = new QGridLayout(AvrFuseBit);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        wdgFuseSettings = new FuseSettings(AvrFuseBit);
        wdgFuseSettings->setObjectName(QString::fromUtf8("wdgFuseSettings"));

        gridLayout_5->addWidget(wdgFuseSettings, 0, 0, 1, 1);

        twdgShowBuffers->addTab(AvrFuseBit, QString());

        gridLayout_2->addWidget(twdgShowBuffers, 1, 0, 1, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMinimumSize(QSize(0, 15));
        progressBar->setMaximumSize(QSize(16777215, 15));
        progressBar->setValue(0);

        gridLayout->addWidget(progressBar, 1, 0, 1, 1);

        txtLog = new QPlainTextEdit(groupBox);
        txtLog->setObjectName(QString::fromUtf8("txtLog"));
        txtLog->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        gridLayout->addWidget(txtLog, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 740, 20));
        mnuFile = new QMenu(menuBar);
        mnuFile->setObjectName(QString::fromUtf8("mnuFile"));
        mnuFile_2 = new QMenu(menuBar);
        mnuFile_2->setObjectName(QString::fromUtf8("mnuFile_2"));
        mnuSeting = new QMenu(menuBar);
        mnuSeting->setObjectName(QString::fromUtf8("mnuSeting"));
        mnuLanguge = new QMenu(menuBar);
        mnuLanguge->setObjectName(QString::fromUtf8("mnuLanguge"));
        mnuHelp = new QMenu(menuBar);
        mnuHelp->setObjectName(QString::fromUtf8("mnuHelp"));
        mnuCommand = new QMenu(menuBar);
        mnuCommand->setObjectName(QString::fromUtf8("mnuCommand"));
        menuUtilites = new QMenu(menuBar);
        menuUtilites->setObjectName(QString::fromUtf8("menuUtilites"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar_2 = new QToolBar(MainWindow);
        toolBar_2->setObjectName(QString::fromUtf8("toolBar_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(toolBar_2->sizePolicy().hasHeightForWidth());
        toolBar_2->setSizePolicy(sizePolicy1);
        toolBar_2->setMinimumSize(QSize(0, 0));
        toolBar_2->setBaseSize(QSize(0, 40));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_2);
        QWidget::setTabOrder(txtLog, twdgShowBuffers);

        menuBar->addAction(mnuFile->menuAction());
        menuBar->addAction(mnuFile_2->menuAction());
        menuBar->addAction(mnuCommand->menuAction());
        menuBar->addAction(mnuSeting->menuAction());
        menuBar->addAction(menuUtilites->menuAction());
        menuBar->addAction(mnuLanguge->menuAction());
        menuBar->addAction(mnuHelp->menuAction());
        mnuFile->addSeparator();
        mnuFile->addAction(mnuOpen);
        mnuFile->addAction(mnuOpenEEProm);
        mnuFile->addAction(actionmnuMost_recent_file);
        mnuFile->addSeparator();
        mnuFile->addAction(actionUpdate_file);
        mnuFile->addSeparator();
        mnuFile->addAction(mnuSave);
        mnuFile->addAction(actionSave_as);
        mnuFile->addSeparator();
        mnuFile->addAction(actionExit);
        mnuFile_2->addAction(actionFill_Bufer);
        mnuFile_2->addAction(actionClaer_Bufer);
        mnuFile_2->addSeparator();
        mnuFile_2->addAction(actionPaste_to_Buffer);
        mnuFile_2->addAction(actionCopy_bUfeer);
        mnuFile_2->addSeparator();
        mnuFile_2->addAction(actionCompaer_Buffer);
        mnuFile_2->addAction(actionActive_Buffer);
        mnuFile_2->addSeparator();
        mnuSeting->addAction(actionHardware_Chek);
        mnuSeting->addSeparator();
        mnuSeting->addAction(actionSoftware);
        mnuLanguge->addAction(actionEnglesh);
        mnuLanguge->addAction(action_3);
        mnuHelp->addAction(actionAbout_MultiProgrammer);
        mnuHelp->addAction(actionAbout_SignalGroup);
        mnuCommand->addAction(actionDetect_hipC);
        mnuCommand->addAction(actionBlanck_Check);
        mnuCommand->addSeparator();
        mnuCommand->addAction(actionRead_All);
        mnuCommand->addAction(actionProgram_All);
        mnuCommand->addAction(actionVerify_All);
        mnuCommand->addAction(actionErase_All);
        mnuCommand->addSeparator();
        mnuCommand->addAction(actionRead_fuse_bit);
        mnuCommand->addAction(actionWrite_Fuse_bit);
        mnuCommand->addSeparator();
        mnuCommand->addAction(actionInformation_of_the_Device);
        mnuCommand->addAction(actionDIP_ZIF_Socket_Device_position);
        menuUtilites->addAction(actionIcd);
        menuUtilites->addAction(actionConverter);
        menuUtilites->addAction(actionDis_Asmbler);
        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        twdgShowBuffers->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MultiProg", nullptr));
        mnuOpen->setText(QCoreApplication::translate("MainWindow", "Open ", nullptr));
        actionmnuMost_recent_file->setText(QCoreApplication::translate("MainWindow", "Most Recent file", nullptr));
        actionUpdate_file->setText(QCoreApplication::translate("MainWindow", "Update file", nullptr));
        action_2->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        mnuSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        actionSave_as->setText(QCoreApplication::translate("MainWindow", "Save as", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionEnglesh->setText(QCoreApplication::translate("MainWindow", "Englesh", nullptr));
        action_3->setText(QCoreApplication::translate("MainWindow", "\331\201\330\247\330\261\330\263\331\212", nullptr));
        actionRead_All->setText(QCoreApplication::translate("MainWindow", "Read All", nullptr));
        actionProgram_All->setText(QCoreApplication::translate("MainWindow", "Program All", nullptr));
        actionVerify_All->setText(QCoreApplication::translate("MainWindow", "Verify All", nullptr));
        actionErase_All->setText(QCoreApplication::translate("MainWindow", "Erase All", nullptr));
        actionDetect_hipC->setText(QCoreApplication::translate("MainWindow", "Detect Chip", nullptr));
        actionBlanck_Check->setText(QCoreApplication::translate("MainWindow", "Blanck Check", nullptr));
        actionHardware_Chek->setText(QCoreApplication::translate("MainWindow", "Hardware Check", nullptr));
        actionIo_check->setText(QCoreApplication::translate("MainWindow", "Io check", nullptr));
        actionSoftware->setText(QCoreApplication::translate("MainWindow", "Software", nullptr));
        actionRead_fuse_bit->setText(QCoreApplication::translate("MainWindow", "Read fuse bit", nullptr));
        actionWrite_Fuse_bit->setText(QCoreApplication::translate("MainWindow", "Write Fuse bit", nullptr));
        actionFill_Bufer->setText(QCoreApplication::translate("MainWindow", "Fill Bufer", nullptr));
        actionClaer_Bufer->setText(QCoreApplication::translate("MainWindow", "Claer Bufer", nullptr));
        actionCopy_bUfeer->setText(QCoreApplication::translate("MainWindow", "Copy bUfeer", nullptr));
        actionPaste_to_Buffer->setText(QCoreApplication::translate("MainWindow", "Paste to Buffer", nullptr));
        actionActive_Buffer->setText(QCoreApplication::translate("MainWindow", "Active Buffer", nullptr));
        actionCompaer_Buffer->setText(QCoreApplication::translate("MainWindow", "Compaer Buffer", nullptr));
        actionPic->setText(QCoreApplication::translate("MainWindow", "Microchip Pic", nullptr));
        actionPic->setIconText(QCoreApplication::translate("MainWindow", "Microchip Pic", nullptr));
        actionAvr->setText(QCoreApplication::translate("MainWindow", "Atmel Avr", nullptr));
        actionHadijon->setText(QCoreApplication::translate("MainWindow", "hadijon", nullptr));
        actionInformation_of_the_Device->setText(QCoreApplication::translate("MainWindow", "Information of the Device", nullptr));
        actionDIP_ZIF_Socket_Device_position->setText(QCoreApplication::translate("MainWindow", "DIP ZIF Socket Device position", nullptr));
        actionIcd->setText(QCoreApplication::translate("MainWindow", "ICD", nullptr));
        actionConverter->setText(QCoreApplication::translate("MainWindow", "Converter", nullptr));
        actionDis_Asmbler->setText(QCoreApplication::translate("MainWindow", "Dis-Asmbler", nullptr));
        mnuOpenEEProm->setText(QCoreApplication::translate("MainWindow", "Open EEProm", nullptr));
        actionAbout_MultiProgrammer->setText(QCoreApplication::translate("MainWindow", "About MultiProgrammer", nullptr));
        actionAbout_SignalGroup->setText(QCoreApplication::translate("MainWindow", "About SignalGroup", nullptr));
        actionAbout_BSP->setText(QCoreApplication::translate("MainWindow", "About BSP", nullptr));
        twdgShowBuffers->setTabText(twdgShowBuffers->indexOf(tab), QCoreApplication::translate("MainWindow", "Flash", nullptr));
        twdgShowBuffers->setTabText(twdgShowBuffers->indexOf(tab_2), QCoreApplication::translate("MainWindow", "EEprom", nullptr));
        twdgShowBuffers->setTabText(twdgShowBuffers->indexOf(AvrFuseBit), QCoreApplication::translate("MainWindow", " Fuse Bit", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Log", nullptr));
        mnuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        mnuFile_2->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        mnuSeting->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        mnuLanguge->setTitle(QCoreApplication::translate("MainWindow", "Languge", nullptr));
        mnuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        mnuCommand->setTitle(QCoreApplication::translate("MainWindow", "Command", nullptr));
        menuUtilites->setTitle(QCoreApplication::translate("MainWindow", "Utilites", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
        toolBar_2->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar_2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
