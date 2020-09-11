/****************************************************************************
** Meta object code from reading C++ file 'panel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "panel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'panel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Panel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,    7,    6,    6, 0x0a,
      49,    6,    6,    6, 0x0a,
      72,   69,    6,    6, 0x0a,
      97,   69,    6,    6, 0x0a,
     116,   69,    6,    6, 0x0a,
     140,   69,    6,    6, 0x0a,
     162,    6,    6,    6, 0x0a,
     180,    6,    6,    6, 0x0a,
     204,  197,    6,    6, 0x0a,
     228,    6,    6,    6, 0x0a,
     255,  249,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Panel[] = {
    "Panel\0\0lb,dx,dy\0scrool_lancer(OdButton*,int,int)\0"
    "done_scrolling(int)\0pb\0change_folder(OdButton*)\0"
    "run_app(OdButton*)\0slot_pressed(OdButton*)\0"
    "run_intern(OdButton*)\0slot_align_edge()\0"
    "slot_anim_done()\0folder\0slot_floder_ch(QString)\0"
    "slot_change_folder()\0proom\0"
    "fit_to_parent(QSize)\0"
};

void Panel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Panel *_t = static_cast<Panel *>(_o);
        switch (_id) {
        case 0: _t->scrool_lancer((*reinterpret_cast< OdButton*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->done_scrolling((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->change_folder((*reinterpret_cast< OdButton*(*)>(_a[1]))); break;
        case 3: _t->run_app((*reinterpret_cast< OdButton*(*)>(_a[1]))); break;
        case 4: _t->slot_pressed((*reinterpret_cast< OdButton*(*)>(_a[1]))); break;
        case 5: _t->run_intern((*reinterpret_cast< OdButton*(*)>(_a[1]))); break;
        case 6: _t->slot_align_edge(); break;
        case 7: _t->slot_anim_done(); break;
        case 8: _t->slot_floder_ch((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->slot_change_folder(); break;
        case 10: _t->fit_to_parent((*reinterpret_cast< const QSize(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Panel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Panel::staticMetaObject = {
    { &QWidget2::staticMetaObject, qt_meta_stringdata_Panel,
      qt_meta_data_Panel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Panel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Panel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Panel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Panel))
        return static_cast<void*>(const_cast< Panel*>(this));
    return QWidget2::qt_metacast(_clname);
}

int Panel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget2::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
