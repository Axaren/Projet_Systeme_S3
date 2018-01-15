TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra
SOURCES += main.cpp \
    ../utils/EnvoiDonnees.cpp \
    ../utils/make-sockaddr.cpp

include(deployment.pri)
qtcAddDeployment()

LIBS+= -pthread

HEADERS += \
    ../utils/EnvoiDonnees.h \
    ../utils/make-sockaddr.h

