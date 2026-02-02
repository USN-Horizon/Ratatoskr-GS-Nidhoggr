QT += quick widgets graphs qml core gui
CONFIG += c++17
CONFIG += console

# CONFIG += qmltypes
# QML_IMPORT_NAME = FmControllers
# QML_IMPORT_MAJOR_VERSION = 1

INCLUDEPATH = src

SOURCES += \
        src/main.cpp \
        src/fermiditycollection.cpp \
        src/models/flightstatemodel.cpp \
        src/models/tabbarvertical.cpp \
        src/utils/flightstateutils.cpp \
        src/models/timeseriesmodel.cpp \
        src/models/timewindowproxymodel.cpp \
        src/testcontroller.cpp \
        src/timer.cpp \
        src/utils/flightlogfactory.cpp \
        src/utils/humidityutils.cpp \
        src/models/humiditymodel.cpp \
        src/utils/timeseriesfactory.cpp

HEADERS +=  \
    src/fermiditycollection.h \
    src/models/flightstatemodel.h \
    src/models/tabbarvertical.h \
    src/utils/flightstateutils.h \
    src/models/timeseriesmodel.h \
    src/models/timewindowproxymodel.h \
    src/testcontroller.h \
    src/timer.h \
    src/utils/flightlogfactory.h \
    src/utils/humidityutils.h \
    src/models/humiditymodel.h \
    src/utils/timeseriesfactory.h

		
RESOURCES += qml/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

