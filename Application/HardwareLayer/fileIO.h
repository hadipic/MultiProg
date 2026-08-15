#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>
#include "usbTools.h"
#include "HardwareLayer/Devices/devices.h"

class FileIO : public QObject
{
    Q_OBJECT
public:
    explicit FileIO(QObject *parent = 0);
    USBTools *usb;

    int load(DeviceInfo*dev, QString loadfile);
    int load(DeviceInfo *deviceInfo, char *dev, char *loadfile);

    int htoi(const char *hex, int length);

    int  save(DeviceInfo *deviceInfo, QString loadfile);
    void save(DeviceInfo *deviceInfo, char *dev, char *savefile);


    int loadEE(QString dev, QString loadfile);
    void loadEE(char *dev, char *loadfile);

    int saveEE(QString dev, QString loadfile);
    void saveEE(char *dev, char *savefile);


    void saveToMemCODE_W  (const Memory &memory, unsigned char *mem);
    void loadFromMemCODE_W(const Memory &memory, unsigned char *mem);
private:

signals:
    
public slots:
    
};


#endif // FILEIO_H
