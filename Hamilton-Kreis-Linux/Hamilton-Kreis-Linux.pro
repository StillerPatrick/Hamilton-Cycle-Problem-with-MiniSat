TEMPLATE = app
CONFIG += console
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

