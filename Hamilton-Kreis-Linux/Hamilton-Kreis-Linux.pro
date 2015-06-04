TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    HandleFile.cpp

OTHER_FILES += \
    Hamilton-Kreis-Linux.pro.user

HEADERS += \
    Edge.h \
    HandleFile.h \
    Node.h

