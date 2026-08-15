#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QDebug>
#include <QSettings>
#include <QThread>

#include "Libs/globalVariables.h"
#include "Libs/commonFunctions.h"
#include "HardwareLayer/Devices/devices.h"

#include "Forms/ioCheck.h"
#include "Forms/convert.h"
#include "Forms/Icd.h"
#include "Forms/dis_asmble.h"

#include <HardwareLayer/fileIO.h>

namespace Ui {
    class MainWindow;
}
namespace MemoryType
{
    enum MemoryType{Flash=0, EEProm=1, Flash_EEProm=2};
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createActions();

    void createToolbars();

    IOCheck     ioCheck;
    convert     conv;
    Icd         icd;
    Dis_asmble  dis_asm;


    Ui::MainWindow *ui;
    template <typename T>
    void clearMemory(T *&mem, quint32 &destSize, quint32 size, quint32 value);
private:

    QAction *stutAct;
    QAction *newAct;
    QAction *openAct;
    QAction *openEEAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *mnuOpen;

    QAction *readchip;
    QAction *writechip;
    QAction *_detectHardware;
    QAction *_none;

    QString fileName;

    QSettings *settings;
    qint8 save_falg;

    void loadSettings();
    void saveSettings();


    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

  //  QPlainTextEdit *textEdit;
    QString curFile;

    QMenu *fileMenu;
    QMenu *editMenu;

    DeviceFamily *deviceFamily;
    DeviceType *deviceType;
    DeviceInfo *deviceInfo;

    FileIO fileIO;
    USBTools *usb;
    QThread workerThread;

    void loadFromBuffer(MemoryType::MemoryType memoryType, bool forEmpty=false);
private slots:

    void newFile();
    void open();
    void openEE();
    bool save();
    bool saveAs();
    void  io_show();
    void  con_show();
    void  Icd_show();
    void  Dis_asm_show();

    void on_Progress_Changed(int step, int max);
    void Print_Status(QString msg11 ,int j1,int i1);

    //void readSettings();
    //void writeSettings();


    void readChip();
    void readDataReady();
    void writeChip();
    void detectHardware();

    void on_MainWindow_toolButtonStyleChanged(const Qt::ToolButtonStyle &toolButtonStyle);

    //void on_grbEnbableIo_clicked(bool checked);
    //void on_io_chek_currentChanged(int index);


    void on_DeviceSelected(DeviceFamily *family, DeviceType *type, DeviceInfo *deviceInfo);
signals:
    void read(int ee, int r);
};

#endif // MAINWINDOW_H
