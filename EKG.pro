#-------------------------------------------------
#
# Project created by Bijan Binaee 2010-03-09T04:32:52
#
#-------------------------------------------------
QT       += core gui


SOURCES += \
    main.cpp \
    ekgwindow.cpp \
    rythmchk.cpp \
    qrsfilt.cpp \
    qrsdet2.cpp \
    postclas.cpp \
    noisechk.cpp \
    match.cpp \
    classify.cpp \
    bdac.cpp \
    analbeat.cpp \
    sigdetect.cpp \
    wekawriter.cpp \
    diseawriter.cpp \
    r_feature.cpp \
    listrw.cpp \
    ekgsignal.cpp \
    GButton.cpp

DEPENDPATH  += /usr/include/qwt-qt4
INCLUDEPATH += /usr/include/qwt-qt4
LIBS        += -lqwt-qt4 \
               -lwfdb

#QMAKE_CXXFLAGS += -E
QMAKE_CXXFLAGS += -mtune=core2

HEADERS += \
    ekgwindow.h \
    rythmchk.h \
    qrsdet.h \
    postclas.h \
    match.h \
    inputs.h \
    ecgcodes.h \
    bdac.h \
    analbeat.h \
    sigdetect.h \
    wekawriter.h \
    diseawriter.h \
    r_feature.h \
    listrw.h \
    ekgsignal.h \
    GButton.h

RESOURCES += \
    EKG.qrc
