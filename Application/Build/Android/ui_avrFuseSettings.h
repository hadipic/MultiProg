/********************************************************************************
** Form generated from reading UI file 'avrFuseSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AVRFUSESETTINGS_H
#define UI_AVRFUSESETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AvrFuseSettings
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabAvrFuseBits;
    QWidget *tab;
    QGridLayout *gridLayout_7;
    QGroupBox *GRb_lowfuse;
    QGridLayout *gridLayout_3;
    QCheckBox *chbAVRLow7;
    QCheckBox *chbAVRLow6;
    QCheckBox *chbAVRLow5;
    QCheckBox *chbAVRLow4;
    QCheckBox *chbAVRLow3;
    QCheckBox *chbAVRLow2;
    QCheckBox *chbAVRLow1;
    QCheckBox *chbAVRLow0;
    QGroupBox *GRb__extendfuse_3;
    QGridLayout *gridLayout_5;
    QCheckBox *chbAVRExt7;
    QCheckBox *chbAVRExt6;
    QCheckBox *chbAVRExt5;
    QCheckBox *chbAVRExt4;
    QCheckBox *chbAVRExt3;
    QCheckBox *chbAVRExt2;
    QCheckBox *chbAVRExt1;
    QCheckBox *chbAVRExt0;
    QGroupBox *GRb_lowfuse_4;
    QGridLayout *gridLayout_6;
    QCheckBox *chbAVRLock7;
    QCheckBox *chbAVRLock6;
    QCheckBox *chbAVRLock5;
    QCheckBox *chbAVRLock4;
    QCheckBox *chbAVRLock3;
    QCheckBox *chbAVRLock2;
    QCheckBox *chbAVRLock1;
    QCheckBox *chbAVRLock0;
    QGroupBox *GRb_highfuse;
    QGridLayout *gridLayout_4;
    QCheckBox *chbAVRHigh7;
    QCheckBox *chbAVRHigh6;
    QCheckBox *chbAVRHigh5;
    QCheckBox *chbAVRHigh4;
    QCheckBox *chbAVRHigh3;
    QCheckBox *chbAVRHigh2;
    QCheckBox *chbAVRHigh1;
    QCheckBox *chbAVRHigh0;
    QGroupBox *groupBox_3;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QTextEdit *txtE_low_2;
    QTextEdit *txtE_low_3;
    QTextEdit *txtE_low_4;
    QTextEdit *txtE_low_5;
    QWidget *tab_2;
    QGridLayout *gridLayout_9;
    QScrollArea *scrollAreaAVRNavigation;
    QWidget *scrollAreaAVRWidgetContents;
    QGridLayout *gridLayout_8;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QPushButton *btnAVRFuzeRead;
    QPushButton *btnAVRFuzeDefault;
    QPushButton *btnAVRFuzeWrite;
    QLabel *La_lowfuse;
    QLabel *La_lowfuse_2;
    QLabel *La_lowfuse_3;
    QTextEdit *txtAVRLow;
    QTextEdit *txtAVRHigh;
    QTextEdit *txtAVRExtend;
    QGroupBox *groupBox_2;
    QPushButton *btnAVRLockWrite;
    QLabel *La_lowfuse_4;
    QTextEdit *txtAVRLock;
    QPushButton *btnAVRLockRead;

    void setupUi(QWidget *AvrFuseSettings)
    {
        if (AvrFuseSettings->objectName().isEmpty())
            AvrFuseSettings->setObjectName(QStringLiteral("AvrFuseSettings"));
        AvrFuseSettings->resize(675, 290);
        gridLayout = new QGridLayout(AvrFuseSettings);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabAvrFuseBits = new QTabWidget(AvrFuseSettings);
        tabAvrFuseBits->setObjectName(QStringLiteral("tabAvrFuseBits"));
        tabAvrFuseBits->setTabPosition(QTabWidget::South);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_7 = new QGridLayout(tab);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        GRb_lowfuse = new QGroupBox(tab);
        GRb_lowfuse->setObjectName(QStringLiteral("GRb_lowfuse"));
        gridLayout_3 = new QGridLayout(GRb_lowfuse);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        chbAVRLow7 = new QCheckBox(GRb_lowfuse);
        chbAVRLow7->setObjectName(QStringLiteral("chbAVRLow7"));

        gridLayout_3->addWidget(chbAVRLow7, 0, 0, 1, 1);

        chbAVRLow6 = new QCheckBox(GRb_lowfuse);
        chbAVRLow6->setObjectName(QStringLiteral("chbAVRLow6"));

        gridLayout_3->addWidget(chbAVRLow6, 1, 0, 1, 1);

        chbAVRLow5 = new QCheckBox(GRb_lowfuse);
        chbAVRLow5->setObjectName(QStringLiteral("chbAVRLow5"));

        gridLayout_3->addWidget(chbAVRLow5, 2, 0, 1, 1);

        chbAVRLow4 = new QCheckBox(GRb_lowfuse);
        chbAVRLow4->setObjectName(QStringLiteral("chbAVRLow4"));

        gridLayout_3->addWidget(chbAVRLow4, 3, 0, 1, 1);

        chbAVRLow3 = new QCheckBox(GRb_lowfuse);
        chbAVRLow3->setObjectName(QStringLiteral("chbAVRLow3"));

        gridLayout_3->addWidget(chbAVRLow3, 4, 0, 1, 1);

        chbAVRLow2 = new QCheckBox(GRb_lowfuse);
        chbAVRLow2->setObjectName(QStringLiteral("chbAVRLow2"));

        gridLayout_3->addWidget(chbAVRLow2, 5, 0, 1, 1);

        chbAVRLow1 = new QCheckBox(GRb_lowfuse);
        chbAVRLow1->setObjectName(QStringLiteral("chbAVRLow1"));

        gridLayout_3->addWidget(chbAVRLow1, 6, 0, 1, 1);

        chbAVRLow0 = new QCheckBox(GRb_lowfuse);
        chbAVRLow0->setObjectName(QStringLiteral("chbAVRLow0"));

        gridLayout_3->addWidget(chbAVRLow0, 7, 0, 1, 1);


        gridLayout_7->addWidget(GRb_lowfuse, 0, 0, 1, 1);

        GRb__extendfuse_3 = new QGroupBox(tab);
        GRb__extendfuse_3->setObjectName(QStringLiteral("GRb__extendfuse_3"));
        gridLayout_5 = new QGridLayout(GRb__extendfuse_3);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        chbAVRExt7 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt7->setObjectName(QStringLiteral("chbAVRExt7"));

        gridLayout_5->addWidget(chbAVRExt7, 0, 0, 1, 1);

        chbAVRExt6 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt6->setObjectName(QStringLiteral("chbAVRExt6"));

        gridLayout_5->addWidget(chbAVRExt6, 1, 0, 1, 1);

        chbAVRExt5 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt5->setObjectName(QStringLiteral("chbAVRExt5"));

        gridLayout_5->addWidget(chbAVRExt5, 2, 0, 1, 1);

        chbAVRExt4 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt4->setObjectName(QStringLiteral("chbAVRExt4"));

        gridLayout_5->addWidget(chbAVRExt4, 3, 0, 1, 1);

        chbAVRExt3 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt3->setObjectName(QStringLiteral("chbAVRExt3"));

        gridLayout_5->addWidget(chbAVRExt3, 4, 0, 1, 1);

        chbAVRExt2 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt2->setObjectName(QStringLiteral("chbAVRExt2"));

        gridLayout_5->addWidget(chbAVRExt2, 5, 0, 1, 1);

        chbAVRExt1 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt1->setObjectName(QStringLiteral("chbAVRExt1"));

        gridLayout_5->addWidget(chbAVRExt1, 6, 0, 1, 1);

        chbAVRExt0 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt0->setObjectName(QStringLiteral("chbAVRExt0"));

        gridLayout_5->addWidget(chbAVRExt0, 7, 0, 1, 1);


        gridLayout_7->addWidget(GRb__extendfuse_3, 0, 2, 1, 1);

        GRb_lowfuse_4 = new QGroupBox(tab);
        GRb_lowfuse_4->setObjectName(QStringLiteral("GRb_lowfuse_4"));
        gridLayout_6 = new QGridLayout(GRb_lowfuse_4);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        chbAVRLock7 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock7->setObjectName(QStringLiteral("chbAVRLock7"));

        gridLayout_6->addWidget(chbAVRLock7, 0, 0, 1, 1);

        chbAVRLock6 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock6->setObjectName(QStringLiteral("chbAVRLock6"));

        gridLayout_6->addWidget(chbAVRLock6, 1, 0, 1, 1);

        chbAVRLock5 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock5->setObjectName(QStringLiteral("chbAVRLock5"));

        gridLayout_6->addWidget(chbAVRLock5, 2, 0, 1, 1);

        chbAVRLock4 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock4->setObjectName(QStringLiteral("chbAVRLock4"));

        gridLayout_6->addWidget(chbAVRLock4, 3, 0, 1, 1);

        chbAVRLock3 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock3->setObjectName(QStringLiteral("chbAVRLock3"));

        gridLayout_6->addWidget(chbAVRLock3, 4, 0, 1, 1);

        chbAVRLock2 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock2->setObjectName(QStringLiteral("chbAVRLock2"));

        gridLayout_6->addWidget(chbAVRLock2, 5, 0, 1, 1);

        chbAVRLock1 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock1->setObjectName(QStringLiteral("chbAVRLock1"));

        gridLayout_6->addWidget(chbAVRLock1, 6, 0, 1, 1);

        chbAVRLock0 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock0->setObjectName(QStringLiteral("chbAVRLock0"));

        gridLayout_6->addWidget(chbAVRLock0, 7, 0, 1, 1);


        gridLayout_7->addWidget(GRb_lowfuse_4, 0, 3, 1, 1);

        GRb_highfuse = new QGroupBox(tab);
        GRb_highfuse->setObjectName(QStringLiteral("GRb_highfuse"));
        gridLayout_4 = new QGridLayout(GRb_highfuse);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        chbAVRHigh7 = new QCheckBox(GRb_highfuse);
        chbAVRHigh7->setObjectName(QStringLiteral("chbAVRHigh7"));

        gridLayout_4->addWidget(chbAVRHigh7, 0, 0, 1, 1);

        chbAVRHigh6 = new QCheckBox(GRb_highfuse);
        chbAVRHigh6->setObjectName(QStringLiteral("chbAVRHigh6"));

        gridLayout_4->addWidget(chbAVRHigh6, 1, 0, 1, 1);

        chbAVRHigh5 = new QCheckBox(GRb_highfuse);
        chbAVRHigh5->setObjectName(QStringLiteral("chbAVRHigh5"));

        gridLayout_4->addWidget(chbAVRHigh5, 2, 0, 1, 1);

        chbAVRHigh4 = new QCheckBox(GRb_highfuse);
        chbAVRHigh4->setObjectName(QStringLiteral("chbAVRHigh4"));

        gridLayout_4->addWidget(chbAVRHigh4, 3, 0, 1, 1);

        chbAVRHigh3 = new QCheckBox(GRb_highfuse);
        chbAVRHigh3->setObjectName(QStringLiteral("chbAVRHigh3"));

        gridLayout_4->addWidget(chbAVRHigh3, 4, 0, 1, 1);

        chbAVRHigh2 = new QCheckBox(GRb_highfuse);
        chbAVRHigh2->setObjectName(QStringLiteral("chbAVRHigh2"));

        gridLayout_4->addWidget(chbAVRHigh2, 5, 0, 1, 1);

        chbAVRHigh1 = new QCheckBox(GRb_highfuse);
        chbAVRHigh1->setObjectName(QStringLiteral("chbAVRHigh1"));

        gridLayout_4->addWidget(chbAVRHigh1, 6, 0, 1, 1);

        chbAVRHigh0 = new QCheckBox(GRb_highfuse);
        chbAVRHigh0->setObjectName(QStringLiteral("chbAVRHigh0"));

        gridLayout_4->addWidget(chbAVRHigh0, 7, 0, 1, 1);


        gridLayout_7->addWidget(GRb_highfuse, 0, 1, 1, 1);

        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 60, 21, 16));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 100, 21, 16));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 140, 21, 16));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 180, 21, 16));
        txtE_low_2 = new QTextEdit(groupBox_3);
        txtE_low_2->setObjectName(QStringLiteral("txtE_low_2"));
        txtE_low_2->setEnabled(true);
        txtE_low_2->setGeometry(QRect(30, 60, 31, 20));
        txtE_low_2->setMaximumSize(QSize(16777215, 20));
#ifndef QT_NO_WHATSTHIS
        txtE_low_2->setWhatsThis(QStringLiteral(""));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
        txtE_low_2->setAccessibleName(QStringLiteral(""));
#endif // QT_NO_ACCESSIBILITY
        txtE_low_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtE_low_2->setUndoRedoEnabled(false);
        txtE_low_2->setOverwriteMode(true);
        txtE_low_2->setTabStopWidth(2);
        txtE_low_2->setAcceptRichText(true);
        txtE_low_3 = new QTextEdit(groupBox_3);
        txtE_low_3->setObjectName(QStringLiteral("txtE_low_3"));
        txtE_low_3->setEnabled(false);
        txtE_low_3->setGeometry(QRect(30, 100, 31, 20));
        txtE_low_3->setMaximumSize(QSize(16777215, 20));
#ifndef QT_NO_WHATSTHIS
        txtE_low_3->setWhatsThis(QStringLiteral(""));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
        txtE_low_3->setAccessibleName(QStringLiteral(""));
#endif // QT_NO_ACCESSIBILITY
        txtE_low_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtE_low_3->setUndoRedoEnabled(false);
        txtE_low_3->setOverwriteMode(false);
        txtE_low_3->setTabStopWidth(2);
        txtE_low_3->setAcceptRichText(false);
        txtE_low_4 = new QTextEdit(groupBox_3);
        txtE_low_4->setObjectName(QStringLiteral("txtE_low_4"));
        txtE_low_4->setEnabled(false);
        txtE_low_4->setGeometry(QRect(30, 140, 31, 20));
        txtE_low_4->setMaximumSize(QSize(16777215, 20));
#ifndef QT_NO_WHATSTHIS
        txtE_low_4->setWhatsThis(QStringLiteral(""));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
        txtE_low_4->setAccessibleName(QStringLiteral(""));
#endif // QT_NO_ACCESSIBILITY
        txtE_low_4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtE_low_4->setUndoRedoEnabled(false);
        txtE_low_4->setOverwriteMode(false);
        txtE_low_4->setTabStopWidth(2);
        txtE_low_4->setAcceptRichText(false);
        txtE_low_5 = new QTextEdit(groupBox_3);
        txtE_low_5->setObjectName(QStringLiteral("txtE_low_5"));
        txtE_low_5->setEnabled(false);
        txtE_low_5->setGeometry(QRect(30, 180, 31, 20));
        txtE_low_5->setMaximumSize(QSize(16777215, 20));
#ifndef QT_NO_WHATSTHIS
        txtE_low_5->setWhatsThis(QStringLiteral(""));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
        txtE_low_5->setAccessibleName(QStringLiteral(""));
#endif // QT_NO_ACCESSIBILITY
        txtE_low_5->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtE_low_5->setUndoRedoEnabled(false);
        txtE_low_5->setOverwriteMode(false);
        txtE_low_5->setTabStopWidth(2);
        txtE_low_5->setAcceptRichText(false);

        gridLayout_7->addWidget(groupBox_3, 0, 4, 1, 1);

        tabAvrFuseBits->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_9 = new QGridLayout(tab_2);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        scrollAreaAVRNavigation = new QScrollArea(tab_2);
        scrollAreaAVRNavigation->setObjectName(QStringLiteral("scrollAreaAVRNavigation"));
        scrollAreaAVRNavigation->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollAreaAVRNavigation->setWidgetResizable(true);
        scrollAreaAVRWidgetContents = new QWidget();
        scrollAreaAVRWidgetContents->setObjectName(QStringLiteral("scrollAreaAVRWidgetContents"));
        scrollAreaAVRWidgetContents->setGeometry(QRect(0, 0, 455, 227));
        gridLayout_8 = new QGridLayout(scrollAreaAVRWidgetContents);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

        gridLayout_8->addLayout(verticalLayout, 0, 0, 1, 1);

        scrollAreaAVRNavigation->setWidget(scrollAreaAVRWidgetContents);

        gridLayout_9->addWidget(scrollAreaAVRNavigation, 0, 0, 1, 1);

        tabAvrFuseBits->addTab(tab_2, QString());

        gridLayout->addWidget(tabAvrFuseBits, 0, 0, 3, 1);

        groupBox = new QGroupBox(AvrFuseSettings);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(170, 160));
        groupBox->setMaximumSize(QSize(16777215, 180));
        btnAVRFuzeRead = new QPushButton(groupBox);
        btnAVRFuzeRead->setObjectName(QStringLiteral("btnAVRFuzeRead"));
        btnAVRFuzeRead->setGeometry(QRect(110, 40, 51, 23));
        btnAVRFuzeDefault = new QPushButton(groupBox);
        btnAVRFuzeDefault->setObjectName(QStringLiteral("btnAVRFuzeDefault"));
        btnAVRFuzeDefault->setGeometry(QRect(110, 70, 51, 23));
        btnAVRFuzeWrite = new QPushButton(groupBox);
        btnAVRFuzeWrite->setObjectName(QStringLiteral("btnAVRFuzeWrite"));
        btnAVRFuzeWrite->setGeometry(QRect(110, 100, 51, 23));
        La_lowfuse = new QLabel(groupBox);
        La_lowfuse->setObjectName(QStringLiteral("La_lowfuse"));
        La_lowfuse->setGeometry(QRect(10, 40, 61, 16));
        La_lowfuse_2 = new QLabel(groupBox);
        La_lowfuse_2->setObjectName(QStringLiteral("La_lowfuse_2"));
        La_lowfuse_2->setGeometry(QRect(10, 70, 61, 16));
        La_lowfuse_3 = new QLabel(groupBox);
        La_lowfuse_3->setObjectName(QStringLiteral("La_lowfuse_3"));
        La_lowfuse_3->setGeometry(QRect(10, 100, 71, 16));
        txtAVRLow = new QTextEdit(groupBox);
        txtAVRLow->setObjectName(QStringLiteral("txtAVRLow"));
        txtAVRLow->setEnabled(true);
        txtAVRLow->setGeometry(QRect(70, 40, 31, 21));
#ifndef QT_NO_WHATSTHIS
        txtAVRLow->setWhatsThis(QStringLiteral(""));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
        txtAVRLow->setAccessibleName(QStringLiteral(""));
#endif // QT_NO_ACCESSIBILITY
        txtAVRLow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtAVRLow->setUndoRedoEnabled(false);
        txtAVRLow->setOverwriteMode(true);
        txtAVRLow->setTabStopWidth(2);
        txtAVRLow->setAcceptRichText(true);
        txtAVRHigh = new QTextEdit(groupBox);
        txtAVRHigh->setObjectName(QStringLiteral("txtAVRHigh"));
        txtAVRHigh->setGeometry(QRect(70, 70, 31, 21));
        txtAVRHigh->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtAVRHigh->setTabStopWidth(2);
        txtAVRExtend = new QTextEdit(groupBox);
        txtAVRExtend->setObjectName(QStringLiteral("txtAVRExtend"));
        txtAVRExtend->setGeometry(QRect(70, 100, 31, 21));
        txtAVRExtend->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout->addWidget(groupBox, 0, 1, 1, 1);

        groupBox_2 = new QGroupBox(AvrFuseSettings);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(170, 100));
        groupBox_2->setMaximumSize(QSize(16777215, 100));
        btnAVRLockWrite = new QPushButton(groupBox_2);
        btnAVRLockWrite->setObjectName(QStringLiteral("btnAVRLockWrite"));
        btnAVRLockWrite->setGeometry(QRect(110, 50, 51, 23));
        La_lowfuse_4 = new QLabel(groupBox_2);
        La_lowfuse_4->setObjectName(QStringLiteral("La_lowfuse_4"));
        La_lowfuse_4->setGeometry(QRect(10, 30, 61, 16));
        txtAVRLock = new QTextEdit(groupBox_2);
        txtAVRLock->setObjectName(QStringLiteral("txtAVRLock"));
        txtAVRLock->setGeometry(QRect(70, 30, 31, 21));
        txtAVRLock->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtAVRLock->setUndoRedoEnabled(false);
        btnAVRLockRead = new QPushButton(groupBox_2);
        btnAVRLockRead->setObjectName(QStringLiteral("btnAVRLockRead"));
        btnAVRLockRead->setGeometry(QRect(110, 20, 51, 23));

        gridLayout->addWidget(groupBox_2, 1, 1, 1, 1);


        retranslateUi(AvrFuseSettings);

        tabAvrFuseBits->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(AvrFuseSettings);
    } // setupUi

    void retranslateUi(QWidget *AvrFuseSettings)
    {
        AvrFuseSettings->setWindowTitle(QApplication::translate("AvrFuseSettings", "Form", 0));
        GRb_lowfuse->setTitle(QApplication::translate("AvrFuseSettings", "Low Fuse Bits", 0));
        chbAVRLow7->setText(QApplication::translate("AvrFuseSettings", "CLKDIV8", 0));
        chbAVRLow6->setText(QApplication::translate("AvrFuseSettings", "CKOUT", 0));
        chbAVRLow5->setText(QApplication::translate("AvrFuseSettings", "SUT1", 0));
        chbAVRLow4->setText(QApplication::translate("AvrFuseSettings", "SUT0", 0));
        chbAVRLow3->setText(QApplication::translate("AvrFuseSettings", "CKSEL3", 0));
        chbAVRLow2->setText(QApplication::translate("AvrFuseSettings", "CKSEL2", 0));
        chbAVRLow1->setText(QApplication::translate("AvrFuseSettings", "CKSEL1", 0));
        chbAVRLow0->setText(QApplication::translate("AvrFuseSettings", "CKSEL0", 0));
        GRb__extendfuse_3->setTitle(QApplication::translate("AvrFuseSettings", "Extend Fuse Bits", 0));
        chbAVRExt7->setText(QApplication::translate("AvrFuseSettings", "NC", 0));
        chbAVRExt6->setText(QApplication::translate("AvrFuseSettings", "NC", 0));
        chbAVRExt5->setText(QApplication::translate("AvrFuseSettings", "NC", 0));
        chbAVRExt4->setText(QApplication::translate("AvrFuseSettings", "NC", 0));
        chbAVRExt3->setText(QApplication::translate("AvrFuseSettings", "BODLEVEL2", 0));
        chbAVRExt2->setText(QApplication::translate("AvrFuseSettings", "BODLEVEL1", 0));
        chbAVRExt1->setText(QApplication::translate("AvrFuseSettings", "BODLEVEL0", 0));
        chbAVRExt0->setText(QApplication::translate("AvrFuseSettings", "RSTDISBL", 0));
        GRb_lowfuse_4->setTitle(QApplication::translate("AvrFuseSettings", "Lock Bits", 0));
        chbAVRLock7->setText(QApplication::translate("AvrFuseSettings", "NC", 0));
        chbAVRLock6->setText(QApplication::translate("AvrFuseSettings", "NC", 0));
        chbAVRLock5->setText(QApplication::translate("AvrFuseSettings", "BLB12", 0));
        chbAVRLock4->setText(QApplication::translate("AvrFuseSettings", "BLB11", 0));
        chbAVRLock3->setText(QApplication::translate("AvrFuseSettings", "BLB02", 0));
        chbAVRLock2->setText(QApplication::translate("AvrFuseSettings", "BLB01", 0));
        chbAVRLock1->setText(QApplication::translate("AvrFuseSettings", "LB2", 0));
        chbAVRLock0->setText(QApplication::translate("AvrFuseSettings", "LB1", 0));
        GRb_highfuse->setTitle(QApplication::translate("AvrFuseSettings", "High Fuse Bits", 0));
        chbAVRHigh7->setText(QApplication::translate("AvrFuseSettings", "OCDEN", 0));
        chbAVRHigh6->setText(QApplication::translate("AvrFuseSettings", "JTAGEN", 0));
        chbAVRHigh5->setText(QApplication::translate("AvrFuseSettings", "SPIEN", 0));
        chbAVRHigh4->setText(QApplication::translate("AvrFuseSettings", "WDTON", 0));
        chbAVRHigh3->setText(QApplication::translate("AvrFuseSettings", "EESAVE", 0));
        chbAVRHigh2->setText(QApplication::translate("AvrFuseSettings", "BOOTSZ1", 0));
        chbAVRHigh1->setText(QApplication::translate("AvrFuseSettings", "BO0TSZ0", 0));
        chbAVRHigh0->setText(QApplication::translate("AvrFuseSettings", "BOOTRST", 0));
        groupBox_3->setTitle(QApplication::translate("AvrFuseSettings", "Calibration", 0));
        label->setText(QApplication::translate("AvrFuseSettings", "8.0", 0));
        label_2->setText(QApplication::translate("AvrFuseSettings", "4.0", 0));
        label_3->setText(QApplication::translate("AvrFuseSettings", "2.0", 0));
        label_4->setText(QApplication::translate("AvrFuseSettings", "8.0", 0));
        txtE_low_2->setHtml(QApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">00</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p></body></html>", 0));
        txtE_low_3->setHtml(QApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">00</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p></body></html>", 0));
        txtE_low_4->setHtml(QApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">00</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p></body></html>", 0));
        txtE_low_5->setHtml(QApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">00</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p></body></html>", 0));
        tabAvrFuseBits->setTabText(tabAvrFuseBits->indexOf(tab), QApplication::translate("AvrFuseSettings", "ConfigBit", 0));
        tabAvrFuseBits->setTabText(tabAvrFuseBits->indexOf(tab_2), QApplication::translate("AvrFuseSettings", "Navigation", 0));
        groupBox->setTitle(QApplication::translate("AvrFuseSettings", "Fuse", 0));
        btnAVRFuzeRead->setText(QApplication::translate("AvrFuseSettings", "Read", 0));
        btnAVRFuzeDefault->setText(QApplication::translate("AvrFuseSettings", "Default", 0));
        btnAVRFuzeWrite->setText(QApplication::translate("AvrFuseSettings", "Write", 0));
        La_lowfuse->setText(QApplication::translate("AvrFuseSettings", "Low Value", 0));
        La_lowfuse_2->setText(QApplication::translate("AvrFuseSettings", "High Value", 0));
        La_lowfuse_3->setText(QApplication::translate("AvrFuseSettings", "Ext  Value", 0));
        txtAVRLow->setHtml(QApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">62</span></p></body></html>", 0));
        txtAVRHigh->setHtml(QApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">99</span></p></body></html>", 0));
        txtAVRExtend->setHtml(QApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">ff</span></p></body></html>", 0));
        groupBox_2->setTitle(QApplication::translate("AvrFuseSettings", "Lock", 0));
        btnAVRLockWrite->setText(QApplication::translate("AvrFuseSettings", "Write", 0));
        La_lowfuse_4->setText(QApplication::translate("AvrFuseSettings", "Lock Value", 0));
        txtAVRLock->setHtml(QApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">f3</span></p></body></html>", 0));
        btnAVRLockRead->setText(QApplication::translate("AvrFuseSettings", "Read", 0));
    } // retranslateUi

};

namespace Ui {
    class AvrFuseSettings: public Ui_AvrFuseSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AVRFUSESETTINGS_H
