TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c

TEMPLATE = lib


DISTFILES += \
    py1.py \
    runcpy.py

DEFINES += CPY1_LIBRARY


###################### unix ############################
unix {
    DEFINES += _UNIX

    target.path = /usr/local/lib
    INSTALLS += target

    message("Building for unix")
    INCLUDEPATH += ../ctp-api/20160606_tradeapi_linux64
    INCLUDEPATH += /usr/local/include
    # INCLUDEPATH += /data/py2713/include/python2.7
    INCLUDEPATH += /usr/include/python2.7

     # don't forget linke thost..so to libthost...so
    LIBS += -lthostmduserapi -lthosttraderapi -lrt -lpthread -lpython2.7

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        message("Build for release version")
    }
}

###################### windows #########################
windows {
    DEFINES += _WINDOWS
    INCLUDEPATH += c:\Python27\include
    message("Building for Windows")
    LIBS += -Lc:\python27\libs -lpython27 -lpyexpat

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        message("Build for release version")
    }
}
