/****************************************************************************
** Meta object code from reading C++ file 'qtserial.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qtserial.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtserial.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QtSerial_t {
    QByteArrayData data[7];
    char stringdata0[77];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtSerial_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtSerial_t qt_meta_stringdata_QtSerial = {
    {
QT_MOC_LITERAL(0, 0, 8), // "QtSerial"
QT_MOC_LITERAL(1, 9, 15), // "handleReadyRead"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 11), // "handleError"
QT_MOC_LITERAL(4, 38, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(5, 67, 5), // "error"
QT_MOC_LITERAL(6, 73, 3) // "Run"

    },
    "QtSerial\0handleReadyRead\0\0handleError\0"
    "QSerialPort::SerialPortError\0error\0"
    "Run"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtSerial[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    1,   30,    2, 0x08 /* Private */,
       6,    0,   33,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,

       0        // eod
};

void QtSerial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtSerial *_t = static_cast<QtSerial *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleReadyRead(); break;
        case 1: _t->handleError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        case 2: _t->Run(); break;
        default: ;
        }
    }
}

const QMetaObject QtSerial::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtSerial.data,
      qt_meta_data_QtSerial,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QtSerial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtSerial::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QtSerial.stringdata0))
        return static_cast<void*>(const_cast< QtSerial*>(this));
    return QObject::qt_metacast(_clname);
}

int QtSerial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
