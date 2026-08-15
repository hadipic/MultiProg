/********************************************************************************
** Form generated from reading UI file 'hexEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEXEDITOR_H
#define UI_HEXEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QWidget>
#include "widgets/hexEditor/asciiPlainTextEdit.h"
#include "widgets/hexEditor/hexPlainTextEdit.h"

QT_BEGIN_NAMESPACE

class Ui_HexEditor
{
public:
    QAction *actionSetscrol;
    QGridLayout *gridLayout;
    HexPlainTextEdit *hexTextEdit;
    AsciiPlainTextEdit *asciiTextEdit;

    void setupUi(QWidget *HexEditor)
    {
        if (HexEditor->objectName().isEmpty())
            HexEditor->setObjectName(QString::fromUtf8("HexEditor"));
        HexEditor->resize(730, 399);
        HexEditor->setMinimumSize(QSize(300, 0));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        HexEditor->setFont(font);
        actionSetscrol = new QAction(HexEditor);
        actionSetscrol->setObjectName(QString::fromUtf8("actionSetscrol"));
        gridLayout = new QGridLayout(HexEditor);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hexTextEdit = new HexPlainTextEdit(HexEditor);
        hexTextEdit->setObjectName(QString::fromUtf8("hexTextEdit"));
        hexTextEdit->setMinimumSize(QSize(500, 0));
        hexTextEdit->setMaximumSize(QSize(16777215, 16777215));
        hexTextEdit->setFont(font);
        hexTextEdit->setLineWidth(1);
        hexTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        hexTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        hexTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        hexTextEdit->setOverwriteMode(true);
        hexTextEdit->setCursorWidth(2);
        hexTextEdit->setMaximumBlockCount(0);
        hexTextEdit->setCenterOnScroll(true);

        gridLayout->addWidget(hexTextEdit, 0, 0, 1, 1);

        asciiTextEdit = new AsciiPlainTextEdit(HexEditor);
        asciiTextEdit->setObjectName(QString::fromUtf8("asciiTextEdit"));
        asciiTextEdit->setMinimumSize(QSize(200, 0));
        asciiTextEdit->setMaximumSize(QSize(16777215, 16777215));
        asciiTextEdit->setFont(font);
        asciiTextEdit->setStyleSheet(QString::fromUtf8("border=0;"));
        asciiTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        asciiTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        asciiTextEdit->setOverwriteMode(true);
        asciiTextEdit->setCenterOnScroll(true);

        gridLayout->addWidget(asciiTextEdit, 0, 1, 1, 1);


        retranslateUi(HexEditor);

        QMetaObject::connectSlotsByName(HexEditor);
    } // setupUi

    void retranslateUi(QWidget *HexEditor)
    {
        HexEditor->setWindowTitle(QCoreApplication::translate("HexEditor", "Form", nullptr));
        actionSetscrol->setText(QCoreApplication::translate("HexEditor", "setscrol", nullptr));
        hexTextEdit->setPlainText(QString());
        asciiTextEdit->setPlainText(QString());
    } // retranslateUi

};

namespace Ui {
    class HexEditor: public Ui_HexEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEXEDITOR_H
