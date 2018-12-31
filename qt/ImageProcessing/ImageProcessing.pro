#-------------------------------------------------
#
# Project created by QtCreator 2018-11-22T22:25:55
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = ImageProcessing
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS +=  -lstdc++fs `pkg-config opencv --cflags --libs` -pthread
QMAKE_CXXFLAGS += -O3 -std=c++17

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    image.cpp \
    canny.cpp \
    fuzzy_edge_detection.cpp \
    pca.cpp \
    distortion.cpp \
    displaylabel.cpp \
    kmeans.cpp \
    binarization_fcm.cpp \
    binarization_kmeans.cpp

HEADERS += \
        mainwindow.h \
    image.hpp \
    canny.hpp \
    binarization.hpp \
    fuzzy_edge_detection.hpp \
    pca.hpp \
    distortion.hpp \
    displaylabel.h \
    kmeans.hpp

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=

DISTFILES +=
