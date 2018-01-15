TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pedantic -Wall -Wextra
SOURCES += \
    ../utils/make-sockaddr.cpp \
    ../utils/EnvoiDonnees.cpp \
    consultation.cpp

HEADERS += \
    ../utils/make-sockaddr.h \
    ../utils/EnvoiDonnees.h

