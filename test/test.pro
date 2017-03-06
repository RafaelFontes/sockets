CONFIG += console c++11 c++14
CONFIG -= app_bundle
CONFIG -= qt debug_and_release

INCLUDEPATH += ../include

LIBS += -L../lib -llibsocket -lprotocol
DESTDIR=../bin

Release:LIBS+=-llibsocket
Debug:LIBS+=-lserverd

TARGET=server-test
Debug:TARGET=server-test-d

Release:OBJECTS_DIR = ../build/test/release/.obj
Release:MOC_DIR = ../build/test/release/.moc
Release:RCC_DIR = ../build/test/release/.rcc
Release:UI_DIR = ../build/test/release/.ui

Debug:OBJECTS_DIR = ../build/test/debug/.obj
Debug:MOC_DIR = ../build/test/debug/.moc
Debug:RCC_DIR = ../build/test/debug/.rcc
Debug:UI_DIR = ../build/test/debug/.ui

SOURCES += \
    main.cpp

HEADERS += \
    sock-test.hpp


system(cd .. && perl post-build.pl)
