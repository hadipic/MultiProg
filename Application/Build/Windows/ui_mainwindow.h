/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
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
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(740, 670);
        MainWindow->setMinimumSize(QSize(740, 0));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/CHIP.ICO"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setWindowOpacity(1);
        MainWindow->setInputMethodHints(Qt::ImhTime);
        MainWindow->setIconSize(QSize(32, 32));
        MainWindow->setDocumentMode(true);
        MainWindow->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::VerticalTabs);
        mnuOpen = new QAction(MainWindow);
        mnuOpen->setObjectName(QStringLiteral("mnuOpen"));
        mnuOpen->setCheckable(false);
        mnuOpen->setChecked(false);
        mnuOpen->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        mnuOpen->setIcon(icon1);
        actionmnuMost_recent_file = new QAction(MainWindow);
        actionmnuMost_recent_file->setObjectName(QStringLiteral("actionmnuMost_recent_file"));
        actionUpdate_file = new QAction(MainWindow);
        actionUpdate_file->setObjectName(QStringLiteral("actionUpdate_file"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icon/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUpdate_file->setIcon(icon2);
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QStringLiteral("action_2"));
        mnuSave = new QAction(MainWindow);
        mnuSave->setObjectName(QStringLiteral("mnuSave"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        mnuSave->setIcon(icon3);
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QStringLiteral("actionSave_as"));
        actionSave_as->setIcon(icon3);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icon/Exit.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon4);
        actionEnglesh = new QAction(MainWindow);
        actionEnglesh->setObjectName(QStringLiteral("actionEnglesh"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icon/eng.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        actionEnglesh->setIcon(icon5);
        action_3 = new QAction(MainWindow);
        action_3->setObjectName(QStringLiteral("action_3"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/icon/ir.PNG"), QSize(), QIcon::Normal, QIcon::Off);
        action_3->setIcon(icon6);
        actionRead_All = new QAction(MainWindow);
        actionRead_All->setObjectName(QStringLiteral("actionRead_All"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icon/read.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRead_All->setIcon(icon7);
        actionProgram_All = new QAction(MainWindow);
        actionProgram_All->setObjectName(QStringLiteral("actionProgram_All"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/write.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionProgram_All->setIcon(icon8);
        actionVerify_All = new QAction(MainWindow);
        actionVerify_All->setObjectName(QStringLiteral("actionVerify_All"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icon/ICOON.ICO"), QSize(), QIcon::Normal, QIcon::Off);
        actionVerify_All->setIcon(icon9);
        actionErase_All = new QAction(MainWindow);
        actionErase_All->setObjectName(QStringLiteral("actionErase_All"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icon/ICOOFF.ICO"), QSize(), QIcon::Normal, QIcon::Off);
        actionErase_All->setIcon(icon10);
        actionDetect_hipC = new QAction(MainWindow);
        actionDetect_hipC->setObjectName(QStringLiteral("actionDetect_hipC"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icon/ArtWork Icon 16.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionDetect_hipC->setIcon(icon11);
        actionBlanck_Check = new QAction(MainWindow);
        actionBlanck_Check->setObjectName(QStringLiteral("actionBlanck_Check"));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/icon/Pld_cupl.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionBlanck_Check->setIcon(icon12);
        actionHardware_Chek = new QAction(MainWindow);
        actionHardware_Chek->setObjectName(QStringLiteral("actionHardware_Chek"));
        actionHardware_Chek->setCheckable(false);
        actionHardware_Chek->setChecked(false);
        actionHardware_Chek->setEnabled(true);
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/icon/pcb3d.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionHardware_Chek->setIcon(icon13);
        actionIo_check = new QAction(MainWindow);
        actionIo_check->setObjectName(QStringLiteral("actionIo_check"));
        actionSoftware = new QAction(MainWindow);
        actionSoftware->setObjectName(QStringLiteral("actionSoftware"));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/icon/ASCIITable.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionSoftware->setIcon(icon14);
        actionRead_fuse_bit = new QAction(MainWindow);
        actionRead_fuse_bit->setObjectName(QStringLiteral("actionRead_fuse_bit"));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/icon/readtqfp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRead_fuse_bit->setIcon(icon15);
        actionWrite_Fuse_bit = new QAction(MainWindow);
        actionWrite_Fuse_bit->setObjectName(QStringLiteral("actionWrite_Fuse_bit"));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/icon/writetqfp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWrite_Fuse_bit->setIcon(icon16);
        actionFill_Bufer = new QAction(MainWindow);
        actionFill_Bufer->setObjectName(QStringLiteral("actionFill_Bufer"));
        actionFill_Bufer->setCheckable(false);
        actionFill_Bufer->setEnabled(true);
        QIcon icon17;
        icon17.addFile(QStringLiteral("icon/ManufacturedItem.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionFill_Bufer->setIcon(icon17);
        actionClaer_Bufer = new QAction(MainWindow);
        actionClaer_Bufer->setObjectName(QStringLiteral("actionClaer_Bufer"));
        actionClaer_Bufer->setIcon(icon10);
        actionCopy_bUfeer = new QAction(MainWindow);
        actionCopy_bUfeer->setObjectName(QStringLiteral("actionCopy_bUfeer"));
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/icon/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy_bUfeer->setIcon(icon18);
        actionPaste_to_Buffer = new QAction(MainWindow);
        actionPaste_to_Buffer->setObjectName(QStringLiteral("actionPaste_to_Buffer"));
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/images/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste_to_Buffer->setIcon(icon19);
        actionActive_Buffer = new QAction(MainWindow);
        actionActive_Buffer->setObjectName(QStringLiteral("actionActive_Buffer"));
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/icon/Chip_fp2.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionActive_Buffer->setIcon(icon20);
        actionCompaer_Buffer = new QAction(MainWindow);
        actionCompaer_Buffer->setObjectName(QStringLiteral("actionCompaer_Buffer"));
        QIcon icon21;
        icon21.addFile(QStringLiteral(":/icon/activate.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCompaer_Buffer->setIcon(icon21);
        actionPic = new QAction(MainWindow);
        actionPic->setObjectName(QStringLiteral("actionPic"));
        actionPic->setCheckable(true);
        actionPic->setChecked(true);
        QIcon icon22;
        icon22.addFile(QStringLiteral("icon/Microchip.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionPic->setIcon(icon22);
        actionPic->setIconVisibleInMenu(false);
        actionAvr = new QAction(MainWindow);
        actionAvr->setObjectName(QStringLiteral("actionAvr"));
        actionHadijon = new QAction(MainWindow);
        actionHadijon->setObjectName(QStringLiteral("actionHadijon"));
        actionInformation_of_the_Device = new QAction(MainWindow);
        actionInformation_of_the_Device->setObjectName(QStringLiteral("actionInformation_of_the_Device"));
        QIcon icon23;
        icon23.addFile(QStringLiteral(":/icon/Crayin Icon 35.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionInformation_of_the_Device->setIcon(icon23);
        actionDIP_ZIF_Socket_Device_position = new QAction(MainWindow);
        actionDIP_ZIF_Socket_Device_position->setObjectName(QStringLiteral("actionDIP_ZIF_Socket_Device_position"));
        QIcon icon24;
        icon24.addFile(QStringLiteral(":/icon/critical.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionDIP_ZIF_Socket_Device_position->setIcon(icon24);
        actionIcd = new QAction(MainWindow);
        actionIcd->setObjectName(QStringLiteral("actionIcd"));
        QIcon icon25;
        icon25.addFile(QStringLiteral(":/icon/ArtWork Icon 38.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionIcd->setIcon(icon25);
        actionConverter = new QAction(MainWindow);
        actionConverter->setObjectName(QStringLiteral("actionConverter"));
        QIcon icon26;
        icon26.addFile(QStringLiteral(":/icon/Tracer Icon 16.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionConverter->setIcon(icon26);
        actionDis_Asmbler = new QAction(MainWindow);
        actionDis_Asmbler->setObjectName(QStringLiteral("actionDis_Asmbler"));
        QIcon icon27;
        icon27.addFile(QStringLiteral(":/icon/ArtWork Icon 72.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionDis_Asmbler->setIcon(icon27);
        mnuOpenEEProm = new QAction(MainWindow);
        mnuOpenEEProm->setObjectName(QStringLiteral("mnuOpenEEProm"));
        mnuOpenEEProm->setIcon(icon1);
        actionAbout_MultiProgrammer = new QAction(MainWindow);
        actionAbout_MultiProgrammer->setObjectName(QStringLiteral("actionAbout_MultiProgrammer"));
        actionAbout_SignalGroup = new QAction(MainWindow);
        actionAbout_SignalGroup->setObjectName(QStringLiteral("actionAbout_SignalGroup"));
        actionAbout_BSP = new QAction(MainWindow);
        actionAbout_BSP->setObjectName(QStringLiteral("actionAbout_BSP"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        deviceListWidget = new DeviceListWidget(centralWidget);
        deviceListWidget->setObjectName(QStringLiteral("deviceListWidget"));
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
        twdgShowBuffers->setObjectName(QStringLiteral("twdgShowBuffers"));
        twdgShowBuffers->setMinimumSize(QSize(0, 350));
        twdgShowBuffers->setTabPosition(QTabWidget::South);
        twdgShowBuffers->setTabShape(QTabWidget::Triangular);
        twdgShowBuffers->setMovable(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_4 = new QGridLayout(tab);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        wdgHexEditorFlash = new HexEditor(tab);
        wdgHexEditorFlash->setObjectName(QStringLiteral("wdgHexEditorFlash"));

        gridLayout_4->addWidget(wdgHexEditorFlash, 0, 0, 1, 1);

        twdgShowBuffers->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        wdgHexEditorEEProm = new HexEditor(tab_2);
        wdgHexEditorEEProm->setObjectName(QStringLiteral("wdgHexEditorEEProm"));

        gridLayout_3->addWidget(wdgHexEditorEEProm, 0, 0, 1, 1);

        twdgShowBuffers->addTab(tab_2, QString());
        AvrFuseBit = new QWidget();
        AvrFuseBit->setObjectName(QStringLiteral("AvrFuseBit"));
        gridLayout_5 = new QGridLayout(AvrFuseBit);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        wdgFuseSettings = new FuseSettings(AvrFuseBit);
        wdgFuseSettings->setObjectName(QStringLiteral("wdgFuseSettings"));

        gridLayout_5->addWidget(wdgFuseSettings, 0, 0, 1, 1);

        twdgShowBuffers->addTab(AvrFuseBit, QString());

        gridLayout_2->addWidget(twdgShowBuffers, 1, 0, 1, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setMinimumSize(QSize(0, 15));
        progressBar->setMaximumSize(QSize(16777215, 15));
        progressBar->setValue(0);

        gridLayout->addWidget(progressBar, 1, 0, 1, 1);

        txtLog = new QPlainTextEdit(groupBox);
        txtLog->setObjectName(QStringLiteral("txtLog"));
        txtLog->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        gridLayout->addWidget(txtLog, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 2, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 740, 20));
        mnuFile = new QMenu(menuBar);
        mnuFile->setObjectName(QStringLiteral("mnuFile"));
        mnuFile_2 = new QMenu(menuBar);
        mnuFile_2->setObjectName(QStringLiteral("mnuFile_2"));
        mnuSeting = new QMenu(menuBar);
        mnuSeting->setObjectName(QStringLiteral("mnuSeting"));
        mnuLanguge = new QMenu(menuBar);
        mnuLanguge->setObjectName(QStringLiteral("mnuLanguge"));
        mnuHelp = new QMenu(menuBar);
        mnuHelp->setObjectName(QStringLiteral("mnuHelp"));
        mnuCommand = new QMenu(menuBar);
        mnuCommand->setObjectName(QStringLiteral("mnuCommand"));
        menuUtilites = new QMenu(menuBar);
        menuUtilites->setObjectName(QStringLiteral("menuUtilites"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar_2 = new QToolBar(MainWindow);
        toolBar_2->setObjectName(QStringLiteral("toolBar_2"));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MultiProg", 0));
        mnuOpen->setText(QApplication::translate("MainWindow", "Open ", 0));
        actionmnuMost_recent_file->setText(QApplication::translate("MainWindow", "Most Recent file", 0));
        actionUpdate_file->setText(QApplication::translate("MainWindow", "Update file", 0));
        action_2->setText(QApplication::translate("MainWindow", "-", 0));
        mnuSave->setText(QApplication::translate("MainWindow", "Save", 0));
        actionSave_as->setText(QApplication::translate("MainWindow", "Save as", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionEnglesh->setText(QApplication::translate("MainWindow", "Englesh", 0));
        action_3->setText(QApplication::translate("MainWindow", "\331\201\330\247\330\261\330\263\331\212", 0));
        actionRead_All->setText(QApplication::translate("MainWindow", "Read All", 0));
        actionProgram_All->setText(QApplication::translate("MainWindow", "Program All", 0));
        actionVerify_All->setText(QApplication::translate("MainWindow", "Verify All", 0));
        actionErase_All->setText(QApplication::translate("MainWindow", "Erase All", 0));
        actionDetect_hipC->setText(QApplication::translate("MainWindow", "Detect Chip", 0));
        actionBlanck_Check->setText(QApplication::translate("MainWindow", "Blanck Check", 0));
        actionHardware_Chek->setText(QApplication::translate("MainWindow", "Hardware Check", 0));
        actionIo_check->setText(QApplication::translate("MainWindow", "Io check", 0));
        actionSoftware->setText(QApplication::translate("MainWindow", "Software", 0));
        actionRead_fuse_bit->setText(QApplication::translate("MainWindow", "Read fuse bit", 0));
        actionWrite_Fuse_bit->setText(QApplication::translate("MainWindow", "Write Fuse bit", 0));
        actionFill_Bufer->setText(QApplication::translate("MainWindow", "Fill Bufer", 0));
        actionClaer_Bufer->setText(QApplication::translate("MainWindow", "Claer Bufer", 0));
        actionCopy_bUfeer->setText(QApplication::translate("MainWindow", "Copy bUfeer", 0));
        actionPaste_to_Buffer->setText(QApplication::translate("MainWindow", "Paste to Buffer", 0));
        actionActive_Buffer->setText(QApplication::translate("MainWindow", "Active Buffer", 0));
        actionCompaer_Buffer->setText(QApplication::translate("MainWindow", "Compaer Buffer", 0));
        actionPic->setText(QApplication::translate("MainWindow", "Microchip Pic", 0));
        actionPic->setIconText(QApplication::translate("MainWindow", "Microchip Pic", 0));
        actionAvr->setText(QApplication::translate("MainWindow", "Atmel Avr", 0));
        actionHadijon->setText(QApplication::translate("MainWindow", "hadijon", 0));
        actionInformation_of_the_Device->setText(QApplication::translate("MainWindow", "Information of the Device", 0));
        actionDIP_ZIF_Socket_Device_position->setText(QApplication::translate("MainWindow", "DIP ZIF Socket Device position", 0));
        actionIcd->setText(QApplication::translate("MainWindow", "ICD", 0));
        actionConverter->setText(QApplication::translate("MainWindow", "Converter", 0));
        actionDis_Asmbler->setText(QApplication::translate("MainWindow", "Dis-Asmbler", 0));
        mnuOpenEEProm->setText(QApplication::translate("MainWindow", "Open EEProm", 0));
        actionAbout_MultiProgrammer->setText(QApplication::translate("MainWindow", "About MultiProgrammer", 0));
        actionAbout_SignalGroup->setText(QApplication::translate("MainWindow", "About SignalGroup", 0));
        actionAbout_BSP->setText(QApplication::translate("MainWindow", "About BSP", 0));
        twdgShowBuffers->setTabText(twdgShowBuffers->indexOf(tab), QApplication::translate("MainWindow", "Flash", 0));
        twdgShowBuffers->setTabText(twdgShowBuffers->indexOf(tab_2), QApplication::translate("MainWindow", "EEprom", 0));
        twdgShowBuffers->setTabText(twdgShowBuffers->indexOf(AvrFuseBit), QApplication::translate("MainWindow", " Fuse Bit", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Log", 0));
        mnuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        mnuFile_2->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        mnuSeting->setTitle(QApplication::translate("MainWindow", "Settings", 0));
        mnuLanguge->setTitle(QApplication::translate("MainWindow", "Languge", 0));
        mnuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        mnuCommand->setTitle(QApplication::translate("MainWindow", "Command", 0));
        menuUtilites->setTitle(QApplication::translate("MainWindow", "Utilites", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
        toolBar_2->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
