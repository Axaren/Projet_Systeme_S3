TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += thread


SOURCES += \
    serveur.cpp \
    ../utils/make-sockaddr.cpp \
    ../utils/EnvoiDonnees.cpp

HEADERS += \
    ../utils/make-sockaddr.h \
    ../utils/EnvoiDonnees.h
