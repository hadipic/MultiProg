#ifndef FUZESETTINGS_H
#define FUZESETTINGS_H

#include <QWidget>

namespace Ui {
class FuzeSettings;
}

class FuzeSettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit FuzeSettings(QWidget *parent = 0);
    ~FuzeSettings();
    
private:
    Ui::FuzeSettings *ui;
};

#endif // FUZESETTINGS_H
