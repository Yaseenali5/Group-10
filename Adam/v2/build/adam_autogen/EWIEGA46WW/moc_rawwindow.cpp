/****************************************************************************
** Meta object code from reading C++ file 'rawwindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../rawwindow.hpp"
#include <QtGui/qtextcursor.h>
#include <QtGui/qscreen.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rawwindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSRawWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSRawWindowENDCLASS = QtMocHelpers::stringData(
    "RawWindow",
    "setDataLoc",
    "",
    "loadCSV",
    "filterLoc",
    "location"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSRawWindowENDCLASS_t {
    uint offsetsAndSizes[12];
    char stringdata0[10];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[10];
    char stringdata5[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSRawWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSRawWindowENDCLASS_t qt_meta_stringdata_CLASSRawWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 9),  // "RawWindow"
        QT_MOC_LITERAL(10, 10),  // "setDataLoc"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 7),  // "loadCSV"
        QT_MOC_LITERAL(30, 9),  // "filterLoc"
        QT_MOC_LITERAL(40, 8)   // "location"
    },
    "RawWindow",
    "setDataLoc",
    "",
    "loadCSV",
    "filterLoc",
    "location"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSRawWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x08,    1 /* Private */,
       3,    0,   33,    2, 0x08,    2 /* Private */,
       4,    1,   34,    2, 0x08,    3 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

Q_CONSTINIT const QMetaObject RawWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSRawWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSRawWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSRawWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<RawWindow, std::true_type>,
        // method 'setDataLoc'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadCSV'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'filterLoc'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void RawWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RawWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setDataLoc(); break;
        case 1: _t->loadCSV(); break;
        case 2: _t->filterLoc((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *RawWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RawWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSRawWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int RawWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
