#ifndef AVRFUZESETTINGS_H
#define AVRFUZESETTINGS_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include "ui_avrFuseSettings.h"
#include "HardwareLayer/Devices/AVR/progAVR.h"
#include "HardwareLayer/Devices/devices.h"
#include "HardwareLayer/Devices/AVR/avrFuse.h"
#include "HardwareLayer/Devices/AVR/readXMLAVR.h"

namespace Ui
{
class AvrFuseSettings;
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

class AvrFuseSettings : public QWidget
{
	Q_OBJECT
public:
    explicit AvrFuseSettings(QWidget *parent = 0);
    ~AvrFuseSettings();
    Ui::AvrFuseSettings *getUi();
    Ui::AvrFuseSettings *ui;
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


    //  Ui::AvrFuseSettings *ui;
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
    void on_btnAVRFuseRead_clicked();
    void on_btnAVRLockRead_clicked();
    void on_btnAVRFuseWrite_clicked();
    void onFuseRead();
    void on_btnAVRLockWrite_clicked();
};

#endif // AVRFUZESETTINGS_H
