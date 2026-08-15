#ifndef FUSEBYTE_H
#define FUSEBYTE_H
#include <QString>
#include <QList>
#include <QSqlRecord>

struct FuseNavigation
{
    public:
    quint32 value;
    QString text;
};

struct FuseNavigationGroup
{
    public:
    quint32 mask;
    QString name;
    QList<FuseNavigation> members;
};

struct BitInfo
{
public:
    quint8  bitNumber;
    QString bitName;
    QString description;
};

class FuseByte
{
public:
    QList<BitInfo> bitsInfo;
    quint32 defualt;
    quint32 mask;
    quint32 value;
    QString name;
    QList<FuseNavigationGroup> navigationGroups;
    FuseByte();
    FuseByte(quint32 fuseByteID);
    FuseByte(const QSqlRecord &record);
    quint32 saveToDB(quint32 picDeviceID=0);
    void loadFromDB(quint32 fuseByteID);
    void loadFromDB(const QSqlRecord &record);
};

#endif // FUSESTRUCT_H
