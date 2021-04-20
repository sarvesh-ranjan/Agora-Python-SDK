#-------------------------------------------------
#
# Project created by QtCreator 2019-09-24T10:52:31
#
#-------------------------------------------------

QT       += core gui quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SDKPATHNAME=libs
SDKLIBPATHNAME=x86
SDKDLLPATHNAME=x86

!contains(QMAKE_TARGET.arch, x86_64) {
  SDKLIBPATHNAME=x86
  SDKDLLPATHNAME=x86
} else {
  SDKLIBPATHNAME=x86_64
  SDKDLLPATHNAME=x86_64
}

TARGET = OpenVideoCall
TEMPLATE = app


SOURCES += main.cpp \
    agoraconfig.cpp \
    openvideocall.cpp \
    avdevice.cpp \
    agoraobject.cpp \
    nettesting.cpp \
    nettestresult.cpp \
    nettestdetail.cpp \
    agorawindowmanager.cpp \
    enterroom.cpp \
    inroom.cpp \
    agoraqtjson.cpp

HEADERS  += \
    agoraconfig.h \
    openvideocall.h \
    avdevice.h \
    agoraobject.h \
    nettesting.h \
    nettestresult.h \
    nettestdetail.h \
    agorawindowmanager.h \
    enterroom.h \
    inroom.h \
    agoraqtjson.h

FORMS    += \
    openvideocall.ui \
    avdevice.ui \
    nettesting.ui \
    nettestresult.ui \
    nettestdetail.ui \
    enterroom.ui \
    inroom.ui

RESOURCES += \
    openvideocall.qrc

RC_FILE = openvideocall.rc

DISTFILES += \
    uiresource/OVC-win-more users.jpg \
    uiresource/icon-back hover.png \
    uiresource/icon-camera hover.png \
    uiresource/icon-camera off.png \
    uiresource/icon-close hover.png \
    uiresource/icon-hang up hover.png \
    uiresource/icon-hang up.png \
    uiresource/icon-micorophone hover.png \
    uiresource/icon-micorophone off.png \
    uiresource/icon-setting hover.png \
    openvideocall.rc

exists( $$PWD/$${SDKPATHNAME}) {
  AGORASDKPATH = $$PWD/$${SDKPATHNAME}
  AGORASDKDLLPATH = .\\$${SDKPATHNAME}\\$${SDKDLLPATHNAME}
} else {
  AGORASDKPATH = $$PWD/../../$${SDKPATHNAME}
  AGORASDKDLLPATH =..\\..\\$${SDKPATHNAME}\\$${SDKDLLPATHNAME}
}

win32: {
INCLUDEPATH += $${AGORASDKPATH}/include
LIBS += -L$${AGORASDKPATH}/$${SDKLIBPATHNAME} -lagora_rtc_sdk
LIBS += User32.LIB

CONFIG(debug, debug|release) {
  QMAKE_POST_LINK +=  copy $${AGORASDKDLLPATH}\*.dll .\Debug
} else {
  QMAKE_POST_LINK +=  copy $${AGORASDKDLLPATH}\*.dll .\Release
  QMAKE_POST_LINK += && windeployqt Release\OpenVideoCall.exe
}

}
