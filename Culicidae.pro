TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    mqtt.cpp

HEADERS += \
    ../mosquitto/lib/cpp/mosquittopp.h \
    mqtt.h

INCLUDEPATH += $$PWD/../mosquitto/lib
INCLUDEPATH += $$PWD/../mosquitto/lib/cpp
DEPENDPATH += $$PWD/../mosquitto/lib
DEPENDPATH += $$PWD/../mosquitto/lib/cpp
unix:!macx: LIBS += -L$$PWD/../mosquitto/lib/cpp/ -lmosquittopp
unix:!macx: PRE_TARGETDEPS += $$PWD/../mosquitto/lib/cpp/libmosquittopp.a
unix:!macx: LIBS += -L$$PWD/../mosquitto/lib/ -lmosquitto
unix:!macx: PRE_TARGETDEPS += $$PWD/../mosquitto/lib/libmosquitto.a

unix:!macx: LIBS += -lssl
unix:!macx: LIBS += -lcrypto
