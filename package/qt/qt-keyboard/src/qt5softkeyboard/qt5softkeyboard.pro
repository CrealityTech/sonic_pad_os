#-------------------------------------------------
#
# Project created by QtCreator 2013-04-04T23:11:38
#
#-------------------------------------------------

QT       += gui-private widgets

TARGET = Qt5SoftKeyboard
TEMPLATE = lib
CONFIG += plugin

DESTDIR  = $$PWD/../bin/plugins/platforminputcontexts
DEFINES += VIRTUALKEYBOARD_LIBRARY

CONFIG(debug, debug|release){
TARGET = $$join(TARGET,,,d)
LIBS += -L$$PWD/../lib -lqtpinyind
}
else{
LIBS += -L$$PWD/../lib -lqtpinyin
}


INCLUDEPATH += $$PWD/../pinyin/include

SOURCES += \
    inputcontext.cpp \
    platforminputcontextplugin.cpp \
    NumKeyboard.cpp \
    NormalKeyboard.cpp \
    pinyindecoderservice.cpp \
    QAbstractInputMethod.cpp \
    QPinyinInputMethod.cpp \
    SymbolKeyboard.cpp \
    KeyboardContainer.cpp \
    QKeyboardLayout.cpp \
    CandidatesListWidget.cpp


HEADERS +=\
    inputcontext.h \
    platforminputcontextplugin.h \
    NumKeyboard.h \
    NormalKeyboard.h \
    pinyindecoderservice_p.h \
    QAbstractInputMethod.h \
    QPinyinInputMethod.h \
    SymbolKeyboard.h \
    KeyboardContainer.h \
    QKeyboardLayout.h \
    CandidatesListWidget.h

RESOURCES += \
    res.qrc

FORMS += \
    NumKeyboard.ui \
    NormalKeyboard.ui \
    SymbolKeyboard.ui

win32-msvc{
QMAKE_CXXFLAGS += -utf-8
}




