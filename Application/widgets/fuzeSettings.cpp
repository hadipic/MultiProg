#include "widgets\fuzeSettings.h"
#include "ui_fuzeSettings.h"
/*struct DeviceType
{
    QString typeName;
};

struct DeviceInfo
{

};
*/

FuzeSettings::FuzeSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FuzeSettings)
{
    ui->setupUi(this);
}

FuzeSettings::~FuzeSettings()
{
    delete ui;
}
