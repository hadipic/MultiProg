#ifndef FUSESTRUCT_H
#define FUSESTRUCT_H
#include <QString>
#include <QList>

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

struct FuseByte
{
public:
    QList<BitInfo> bitsInfo;
    quint32 defualt;
    quint32 mask;
    quint32 value;
    QString name;
    QList<FuseNavigationGroup> navigationGroups;
};

#endif // FUSESTRUCT_H
