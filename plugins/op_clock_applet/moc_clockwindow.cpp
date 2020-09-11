/****************************************************************************
** Meta object code from reading C++ file 'clockwindow.h'
**
** Created: Fri Sep 20 23:03:18 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "clockwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clockwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ClockWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   30,   32,   32, 0x0a,
      33,   32,   32,   32, 0x08,
      57,   32,   32,   32, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ClockWindow[] = {
    "ClockWindow\0click_on(QString)\0s\0\0"
    "on_pushButton_clicked()\0notify_update()\0"
};

const QMetaObject ClockWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ClockWindow,
      qt_meta_data_ClockWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ClockWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ClockWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ClockWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ClockWindow))
        return static_cast<void*>(const_cast< ClockWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ClockWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: click_on((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: on_pushButton_clicked(); break;
        case 2: notify_update(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
