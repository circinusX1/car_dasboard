/****************************************************************************
** Meta object code from reading C++ file 'button.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "button.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'button.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OdButton[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   10,    9,    9, 0x05,
      43,    9,    9,    9, 0x05,
      66,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      84,    9,    9,    9, 0x0a,
     101,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OdButton[] = {
    "OdButton\0\0,,\0sig_moving(OdButton*,int,int)\0"
    "sig_clicked(OdButton*)\0sig_scrolled(int)\0"
    "slot_show_menu()\0slot_paint_again()\0"
};

void OdButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OdButton *_t = static_cast<OdButton *>(_o);
        switch (_id) {
        case 0: _t->sig_moving((*reinterpret_cast< OdButton*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->sig_clicked((*reinterpret_cast< OdButton*(*)>(_a[1]))); break;
        case 2: _t->sig_scrolled((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slot_show_menu(); break;
        case 4: _t->slot_paint_again(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OdButton::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OdButton::staticMetaObject = {
    { &QToolButton::staticMetaObject, qt_meta_stringdata_OdButton,
      qt_meta_data_OdButton, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OdButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OdButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OdButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OdButton))
        return static_cast<void*>(const_cast< OdButton*>(this));
    return QToolButton::qt_metacast(_clname);
}

int OdButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void OdButton::sig_moving(OdButton * _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OdButton::sig_clicked(OdButton * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OdButton::sig_scrolled(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
