/****************************************************************************
** Meta object code from reading C++ file 'icd.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Forms/icd.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'icd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Icd_t {
    QByteArrayData data[9];
    char stringdata[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Icd_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Icd_t qt_meta_stringdata_Icd = {
    {
QT_MOC_LITERAL(0, 0, 3), // "Icd"
QT_MOC_LITERAL(1, 4, 12), // "run_icd_slot"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 13), // "run_halt_slot"
QT_MOC_LITERAL(4, 32, 13), // "run_step_slot"
QT_MOC_LITERAL(5, 46, 18), // "run_step_over_slot"
QT_MOC_LITERAL(6, 65, 13), // "run_stop_slot"
QT_MOC_LITERAL(7, 79, 16), // "run_refresh_slot"
QT_MOC_LITERAL(8, 96, 4) // "open"

    },
    "Icd\0run_icd_slot\0\0run_halt_slot\0"
    "run_step_slot\0run_step_over_slot\0"
    "run_stop_slot\0run_refresh_slot\0open"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Icd[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Icd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Icd *_t = static_cast<Icd *>(_o);
        switch (_id) {
        case 0: _t->run_icd_slot(); break;
        case 1: _t->run_halt_slot(); break;
        case 2: _t->run_step_slot(); break;
        case 3: _t->run_step_over_slot(); break;
        case 4: _t->run_stop_slot(); break;
        case 5: _t->run_refresh_slot(); break;
        case 6: _t->open(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Icd::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Icd.data,
      qt_meta_data_Icd,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Icd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Icd::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Icd.stringdata))
        return static_cast<void*>(const_cast< Icd*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Icd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
