#-------------------------------------------------
#
# Project created by QtCreator 2012-04-30T15:06:26
#
#-------------------------------------------------

QT       -= gui

TEMPLATE = lib
DEFINES += WASABIENGINE_LIBRARY

SOURCES += \
#    src/WASABIRunner.cc \
    src/WASABIEngine.cc \
    src/SecondaryEmotion.cc \
    src/PrimaryEmotion.cc \
    src/EmotionDynamics.cc \
    src/EmotionConverter.cc \
    src/EmotionContainer.cc \
    src/cogaEmotionalAttendee.cc \
    src/cogaAttendee.cc \
    src/AffectiveState.cc

HEADERS += \
    includes/WASABIEngine.h \
    includes/SecondaryEmotion.h \
    includes/PrimaryEmotion.h \
    includes/EmotionDynamics.h \
    includes/EmotionConverter.h \
    includes/EmotionContainer.h \
    includes/cogaEmotionalAttendee.h \
    includes/cogaAttendee.h \
    includes/AffectiveState.h \
    includes/WASABIEngine_global.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES +=
