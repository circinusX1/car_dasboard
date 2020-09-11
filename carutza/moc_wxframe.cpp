/****************************************************************************
** Meta object code from reading C++ file 'wxframe.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wxframe.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wxframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrmWinBorder[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      43,   13,   13,   13, 0x05,
      70,   13,   13,   13, 0x05,
      85,   13,   13,   13, 0x05,
     100,   13,   13,   13, 0x05,
     115,   13,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_FrmWinBorder[] = {
    "FrmWinBorder\0\0sig_mouse_move(QMouseEvent*)\0"
    "sig_mlbutton(QMouseEvent*)\0sig_mlbutton()\0"
    "sig_mrbutton()\0sig_mmbutton()\0"
    "sig_mrelease()\0"
};

void FrmWinBorder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FrmWinBorder *_t = static_cast<FrmWinBorder *>(_o);
        switch (_id) {
        case 0: _t->sig_mouse_move((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 1: _t->sig_mlbutton((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: _t->sig_mlbutton(); break;
        case 3: _t->sig_mrbutton(); break;
        case 4: _t->sig_mmbutton(); break;
        case 5: _t->sig_mrelease(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FrmWinBorder::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FrmWinBorder::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_FrmWinBorder,
      qt_meta_data_FrmWinBorder, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FrmWinBorder::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FrmWinBorder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FrmWinBorder::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrmWinBorder))
        return static_cast<void*>(const_cast< FrmWinBorder*>(this));
    return QLabel::qt_metacast(_clname);
}

int FrmWinBorder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void FrmWinBorder::sig_mouse_move(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FrmWinBorder::sig_mlbutton(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FrmWinBorder::sig_mlbutton()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void FrmWinBorder::sig_mrbutton()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void FrmWinBorder::sig_mmbutton()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void FrmWinBorder::sig_mrelease()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
static const uint qt_meta_data_FrmSysButs[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      31,   11,   11,   11, 0x05,
      50,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      71,   69,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrmSysButs[] = {
    "FrmSysButs\0\0sig_lbutton_down()\0"
    "sig_rbutton_down()\0sig_mbutton_down()\0"
    "s\0set_state(int)\0"
};

void FrmSysButs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FrmSysButs *_t = static_cast<FrmSysButs *>(_o);
        switch (_id) {
        case 0: _t->sig_lbutton_down(); break;
        case 1: _t->sig_rbutton_down(); break;
        case 2: _t->sig_mbutton_down(); break;
        case 3: _t->set_state((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FrmSysButs::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FrmSysButs::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_FrmSysButs,
      qt_meta_data_FrmSysButs, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FrmSysButs::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FrmSysButs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FrmSysButs::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrmSysButs))
        return static_cast<void*>(const_cast< FrmSysButs*>(this));
    return QLabel::qt_metacast(_clname);
}

int FrmSysButs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void FrmSysButs::sig_lbutton_down()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void FrmSysButs::sig_rbutton_down()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void FrmSysButs::sig_mbutton_down()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
static const uint qt_meta_data_XFrameBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      31,   10,   10,   10, 0x05,
      60,   10,   10,   10, 0x05,
      91,   10,   10,   10, 0x05,
     110,   10,   10,   10, 0x05,
     129,   10,   10,   10, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_XFrameBar[] = {
    "XFrameBar\0\0sig_mouse_release()\0"
    "sig_mouse_move(QMouseEvent*)\0"
    "sig_lbutton_down(QMouseEvent*)\0"
    "sig_rbutton_down()\0sig_mbutton_down()\0"
    "sig_mouse_dblclk()\0"
};

void XFrameBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        XFrameBar *_t = static_cast<XFrameBar *>(_o);
        switch (_id) {
        case 0: _t->sig_mouse_release(); break;
        case 1: _t->sig_mouse_move((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: _t->sig_lbutton_down((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->sig_rbutton_down(); break;
        case 4: _t->sig_mbutton_down(); break;
        case 5: _t->sig_mouse_dblclk(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData XFrameBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject XFrameBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_XFrameBar,
      qt_meta_data_XFrameBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &XFrameBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *XFrameBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *XFrameBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_XFrameBar))
        return static_cast<void*>(const_cast< XFrameBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int XFrameBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void XFrameBar::sig_mouse_release()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void XFrameBar::sig_mouse_move(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void XFrameBar::sig_lbutton_down(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void XFrameBar::sig_rbutton_down()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void XFrameBar::sig_mbutton_down()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void XFrameBar::sig_mouse_dblclk()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
static const uint qt_meta_data_SysButsBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_SysButsBar[] = {
    "SysButsBar\0"
};

void SysButsBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SysButsBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SysButsBar::staticMetaObject = {
    { &XFrameBar::staticMetaObject, qt_meta_stringdata_SysButsBar,
      qt_meta_data_SysButsBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SysButsBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SysButsBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SysButsBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SysButsBar))
        return static_cast<void*>(const_cast< SysButsBar*>(this));
    return XFrameBar::qt_metacast(_clname);
}

int SysButsBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = XFrameBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
