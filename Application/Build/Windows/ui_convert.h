/********************************************************************************
** Form generated from reading UI file 'convert.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONVERT_H
#define UI_CONVERT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
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
            convert->setObjectName(QStringLiteral("convert"));
        convert->setWindowModality(Qt::NonModal);
        convert->resize(384, 178);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(convert->sizePolicy().hasHeightForWidth());
        convert->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/Tracer Icon 16.ico"), QSize(), QIcon::Normal, QIcon::Off);
        convert->setWindowIcon(icon);
        centralwidget = new QWidget(convert);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gb_rezult = new QGroupBox(centralwidget);
        gb_rezult->setObjectName(QStringLiteral("gb_rezult"));
        gb_rezult->setGeometry(QRect(20, 50, 331, 101));
        lab_HEX = new QLabel(gb_rezult);
        lab_HEX->setObjectName(QStringLiteral("lab_HEX"));
        lab_HEX->setGeometry(QRect(20, 20, 46, 13));
        lab_HEX_2 = new QLabel(gb_rezult);
        lab_HEX_2->setObjectName(QStringLiteral("lab_HEX_2"));
        lab_HEX_2->setGeometry(QRect(90, 20, 211, 16));
        label_des = new QLabel(gb_rezult);
        label_des->setObjectName(QStringLiteral("label_des"));
        label_des->setGeometry(QRect(20, 40, 46, 13));
        label_des_2 = new QLabel(gb_rezult);
        label_des_2->setObjectName(QStringLiteral("label_des_2"));
        label_des_2->setGeometry(QRect(90, 40, 231, 16));
        label_accii = new QLabel(gb_rezult);
        label_accii->setObjectName(QStringLiteral("label_accii"));
        label_accii->setGeometry(QRect(20, 80, 46, 13));
        label_accii_2 = new QLabel(gb_rezult);
        label_accii_2->setObjectName(QStringLiteral("label_accii_2"));
        label_accii_2->setGeometry(QRect(90, 80, 141, 16));
        label_bin = new QLabel(gb_rezult);
        label_bin->setObjectName(QStringLiteral("label_bin"));
        label_bin->setGeometry(QRect(20, 60, 46, 13));
        label_bin2 = new QLabel(gb_rezult);
        label_bin2->setObjectName(QStringLiteral("label_bin2"));
        label_bin2->setGeometry(QRect(90, 60, 231, 16));
        cob_type_in = new QComboBox(centralwidget);
        cob_type_in->setObjectName(QStringLiteral("cob_type_in"));
        cob_type_in->setGeometry(QRect(30, 20, 71, 22));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(110, 20, 91, 21));
        textEdit->setFrameShape(QFrame::Box);
        textEdit->setFrameShadow(QFrame::Plain);
        textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEdit->setTabStopWidth(6);
        textEdit->setAcceptRichText(true);
        convert->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(convert);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        convert->setStatusBar(statusbar);

        retranslateUi(convert);

        QMetaObject::connectSlotsByName(convert);
    } // setupUi

    void retranslateUi(QMainWindow *convert)
    {
        convert->setWindowTitle(QApplication::translate("convert", "Converter", 0));
        gb_rezult->setTitle(QApplication::translate("convert", "Results", 0));
        lab_HEX->setText(QApplication::translate("convert", "<html><head/><body><p><span style=\" color:#00aa00;\">Hex</span></p></body></html>", 0));
        lab_HEX_2->setText(QApplication::translate("convert", "0x", 0));
        label_des->setText(QApplication::translate("convert", "<html><head/><body><p><span style=\" color:#00aa00;\">Dec</span></p></body></html>", 0));
        label_des_2->setText(QApplication::translate("convert", "0", 0));
        label_accii->setText(QApplication::translate("convert", "<html><head/><body><p><span style=\" color:#00aa00;\">Ascii</span></p></body></html>", 0));
        label_accii_2->setText(QString());
        label_bin->setText(QApplication::translate("convert", "<html><head/><body><p><span style=\" color:#00aa00;\">Bin</span></p></body></html>", 0));
        label_bin2->setText(QApplication::translate("convert", "<html><head/><body><p>0b</p></body></html>", 0));
        cob_type_in->clear();
        cob_type_in->insertItems(0, QStringList()
         << QApplication::translate("convert", "Bin", 0)
         << QApplication::translate("convert", "Oct", 0)
         << QApplication::translate("convert", "Dec", 0)
         << QApplication::translate("convert", "Hex", 0)
         << QApplication::translate("convert", "ASCII", 0)
        );
        textEdit->setHtml(QApplication::translate("convert", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">12</span></p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class convert: public Ui_convert {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVERT_H
