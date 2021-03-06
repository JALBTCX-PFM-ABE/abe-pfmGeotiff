RC_FILE = pfmGeotiff.rc
RESOURCES = icons.qrc
contains(QT_CONFIG, opengl): QT += opengl
QT += 
INCLUDEPATH += /c/PFM_ABEv7.0.0_Win64/include
LIBS += -L /c/PFM_ABEv7.0.0_Win64/lib -lnvutility -lpfm -lgdal -lxml2 -lpoppler -liconv
DEFINES += WIN32 NVWIN3X
CONFIG += console
QMAKE_LFLAGS += 
######################################################################
# Automatically generated by qmake (2.01a) Wed Jan 22 14:40:47 2020
######################################################################

TEMPLATE = app
TARGET = pfmGeotiff
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += imagePage.hpp \
           imagePageHelp.hpp \
           pfmGeotiff.hpp \
           pfmGeotiffDef.hpp \
           pfmGeotiffHelp.hpp \
           runPage.hpp \
           startPage.hpp \
           startPageHelp.hpp \
           surfacePage.hpp \
           surfacePageHelp.hpp \
           version.hpp
SOURCES += env_in_out.cpp \
           hsvrgb.cpp \
           imagePage.cpp \
           main.cpp \
           palshd.cpp \
           pfmGeotiff.cpp \
           runPage.cpp \
           scribe.cpp \
           set_defaults.cpp \
           startPage.cpp \
           surfacePage.cpp
RESOURCES += icons.qrc
TRANSLATIONS += pfmGeotiff_xx.ts
