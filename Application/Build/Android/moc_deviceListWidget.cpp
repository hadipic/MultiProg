/****************************************************************************
** Meta object code from reading C++ file 'deviceListWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../widgets/deviceListWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'deviceListWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DeviceListWidget_t {
    QByteArrayData data[14];
    char stringdata[230];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DeviceListWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DeviceListWidget_t qt_meta_stringdata_DeviceListWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "DeviceListWidget"
QT_MOC_LITERAL(1, 17, 16), // "onDeviceSelected"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 13), // "DeviceFamily*"
QT_MOC_LITERAL(4, 49, 6), // "family"
QT_MOC_LITERAL(5, 56, 11), // "DeviceType*"
QT_MOC_LITERAL(6, 68, 4), // "type"
QT_MOC_LITERAL(7, 73, 11), // "DeviceInfo*"
QT_MOC_LITERAL(8, 85, 10), // "deviceInfo"
QT_MOC_LITERAL(9, 96, 37), // "on_cboDevicesInfo_currentInde..."
QT_MOC_LITERAL(10, 134, 5), // "index"
QT_MOC_LITERAL(11, 140, 37), // "on_cboDeviceTypes_currentInde..."
QT_MOC_LITERAL(12, 178, 38), // "on_cboDeviceFamily_currentInd..."
QT_MOC_LITERAL(13, 217, 12) // "detectDevice"

    },
    "DeviceListWidget\0onDeviceSelected\0\0"
    "DeviceFamily*\0family\0DeviceType*\0type\0"
    "DeviceInfo*\0deviceInfo\0"
    "on_cboDevicesInfo_currentIndexChanged\0"
    "index\0on_cboDeviceTypes_currentIndexChanged\0"
    "on_cboDeviceFamily_currentIndexChanged\0"
    "detectDevice"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DeviceListWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   46,    2, 0x08 /* Private */,
      11,    1,   49,    2, 0x08 /* Private */,
      12,    1,   52,    2, 0x08 /* Private */,
      13,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, 0x80000000 | 7,    4,    6,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,

       0        // eod
};

void DeviceListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DeviceListWidget *_t = static_cast<DeviceListWidget *>(_o);
        switch (_id) {
        case 0: _t->onDeviceSelected((*reinterpret_cast< DeviceFamily*(*)>(_a[1])),(*reinterpret_cast< DeviceType*(*)>(_a[2])),(*reinterpret_cast< DeviceInfo*(*)>(_a[3]))); break;
        case 1: _t->on_cboDevicesInfo_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_cboDeviceTypes_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_cboDeviceFamily_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->detectDevice(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DeviceListWidget::*_t)(DeviceFamily * , DeviceType * , DeviceInfo * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DeviceListWidget::onDeviceSelected)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject DeviceListWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DeviceListWidget.data,
      qt_meta_data_DeviceListWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DeviceListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DeviceListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DeviceListWidget.stringdata))
        return static_cast<void*>(const_cast< DeviceListWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int DeviceListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DeviceListWidget::onDeviceSelected(DeviceFamily * _t1, DeviceType * _t2, DeviceInfo * _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
