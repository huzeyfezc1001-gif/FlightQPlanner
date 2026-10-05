QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
DEFINES += MAVSDK_ENABLE_MAVLINK_C_API

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FController.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    FController.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

DESTDIR = $$PWD/bin

OBJECTS_DIR = $$PWD/build/obj
MOC_DIR = $$PWD/build/moc
RCC_DIR = $$PWD/build/rcc
UI_DIR = $$PWD/build/ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += include
INCLUDEPATH += $$(DEV_EXTERNALS_PATH)/USBSDK_Thermal/include

macx {
    INCLUDEPATH += /opt/homebrew/include
    INCLUDEPATH += /opt/homebrew/include/mavsdk
    INCLUDEPATH += /opt/homebrew/include/opencv4

    LIBS += -L/opt/homebrew/lib -lmavsdk
}

unix:!macx {
    INCLUDEPATH += /usr/include
    INCLUDEPATH += /usr/include/mavsdk
    INCLUDEPATH += /usr/include/opencv4
    INCLUDEPATH += /usr/include/gazebo-11
    INCLUDEPATH += /usr/include/sdformat-9.7

    LIBS += -L/usr/lib -lmavsdk
    LIBS += -lX11
}


#LIBS += -L/usr/local/lib -lopencv_imgcodecs -lopencv_core -lopencv_highgui -lopencv_imgproc
#win32:CONFIG(release, debug|release): LIBS += -L$$(DEV_EXTERNALS_PATH)/USBSDK_Thermal/libs/release/ -lUSBSDK
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$$(DEV_EXTERNALS_PATH)/USBSDK_Thermal/libs/debug/ -lUSBSDK
#else:unix: LIBS += -L$$(DEV_EXTERNALS_PATH)/USBSDK_Thermal/libs/ -lUSBSDK

RESOURCES += \
    recources.qrc