QT += core

CONFIG += c++17
CONFIG -= app_bundle

TARGET = cutegenericmodel

TEMPLATE = lib
DEFINES += ABSTRACTOBJECTMODEL_LIBRARY

SOURCES += \
    abstractobjectmodel.cpp

HEADERS += \
    abstractobjectmodel_global.h \
    abstractobjectmodel.h

