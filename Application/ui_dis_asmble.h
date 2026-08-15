/********************************************************************************
** Form generated from reading UI file 'dis_asmble.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIS_ASMBLE_H
#define UI_DIS_ASMBLE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
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
            Dis_asmble->setObjectName(QString::fromUtf8("Dis_asmble"));
        Dis_asmble->resize(474, 443);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/ArtWork Icon 72.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Dis_asmble->setWindowIcon(icon);
        actionSave = new QAction(Dis_asmble);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionExit = new QAction(Dis_asmble);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralwidget = new QWidget(Dis_asmble);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        plainTextEdit = new QPlainTextEdit(centralwidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);

        Dis_asmble->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Dis_asmble);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 474, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuSeting = new QMenu(menubar);
        menuSeting->setObjectName(QString::fromUtf8("menuSeting"));
        menuSeting->setEnabled(false);
        Dis_asmble->setMenuBar(menubar);
        statusbar = new QStatusBar(Dis_asmble);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
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
        Dis_asmble->setWindowTitle(QCoreApplication::translate("Dis_asmble", "Dis Asembler", nullptr));
        actionSave->setText(QCoreApplication::translate("Dis_asmble", "Save", nullptr));
        actionExit->setText(QCoreApplication::translate("Dis_asmble", "Exit", nullptr));
        menuFile->setTitle(QCoreApplication::translate("Dis_asmble", "File", nullptr));
        menuSeting->setTitle(QCoreApplication::translate("Dis_asmble", "Seting", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dis_asmble: public Ui_Dis_asmble {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIS_ASMBLE_H
