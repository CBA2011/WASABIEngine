#-------------------------------------------------
#
# Project created by QtCreator 2011-09-20T09:35:11
#
#-------------------------------------------------

QT       += core

TARGET = WASABIEngine
TEMPLATE = lib

DEFINES += WASABIENGINE_LIBRARY

SOURCES += \
    SecondaryEmotion.cc \
    PrimaryEmotion.cc \
    EmotionDynamics.cc \
    EmotionConverter.cc \
    EmotionContainer.cc \
    cogaEmotionalAttendee.cc \
    cogaAttendee.cc \
    AffectiveState.cc \
    WASABIRunner.cc \
    WASABIEngine.cc

HEADERS +=\
        WASABIEngine_global.h \
    SecondaryEmotion.h \
    PrimaryEmotion.h \
    EmotionDynamics.h \
    EmotionConverter.h \
    EmotionContainer.h \
    cogaEmotionalAttendee.h \
    cogaAttendee.h \
    AffectiveState.h \
    WASABIEngine.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE0335EA8
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = WASABIEngine.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    README.txt










