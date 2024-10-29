QT += quick charts qml core gui
CONFIG += c++17

# CONFIG += qmltypes
# QML_IMPORT_NAME = FmControllers
# QML_IMPORT_MAJOR_VERSION = 1

INCLUDEPATH = src

SOURCES += \
        src/main.cpp \
        src/fermiditycollection.cpp \
        src/testcontroller.cpp \
        src/utils/humidityutils.cpp \
        src/models/humiditymodel.cpp

HEADERS +=  \
    src/fermiditycollection.h \
    src/testcontroller.h \
    src/utils/humidityutils.h \
    src/models/humiditymodel.h

		
RESOURCES += qml/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

