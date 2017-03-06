CONFIG += c++11 c++14
CONFIG -= qt
DEFINES += PROTO_LIBRARY
TEMPLATE = lib
CONFIG += staticlib

DESTDIR=../lib
TARGET = protocol

Debug:TARGET=protocold

Release:OBJECTS_DIR = ../build/lib/release/.obj
Release:MOC_DIR = ../build/lib/release/.moc
Release:RCC_DIR = ../build/lib/release/.rcc
Release:UI_DIR = ../build/lib/release/.ui

Debug:OBJECTS_DIR = ../build/lib/debug/.obj
Debug:MOC_DIR = ../build/lib/debug/.moc
Debug:RCC_DIR = ../build/lib/debug/.rcc
Debug:UI_DIR = ../build/lib/debug/.ui

HEADERS += \
    parser.h \
    parser.h \
    export.h \
    buffer_controller.h \
    buffer.h \
    buffer_stack.h

SOURCES += \
    parser.cpp \
    parser.cpp \
    buffer_controller.cpp \
    buffer.cpp \
    buffer_stack.cpp

header_files.files = $$HEADERS
header_files.path = ../include/server
INSTALLS += header_files

system(cd .. && perl post-build.pl)

Release:LIBS += -lserver
Debug:LIBS += -lserverd
