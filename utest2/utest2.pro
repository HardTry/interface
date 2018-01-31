
TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../include
INCLUDEPATH += $$PWD/../../eureka/include/docs



SOURCES += test_ctpmd_interface.cpp
# test2.cpp \

###################### unix ############################
unix {
    DEFINES += _UNIX

    message("Building for unix")
    INCLUDEPATH += ../ctp-api/20160606_tradeapi_linux64
    INCLUDEPATH += /usr/local/include
    INCLUDEPATH += /usr/include/python2.7

    LIBS += -L/usr/local/lib -lgtest -lgtest_main
    # LIBS += -lthosttraderapi -lthostmduserapi
    LIBS += -lpthread -lrt
    # LIBS += -lpython2.7 -lcpy1

    target.path = /usr/local/bin/gmd
    INSTALLS += target

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = utest-2d
        LIBS += -L$$OUT_PWD/../utility/ -lutilityd
        LIBS += -L$$OUT_PWD/../ctpif/ -lctpifd
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = utest-2
        LIBS += -lutility -lctpif
        # LIBS += -L$$OUT_PWD/../utility/ -lutility
        # LIBS += -L$$OUT_PWD/../ctpif/ -lctpif
        message("Build for release version")
    }
}

###################### windows #########################
windows {
    DEFINES += _WINDOWS

    message("Building for Windows")

    INCLUDEPATH += $$PWD/../ctp-api/20160606_tradeapi_win64
    INCLUDEPATH += $$PWD/../gtest
    INCLUDEPATH += "C:\Program Files\libuv\include"
    LIBS += "C:\Program Files\libuv\libuv.lib"

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = utest-2d
        LIBS += -L$$OUT_PWD/../utility/debug/ -lutilityd
        LIBS += -L$$OUT_PWD/../ctpif/debug/ -lctpifd
        LIBS += -L$$PWD/../gtest/win64 -lgtest-d -lgtest_main-d
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = utest-2
        LIBS += -L$$OUT_PWD/../utility/release/ -lutility
        LIBS += -L$$OUT_PWD/../ctpif/release/ -lctpif
        LIBS += -L$$PWD/../gtest/win64 -lgtest -lgtest_main
        message("Build for release version")
    }
}

DISTFILES += \
    shared-read.py \
    helloworld.py
