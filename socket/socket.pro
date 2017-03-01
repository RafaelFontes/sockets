CONFIG += c++11 c++14
CONFIG -= qt
DEFINES += SERVER_LIBRARY
TEMPLATE = lib
CONFIG += staticlib

DESTDIR=../lib
TARGET = server

Debug:TARGET=serverd

Release:OBJECTS_DIR = ../build/lib/release/.obj
Release:MOC_DIR = ../build/lib/release/.moc
Release:RCC_DIR = ../build/lib/release/.rcc
Release:UI_DIR = ../build/lib/release/.ui

Debug:OBJECTS_DIR = ../build/lib/debug/.obj
Debug:MOC_DIR = ../build/lib/debug/.moc
Debug:RCC_DIR = ../build/lib/debug/.rcc
Debug:UI_DIR = ../build/lib/debug/.ui

HEADERS += \
    ssl_server.h \
    export.h \
    sock.h \
    tcp_sock_server.h \
    tls_server_hook.h \
    sock_io.h \
    server_hook.h

SOURCES += \
    ssl_server.cpp \
    sock.cpp \
    tls_server_hook.cpp \
    sock_io.cpp \
    server_hook.cpp

header_files.files = $$HEADERS
header_files.path = ../include/server
INSTALLS += header_files

LIBS += -llibssl -llibcrypto
