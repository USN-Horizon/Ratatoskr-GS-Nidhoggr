/****************************************************************************
** Meta object code from reading C++ file 'fermiditycollection.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/fermiditycollection.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fermiditycollection.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSFermidityCollectionENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSFermidityCollectionENDCLASS = QtMocHelpers::stringData(
    "FermidityCollection",
    "QML.Element",
    "auto",
    "testController",
    "TestController*",
    "humidityModel",
    "HumidityModel*"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSFermidityCollectionENDCLASS_t {
    uint offsetsAndSizes[14];
    char stringdata0[20];
    char stringdata1[12];
    char stringdata2[5];
    char stringdata3[15];
    char stringdata4[16];
    char stringdata5[14];
    char stringdata6[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSFermidityCollectionENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSFermidityCollectionENDCLASS_t qt_meta_stringdata_CLASSFermidityCollectionENDCLASS = {
    {
        QT_MOC_LITERAL(0, 19),  // "FermidityCollection"
        QT_MOC_LITERAL(20, 11),  // "QML.Element"
        QT_MOC_LITERAL(32, 4),  // "auto"
        QT_MOC_LITERAL(37, 14),  // "testController"
        QT_MOC_LITERAL(52, 15),  // "TestController*"
        QT_MOC_LITERAL(68, 13),  // "humidityModel"
        QT_MOC_LITERAL(82, 14)   // "HumidityModel*"
    },
    "FermidityCollection",
    "QML.Element",
    "auto",
    "testController",
    "TestController*",
    "humidityModel",
    "HumidityModel*"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSFermidityCollectionENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       1,   14, // classinfo
       0,    0, // methods
       2,   16, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
       1,    2,

 // properties: name, type, flags
       3, 0x80000000 | 4, 0x00015409, uint(-1), 0,
       5, 0x80000000 | 6, 0x00015409, uint(-1), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject FermidityCollection::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSFermidityCollectionENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSFermidityCollectionENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_metaTypeArray<
        // property 'testController'
        TestController*,
        // property 'humidityModel'
        HumidityModel*,
        // Q_OBJECT / Q_GADGET
        FermidityCollection
    >,
    nullptr
} };

void FermidityCollection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< HumidityModel* >(); break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TestController* >(); break;
        }
    }  else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<FermidityCollection *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< TestController**>(_v) = _t->m_testController; break;
        case 1: *reinterpret_cast< HumidityModel**>(_v) = _t->m_humidityModel; break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
    (void)_o;
}

const QMetaObject *FermidityCollection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FermidityCollection::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSFermidityCollectionENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FermidityCollection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
