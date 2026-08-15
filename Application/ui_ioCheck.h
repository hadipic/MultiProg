/********************************************************************************
** Form generated from reading UI file 'ioCheck.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IOCHECK_H
#define UI_IOCHECK_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IOCheck
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGroupBox *grbEnbableIo;
    QGroupBox *grbRB0;
    QRadioButton *radioButton_88;
    QRadioButton *radioButton_89;
    QRadioButton *radioButton_90;
    QGroupBox *grbRB1;
    QRadioButton *radioButton_91;
    QRadioButton *radioButton_92;
    QRadioButton *radioButton_93;
    QGroupBox *grbRB2;
    QRadioButton *radioButton_94;
    QRadioButton *radioButton_95;
    QRadioButton *radioButton_96;
    QGroupBox *grbRB3;
    QRadioButton *radioButton_97;
    QRadioButton *radioButton_98;
    QRadioButton *radioButton_99;
    QGroupBox *grbRB4;
    QRadioButton *radioButton_100;
    QRadioButton *radioButton_101;
    QRadioButton *radioButton_102;
    QGroupBox *grbRB5;
    QRadioButton *radioButton_103;
    QRadioButton *radioButton_104;
    QRadioButton *radioButton_105;
    QGroupBox *grbRB6;
    QRadioButton *radioButton_106;
    QRadioButton *radioButton_107;
    QRadioButton *radioButton_108;
    QGroupBox *grbRB7;
    QRadioButton *radioButton_112;
    QRadioButton *radioButton_113;
    QRadioButton *radioButton_114;
    QGroupBox *grbRC6;
    QRadioButton *radioButton_115;
    QRadioButton *radioButton_116;
    QRadioButton *radioButton_117;
    QGroupBox *grbRC7;
    QRadioButton *radioButton_109;
    QRadioButton *radioButton_110;
    QRadioButton *radioButton_111;
    QGroupBox *grbRA3;
    QRadioButton *radioButton_118;
    QRadioButton *radioButton_119;
    QRadioButton *radioButton_120;
    QGroupBox *grbRA4;
    QRadioButton *radioButton_121;
    QRadioButton *radioButton_122;
    QRadioButton *radioButton_123;
    QGroupBox *grbRA5;
    QRadioButton *radioButton_160;
    QRadioButton *radioButton_161;
    QRadioButton *radioButton_162;
    QCheckBox *ch_B_vcc;
    QCheckBox *ch_B_vpp;
    QCheckBox *chb_dctodc;
    QSlider *horizontalSlider;
    QLabel *label_adc;

    void setupUi(QMainWindow *IOCheck)
    {
        if (IOCheck->objectName().isEmpty())
            IOCheck->setObjectName(QString::fromUtf8("IOCheck"));
        IOCheck->setEnabled(true);
        IOCheck->resize(533, 305);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(IOCheck->sizePolicy().hasHeightForWidth());
        IOCheck->setSizePolicy(sizePolicy);
        IOCheck->setContextMenuPolicy(Qt::CustomContextMenu);
        IOCheck->setWindowTitle(QString::fromUtf8("hardwar chek"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/Preferences_PCB3D.ico"), QSize(), QIcon::Normal, QIcon::Off);
        IOCheck->setWindowIcon(icon);
        centralwidget = new QWidget(IOCheck);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        grbEnbableIo = new QGroupBox(centralwidget);
        grbEnbableIo->setObjectName(QString::fromUtf8("grbEnbableIo"));
        grbEnbableIo->setEnabled(true);
        grbEnbableIo->setCheckable(true);
        grbEnbableIo->setChecked(false);
        grbRB0 = new QGroupBox(grbEnbableIo);
        grbRB0->setObjectName(QString::fromUtf8("grbRB0"));
        grbRB0->setEnabled(true);
        grbRB0->setGeometry(QRect(10, 230, 131, 31));
        grbRB0->setLayoutDirection(Qt::LeftToRight);
        grbRB0->setTitle(QString::fromUtf8("RB0=0"));
        grbRB0->setFlat(false);
        grbRB0->setCheckable(false);
        radioButton_88 = new QRadioButton(grbRB0);
        radioButton_88->setObjectName(QString::fromUtf8("radioButton_88"));
        radioButton_88->setGeometry(QRect(90, 10, 41, 21));
        radioButton_88->setChecked(true);
        radioButton_89 = new QRadioButton(grbRB0);
        radioButton_89->setObjectName(QString::fromUtf8("radioButton_89"));
        radioButton_89->setGeometry(QRect(10, 10, 31, 21));
        radioButton_89->setCheckable(true);
        radioButton_89->setChecked(false);
        radioButton_89->setAutoRepeat(true);
        radioButton_90 = new QRadioButton(grbRB0);
        radioButton_90->setObjectName(QString::fromUtf8("radioButton_90"));
        radioButton_90->setGeometry(QRect(50, 10, 31, 21));
        grbRB1 = new QGroupBox(grbEnbableIo);
        grbRB1->setObjectName(QString::fromUtf8("grbRB1"));
        grbRB1->setEnabled(true);
        grbRB1->setGeometry(QRect(10, 200, 131, 31));
        radioButton_91 = new QRadioButton(grbRB1);
        radioButton_91->setObjectName(QString::fromUtf8("radioButton_91"));
        radioButton_91->setGeometry(QRect(90, 10, 41, 21));
        radioButton_91->setChecked(true);
        radioButton_92 = new QRadioButton(grbRB1);
        radioButton_92->setObjectName(QString::fromUtf8("radioButton_92"));
        radioButton_92->setGeometry(QRect(10, 10, 31, 21));
        radioButton_92->setCheckable(true);
        radioButton_92->setChecked(false);
        radioButton_92->setAutoRepeat(true);
        radioButton_93 = new QRadioButton(grbRB1);
        radioButton_93->setObjectName(QString::fromUtf8("radioButton_93"));
        radioButton_93->setGeometry(QRect(50, 10, 31, 21));
        grbRB2 = new QGroupBox(grbEnbableIo);
        grbRB2->setObjectName(QString::fromUtf8("grbRB2"));
        grbRB2->setEnabled(true);
        grbRB2->setGeometry(QRect(10, 170, 131, 31));
        radioButton_94 = new QRadioButton(grbRB2);
        radioButton_94->setObjectName(QString::fromUtf8("radioButton_94"));
        radioButton_94->setGeometry(QRect(90, 10, 41, 21));
        radioButton_94->setChecked(true);
        radioButton_95 = new QRadioButton(grbRB2);
        radioButton_95->setObjectName(QString::fromUtf8("radioButton_95"));
        radioButton_95->setEnabled(true);
        radioButton_95->setGeometry(QRect(10, 10, 31, 21));
        radioButton_95->setCheckable(true);
        radioButton_95->setChecked(false);
        radioButton_95->setAutoRepeat(true);
        radioButton_96 = new QRadioButton(grbRB2);
        radioButton_96->setObjectName(QString::fromUtf8("radioButton_96"));
        radioButton_96->setGeometry(QRect(50, 10, 31, 21));
        grbRB3 = new QGroupBox(grbEnbableIo);
        grbRB3->setObjectName(QString::fromUtf8("grbRB3"));
        grbRB3->setEnabled(true);
        grbRB3->setGeometry(QRect(10, 140, 131, 31));
        radioButton_97 = new QRadioButton(grbRB3);
        radioButton_97->setObjectName(QString::fromUtf8("radioButton_97"));
        radioButton_97->setGeometry(QRect(90, 10, 41, 21));
        radioButton_97->setChecked(true);
        radioButton_98 = new QRadioButton(grbRB3);
        radioButton_98->setObjectName(QString::fromUtf8("radioButton_98"));
        radioButton_98->setEnabled(true);
        radioButton_98->setGeometry(QRect(10, 10, 31, 21));
        radioButton_98->setCheckable(true);
        radioButton_98->setChecked(false);
        radioButton_98->setAutoRepeat(true);
        radioButton_99 = new QRadioButton(grbRB3);
        radioButton_99->setObjectName(QString::fromUtf8("radioButton_99"));
        radioButton_99->setGeometry(QRect(50, 10, 31, 21));
        grbRB4 = new QGroupBox(grbEnbableIo);
        grbRB4->setObjectName(QString::fromUtf8("grbRB4"));
        grbRB4->setEnabled(true);
        grbRB4->setGeometry(QRect(10, 110, 131, 31));
        radioButton_100 = new QRadioButton(grbRB4);
        radioButton_100->setObjectName(QString::fromUtf8("radioButton_100"));
        radioButton_100->setGeometry(QRect(90, 10, 41, 21));
        radioButton_100->setChecked(true);
        radioButton_101 = new QRadioButton(grbRB4);
        radioButton_101->setObjectName(QString::fromUtf8("radioButton_101"));
        radioButton_101->setGeometry(QRect(10, 10, 31, 21));
        radioButton_101->setCheckable(true);
        radioButton_101->setChecked(false);
        radioButton_101->setAutoRepeat(true);
        radioButton_102 = new QRadioButton(grbRB4);
        radioButton_102->setObjectName(QString::fromUtf8("radioButton_102"));
        radioButton_102->setGeometry(QRect(50, 10, 31, 21));
        grbRB5 = new QGroupBox(grbEnbableIo);
        grbRB5->setObjectName(QString::fromUtf8("grbRB5"));
        grbRB5->setEnabled(true);
        grbRB5->setGeometry(QRect(10, 80, 131, 31));
        radioButton_103 = new QRadioButton(grbRB5);
        radioButton_103->setObjectName(QString::fromUtf8("radioButton_103"));
        radioButton_103->setGeometry(QRect(90, 10, 41, 21));
        radioButton_103->setChecked(true);
        radioButton_104 = new QRadioButton(grbRB5);
        radioButton_104->setObjectName(QString::fromUtf8("radioButton_104"));
        radioButton_104->setGeometry(QRect(10, 10, 31, 21));
        radioButton_104->setCheckable(true);
        radioButton_104->setChecked(false);
        radioButton_104->setAutoRepeat(true);
        radioButton_105 = new QRadioButton(grbRB5);
        radioButton_105->setObjectName(QString::fromUtf8("radioButton_105"));
        radioButton_105->setGeometry(QRect(50, 10, 31, 21));
        grbRB6 = new QGroupBox(grbEnbableIo);
        grbRB6->setObjectName(QString::fromUtf8("grbRB6"));
        grbRB6->setEnabled(true);
        grbRB6->setGeometry(QRect(10, 50, 131, 31));
        radioButton_106 = new QRadioButton(grbRB6);
        radioButton_106->setObjectName(QString::fromUtf8("radioButton_106"));
        radioButton_106->setGeometry(QRect(90, 10, 41, 21));
        radioButton_106->setChecked(true);
        radioButton_107 = new QRadioButton(grbRB6);
        radioButton_107->setObjectName(QString::fromUtf8("radioButton_107"));
        radioButton_107->setGeometry(QRect(10, 10, 31, 21));
        radioButton_107->setCheckable(true);
        radioButton_107->setChecked(false);
        radioButton_107->setAutoRepeat(true);
        radioButton_108 = new QRadioButton(grbRB6);
        radioButton_108->setObjectName(QString::fromUtf8("radioButton_108"));
        radioButton_108->setGeometry(QRect(50, 10, 31, 21));
        grbRB7 = new QGroupBox(grbEnbableIo);
        grbRB7->setObjectName(QString::fromUtf8("grbRB7"));
        grbRB7->setEnabled(true);
        grbRB7->setGeometry(QRect(10, 20, 131, 31));
        radioButton_112 = new QRadioButton(grbRB7);
        radioButton_112->setObjectName(QString::fromUtf8("radioButton_112"));
        radioButton_112->setGeometry(QRect(90, 10, 41, 21));
        radioButton_112->setChecked(true);
        radioButton_113 = new QRadioButton(grbRB7);
        radioButton_113->setObjectName(QString::fromUtf8("radioButton_113"));
        radioButton_113->setEnabled(true);
        radioButton_113->setGeometry(QRect(10, 10, 31, 21));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(radioButton_113->sizePolicy().hasHeightForWidth());
        radioButton_113->setSizePolicy(sizePolicy1);
        radioButton_113->setCheckable(true);
        radioButton_113->setChecked(false);
        radioButton_113->setAutoRepeat(true);
        radioButton_114 = new QRadioButton(grbRB7);
        radioButton_114->setObjectName(QString::fromUtf8("radioButton_114"));
        radioButton_114->setGeometry(QRect(50, 10, 31, 21));
        grbRC6 = new QGroupBox(grbEnbableIo);
        grbRC6->setObjectName(QString::fromUtf8("grbRC6"));
        grbRC6->setEnabled(true);
        grbRC6->setGeometry(QRect(160, 60, 131, 31));
        radioButton_115 = new QRadioButton(grbRC6);
        radioButton_115->setObjectName(QString::fromUtf8("radioButton_115"));
        radioButton_115->setGeometry(QRect(90, 10, 41, 21));
        radioButton_115->setChecked(true);
        radioButton_116 = new QRadioButton(grbRC6);
        radioButton_116->setObjectName(QString::fromUtf8("radioButton_116"));
        radioButton_116->setEnabled(true);
        radioButton_116->setGeometry(QRect(10, 10, 31, 21));
        radioButton_116->setCheckable(true);
        radioButton_116->setChecked(false);
        radioButton_116->setAutoRepeat(true);
        radioButton_117 = new QRadioButton(grbRC6);
        radioButton_117->setObjectName(QString::fromUtf8("radioButton_117"));
        radioButton_117->setGeometry(QRect(50, 10, 31, 21));
        grbRC7 = new QGroupBox(grbEnbableIo);
        grbRC7->setObjectName(QString::fromUtf8("grbRC7"));
        grbRC7->setEnabled(true);
        grbRC7->setGeometry(QRect(160, 30, 131, 31));
        radioButton_109 = new QRadioButton(grbRC7);
        radioButton_109->setObjectName(QString::fromUtf8("radioButton_109"));
        radioButton_109->setGeometry(QRect(90, 10, 41, 21));
        radioButton_109->setChecked(true);
        radioButton_110 = new QRadioButton(grbRC7);
        radioButton_110->setObjectName(QString::fromUtf8("radioButton_110"));
        radioButton_110->setGeometry(QRect(10, 10, 31, 21));
        radioButton_110->setCheckable(true);
        radioButton_110->setChecked(false);
        radioButton_110->setAutoRepeat(true);
        radioButton_111 = new QRadioButton(grbRC7);
        radioButton_111->setObjectName(QString::fromUtf8("radioButton_111"));
        radioButton_111->setGeometry(QRect(50, 10, 31, 21));
        grbRA3 = new QGroupBox(grbEnbableIo);
        grbRA3->setObjectName(QString::fromUtf8("grbRA3"));
        grbRA3->setEnabled(true);
        grbRA3->setGeometry(QRect(340, 30, 131, 31));
        radioButton_118 = new QRadioButton(grbRA3);
        radioButton_118->setObjectName(QString::fromUtf8("radioButton_118"));
        radioButton_118->setGeometry(QRect(90, 10, 41, 21));
        radioButton_118->setChecked(true);
        radioButton_119 = new QRadioButton(grbRA3);
        radioButton_119->setObjectName(QString::fromUtf8("radioButton_119"));
        radioButton_119->setGeometry(QRect(10, 10, 31, 21));
        radioButton_119->setCheckable(true);
        radioButton_119->setChecked(false);
        radioButton_119->setAutoRepeat(true);
        radioButton_120 = new QRadioButton(grbRA3);
        radioButton_120->setObjectName(QString::fromUtf8("radioButton_120"));
        radioButton_120->setGeometry(QRect(50, 10, 31, 21));
        grbRA4 = new QGroupBox(grbEnbableIo);
        grbRA4->setObjectName(QString::fromUtf8("grbRA4"));
        grbRA4->setEnabled(true);
        grbRA4->setGeometry(QRect(340, 60, 131, 31));
        radioButton_121 = new QRadioButton(grbRA4);
        radioButton_121->setObjectName(QString::fromUtf8("radioButton_121"));
        radioButton_121->setGeometry(QRect(90, 10, 41, 21));
        radioButton_121->setChecked(true);
        radioButton_122 = new QRadioButton(grbRA4);
        radioButton_122->setObjectName(QString::fromUtf8("radioButton_122"));
        radioButton_122->setGeometry(QRect(10, 10, 31, 21));
        radioButton_122->setCheckable(true);
        radioButton_122->setChecked(false);
        radioButton_122->setAutoRepeat(true);
        radioButton_123 = new QRadioButton(grbRA4);
        radioButton_123->setObjectName(QString::fromUtf8("radioButton_123"));
        radioButton_123->setGeometry(QRect(50, 10, 31, 21));
        grbRA5 = new QGroupBox(grbEnbableIo);
        grbRA5->setObjectName(QString::fromUtf8("grbRA5"));
        grbRA5->setEnabled(true);
        grbRA5->setGeometry(QRect(340, 90, 131, 31));
        radioButton_160 = new QRadioButton(grbRA5);
        radioButton_160->setObjectName(QString::fromUtf8("radioButton_160"));
        radioButton_160->setGeometry(QRect(90, 10, 41, 21));
        radioButton_160->setChecked(true);
        radioButton_161 = new QRadioButton(grbRA5);
        radioButton_161->setObjectName(QString::fromUtf8("radioButton_161"));
        radioButton_161->setGeometry(QRect(10, 10, 31, 21));
        radioButton_161->setCheckable(true);
        radioButton_161->setChecked(false);
        radioButton_161->setAutoRepeat(true);
        radioButton_162 = new QRadioButton(grbRA5);
        radioButton_162->setObjectName(QString::fromUtf8("radioButton_162"));
        radioButton_162->setGeometry(QRect(50, 10, 31, 21));
        ch_B_vcc = new QCheckBox(grbEnbableIo);
        ch_B_vcc->setObjectName(QString::fromUtf8("ch_B_vcc"));
        ch_B_vcc->setEnabled(true);
        ch_B_vcc->setGeometry(QRect(170, 150, 70, 21));
        ch_B_vpp = new QCheckBox(grbEnbableIo);
        ch_B_vpp->setObjectName(QString::fromUtf8("ch_B_vpp"));
        ch_B_vpp->setEnabled(true);
        ch_B_vpp->setGeometry(QRect(170, 130, 70, 17));
        chb_dctodc = new QCheckBox(grbEnbableIo);
        chb_dctodc->setObjectName(QString::fromUtf8("chb_dctodc"));
        chb_dctodc->setEnabled(true);
        chb_dctodc->setGeometry(QRect(170, 180, 70, 17));
        chb_dctodc->setChecked(false);
        chb_dctodc->setTristate(false);
        horizontalSlider = new QSlider(grbEnbableIo);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setEnabled(true);
        horizontalSlider->setGeometry(QRect(170, 220, 301, 21));
        horizontalSlider->setAutoFillBackground(false);
        horizontalSlider->setMinimum(50);
        horizontalSlider->setMaximum(140);
        horizontalSlider->setValue(50);
        horizontalSlider->setSliderPosition(50);
        horizontalSlider->setTracking(true);
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider->setInvertedAppearance(false);
        horizontalSlider->setInvertedControls(false);
        horizontalSlider->setTickPosition(QSlider::TicksAbove);
        horizontalSlider->setTickInterval(0);
        label_adc = new QLabel(grbEnbableIo);
        label_adc->setObjectName(QString::fromUtf8("label_adc"));
        label_adc->setEnabled(true);
        label_adc->setGeometry(QRect(270, 180, 121, 16));

        gridLayout->addWidget(grbEnbableIo, 0, 0, 1, 1);

        IOCheck->setCentralWidget(centralwidget);

        retranslateUi(IOCheck);

        QMetaObject::connectSlotsByName(IOCheck);
    } // setupUi

    void retranslateUi(QMainWindow *IOCheck)
    {
        grbEnbableIo->setTitle(QCoreApplication::translate("IOCheck", "ENABLE_IO", nullptr));
        radioButton_88->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_89->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_90->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRB1->setTitle(QCoreApplication::translate("IOCheck", "RB1=0", nullptr));
        radioButton_91->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_92->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_93->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRB2->setTitle(QCoreApplication::translate("IOCheck", "RB2=0", nullptr));
        radioButton_94->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_95->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_96->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRB3->setTitle(QCoreApplication::translate("IOCheck", "RB3=0", nullptr));
        radioButton_97->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_98->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_99->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRB4->setTitle(QCoreApplication::translate("IOCheck", "RB4=0", nullptr));
        radioButton_100->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_101->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_102->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRB5->setTitle(QCoreApplication::translate("IOCheck", "RB5=0", nullptr));
        radioButton_103->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_104->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_105->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRB6->setTitle(QCoreApplication::translate("IOCheck", "RB6=0", nullptr));
        radioButton_106->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_107->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_108->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRB7->setTitle(QCoreApplication::translate("IOCheck", "RB7=0", nullptr));
        radioButton_112->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_113->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_114->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRC6->setTitle(QCoreApplication::translate("IOCheck", "RC6=0", nullptr));
        radioButton_115->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_116->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_117->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRC7->setTitle(QCoreApplication::translate("IOCheck", "RC7=0", nullptr));
        radioButton_109->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_110->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_111->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRA3->setTitle(QCoreApplication::translate("IOCheck", "RA3=0", nullptr));
        radioButton_118->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_119->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_120->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRA4->setTitle(QCoreApplication::translate("IOCheck", "RA4=0", nullptr));
        radioButton_121->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_122->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_123->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        grbRA5->setTitle(QCoreApplication::translate("IOCheck", "RA5=0", nullptr));
        radioButton_160->setText(QCoreApplication::translate("IOCheck", "IN", nullptr));
        radioButton_161->setText(QCoreApplication::translate("IOCheck", "0", nullptr));
        radioButton_162->setText(QCoreApplication::translate("IOCheck", "1", nullptr));
        ch_B_vcc->setText(QCoreApplication::translate("IOCheck", "VDD", nullptr));
        ch_B_vpp->setText(QCoreApplication::translate("IOCheck", "VPP", nullptr));
        chb_dctodc->setText(QCoreApplication::translate("IOCheck", "DC TO DC", nullptr));
        label_adc->setText(QCoreApplication::translate("IOCheck", "VPP Voltage=", nullptr));
        (void)IOCheck;
    } // retranslateUi

};

namespace Ui {
    class IOCheck: public Ui_IOCheck {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IOCHECK_H
