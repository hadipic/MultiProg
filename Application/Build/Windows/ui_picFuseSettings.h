/********************************************************************************
** Form generated from reading UI file 'picFuseSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PICFUSESETTINGS_H
#define UI_PICFUSESETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/hexEditor/hexPlainTextEdit.h"

QT_BEGIN_NAMESPACE

class Ui_PicFuseSettings
{
public:
    QGridLayout *gridLayout_5;
    QLabel *label_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *lblByte;
    QLabel *label_3;
    QLabel *label;
    QScrollArea *scrollAreaAVRNavigation;
    QWidget *scrollAreaAVRWidgetContents;
    QGridLayout *gridLayout_8;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_4;
    QListWidget *lstConfigs;
    HexPlainTextEdit *txtMemID;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *PicFuseSettings)
    {
        if (PicFuseSettings->objectName().isEmpty())
            PicFuseSettings->setObjectName(QStringLiteral("PicFuseSettings"));
        PicFuseSettings->resize(510, 490);
        gridLayout_5 = new QGridLayout(PicFuseSettings);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_2 = new QLabel(PicFuseSettings);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_5->addWidget(label_2, 0, 0, 1, 1);

        groupBox = new QGroupBox(PicFuseSettings);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(400, 0));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lblByte = new QLabel(groupBox);
        lblByte->setObjectName(QStringLiteral("lblByte"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblByte->sizePolicy().hasHeightForWidth());
        lblByte->setSizePolicy(sizePolicy);
        lblByte->setMaximumSize(QSize(16777215, 10));
        QFont font;
        font.setFamily(QStringLiteral("Tahoma"));
        font.setPointSize(10);
        font.setBold(true);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(75);
        font.setStrikeOut(false);
        lblByte->setFont(font);
        lblByte->setAutoFillBackground(false);
        lblByte->setAlignment(Qt::AlignCenter);
        lblByte->setMargin(0);
        lblByte->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        gridLayout->addWidget(lblByte, 0, 0, 1, 3);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMaximumSize(QSize(30, 16777215));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 3, 1, 1);

        scrollAreaAVRNavigation = new QScrollArea(groupBox);
        scrollAreaAVRNavigation->setObjectName(QStringLiteral("scrollAreaAVRNavigation"));
        scrollAreaAVRNavigation->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollAreaAVRNavigation->setWidgetResizable(true);
        scrollAreaAVRWidgetContents = new QWidget();
        scrollAreaAVRWidgetContents->setObjectName(QStringLiteral("scrollAreaAVRWidgetContents"));
        scrollAreaAVRWidgetContents->setGeometry(QRect(0, 0, 356, 381));
        gridLayout_8 = new QGridLayout(scrollAreaAVRWidgetContents);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));

        verticalLayout->addLayout(gridLayout_4);


        gridLayout_8->addLayout(verticalLayout, 0, 0, 1, 1);

        scrollAreaAVRNavigation->setWidget(scrollAreaAVRWidgetContents);

        gridLayout->addWidget(scrollAreaAVRNavigation, 1, 0, 1, 3);

        lstConfigs = new QListWidget(groupBox);
        lstConfigs->setObjectName(QStringLiteral("lstConfigs"));
        lstConfigs->setMinimumSize(QSize(100, 0));
        lstConfigs->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(lstConfigs, 1, 3, 1, 1);

        txtMemID = new HexPlainTextEdit(groupBox);
        txtMemID->setObjectName(QStringLiteral("txtMemID"));
        txtMemID->setMinimumSize(QSize(270, 0));
        txtMemID->setMaximumSize(QSize(270, 25));
        QFont font1;
        font1.setFamily(QStringLiteral("Courier New"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        txtMemID->setFont(font1);
        txtMemID->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtMemID->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        txtMemID->setOverwriteMode(true);

        gridLayout->addWidget(txtMemID, 3, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 2, 1, 1);


        gridLayout_5->addWidget(groupBox, 1, 0, 1, 1);


        retranslateUi(PicFuseSettings);

        QMetaObject::connectSlotsByName(PicFuseSettings);
    } // setupUi

    void retranslateUi(QWidget *PicFuseSettings)
    {
        PicFuseSettings->setWindowTitle(QApplication::translate("PicFuseSettings", "Form", 0));
        label_2->setText(QApplication::translate("PicFuseSettings", "Settings:", 0));
        groupBox->setTitle(QString());
        lblByte->setText(QApplication::translate("PicFuseSettings", "0 0 1 0 1 0 1 0", 0));
        label_3->setText(QApplication::translate("PicFuseSettings", "ID:", 0));
        label->setText(QApplication::translate("PicFuseSettings", "Configurations Words:", 0));
        txtMemID->setPlainText(QString());
    } // retranslateUi

};

namespace Ui {
    class PicFuseSettings: public Ui_PicFuseSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PICFUSESETTINGS_H
