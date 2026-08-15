#ifndef IOCHECK_H
#define IOCHECK_H


#include <QMainWindow>
#include <QAction>
#include <QDebug>
#include "Libs/globalVariables.h"
#include "HardwareLayer/Devices/devices.h"
#include "Forms/ioCheck.h"

namespace Ui {
class IOCheck;
}

class IOCheck : public QMainWindow
{
    Q_OBJECT

public:
    explicit IOCheck(QWidget *parent = 0);
    ~IOCheck();
    
private slots:


    void on_radioButton_89_clicked();
    void on_radioButton_90_clicked();
    void on_radioButton_88_clicked();
    void on_radioButton_92_clicked();
    void on_radioButton_93_clicked();
    void on_radioButton_111_clicked();
    void on_radioButton_110_clicked();
    void on_radioButton_117_clicked();
    void on_radioButton_116_clicked();
    void on_radioButton_120_clicked();
    void on_radioButton_123_clicked();
    void on_radioButton_162_clicked();
    void on_radioButton_119_clicked();
    void on_radioButton_122_clicked();
    void on_radioButton_161_clicked();
    void on_radioButton_160_clicked();
    void on_radioButton_121_clicked();
    void on_radioButton_118_clicked();
    void on_radioButton_109_clicked();
    void on_radioButton_115_clicked();
    void on_radioButton_95_clicked();
    void on_radioButton_98_clicked();
    void on_radioButton_101_clicked();
    void on_radioButton_104_clicked();
    void on_radioButton_107_clicked();
    void on_radioButton_96_clicked();
    void on_radioButton_99_clicked();
    void on_radioButton_102_clicked();
    void on_radioButton_105_clicked();
    void on_radioButton_108_clicked();
    void on_radioButton_114_clicked();
     void on_radioButton_113_clicked();
    void on_radioButton_112_clicked();
    void on_radioButton_106_clicked();
    void on_radioButton_103_clicked();
    void on_radioButton_100_clicked();
    void on_radioButton_97_clicked();
    void on_radioButton_94_clicked();
    void on_radioButton_91_clicked();
    void onIoChanged1();

    void on_grbEnbableIo_clicked(bool checked);
    void on_io_chek_currentChanged(int index);

   // void on_btnRead_clicked();
   // void on_btnWrite_clicked();



private:
    Ui::IOCheck *ui;
    DeviceFamily *deviceFamily;
    DeviceType *deviceType;
    DeviceInfo *deviceInfo;


};

#endif // IOCHECK_H
