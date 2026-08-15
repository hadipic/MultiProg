/********************************************************************************
** Form generated from reading UI file 'hexEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEXEDITOR_H
#define UI_HEXEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
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
            HexEditor->setObjectName(QStringLiteral("HexEditor"));
        HexEditor->resize(730, 399);
        HexEditor->setMinimumSize(QSize(300, 0));
        QFont font;
        font.setFamily(QStringLiteral("Courier New"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        HexEditor->setFont(font);
        actionSetscrol = new QAction(HexEditor);
        actionSetscrol->setObjectName(QStringLiteral("actionSetscrol"));
        gridLayout = new QGridLayout(HexEditor);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        hexTextEdit = new HexPlainTextEdit(HexEditor);
        hexTextEdit->setObjectName(QStringLiteral("hexTextEdit"));
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
        asciiTextEdit->setObjectName(QStringLiteral("asciiTextEdit"));
        asciiTextEdit->setMinimumSize(QSize(200, 0));
        asciiTextEdit->setMaximumSize(QSize(16777215, 16777215));
        asciiTextEdit->setFont(font);
        asciiTextEdit->setStyleSheet(QStringLiteral("border=0;"));
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
        HexEditor->setWindowTitle(QApplication::translate("HexEditor", "Form", 0));
        actionSetscrol->setText(QApplication::translate("HexEditor", "setscrol", 0));
        hexTextEdit->setPlainText(QString());
        asciiTextEdit->setPlainText(QString());
    } // retranslateUi

};

namespace Ui {
    class HexEditor: public Ui_HexEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEXEDITOR_H
