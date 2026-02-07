CODECFORTR = UTF-8

QT       += core gui xml svg concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    appwidget.cpp \
    basewindow.cpp \
    logpage.cpp \
    logpagetest.cpp \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    appwidget.h \
    basewindow.h \
    logpage.h \
    logpagetest.h \
    mainwindow.h \
    structdefine.h \

FORMS += \
    logpage.ui \
    logpagetest.ui \
    mainwindow.ui

RESOURCES += \
    res/qss.qrc \
    res/main.qrc
    res/icons.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32{

    INCLUDEPATH += D:\opencv\opencv-build_MinGW_x64\install\include

    LIBS +=-L  D:\opencv\opencv-build_MinGW_x64\install\x64\mingw\lib\libopencv_*.a
#    INCLUDEPATH += D:\opencv\opencv-4.10.0_build_msvc\install\include
#    LIBS +=-L D:/opencv/opencv-4.10.0_build_msvc/install/x64/vc15/lib/ -lopencv_world4100d
#win32:CONFIG(release, debug|release): LIBS += -LD:/opencv/opencv-4.10.0_build_msvc/install/x64/vc15/lib/ -lopencv_world4100
#else:win32:CONFIG(debug, debug|release): LIBS += -LD:/opencv/opencv-4.10.0_build_msvc/install/x64/vc15/lib/ -lopencv_world4100d

#INCLUDEPATH += D:/opencv/opencv-4.10.0_build_msvc/install/include
#DEPENDPATH += D:/opencv/opencv-4.10.0_build_msvc/install/include
}
linux{
    INCLUDEPATH += /home/hello/opencv/opencv-4.5.4/build/modules
    LIBS += /home/hello/opencv/opencv-4.5.4/build/lib/libopencv_*
#    INCLUDEPATH += /usr/local/include
#    LIBS +=/usr/local/lib/libboost_*
}
include(components/components.pri)
include(view/view.pri)
include(module/module.pri)
