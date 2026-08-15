#ifndef ICD_H
#define ICD_H


#include <QMainWindow>
#include <QAction>
#include <QDebug>
#include "Libs/globalVariables.h"
#include "HardwareLayer/Devices/devices.h"

namespace Ui {
class Icd;
}

class Icd : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Icd(QWidget *parent = 0);
    ~Icd();
   void createActions();
   void createToolbars();


private slots:

    void run_icd_slot();
    void run_halt_slot();
    void run_step_slot();
    void run_step_over_slot();
    void run_stop_slot();
    void run_refresh_slot();
    void open();


private:

    QAction *icd_run;
    QAction *icd_halt;
    QAction *icd_step;
    QAction *icd_step_over;
    QAction *icd_stop;
    QAction *icd_refresh;
    QAction *openAct;



    Ui::Icd *ui;
};

#endif // ICD_H
