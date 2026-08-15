/********************************************************************************
** Form generated from reading UI file 'avrFuseSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AVRFUSESETTINGS_H
#define UI_AVRFUSESETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
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
            AvrFuseSettings->setObjectName(QString::fromUtf8("AvrFuseSettings"));
        AvrFuseSettings->resize(675, 290);
        gridLayout = new QGridLayout(AvrFuseSettings);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabAvrFuseBits = new QTabWidget(AvrFuseSettings);
        tabAvrFuseBits->setObjectName(QString::fromUtf8("tabAvrFuseBits"));
        tabAvrFuseBits->setTabPosition(QTabWidget::South);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_7 = new QGridLayout(tab);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        GRb_lowfuse = new QGroupBox(tab);
        GRb_lowfuse->setObjectName(QString::fromUtf8("GRb_lowfuse"));
        gridLayout_3 = new QGridLayout(GRb_lowfuse);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        chbAVRLow7 = new QCheckBox(GRb_lowfuse);
        chbAVRLow7->setObjectName(QString::fromUtf8("chbAVRLow7"));

        gridLayout_3->addWidget(chbAVRLow7, 0, 0, 1, 1);

        chbAVRLow6 = new QCheckBox(GRb_lowfuse);
        chbAVRLow6->setObjectName(QString::fromUtf8("chbAVRLow6"));

        gridLayout_3->addWidget(chbAVRLow6, 1, 0, 1, 1);

        chbAVRLow5 = new QCheckBox(GRb_lowfuse);
        chbAVRLow5->setObjectName(QString::fromUtf8("chbAVRLow5"));

        gridLayout_3->addWidget(chbAVRLow5, 2, 0, 1, 1);

        chbAVRLow4 = new QCheckBox(GRb_lowfuse);
        chbAVRLow4->setObjectName(QString::fromUtf8("chbAVRLow4"));

        gridLayout_3->addWidget(chbAVRLow4, 3, 0, 1, 1);

        chbAVRLow3 = new QCheckBox(GRb_lowfuse);
        chbAVRLow3->setObjectName(QString::fromUtf8("chbAVRLow3"));

        gridLayout_3->addWidget(chbAVRLow3, 4, 0, 1, 1);

        chbAVRLow2 = new QCheckBox(GRb_lowfuse);
        chbAVRLow2->setObjectName(QString::fromUtf8("chbAVRLow2"));

        gridLayout_3->addWidget(chbAVRLow2, 5, 0, 1, 1);

        chbAVRLow1 = new QCheckBox(GRb_lowfuse);
        chbAVRLow1->setObjectName(QString::fromUtf8("chbAVRLow1"));

        gridLayout_3->addWidget(chbAVRLow1, 6, 0, 1, 1);

        chbAVRLow0 = new QCheckBox(GRb_lowfuse);
        chbAVRLow0->setObjectName(QString::fromUtf8("chbAVRLow0"));

        gridLayout_3->addWidget(chbAVRLow0, 7, 0, 1, 1);


        gridLayout_7->addWidget(GRb_lowfuse, 0, 0, 1, 1);

        GRb__extendfuse_3 = new QGroupBox(tab);
        GRb__extendfuse_3->setObjectName(QString::fromUtf8("GRb__extendfuse_3"));
        gridLayout_5 = new QGridLayout(GRb__extendfuse_3);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        chbAVRExt7 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt7->setObjectName(QString::fromUtf8("chbAVRExt7"));

        gridLayout_5->addWidget(chbAVRExt7, 0, 0, 1, 1);

        chbAVRExt6 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt6->setObjectName(QString::fromUtf8("chbAVRExt6"));

        gridLayout_5->addWidget(chbAVRExt6, 1, 0, 1, 1);

        chbAVRExt5 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt5->setObjectName(QString::fromUtf8("chbAVRExt5"));

        gridLayout_5->addWidget(chbAVRExt5, 2, 0, 1, 1);

        chbAVRExt4 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt4->setObjectName(QString::fromUtf8("chbAVRExt4"));

        gridLayout_5->addWidget(chbAVRExt4, 3, 0, 1, 1);

        chbAVRExt3 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt3->setObjectName(QString::fromUtf8("chbAVRExt3"));

        gridLayout_5->addWidget(chbAVRExt3, 4, 0, 1, 1);

        chbAVRExt2 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt2->setObjectName(QString::fromUtf8("chbAVRExt2"));

        gridLayout_5->addWidget(chbAVRExt2, 5, 0, 1, 1);

        chbAVRExt1 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt1->setObjectName(QString::fromUtf8("chbAVRExt1"));

        gridLayout_5->addWidget(chbAVRExt1, 6, 0, 1, 1);

        chbAVRExt0 = new QCheckBox(GRb__extendfuse_3);
        chbAVRExt0->setObjectName(QString::fromUtf8("chbAVRExt0"));

        gridLayout_5->addWidget(chbAVRExt0, 7, 0, 1, 1);


        gridLayout_7->addWidget(GRb__extendfuse_3, 0, 2, 1, 1);

        GRb_lowfuse_4 = new QGroupBox(tab);
        GRb_lowfuse_4->setObjectName(QString::fromUtf8("GRb_lowfuse_4"));
        gridLayout_6 = new QGridLayout(GRb_lowfuse_4);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        chbAVRLock7 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock7->setObjectName(QString::fromUtf8("chbAVRLock7"));

        gridLayout_6->addWidget(chbAVRLock7, 0, 0, 1, 1);

        chbAVRLock6 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock6->setObjectName(QString::fromUtf8("chbAVRLock6"));

        gridLayout_6->addWidget(chbAVRLock6, 1, 0, 1, 1);

        chbAVRLock5 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock5->setObjectName(QString::fromUtf8("chbAVRLock5"));

        gridLayout_6->addWidget(chbAVRLock5, 2, 0, 1, 1);

        chbAVRLock4 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock4->setObjectName(QString::fromUtf8("chbAVRLock4"));

        gridLayout_6->addWidget(chbAVRLock4, 3, 0, 1, 1);

        chbAVRLock3 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock3->setObjectName(QString::fromUtf8("chbAVRLock3"));

        gridLayout_6->addWidget(chbAVRLock3, 4, 0, 1, 1);

        chbAVRLock2 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock2->setObjectName(QString::fromUtf8("chbAVRLock2"));

        gridLayout_6->addWidget(chbAVRLock2, 5, 0, 1, 1);

        chbAVRLock1 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock1->setObjectName(QString::fromUtf8("chbAVRLock1"));

        gridLayout_6->addWidget(chbAVRLock1, 6, 0, 1, 1);

        chbAVRLock0 = new QCheckBox(GRb_lowfuse_4);
        chbAVRLock0->setObjectName(QString::fromUtf8("chbAVRLock0"));

        gridLayout_6->addWidget(chbAVRLock0, 7, 0, 1, 1);


        gridLayout_7->addWidget(GRb_lowfuse_4, 0, 3, 1, 1);

        GRb_highfuse = new QGroupBox(tab);
        GRb_highfuse->setObjectName(QString::fromUtf8("GRb_highfuse"));
        gridLayout_4 = new QGridLayout(GRb_highfuse);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        chbAVRHigh7 = new QCheckBox(GRb_highfuse);
        chbAVRHigh7->setObjectName(QString::fromUtf8("chbAVRHigh7"));

        gridLayout_4->addWidget(chbAVRHigh7, 0, 0, 1, 1);

        chbAVRHigh6 = new QCheckBox(GRb_highfuse);
        chbAVRHigh6->setObjectName(QString::fromUtf8("chbAVRHigh6"));

        gridLayout_4->addWidget(chbAVRHigh6, 1, 0, 1, 1);

        chbAVRHigh5 = new QCheckBox(GRb_highfuse);
        chbAVRHigh5->setObjectName(QString::fromUtf8("chbAVRHigh5"));

        gridLayout_4->addWidget(chbAVRHigh5, 2, 0, 1, 1);

        chbAVRHigh4 = new QCheckBox(GRb_highfuse);
        chbAVRHigh4->setObjectName(QString::fromUtf8("chbAVRHigh4"));

        gridLayout_4->addWidget(chbAVRHigh4, 3, 0, 1, 1);

        chbAVRHigh3 = new QCheckBox(GRb_highfuse);
        chbAVRHigh3->setObjectName(QString::fromUtf8("chbAVRHigh3"));

        gridLayout_4->addWidget(chbAVRHigh3, 4, 0, 1, 1);

        chbAVRHigh2 = new QCheckBox(GRb_highfuse);
        chbAVRHigh2->setObjectName(QString::fromUtf8("chbAVRHigh2"));

        gridLayout_4->addWidget(chbAVRHigh2, 5, 0, 1, 1);

        chbAVRHigh1 = new QCheckBox(GRb_highfuse);
        chbAVRHigh1->setObjectName(QString::fromUtf8("chbAVRHigh1"));

        gridLayout_4->addWidget(chbAVRHigh1, 6, 0, 1, 1);

        chbAVRHigh0 = new QCheckBox(GRb_highfuse);
        chbAVRHigh0->setObjectName(QString::fromUtf8("chbAVRHigh0"));

        gridLayout_4->addWidget(chbAVRHigh0, 7, 0, 1, 1);


        gridLayout_7->addWidget(GRb_highfuse, 0, 1, 1, 1);

        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 60, 21, 16));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 100, 21, 16));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 140, 21, 16));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 180, 21, 16));
        txtE_low_2 = new QTextEdit(groupBox_3);
        txtE_low_2->setObjectName(QString::fromUtf8("txtE_low_2"));
        txtE_low_2->setEnabled(true);
        txtE_low_2->setGeometry(QRect(30, 60, 31, 20));
        txtE_low_2->setMaximumSize(QSize(16777215, 20));
#if QT_CONFIG(whatsthis)
        txtE_low_2->setWhatsThis(QString::fromUtf8(""));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(accessibility)
        txtE_low_2->setAccessibleName(QString::fromUtf8(""));
#endif // QT_CONFIG(accessibility)
        txtE_low_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtE_low_2->setUndoRedoEnabled(false);
        txtE_low_2->setOverwriteMode(true);
        txtE_low_2->setTabStopWidth(2);
        txtE_low_2->setAcceptRichText(true);
        txtE_low_3 = new QTextEdit(groupBox_3);
        txtE_low_3->setObjectName(QString::fromUtf8("txtE_low_3"));
        txtE_low_3->setEnabled(false);
        txtE_low_3->setGeometry(QRect(30, 100, 31, 20));
        txtE_low_3->setMaximumSize(QSize(16777215, 20));
#if QT_CONFIG(whatsthis)
        txtE_low_3->setWhatsThis(QString::fromUtf8(""));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(accessibility)
        txtE_low_3->setAccessibleName(QString::fromUtf8(""));
#endif // QT_CONFIG(accessibility)
        txtE_low_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtE_low_3->setUndoRedoEnabled(false);
        txtE_low_3->setOverwriteMode(false);
        txtE_low_3->setTabStopWidth(2);
        txtE_low_3->setAcceptRichText(false);
        txtE_low_4 = new QTextEdit(groupBox_3);
        txtE_low_4->setObjectName(QString::fromUtf8("txtE_low_4"));
        txtE_low_4->setEnabled(false);
        txtE_low_4->setGeometry(QRect(30, 140, 31, 20));
        txtE_low_4->setMaximumSize(QSize(16777215, 20));
#if QT_CONFIG(whatsthis)
        txtE_low_4->setWhatsThis(QString::fromUtf8(""));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(accessibility)
        txtE_low_4->setAccessibleName(QString::fromUtf8(""));
#endif // QT_CONFIG(accessibility)
        txtE_low_4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtE_low_4->setUndoRedoEnabled(false);
        txtE_low_4->setOverwriteMode(false);
        txtE_low_4->setTabStopWidth(2);
        txtE_low_4->setAcceptRichText(false);
        txtE_low_5 = new QTextEdit(groupBox_3);
        txtE_low_5->setObjectName(QString::fromUtf8("txtE_low_5"));
        txtE_low_5->setEnabled(false);
        txtE_low_5->setGeometry(QRect(30, 180, 31, 20));
        txtE_low_5->setMaximumSize(QSize(16777215, 20));
#if QT_CONFIG(whatsthis)
        txtE_low_5->setWhatsThis(QString::fromUtf8(""));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(accessibility)
        txtE_low_5->setAccessibleName(QString::fromUtf8(""));
#endif // QT_CONFIG(accessibility)
        txtE_low_5->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtE_low_5->setUndoRedoEnabled(false);
        txtE_low_5->setOverwriteMode(false);
        txtE_low_5->setTabStopWidth(2);
        txtE_low_5->setAcceptRichText(false);

        gridLayout_7->addWidget(groupBox_3, 0, 4, 1, 1);

        tabAvrFuseBits->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_9 = new QGridLayout(tab_2);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        scrollAreaAVRNavigation = new QScrollArea(tab_2);
        scrollAreaAVRNavigation->setObjectName(QString::fromUtf8("scrollAreaAVRNavigation"));
        scrollAreaAVRNavigation->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollAreaAVRNavigation->setWidgetResizable(true);
        scrollAreaAVRWidgetContents = new QWidget();
        scrollAreaAVRWidgetContents->setObjectName(QString::fromUtf8("scrollAreaAVRWidgetContents"));
        scrollAreaAVRWidgetContents->setGeometry(QRect(0, 0, 455, 227));
        gridLayout_8 = new QGridLayout(scrollAreaAVRWidgetContents);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        gridLayout_8->addLayout(verticalLayout, 0, 0, 1, 1);

        scrollAreaAVRNavigation->setWidget(scrollAreaAVRWidgetContents);

        gridLayout_9->addWidget(scrollAreaAVRNavigation, 0, 0, 1, 1);

        tabAvrFuseBits->addTab(tab_2, QString());

        gridLayout->addWidget(tabAvrFuseBits, 0, 0, 3, 1);

        groupBox = new QGroupBox(AvrFuseSettings);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(170, 160));
        groupBox->setMaximumSize(QSize(16777215, 180));
        btnAVRFuzeRead = new QPushButton(groupBox);
        btnAVRFuzeRead->setObjectName(QString::fromUtf8("btnAVRFuzeRead"));
        btnAVRFuzeRead->setGeometry(QRect(110, 40, 51, 23));
        btnAVRFuzeDefault = new QPushButton(groupBox);
        btnAVRFuzeDefault->setObjectName(QString::fromUtf8("btnAVRFuzeDefault"));
        btnAVRFuzeDefault->setGeometry(QRect(110, 70, 51, 23));
        btnAVRFuzeWrite = new QPushButton(groupBox);
        btnAVRFuzeWrite->setObjectName(QString::fromUtf8("btnAVRFuzeWrite"));
        btnAVRFuzeWrite->setGeometry(QRect(110, 100, 51, 23));
        La_lowfuse = new QLabel(groupBox);
        La_lowfuse->setObjectName(QString::fromUtf8("La_lowfuse"));
        La_lowfuse->setGeometry(QRect(10, 40, 61, 16));
        La_lowfuse_2 = new QLabel(groupBox);
        La_lowfuse_2->setObjectName(QString::fromUtf8("La_lowfuse_2"));
        La_lowfuse_2->setGeometry(QRect(10, 70, 61, 16));
        La_lowfuse_3 = new QLabel(groupBox);
        La_lowfuse_3->setObjectName(QString::fromUtf8("La_lowfuse_3"));
        La_lowfuse_3->setGeometry(QRect(10, 100, 71, 16));
        txtAVRLow = new QTextEdit(groupBox);
        txtAVRLow->setObjectName(QString::fromUtf8("txtAVRLow"));
        txtAVRLow->setEnabled(true);
        txtAVRLow->setGeometry(QRect(70, 40, 31, 21));
#if QT_CONFIG(whatsthis)
        txtAVRLow->setWhatsThis(QString::fromUtf8(""));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(accessibility)
        txtAVRLow->setAccessibleName(QString::fromUtf8(""));
#endif // QT_CONFIG(accessibility)
        txtAVRLow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtAVRLow->setUndoRedoEnabled(false);
        txtAVRLow->setOverwriteMode(true);
        txtAVRLow->setTabStopWidth(2);
        txtAVRLow->setAcceptRichText(true);
        txtAVRHigh = new QTextEdit(groupBox);
        txtAVRHigh->setObjectName(QString::fromUtf8("txtAVRHigh"));
        txtAVRHigh->setGeometry(QRect(70, 70, 31, 21));
        txtAVRHigh->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtAVRHigh->setTabStopWidth(2);
        txtAVRExtend = new QTextEdit(groupBox);
        txtAVRExtend->setObjectName(QString::fromUtf8("txtAVRExtend"));
        txtAVRExtend->setGeometry(QRect(70, 100, 31, 21));
        txtAVRExtend->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout->addWidget(groupBox, 0, 1, 1, 1);

        groupBox_2 = new QGroupBox(AvrFuseSettings);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(170, 100));
        groupBox_2->setMaximumSize(QSize(16777215, 100));
        btnAVRLockWrite = new QPushButton(groupBox_2);
        btnAVRLockWrite->setObjectName(QString::fromUtf8("btnAVRLockWrite"));
        btnAVRLockWrite->setGeometry(QRect(110, 50, 51, 23));
        La_lowfuse_4 = new QLabel(groupBox_2);
        La_lowfuse_4->setObjectName(QString::fromUtf8("La_lowfuse_4"));
        La_lowfuse_4->setGeometry(QRect(10, 30, 61, 16));
        txtAVRLock = new QTextEdit(groupBox_2);
        txtAVRLock->setObjectName(QString::fromUtf8("txtAVRLock"));
        txtAVRLock->setGeometry(QRect(70, 30, 31, 21));
        txtAVRLock->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtAVRLock->setUndoRedoEnabled(false);
        btnAVRLockRead = new QPushButton(groupBox_2);
        btnAVRLockRead->setObjectName(QString::fromUtf8("btnAVRLockRead"));
        btnAVRLockRead->setGeometry(QRect(110, 20, 51, 23));

        gridLayout->addWidget(groupBox_2, 1, 1, 1, 1);


        retranslateUi(AvrFuseSettings);

        tabAvrFuseBits->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(AvrFuseSettings);
    } // setupUi

    void retranslateUi(QWidget *AvrFuseSettings)
    {
        AvrFuseSettings->setWindowTitle(QCoreApplication::translate("AvrFuseSettings", "Form", nullptr));
        GRb_lowfuse->setTitle(QCoreApplication::translate("AvrFuseSettings", "Low Fuse Bits", nullptr));
        chbAVRLow7->setText(QCoreApplication::translate("AvrFuseSettings", "CLKDIV8", nullptr));
        chbAVRLow6->setText(QCoreApplication::translate("AvrFuseSettings", "CKOUT", nullptr));
        chbAVRLow5->setText(QCoreApplication::translate("AvrFuseSettings", "SUT1", nullptr));
        chbAVRLow4->setText(QCoreApplication::translate("AvrFuseSettings", "SUT0", nullptr));
        chbAVRLow3->setText(QCoreApplication::translate("AvrFuseSettings", "CKSEL3", nullptr));
        chbAVRLow2->setText(QCoreApplication::translate("AvrFuseSettings", "CKSEL2", nullptr));
        chbAVRLow1->setText(QCoreApplication::translate("AvrFuseSettings", "CKSEL1", nullptr));
        chbAVRLow0->setText(QCoreApplication::translate("AvrFuseSettings", "CKSEL0", nullptr));
        GRb__extendfuse_3->setTitle(QCoreApplication::translate("AvrFuseSettings", "Extend Fuse Bits", nullptr));
        chbAVRExt7->setText(QCoreApplication::translate("AvrFuseSettings", "NC", nullptr));
        chbAVRExt6->setText(QCoreApplication::translate("AvrFuseSettings", "NC", nullptr));
        chbAVRExt5->setText(QCoreApplication::translate("AvrFuseSettings", "NC", nullptr));
        chbAVRExt4->setText(QCoreApplication::translate("AvrFuseSettings", "NC", nullptr));
        chbAVRExt3->setText(QCoreApplication::translate("AvrFuseSettings", "BODLEVEL2", nullptr));
        chbAVRExt2->setText(QCoreApplication::translate("AvrFuseSettings", "BODLEVEL1", nullptr));
        chbAVRExt1->setText(QCoreApplication::translate("AvrFuseSettings", "BODLEVEL0", nullptr));
        chbAVRExt0->setText(QCoreApplication::translate("AvrFuseSettings", "RSTDISBL", nullptr));
        GRb_lowfuse_4->setTitle(QCoreApplication::translate("AvrFuseSettings", "Lock Bits", nullptr));
        chbAVRLock7->setText(QCoreApplication::translate("AvrFuseSettings", "NC", nullptr));
        chbAVRLock6->setText(QCoreApplication::translate("AvrFuseSettings", "NC", nullptr));
        chbAVRLock5->setText(QCoreApplication::translate("AvrFuseSettings", "BLB12", nullptr));
        chbAVRLock4->setText(QCoreApplication::translate("AvrFuseSettings", "BLB11", nullptr));
        chbAVRLock3->setText(QCoreApplication::translate("AvrFuseSettings", "BLB02", nullptr));
        chbAVRLock2->setText(QCoreApplication::translate("AvrFuseSettings", "BLB01", nullptr));
        chbAVRLock1->setText(QCoreApplication::translate("AvrFuseSettings", "LB2", nullptr));
        chbAVRLock0->setText(QCoreApplication::translate("AvrFuseSettings", "LB1", nullptr));
        GRb_highfuse->setTitle(QCoreApplication::translate("AvrFuseSettings", "High Fuse Bits", nullptr));
        chbAVRHigh7->setText(QCoreApplication::translate("AvrFuseSettings", "OCDEN", nullptr));
        chbAVRHigh6->setText(QCoreApplication::translate("AvrFuseSettings", "JTAGEN", nullptr));
        chbAVRHigh5->setText(QCoreApplication::translate("AvrFuseSettings", "SPIEN", nullptr));
        chbAVRHigh4->setText(QCoreApplication::translate("AvrFuseSettings", "WDTON", nullptr));
        chbAVRHigh3->setText(QCoreApplication::translate("AvrFuseSettings", "EESAVE", nullptr));
        chbAVRHigh2->setText(QCoreApplication::translate("AvrFuseSettings", "BOOTSZ1", nullptr));
        chbAVRHigh1->setText(QCoreApplication::translate("AvrFuseSettings", "BO0TSZ0", nullptr));
        chbAVRHigh0->setText(QCoreApplication::translate("AvrFuseSettings", "BOOTRST", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("AvrFuseSettings", "Calibration", nullptr));
        label->setText(QCoreApplication::translate("AvrFuseSettings", "8.0", nullptr));
        label_2->setText(QCoreApplication::translate("AvrFuseSettings", "4.0", nullptr));
        label_3->setText(QCoreApplication::translate("AvrFuseSettings", "2.0", nullptr));
        label_4->setText(QCoreApplication::translate("AvrFuseSettings", "8.0", nullptr));
        txtE_low_2->setHtml(QCoreApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">00</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p></body></html>", nullptr));
        txtE_low_3->setHtml(QCoreApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">00</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p></body></html>", nullptr));
        txtE_low_4->setHtml(QCoreApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">00</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p></body></html>", nullptr));
        txtE_low_5->setHtml(QCoreApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">00</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p></body></html>", nullptr));
        tabAvrFuseBits->setTabText(tabAvrFuseBits->indexOf(tab), QCoreApplication::translate("AvrFuseSettings", "ConfigBit", nullptr));
        tabAvrFuseBits->setTabText(tabAvrFuseBits->indexOf(tab_2), QCoreApplication::translate("AvrFuseSettings", "Navigation", nullptr));
        groupBox->setTitle(QCoreApplication::translate("AvrFuseSettings", "Fuse", nullptr));
        btnAVRFuzeRead->setText(QCoreApplication::translate("AvrFuseSettings", "Read", nullptr));
        btnAVRFuzeDefault->setText(QCoreApplication::translate("AvrFuseSettings", "Default", nullptr));
        btnAVRFuzeWrite->setText(QCoreApplication::translate("AvrFuseSettings", "Write", nullptr));
        La_lowfuse->setText(QCoreApplication::translate("AvrFuseSettings", "Low Value", nullptr));
        La_lowfuse_2->setText(QCoreApplication::translate("AvrFuseSettings", "High Value", nullptr));
        La_lowfuse_3->setText(QCoreApplication::translate("AvrFuseSettings", "Ext  Value", nullptr));
        txtAVRLow->setHtml(QCoreApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">62</span></p></body></html>", nullptr));
        txtAVRHigh->setHtml(QCoreApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">99</span></p></body></html>", nullptr));
        txtAVRExtend->setHtml(QCoreApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">ff</span></p></body></html>", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("AvrFuseSettings", "Lock", nullptr));
        btnAVRLockWrite->setText(QCoreApplication::translate("AvrFuseSettings", "Write", nullptr));
        La_lowfuse_4->setText(QCoreApplication::translate("AvrFuseSettings", "Lock Value", nullptr));
        txtAVRLock->setHtml(QCoreApplication::translate("AvrFuseSettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">f3</span></p></body></html>", nullptr));
        btnAVRLockRead->setText(QCoreApplication::translate("AvrFuseSettings", "Read", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AvrFuseSettings: public Ui_AvrFuseSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AVRFUSESETTINGS_H
