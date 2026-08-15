/********************************************************************************
** Form generated from reading UI file 'convert.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONVERT_H
#define UI_CONVERT_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_convert
{
public:
    QWidget *centralwidget;
    QGroupBox *gb_rezult;
    QLabel *lab_HEX;
    QLabel *lab_HEX_2;
    QLabel *label_des;
    QLabel *label_des_2;
    QLabel *label_accii;
    QLabel *label_accii_2;
    QLabel *label_bin;
    QLabel *label_bin2;
    QComboBox *cob_type_in;
    QTextEdit *textEdit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *convert)
    {
        if (convert->objectName().isEmpty())
            convert->setObjectName(QString::fromUtf8("convert"));
        convert->setWindowModality(Qt::NonModal);
        convert->resize(384, 178);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(convert->sizePolicy().hasHeightForWidth());
        convert->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/Tracer Icon 16.ico"), QSize(), QIcon::Normal, QIcon::Off);
        convert->setWindowIcon(icon);
        centralwidget = new QWidget(convert);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gb_rezult = new QGroupBox(centralwidget);
        gb_rezult->setObjectName(QString::fromUtf8("gb_rezult"));
        gb_rezult->setGeometry(QRect(20, 50, 331, 101));
        lab_HEX = new QLabel(gb_rezult);
        lab_HEX->setObjectName(QString::fromUtf8("lab_HEX"));
        lab_HEX->setGeometry(QRect(20, 20, 46, 13));
        lab_HEX_2 = new QLabel(gb_rezult);
        lab_HEX_2->setObjectName(QString::fromUtf8("lab_HEX_2"));
        lab_HEX_2->setGeometry(QRect(90, 20, 211, 16));
        label_des = new QLabel(gb_rezult);
        label_des->setObjectName(QString::fromUtf8("label_des"));
        label_des->setGeometry(QRect(20, 40, 46, 13));
        label_des_2 = new QLabel(gb_rezult);
        label_des_2->setObjectName(QString::fromUtf8("label_des_2"));
        label_des_2->setGeometry(QRect(90, 40, 231, 16));
        label_accii = new QLabel(gb_rezult);
        label_accii->setObjectName(QString::fromUtf8("label_accii"));
        label_accii->setGeometry(QRect(20, 80, 46, 13));
        label_accii_2 = new QLabel(gb_rezult);
        label_accii_2->setObjectName(QString::fromUtf8("label_accii_2"));
        label_accii_2->setGeometry(QRect(90, 80, 141, 16));
        label_bin = new QLabel(gb_rezult);
        label_bin->setObjectName(QString::fromUtf8("label_bin"));
        label_bin->setGeometry(QRect(20, 60, 46, 13));
        label_bin2 = new QLabel(gb_rezult);
        label_bin2->setObjectName(QString::fromUtf8("label_bin2"));
        label_bin2->setGeometry(QRect(90, 60, 231, 16));
        cob_type_in = new QComboBox(centralwidget);
        cob_type_in->addItem(QString());
        cob_type_in->addItem(QString());
        cob_type_in->addItem(QString());
        cob_type_in->addItem(QString());
        cob_type_in->addItem(QString());
        cob_type_in->setObjectName(QString::fromUtf8("cob_type_in"));
        cob_type_in->setGeometry(QRect(30, 20, 71, 22));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(110, 20, 91, 21));
        textEdit->setFrameShape(QFrame::Box);
        textEdit->setFrameShadow(QFrame::Plain);
        textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEdit->setTabStopWidth(6);
        textEdit->setAcceptRichText(true);
        convert->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(convert);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        convert->setStatusBar(statusbar);

        retranslateUi(convert);

        QMetaObject::connectSlotsByName(convert);
    } // setupUi

    void retranslateUi(QMainWindow *convert)
    {
        convert->setWindowTitle(QCoreApplication::translate("convert", "Converter", nullptr));
        gb_rezult->setTitle(QCoreApplication::translate("convert", "Results", nullptr));
        lab_HEX->setText(QCoreApplication::translate("convert", "<html><head/><body><p><span style=\" color:#00aa00;\">Hex</span></p></body></html>", nullptr));
        lab_HEX_2->setText(QCoreApplication::translate("convert", "0x", nullptr));
        label_des->setText(QCoreApplication::translate("convert", "<html><head/><body><p><span style=\" color:#00aa00;\">Dec</span></p></body></html>", nullptr));
        label_des_2->setText(QCoreApplication::translate("convert", "0", nullptr));
        label_accii->setText(QCoreApplication::translate("convert", "<html><head/><body><p><span style=\" color:#00aa00;\">Ascii</span></p></body></html>", nullptr));
        label_accii_2->setText(QString());
        label_bin->setText(QCoreApplication::translate("convert", "<html><head/><body><p><span style=\" color:#00aa00;\">Bin</span></p></body></html>", nullptr));
        label_bin2->setText(QCoreApplication::translate("convert", "<html><head/><body><p>0b</p></body></html>", nullptr));
        cob_type_in->setItemText(0, QCoreApplication::translate("convert", "Bin", nullptr));
        cob_type_in->setItemText(1, QCoreApplication::translate("convert", "Oct", nullptr));
        cob_type_in->setItemText(2, QCoreApplication::translate("convert", "Dec", nullptr));
        cob_type_in->setItemText(3, QCoreApplication::translate("convert", "Hex", nullptr));
        cob_type_in->setItemText(4, QCoreApplication::translate("convert", "ASCII", nullptr));

        textEdit->setHtml(QCoreApplication::translate("convert", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">12</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class convert: public Ui_convert {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVERT_H
