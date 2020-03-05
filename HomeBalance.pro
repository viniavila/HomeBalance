QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HomeBalance
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

include($$PWD/Core/Core.pri)
include($$PWD/UI/UI.pri)

SOURCES += main.cpp
RESOURCES += HBResources.qrc
TRANSLATIONS += \
    $$PWD/Translations/pt_BR.ts

QMAKE_CC = clang++
QMAKE_CXX = clang++
QMAKE_LINK = clang++

unix:!macos {
    target.path += /usr/local/bin
    INSTALLS += target
}

