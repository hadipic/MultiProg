#ifndef DIS_ASMBLE_H
#define DIS_ASMBLE_H

#include <QMainWindow>
#include <QAction>
#include <QDebug>
#include "Libs/globalVariables.h"
#include "HardwareLayer/Devices/devices.h"


namespace Ui {
class Dis_asmble;
}

class Dis_asmble : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Dis_asmble(QWidget *parent = 0);
    ~Dis_asmble();
    


private:

    Ui::Dis_asmble *ui;
};

#endif // DIS_ASMBLE_H
