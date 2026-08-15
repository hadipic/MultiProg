/********************************************************************************
** Form generated from reading UI file 'dis_asmble.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIS_ASMBLE_H
#define UI_DIS_ASMBLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dis_asmble
{
public:
    QAction *actionSave;
    QAction *actionExit;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEdit;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuSeting;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Dis_asmble)
    {
        if (Dis_asmble->objectName().isEmpty())
            Dis_asmble->setObjectName(QStringLiteral("Dis_asmble"));
        Dis_asmble->resize(474, 443);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/ArtWork Icon 72.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Dis_asmble->setWindowIcon(icon);
        actionSave = new QAction(Dis_asmble);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionExit = new QAction(Dis_asmble);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralwidget = new QWidget(Dis_asmble);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        plainTextEdit = new QPlainTextEdit(centralwidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);

        Dis_asmble->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Dis_asmble);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 474, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuSeting = new QMenu(menubar);
        menuSeting->setObjectName(QStringLiteral("menuSeting"));
        menuSeting->setEnabled(false);
        Dis_asmble->setMenuBar(menubar);
        statusbar = new QStatusBar(Dis_asmble);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        Dis_asmble->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuSeting->menuAction());
        menuFile->addAction(actionSave);
        menuFile->addAction(actionExit);

        retranslateUi(Dis_asmble);

        QMetaObject::connectSlotsByName(Dis_asmble);
    } // setupUi

    void retranslateUi(QMainWindow *Dis_asmble)
    {
        Dis_asmble->setWindowTitle(QApplication::translate("Dis_asmble", "Dis Asembler", 0));
        actionSave->setText(QApplication::translate("Dis_asmble", "Save", 0));
        actionExit->setText(QApplication::translate("Dis_asmble", "Exit", 0));
        menuFile->setTitle(QApplication::translate("Dis_asmble", "File", 0));
        menuSeting->setTitle(QApplication::translate("Dis_asmble", "Seting", 0));
    } // retranslateUi

};

namespace Ui {
    class Dis_asmble: public Ui_Dis_asmble {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIS_ASMBLE_H
