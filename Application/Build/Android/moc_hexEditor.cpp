/****************************************************************************
** Meta object code from reading C++ file 'hexEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../widgets/hexEditor/hexEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hexEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_HexEditor_t {
    QByteArrayData data[14];
    char stringdata[329];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HexEditor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HexEditor_t qt_meta_stringdata_HexEditor = {
    {
QT_MOC_LITERAL(0, 0, 9), // "HexEditor"
QT_MOC_LITERAL(1, 10, 28), // "on_asciiTextEdit_textChanged"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 33), // "on_asciiTextEdit_selectionCha..."
QT_MOC_LITERAL(4, 74, 38), // "on_asciiTextEdit_cursorPositi..."
QT_MOC_LITERAL(5, 113, 36), // "on_hexTextEdit_cursorPosition..."
QT_MOC_LITERAL(6, 150, 31), // "on_hexTextEdit_selectionChanged"
QT_MOC_LITERAL(7, 182, 26), // "on_hexTextEdit_textChanged"
QT_MOC_LITERAL(8, 209, 28), // "on_hexTextEdit_copyAvailable"
QT_MOC_LITERAL(9, 238, 1), // "b"
QT_MOC_LITERAL(10, 240, 32), // "on_hexTextEdit_blockCountChanged"
QT_MOC_LITERAL(11, 273, 13), // "newBlockCount"
QT_MOC_LITERAL(12, 287, 36), // "on_asciiTextEdit_modification..."
QT_MOC_LITERAL(13, 324, 4) // "arg1"

    },
    "HexEditor\0on_asciiTextEdit_textChanged\0"
    "\0on_asciiTextEdit_selectionChanged\0"
    "on_asciiTextEdit_cursorPositionChanged\0"
    "on_hexTextEdit_cursorPositionChanged\0"
    "on_hexTextEdit_selectionChanged\0"
    "on_hexTextEdit_textChanged\0"
    "on_hexTextEdit_copyAvailable\0b\0"
    "on_hexTextEdit_blockCountChanged\0"
    "newBlockCount\0on_asciiTextEdit_modificationChanged\0"
    "arg1"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HexEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    1,   65,    2, 0x08 /* Private */,
      10,    1,   68,    2, 0x08 /* Private */,
      12,    1,   71,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Bool,   13,

       0        // eod
};

void HexEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HexEditor *_t = static_cast<HexEditor *>(_o);
        switch (_id) {
        case 0: _t->on_asciiTextEdit_textChanged(); break;
        case 1: _t->on_asciiTextEdit_selectionChanged(); break;
        case 2: _t->on_asciiTextEdit_cursorPositionChanged(); break;
        case 3: _t->on_hexTextEdit_cursorPositionChanged(); break;
        case 4: _t->on_hexTextEdit_selectionChanged(); break;
        case 5: _t->on_hexTextEdit_textChanged(); break;
        case 6: _t->on_hexTextEdit_copyAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_hexTextEdit_blockCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_asciiTextEdit_modificationChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject HexEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_HexEditor.data,
      qt_meta_data_HexEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HexEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HexEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HexEditor.stringdata))
        return static_cast<void*>(const_cast< HexEditor*>(this));
    return QWidget::qt_metacast(_clname);
}

int HexEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
