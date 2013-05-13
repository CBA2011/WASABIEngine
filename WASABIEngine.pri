#-------------------------------------------------
#
# Project created by QtCreator 2012-04-30T15:06:26
#
#-------------------------------------------------

QT       -= gui

TEMPLATE = lib
DEFINES += WASABIENGINE_LIBRARY

SOURCES += \
#    WASABIRunner.cc \
    WASABIEngine.cc \
    SecondaryEmotion.cc \
    PrimaryEmotion.cc \
    EmotionDynamics.cc \
    EmotionConverter.cc \
    EmotionContainer.cc \
    cogaEmotionalAttendee.cc \
    cogaAttendee.cc \
    AffectiveState.cc

HEADERS += \
    WASABIEngine.h \
    SecondaryEmotion.h \
    PrimaryEmotion.h \
    EmotionDynamics.h \
    EmotionConverter.h \
    EmotionContainer.h \
    cogaEmotionalAttendee.h \
    cogaAttendee.h \
    AffectiveState.h \
    WASABIEngine_global.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES +=
