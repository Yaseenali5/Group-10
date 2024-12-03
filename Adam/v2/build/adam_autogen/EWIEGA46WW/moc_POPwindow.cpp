/****************************************************************************
** Meta object code from reading C++ file 'POPwindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../POPwindow.hpp"
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
#error "The header file 'POPwindow.hpp' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSPOPWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSPOPWindowENDCLASS = QtMocHelpers::stringData(
    "POPWindow",
    "setDataLoc",
    "",
    "loadCSV",
    "filterPOP",
    "POPname",
    "filterUPOP",
    "uPOPname",
    "filterLoc",
    "location",
    "loadGraph",
    "POPHealthRisks",
    "pop",
    "POPSafetyInfo"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSPOPWindowENDCLASS_t {
    uint offsetsAndSizes[28];
    char stringdata0[10];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[10];
    char stringdata5[8];
    char stringdata6[11];
    char stringdata7[9];
    char stringdata8[10];
    char stringdata9[9];
    char stringdata10[10];
    char stringdata11[15];
    char stringdata12[4];
    char stringdata13[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSPOPWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSPOPWindowENDCLASS_t qt_meta_stringdata_CLASSPOPWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 9),  // "POPWindow"
        QT_MOC_LITERAL(10, 10),  // "setDataLoc"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 7),  // "loadCSV"
        QT_MOC_LITERAL(30, 9),  // "filterPOP"
        QT_MOC_LITERAL(40, 7),  // "POPname"
        QT_MOC_LITERAL(48, 10),  // "filterUPOP"
        QT_MOC_LITERAL(59, 8),  // "uPOPname"
        QT_MOC_LITERAL(68, 9),  // "filterLoc"
        QT_MOC_LITERAL(78, 8),  // "location"
        QT_MOC_LITERAL(87, 9),  // "loadGraph"
        QT_MOC_LITERAL(97, 14),  // "POPHealthRisks"
        QT_MOC_LITERAL(112, 3),  // "pop"
        QT_MOC_LITERAL(116, 13)   // "POPSafetyInfo"
    },
    "POPWindow",
    "setDataLoc",
    "",
    "loadCSV",
    "filterPOP",
    "POPname",
    "filterUPOP",
    "uPOPname",
    "filterLoc",
    "location",
    "loadGraph",
    "POPHealthRisks",
    "pop",
    "POPSafetyInfo"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPOPWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x08,    1 /* Private */,
       3,    0,   63,    2, 0x08,    2 /* Private */,
       4,    1,   64,    2, 0x08,    3 /* Private */,
       6,    1,   67,    2, 0x08,    5 /* Private */,
       8,    1,   70,    2, 0x08,    7 /* Private */,
      10,    0,   73,    2, 0x08,    9 /* Private */,
      11,    1,   74,    2, 0x08,   10 /* Private */,
      13,    1,   77,    2, 0x08,   12 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   12,

       0        // eod
};

Q_CONSTINIT const QMetaObject POPWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSPOPWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPOPWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPOPWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<POPWindow, std::true_type>,
        // method 'setDataLoc'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadCSV'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'filterPOP'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'filterUPOP'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'filterLoc'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'loadGraph'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'POPHealthRisks'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'POPSafetyInfo'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void POPWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<POPWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setDataLoc(); break;
        case 1: _t->loadCSV(); break;
        case 2: _t->filterPOP((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->filterUPOP((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->filterLoc((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->loadGraph(); break;
        case 6: _t->POPHealthRisks((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->POPSafetyInfo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *POPWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *POPWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPOPWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int POPWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
