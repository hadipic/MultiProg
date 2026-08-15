#ifndef FUZESETTINGS_H
#define FUZESETTINGS_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include "ui_fuseSettings.h"
#include "HardwareLayer/Devices/AVR/progAVR.h"
#include "HardwareLayer/Devices/devices.h"
#include "HardwareLayer/Devices/AVR/avrFuse.h"
#include "HardwareLayer/Devices/AVR/readXMLAVRList.h"

namespace Ui
{
class FuseSettings;
}

struct CheckBoxNav
{
public:
    CheckBoxNav(QCheckBox* checkBox, int mask, AvrFuseType::AvrFuseType fuseType)
    {
        this->checkBox=checkBox;
        this->mask=mask;
        this->fuseType=fuseType;
    }
    AvrFuseType::AvrFuseType fuseType;
    QCheckBox* checkBox;
    int mask;
};

struct ComboBoxNav
{
public:
    ComboBoxNav(QComboBox* comboBox, int mask, AvrFuseType::AvrFuseType fuseType)
    {
        this->comboBox=comboBox;
        this->mask=mask;
        this->fuseType=fuseType;
    }
    AvrFuseType::AvrFuseType fuseType;
    QComboBox* comboBox;
    int mask;
};

class FuseSettings : public QWidget
{
	Q_OBJECT
public:
    explicit FuseSettings(QWidget *parent = 0);
    ~FuseSettings();
    Ui::FuseSettings *getUi();
    Ui::FuseSettings *ui;
    void setDeviceInfo(DeviceInfo *deviceInfo);
    //------------- AVR ---------------------------
    void loadAVRFuseSettings();
    //----------------------------------------

	void setAvrNavigationFromFuseCheckbox(QCheckBox *checkBox);
private:
    DeviceInfo *deviceInfo;
    //------------- AVR ---------------------------
    void setAvrCheckBoxFromFuse();
    ProgAVR *progAVR;
    QList<QCheckBox*> lowCheckBoxes;
    QList<QCheckBox*> highCheckBoxes;
    QList<QCheckBox*> extendCheckBoxes;
    QList<QCheckBox*> lockCheckBoxes;



    QList<CheckBoxNav> avrNavCheckBoxes;
    QList<ComboBoxNav> avrNavComboBoxes;

    AvrFuse *avrFuse;
    QLabel *lblLow, *lblHigh, *lblExtend, *lblLock;
    void loadAVR1Fuse(QList<QCheckBox *> *checkBoxList, FuseByte *fuseByte, QTextEdit *textBox);
    //----------------------------------------


    //  Ui::FuseSettings *ui;
    void makeGroupCheckBoxes(FuseByte *fuseByte, QList<QCheckBox *> *checkBoxList);
    bool makeAvrNavigation(FuseByte *fuseByte, QLabel *&label, QString labelText, AvrFuseType::AvrFuseType fuseType);
public slots:
    void onAVRCheckBoxChanged();
    void onAvrNavCheckBoxChanged();
    void onAvrNavComboBoxChanged(int index);
private slots:

    void avrSettingsTextChanged();
    void on_txtAVRLow_textChanged();
    void on_txtAVRHigh_textChanged();
    void on_txtAVRExtend_textChanged();
    void on_txtAVRLock_textChanged();
    void on_btnAVRFuzeRead_clicked();
    void on_btnAVRLockRead_clicked();
    void on_btnAVRFuzeWrite_clicked();
};

#endif // FUZESETTINGS_H
