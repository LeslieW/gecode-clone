/****************************************************************************
** Meta object code from reading C++ file 'vararrayview.hh'
**
** Created: Tue May 6 16:33:09 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gecode/gist/visualisation/vararrayview.hh"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vararrayview.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Gecode__Gist__Visualisation__VarArrayView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      47,   43,   42,   42, 0x05,

 // slots: signature, parameters, type, tag, flags
      76,   71,   42,   42, 0x0a,
     117,   43,   42,   42, 0x0a,
     133,   42,   42,   42, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Gecode__Gist__Visualisation__VarArrayView[] = {
    "Gecode::Gist::Visualisation::VarArrayView\0"
    "\0pit\0pointInTimeChanged(int)\0,pit\0"
    "display(Gecode::Reflection::VarMap&,int)\0"
    "displayOld(int)\0on_muteButton_clicked()\0"
};

const QMetaObject Gecode::Gist::Visualisation::VarArrayView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Gecode__Gist__Visualisation__VarArrayView,
      qt_meta_data_Gecode__Gist__Visualisation__VarArrayView, 0 }
};

const QMetaObject *Gecode::Gist::Visualisation::VarArrayView::metaObject() const
{
    return &staticMetaObject;
}

void *Gecode::Gist::Visualisation::VarArrayView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Gecode__Gist__Visualisation__VarArrayView))
	return static_cast<void*>(const_cast< VarArrayView*>(this));
    return QWidget::qt_metacast(_clname);
}

int Gecode::Gist::Visualisation::VarArrayView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pointInTimeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: display((*reinterpret_cast< Gecode::Reflection::VarMap(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: displayOld((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: on_muteButton_clicked(); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Gecode::Gist::Visualisation::VarArrayView::pointInTimeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}