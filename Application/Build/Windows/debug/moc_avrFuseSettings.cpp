/****************************************************************************
** Meta object code from reading C++ file 'avrFuseSettings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../widgets/fuseSettings/avrFuseSettings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'avrFuseSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AvrFuseSettings_t {
    QByteArrayData data[16];
    char stringdata[337];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AvrFuseSettings_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AvrFuseSettings_t qt_meta_stringdata_AvrFuseSettings = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AvrFuseSettings"
QT_MOC_LITERAL(1, 16, 20), // "onAVRCheckBoxChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 23), // "onAvrNavCheckBoxChanged"
QT_MOC_LITERAL(4, 62, 23), // "onAvrNavComboBoxChanged"
QT_MOC_LITERAL(5, 86, 5), // "index"
QT_MOC_LITERAL(6, 92, 22), // "avrSettingsTextChanged"
QT_MOC_LITERAL(7, 115, 24), // "on_txtAVRLow_textChanged"
QT_MOC_LITERAL(8, 140, 25), // "on_txtAVRHigh_textChanged"
QT_MOC_LITERAL(9, 166, 27), // "on_txtAVRExtend_textChanged"
QT_MOC_LITERAL(10, 194, 25), // "on_txtAVRLock_textChanged"
QT_MOC_LITERAL(11, 220, 25), // "on_btnAVRFuseRead_clicked"
QT_MOC_LITERAL(12, 246, 25), // "on_btnAVRLockRead_clicked"
QT_MOC_LITERAL(13, 272, 26), // "on_btnAVRFuseWrite_clicked"
QT_MOC_LITERAL(14, 299, 10), // "onFuseRead"
QT_MOC_LITERAL(15, 310, 26) // "on_btnAVRLockWrite_clicked"

    },
    "AvrFuseSettings\0onAVRCheckBoxChanged\0"
    "\0onAvrNavCheckBoxChanged\0"
    "onAvrNavComboBoxChanged\0index\0"
    "avrSettingsTextChanged\0on_txtAVRLow_textChanged\0"
    "on_txtAVRHigh_textChanged\0"
    "on_txtAVRExtend_textChanged\0"
    "on_txtAVRLock_textChanged\0"
    "on_btnAVRFuseRead_clicked\0"
    "on_btnAVRLockRead_clicked\0"
    "on_btnAVRFuseWrite_clicked\0onFuseRead\0"
    "on_btnAVRLockWrite_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AvrFuseSettings[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x0a /* Public */,
       3,    0,   80,    2, 0x0a /* Public */,
       4,    1,   81,    2, 0x0a /* Public */,
       6,    0,   84,    2, 0x08 /* Private */,
       7,    0,   85,    2, 0x08 /* Private */,
       8,    0,   86,    2, 0x08 /* Private */,
       9,    0,   87,    2, 0x08 /* Private */,
      10,    0,   88,    2, 0x08 /* Private */,
      11,    0,   89,    2, 0x08 /* Private */,
      12,    0,   90,    2, 0x08 /* Private */,
      13,    0,   91,    2, 0x08 /* Private */,
      14,    0,   92,    2, 0x08 /* Private */,
      15,    0,   93,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AvrFuseSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AvrFuseSettings *_t = static_cast<AvrFuseSettings *>(_o);
        switch (_id) {
        case 0: _t->onAVRCheckBoxChanged(); break;
        case 1: _t->onAvrNavCheckBoxChanged(); break;
        case 2: _t->onAvrNavComboBoxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->avrSettingsTextChanged(); break;
        case 4: _t->on_txtAVRLow_textChanged(); break;
        case 5: _t->on_txtAVRHigh_textChanged(); break;
        case 6: _t->on_txtAVRExtend_textChanged(); break;
        case 7: _t->on_txtAVRLock_textChanged(); break;
        case 8: _t->on_btnAVRFuseRead_clicked(); break;
        case 9: _t->on_btnAVRLockRead_clicked(); break;
        case 10: _t->on_btnAVRFuseWrite_clicked(); break;
        case 11: _t->onFuseRead(); break;
        case 12: _t->on_btnAVRLockWrite_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject AvrFuseSettings::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AvrFuseSettings.data,
      qt_meta_data_AvrFuseSettings,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AvrFuseSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AvrFuseSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AvrFuseSettings.stringdata))
        return static_cast<void*>(const_cast< AvrFuseSettings*>(this));
    return QWidget::qt_metacast(_clname);
}

int AvrFuseSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
