#-------------------------------------------------
#
# Project created by Bijan Binaee 2010-03-09T04:32:52
#
#-------------------------------------------------
QT       += core gui


SOURCES += \
    main.cpp \
    ekgwindow.cpp \
    sigdetect.cpp \
    wekawriter.cpp \
    diseawriter.cpp \
    r_feature.cpp \
    listrw.cpp \
    ekgsignal.cpp \
    GButton.cpp \
    ekgscan.cpp \
    ekgnn.cpp \
    qrsdet.cpp \
    EKGFramWork.cpp \
    ekgimagep.cpp \
    qscan.cpp

DEPENDPATH  += /usr/include/qwt-qt4
INCLUDEPATH += /usr/include/qwt-qt4
LIBS        += -lqwt-qt4 \
               -lwfdb \
               -lm \
               -lfann \
               -ltbb \
               -lfftw3 \
               -lksane

#QMAKE_CXXFLAGS += -E
QMAKE_CXXFLAGS += -mtune=core2

HEADERS += \
    ekgwindow.h \
    rythmchk.h \
    sigdetect.h \
    wekawriter.h \
    diseawriter.h \
    r_feature.h \
    listrw.h \
    ekgsignal.h \
    GButton.h \
    ekgscan.h \
    ekgnn.h \
    header.h \
    qrsdet.h \
    EKGFramWork.h \
    ekgimagep.h \
    qscan.h

RESOURCES += \
    EKG.qrc
