/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[16];
    char stringdata0[321];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 26), // "on_openImageButton_clicked"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 25), // "on_binarizeButton_clicked"
QT_MOC_LITERAL(4, 65, 26), // "on_saveImageButton_clicked"
QT_MOC_LITERAL(5, 92, 22), // "on_cannyButton_clicked"
QT_MOC_LITERAL(6, 115, 29), // "on_cannySlider_sliderReleased"
QT_MOC_LITERAL(7, 145, 25), // "on_toolBox_currentChanged"
QT_MOC_LITERAL(8, 171, 5), // "index"
QT_MOC_LITERAL(9, 177, 22), // "on_fuzzyButton_clicked"
QT_MOC_LITERAL(10, 200, 29), // "on_fuzzySlider_sliderReleased"
QT_MOC_LITERAL(11, 230, 20), // "on_pcaButton_clicked"
QT_MOC_LITERAL(12, 251, 27), // "on_distortionButton_clicked"
QT_MOC_LITERAL(13, 279, 17), // "remove_distortion"
QT_MOC_LITERAL(14, 297, 16), // "QVector<QPoint>&"
QT_MOC_LITERAL(15, 314, 6) // "points"

    },
    "MainWindow\0on_openImageButton_clicked\0"
    "\0on_binarizeButton_clicked\0"
    "on_saveImageButton_clicked\0"
    "on_cannyButton_clicked\0"
    "on_cannySlider_sliderReleased\0"
    "on_toolBox_currentChanged\0index\0"
    "on_fuzzyButton_clicked\0"
    "on_fuzzySlider_sliderReleased\0"
    "on_pcaButton_clicked\0on_distortionButton_clicked\0"
    "remove_distortion\0QVector<QPoint>&\0"
    "points"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    0,   73,    2, 0x08 /* Private */,
       7,    1,   74,    2, 0x08 /* Private */,
       9,    0,   77,    2, 0x08 /* Private */,
      10,    0,   78,    2, 0x08 /* Private */,
      11,    0,   79,    2, 0x08 /* Private */,
      12,    0,   80,    2, 0x08 /* Private */,
      13,    1,   81,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_openImageButton_clicked(); break;
        case 1: _t->on_binarizeButton_clicked(); break;
        case 2: _t->on_saveImageButton_clicked(); break;
        case 3: _t->on_cannyButton_clicked(); break;
        case 4: _t->on_cannySlider_sliderReleased(); break;
        case 5: _t->on_toolBox_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_fuzzyButton_clicked(); break;
        case 7: _t->on_fuzzySlider_sliderReleased(); break;
        case 8: _t->on_pcaButton_clicked(); break;
        case 9: _t->on_distortionButton_clicked(); break;
        case 10: _t->remove_distortion((*reinterpret_cast< QVector<QPoint>(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
