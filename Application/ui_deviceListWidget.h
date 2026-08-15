/********************************************************************************
** Form generated from reading UI file 'deviceListWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICELISTWIDGET_H
#define UI_DEVICELISTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
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
            DeviceListWidget->setObjectName(QString::fromUtf8("DeviceListWidget"));
        DeviceListWidget->setWindowModality(Qt::WindowModal);
        DeviceListWidget->setEnabled(true);
        DeviceListWidget->resize(355, 40);
        DeviceListWidget->setMinimumSize(QSize(200, 39));
        DeviceListWidget->setMaximumSize(QSize(400, 16777215));
        cboDeviceFamily = new QComboBox(DeviceListWidget);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        cboDeviceFamily->addItem(icon, QString());
        cboDeviceFamily->addItem(QString());
        cboDeviceFamily->setObjectName(QString::fromUtf8("cboDeviceFamily"));
        cboDeviceFamily->setGeometry(QRect(9, 9, 100, 21));
        cboDeviceFamily->setMinimumSize(QSize(0, 21));
        cboDeviceFamily->setMaximumSize(QSize(100, 16777215));
        cboDeviceTypes = new QComboBox(DeviceListWidget);
        cboDeviceTypes->addItem(icon, QString());
        cboDeviceTypes->addItem(QString());
        cboDeviceTypes->setObjectName(QString::fromUtf8("cboDeviceTypes"));
        cboDeviceTypes->setGeometry(QRect(115, 9, 100, 22));
        cboDeviceTypes->setMaximumSize(QSize(100, 16777215));
        cboDevicesInfo = new QComboBox(DeviceListWidget);
        cboDevicesInfo->setObjectName(QString::fromUtf8("cboDevicesInfo"));
        cboDevicesInfo->setGeometry(QRect(221, 10, 100, 20));
        cboDevicesInfo->setMinimumSize(QSize(100, 0));
        cboDevicesInfo->setMaximumSize(QSize(100, 16777215));
        btnDetectDevice = new QToolButton(DeviceListWidget);
        btnDetectDevice->setObjectName(QString::fromUtf8("btnDetectDevice"));
        btnDetectDevice->setGeometry(QRect(320, 4, 31, 31));
        btnDetectDevice->setAutoFillBackground(false);
        btnDetectDevice->setInputMethodHints(Qt::ImhNone);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icon/ArtWork Icon 16.ico"), QSize(), QIcon::Normal, QIcon::Off);
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
        DeviceListWidget->setWindowTitle(QCoreApplication::translate("DeviceListWidget", "Form", nullptr));
        cboDeviceFamily->setItemText(0, QCoreApplication::translate("DeviceListWidget", "<b>a</b>", nullptr));
        cboDeviceFamily->setItemText(1, QCoreApplication::translate("DeviceListWidget", "New Item", nullptr));

        cboDeviceTypes->setItemText(0, QCoreApplication::translate("DeviceListWidget", "<b>a</b>", nullptr));
        cboDeviceTypes->setItemText(1, QCoreApplication::translate("DeviceListWidget", "New Item", nullptr));

        btnDetectDevice->setText(QCoreApplication::translate("DeviceListWidget", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeviceListWidget: public Ui_DeviceListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICELISTWIDGET_H
