TEMPLATE = app

QT += webenginewidgets

SOURCES += main.cpp \
    webview.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/webenginewidgets/minimal
INSTALLS += target
HEADERS += \
    webview.h

