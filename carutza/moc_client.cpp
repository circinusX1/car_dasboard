/****************************************************************************
** Meta object code from reading C++ file 'client.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "client.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_XClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      25,    8,    8,    8, 0x0a,
      45,    8,    8,    8, 0x0a,
      61,    8,    8,    8, 0x0a,
      76,    8,    8,    8, 0x0a,
      91,    8,    8,    8, 0x0a,
     104,    8,    8,    8, 0x0a,
     130,    8,    8,    8, 0x0a,
     150,    8,    8,    8, 0x0a,
     183,    8,    8,    8, 0x0a,
     215,    8,    8,    8, 0x0a,
     247,    8,    8,    8, 0x0a,
     278,    8,    8,    8, 0x0a,
     300,    8,    8,    8, 0x0a,
     316,    8,    8,    8, 0x0a,
     328,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_XClient[] = {
    "XClient\0\0slot_minimize()\0slot_minimize_now()\0"
    "slot_maximize()\0slot_map_win()\0"
    "slot_map_now()\0slot_unmap()\0"
    "slot_remove_dock_widget()\0slot_destroy_xcli()\0"
    "slot_caption_ldown(QMouseEvent*)\0"
    "slot_caption_move(QMouseEvent*)\0"
    "slot_bottom_ldown(QMouseEvent*)\0"
    "slot_bottom_move(QMouseEvent*)\0"
    "slot_bottmo_release()\0slot_show_top()\0"
    "slot_grab()\0slot_dblclk_mini(bool)\0"
};

void XClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        XClient *_t = static_cast<XClient *>(_o);
        switch (_id) {
        case 0: _t->slot_minimize(); break;
        case 1: _t->slot_minimize_now(); break;
        case 2: _t->slot_maximize(); break;
        case 3: _t->slot_map_win(); break;
        case 4: _t->slot_map_now(); break;
        case 5: _t->slot_unmap(); break;
        case 6: _t->slot_remove_dock_widget(); break;
        case 7: _t->slot_destroy_xcli(); break;
        case 8: _t->slot_caption_ldown((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 9: _t->slot_caption_move((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 10: _t->slot_bottom_ldown((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 11: _t->slot_bottom_move((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 12: _t->slot_bottmo_release(); break;
        case 13: _t->slot_show_top(); break;
        case 14: _t->slot_grab(); break;
        case 15: _t->slot_dblclk_mini((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData XClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject XClient::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_XClient,
      qt_meta_data_XClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &XClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *XClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *XClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_XClient))
        return static_cast<void*>(const_cast< XClient*>(this));
    return QWidget::qt_metacast(_clname);
}

int XClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
