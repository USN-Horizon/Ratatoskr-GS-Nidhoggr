/****************************************************************************
** Meta object code from reading C++ file 'flightstatemodel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/models/flightstatemodel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'flightstatemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
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
struct qt_meta_tag_ZN16FlightStateModelE_t {};
} // unnamed namespace

template <> constexpr inline auto FlightStateModel::qt_create_metaobjectdata<qt_meta_tag_ZN16FlightStateModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "FlightStateModel",
        "currentStateChanged",
        "",
        "stateTransition",
        "FlightState",
        "fromState",
        "toState",
        "FlightEvent",
        "event",
        "setCurrentStateByInt",
        "stateInt",
        "appendStateData",
        "time",
        "state",
        "currentState",
        "currentStateName",
        "currentStateColor"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'currentStateChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'stateTransition'
        QtMocHelpers::SignalData<void(FlightState, FlightState, FlightEvent)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 }, { 0x80000000 | 4, 6 }, { 0x80000000 | 7, 8 },
        }}),
        // Method 'setCurrentStateByInt'
        QtMocHelpers::MethodData<void(int)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 10 },
        }}),
        // Method 'appendStateData'
        QtMocHelpers::MethodData<void(qreal, int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QReal, 12 }, { QMetaType::Int, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentState'
        QtMocHelpers::PropertyData<FlightState>(14, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'currentStateName'
        QtMocHelpers::PropertyData<QString>(15, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'currentStateColor'
        QtMocHelpers::PropertyData<QColor>(16, QMetaType::QColor, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<FlightStateModel, qt_meta_tag_ZN16FlightStateModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject FlightStateModel::staticMetaObject = { {
    QMetaObject::SuperData::link<TimeSeriesModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16FlightStateModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16FlightStateModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16FlightStateModelE_t>.metaTypes,
    nullptr
} };

void FlightStateModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FlightStateModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->currentStateChanged(); break;
        case 1: _t->stateTransition((*reinterpret_cast< std::add_pointer_t<FlightState>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<FlightState>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<FlightEvent>>(_a[3]))); break;
        case 2: _t->setCurrentStateByInt((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->appendStateData((*reinterpret_cast< std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (FlightStateModel::*)()>(_a, &FlightStateModel::currentStateChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (FlightStateModel::*)(FlightState , FlightState , FlightEvent )>(_a, &FlightStateModel::stateTransition, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<FlightState*>(_v) = _t->currentState(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->currentStateName(); break;
        case 2: *reinterpret_cast<QColor*>(_v) = _t->currentStateColor(); break;
        default: break;
        }
    }
}

const QMetaObject *FlightStateModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FlightStateModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16FlightStateModelE_t>.strings))
        return static_cast<void*>(this);
    return TimeSeriesModel::qt_metacast(_clname);
}

int FlightStateModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TimeSeriesModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FlightStateModel::currentStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FlightStateModel::stateTransition(FlightState _t1, FlightState _t2, FlightEvent _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3);
}
QT_WARNING_POP
