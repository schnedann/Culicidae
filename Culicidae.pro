TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    ../mosquitto/lib/cpp/mosquittopp.h

INCLUDEPATH += $$PWD/../mosquitto/lib
INCLUDEPATH += $$PWD/../mosquitto/lib/cpp
DEPENDPATH += $$PWD/../mosquitto/lib
DEPENDPATH += $$PWD/../mosquitto/lib/cpp
unix:!macx: LIBS += -L$$PWD/../mosquitto/lib/cpp/ -lmosquittopp
unix:!macx: PRE_TARGETDEPS += $$PWD/../mosquitto/lib/cpp/libmosquittopp.a
