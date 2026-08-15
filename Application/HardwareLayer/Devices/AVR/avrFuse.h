#ifndef AVRFUSE_H
#define AVRFUSE_H
#include <QString>
#include <QList>
#include "../fuseByte.h"

namespace AvrFuseType
{
    enum AvrFuseType{NONE=0, FuseLow, FuseHigh, FuseExtend, FuseLock};
}


class AvrFuse
{
public:
    AvrFuse();
    AvrFuse(QString avrName);
    FuseByte low;
    FuseByte high;
    FuseByte extend;
    FuseByte lock;
    void loadFromDB(QString avrName);
    //SaveToJson();
    //ReadFromJson();
};

#endif // AVRFUSE_H
