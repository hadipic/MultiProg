/********************************************************************************
** Form generated from reading UI file 'icd.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICD_H
#define UI_ICD_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Icd
{
public:
    QAction *actionLoad_Conf;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *plainTextEdit_source;
    QPlainTextEdit *plainTextEdit_source_2;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *Icd)
    {
        if (Icd->objectName().isEmpty())
            Icd->setObjectName(QString::fromUtf8("Icd"));
        Icd->resize(744, 362);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/ArtWork Icon 38.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Icd->setWindowIcon(icon);
        actionLoad_Conf = new QAction(Icd);
        actionLoad_Conf->setObjectName(QString::fromUtf8("actionLoad_Conf"));
        centralwidget = new QWidget(Icd);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        plainTextEdit_source = new QPlainTextEdit(centralwidget);
        plainTextEdit_source->setObjectName(QString::fromUtf8("plainTextEdit_source"));

        horizontalLayout->addWidget(plainTextEdit_source);

        plainTextEdit_source_2 = new QPlainTextEdit(centralwidget);
        plainTextEdit_source_2->setObjectName(QString::fromUtf8("plainTextEdit_source_2"));

        horizontalLayout->addWidget(plainTextEdit_source_2);

        Icd->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Icd);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 744, 20));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        Icd->setMenuBar(menubar);
        statusbar = new QStatusBar(Icd);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Icd->setStatusBar(statusbar);
        toolBar = new QToolBar(Icd);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMinimumSize(QSize(3, 0));
        Icd->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionLoad_Conf);

        retranslateUi(Icd);

        QMetaObject::connectSlotsByName(Icd);
    } // setupUi

    void retranslateUi(QMainWindow *Icd)
    {
        Icd->setWindowTitle(QCoreApplication::translate("Icd", "Icd", nullptr));
        actionLoad_Conf->setText(QCoreApplication::translate("Icd", "Load Coff File", nullptr));
        menuFile->setTitle(QCoreApplication::translate("Icd", "File", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("Icd", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Icd: public Ui_Icd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICD_H
