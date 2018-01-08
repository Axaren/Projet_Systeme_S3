TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra

SOURCES += main.cpp \
    ../utils/EnvoiDonnees.cpp \
    ../utils/make-sockaddr.cpp

HEADERS += \
    ../utils/EnvoiDonnees.h \
    ../utils/make-sockaddr.h
