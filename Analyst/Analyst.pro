#-------------------------------------------------
#
# Project created by QtCreator 2016-12-14T09:33:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++14

TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += \
    mainwindow.ui



INCLUDEPATH += ../include


###################### unix ############################
unix {
    # QT += opengl
    DEFINES += _UNIX
    # DEFINES += QCUSTOMPLOT_USE_OPENGL
    # DEFINES += QCP_OPENGL_PBUFFER

    message("Building for unix")
    INCLUDEPATH += ../ctp-api/20160606_tradeapi_linux64
    INCLUDEPATH += /usr/local/include
    LIBS += -lpthread -lrt

    target.path = /usr/local/bin/gmd
    INSTALLS += target

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = Analystd
        LIBS += -L$$OUT_PWD/../utility/ -lutilityd
        LIBS += -L$$OUT_PWD/../ctpif/ -lctpifd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = Analyst
        LIBS += -L$$OUT_PWD/../utility/ -lutility
        LIBS += -L$$OUT_PWD/../ctpif/ -lctpif
        message("Build for release version")
    }
}

###################### windows #########################
windows {
    DEFINES += _WINDOWS

    message("Building for Windows")

    INCLUDEPATH += $$PWD/../ctp-api/20160606_tradeapi_win64
    INCLUDEPATH += $$PWD/../gtest
    # LIBS += -L$$PWD/../gtest/win64 -lgtest -lgtest_main
    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = Analystd
        LIBS += -L$$OUT_PWD/../utility/debug/ -lutilityd
        LIBS += -L$$OUT_PWD/../ctpif/debug/ -lctpifd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = Analyst
        LIBS += -L$$OUT_PWD/../utility/release/ -lutility
        LIBS += -L$$OUT_PWD/../ctpif/release/ -lctpif
        message("Build for release version")
    }
}

RESOURCES += \
    analyst.qrc
