/********************************************************************************
** Form generated from reading UI file 'deviceListWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICELISTWIDGET_H
#define UI_DEVICELISTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeviceListWidget
{
public:
    QComboBox *cboDeviceFamily;
    QComboBox *cboDeviceTypes;
    QComboBox *cboDevicesInfo;
    QToolButton *btnDetectDevice;

    void setupUi(QWidget *DeviceListWidget)
    {
        if (DeviceListWidget->objectName().isEmpty())
            DeviceListWidget->setObjectName(QStringLiteral("DeviceListWidget"));
        DeviceListWidget->setWindowModality(Qt::WindowModal);
        DeviceListWidget->setEnabled(true);
        DeviceListWidget->resize(355, 40);
        DeviceListWidget->setMinimumSize(QSize(200, 39));
        DeviceListWidget->setMaximumSize(QSize(400, 16777215));
        cboDeviceFamily = new QComboBox(DeviceListWidget);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        cboDeviceFamily->addItem(icon, QString());
        cboDeviceFamily->addItem(QString());
        cboDeviceFamily->setObjectName(QStringLiteral("cboDeviceFamily"));
        cboDeviceFamily->setGeometry(QRect(9, 9, 100, 21));
        cboDeviceFamily->setMinimumSize(QSize(0, 21));
        cboDeviceFamily->setMaximumSize(QSize(100, 16777215));
        cboDeviceTypes = new QComboBox(DeviceListWidget);
        cboDeviceTypes->addItem(icon, QString());
        cboDeviceTypes->addItem(QString());
        cboDeviceTypes->setObjectName(QStringLiteral("cboDeviceTypes"));
        cboDeviceTypes->setGeometry(QRect(115, 9, 100, 22));
        cboDeviceTypes->setMaximumSize(QSize(100, 16777215));
        cboDevicesInfo = new QComboBox(DeviceListWidget);
        cboDevicesInfo->setObjectName(QStringLiteral("cboDevicesInfo"));
        cboDevicesInfo->setGeometry(QRect(221, 10, 100, 20));
        cboDevicesInfo->setMinimumSize(QSize(100, 0));
        cboDevicesInfo->setMaximumSize(QSize(100, 16777215));
        btnDetectDevice = new QToolButton(DeviceListWidget);
        btnDetectDevice->setObjectName(QStringLiteral("btnDetectDevice"));
        btnDetectDevice->setGeometry(QRect(320, 4, 31, 31));
        btnDetectDevice->setAutoFillBackground(false);
        btnDetectDevice->setInputMethodHints(Qt::ImhNone);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icon/ArtWork Icon 16.ico"), QSize(), QIcon::Normal, QIcon::Off);
        btnDetectDevice->setIcon(icon1);
        btnDetectDevice->setIconSize(QSize(32, 32));
        btnDetectDevice->setCheckable(false);
        btnDetectDevice->setToolButtonStyle(Qt::ToolButtonIconOnly);
        btnDetectDevice->setAutoRaise(true);

        retranslateUi(DeviceListWidget);

        QMetaObject::connectSlotsByName(DeviceListWidget);
    } // setupUi

    void retranslateUi(QWidget *DeviceListWidget)
    {
        DeviceListWidget->setWindowTitle(QApplication::translate("DeviceListWidget", "Form", 0));
        cboDeviceFamily->setItemText(0, QApplication::translate("DeviceListWidget", "<b>a</b>", 0));
        cboDeviceFamily->setItemText(1, QApplication::translate("DeviceListWidget", "New Item", 0));

        cboDeviceTypes->setItemText(0, QApplication::translate("DeviceListWidget", "<b>a</b>", 0));
        cboDeviceTypes->setItemText(1, QApplication::translate("DeviceListWidget", "New Item", 0));

        btnDetectDevice->setText(QApplication::translate("DeviceListWidget", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class DeviceListWidget: public Ui_DeviceListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICELISTWIDGET_H
