/****************************************************************************
** Meta object code from reading C++ file 'HistogramWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "src/HistogramWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HistogramWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HistogramWindow_t {
    QByteArrayData data[9];
    char stringdata0[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HistogramWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HistogramWindow_t qt_meta_stringdata_HistogramWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "HistogramWindow"
QT_MOC_LITERAL(1, 16, 10), // "sigStretch"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 13), // "plotHistogram"
QT_MOC_LITERAL(4, 42, 10), // "plotMinmax"
QT_MOC_LITERAL(5, 53, 13), // "ActionStretch"
QT_MOC_LITERAL(6, 67, 11), // "ActionApply"
QT_MOC_LITERAL(7, 79, 17), // "ActionOpenCumHist"
QT_MOC_LITERAL(8, 97, 18) // "ActionLockChannels"

    },
    "HistogramWindow\0sigStretch\0\0plotHistogram\0"
    "plotMinmax\0ActionStretch\0ActionApply\0"
    "ActionOpenCumHist\0ActionLockChannels"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HistogramWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   50,    2, 0x0a /* Public */,
       3,    0,   53,    2, 0x2a /* Public | MethodCloned */,
       5,    0,   54,    2, 0x0a /* Public */,
       6,    0,   55,    2, 0x0a /* Public */,
       7,    0,   56,    2, 0x0a /* Public */,
       8,    0,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Float,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HistogramWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HistogramWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { float _r = _t->sigStretch();
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->plotHistogram((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->plotHistogram(); break;
        case 3: _t->ActionStretch(); break;
        case 4: _t->ActionApply(); break;
        case 5: _t->ActionOpenCumHist(); break;
        case 6: _t->ActionLockChannels(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = float (HistogramWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HistogramWindow::sigStretch)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HistogramWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_HistogramWindow.data,
    qt_meta_data_HistogramWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HistogramWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HistogramWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HistogramWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HistogramWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
float HistogramWindow::sigStretch()
{
    float _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t0))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
