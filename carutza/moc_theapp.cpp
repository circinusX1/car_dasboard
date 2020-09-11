/****************************************************************************
** Meta object code from reading C++ file 'theapp.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "theapp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'theapp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TheApp[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      23,    7,    7,    7, 0x0a,
      44,    7,    7,    7, 0x0a,
      63,    7,    7,    7, 0x0a,
      84,    7,    7,    7, 0x0a,
      99,    7,    7,    7, 0x0a,
     120,    7,    7,    7, 0x0a,
     144,  140,    7,    7, 0x0a,
     173,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TheApp[] = {
    "TheApp\0\0slot_alt_tab()\0slot_focus_topmost()\0"
    "slot_namage_apps()\0slot_unmanage_apps()\0"
    "slot_refresh()\0slot_normal_cursor()\0"
    "slot_bring_to_top()\0sym\0"
    "slot_redirect_to_lancer(int)\0"
    "slot_run_app()\0"
};

void TheApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TheApp *_t = static_cast<TheApp *>(_o);
        switch (_id) {
        case 0: _t->slot_alt_tab(); break;
        case 1: _t->slot_focus_topmost(); break;
        case 2: _t->slot_namage_apps(); break;
        case 3: _t->slot_unmanage_apps(); break;
        case 4: _t->slot_refresh(); break;
        case 5: _t->slot_normal_cursor(); break;
        case 6: _t->slot_bring_to_top(); break;
        case 7: _t->slot_redirect_to_lancer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->slot_run_app(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TheApp::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TheApp::staticMetaObject = {
    { &QApplication::staticMetaObject, qt_meta_stringdata_TheApp,
      qt_meta_data_TheApp, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TheApp::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TheApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TheApp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TheApp))
        return static_cast<void*>(const_cast< TheApp*>(this));
    return QApplication::qt_metacast(_clname);
}

int TheApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QApplication::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
