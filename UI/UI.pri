include($$PWD/Dialogs/Dialogs.pri)
include($$PWD/Inputs/Inputs.pri)
include($$PWD/Filter/Filter.pri)

INCLUDEPATH += $$PWD

FORMS += \
    $$PWD/HBUserInterface.ui

HEADERS += \
    $$PWD/HBUserInterface.h \
    $$PWD/HBDataManager.h \
    $$PWD/HBDataModel.h \
    $$PWD/HBGlobalFunctions.h

SOURCES += \
    $$PWD/HBUserInterface.cpp \
    $$PWD/HBDataManager.cpp \
    $$PWD/HBDataModel.cpp \
    $$PWD/HBGlobalFunctions.cpp
