win32:QT += core gui
#unix:QT += androidextras

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml sql
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

TARGET = MultiProg
TEMPLATE = app
RC_FILE = MultiProg.rc
HEADERS       = \
    mainwindow.h \
    HardwareLayer/usbTools.h \
    HardwareLayer/instructions.h \
    Libs/commonFunctions.h \
    widgets/hexEditor/hexPlainTextEdit.h \
    widgets/hexEditor/hexEditor.h \
    widgets/hexEditor/asciiPlainTextEdit.h \
    widgets/fuseSettings/fuseSettings.h \
    HardwareLayer/Devices/devices.h \
    widgets/deviceListWidget.h \
    HardwareLayer/Devices/progDevice.h \
    HardwareLayer/Devices/Memory/progEEPROM.h \
    HardwareLayer/Devices/PIC/progP12.h \
    HardwareLayer/Devices/PIC/progP16.h \
    HardwareLayer/Devices/PIC/progP18.h \
    HardwareLayer/Devices/PIC/progP24.h \
    HardwareLayer/Devices/AVR/progAVR.h \
    HardwareLayer/Devices/readWriteInfo.h \
    Libs/globalVariables.h \
    Libs/strings.h \
    Forms/ioCheck.h \
    Forms/convert.h \
    Forms/icd.h \
    Forms/dis_asmble.h \
    HardwareLayer/Protcol/icd_protcol.h \
    HardwareLayer/Protcol/i2c_spi_protcol.h \
    HardwareLayer/fileIO.h \
    widgets/scrollTest.h \
    HardwareLayer/Devices/AVR/avrFuse.h \
    widgets/fuseSettings/avrFuseSettings.h \
    widgets/fuseSettings/picFuseSettings.h \
    HardwareLayer/Devices/PIC/readXMLPIC.h \
    Libs/xmlFunctions.h \
    Libs/databaseLayer.h \
    HardwareLayer/Devices/fuseByte.h \
    HardwareLayer/Devices/AVR/readXMLAVR.h \
    HardwareLayer/Devices/PIC/picInfo.h
SOURCES       = main.cpp \
    mainwindow.cpp \
    HardwareLayer/usbTools.cpp \
    HardwareLayer/Devices/Memory/progEEPROM.cpp \
    Libs/commonFunctions.cpp \
    widgets/hexEditor/hexPlainTextEdit.cpp \
    widgets/hexEditor/hexEditor.cpp \
    widgets/hexEditor/asciiPlainTextEdit.cpp \
    widgets/fuseSettings/fuseSettings.cpp \
    widgets/fuseSettings/avrFuseSettings.cpp \
    HardwareLayer/Devices/devices.cpp \
    widgets/deviceListWidget.cpp \
    HardwareLayer/Devices/progDevice.cpp \
    Libs/globalVariables.cpp \
    HardwareLayer/Devices/PIC/progP12.cpp \
    HardwareLayer/Devices/PIC/progP16.cpp \
    HardwareLayer/Devices/PIC/progP18.cpp \
    HardwareLayer/Devices/PIC/progP24.cpp \
    HardwareLayer/Devices/AVR/progAVR.cpp \
    HardwareLayer/Devices/readWriteInfo.cpp \
    Libs/strings.cpp \
    Forms/ioCheck.cpp \
    Forms/convert.cpp \
    Forms/icd.cpp \
    Forms/dis_asmble.cpp \
    HardwareLayer/Protcol/icd_protcol.cpp \
    HardwareLayer/Protcol/i2c_spi_protcol.cpp \
    HardwareLayer/fileIO.cpp \
    widgets/scrollTest.cpp \
    HardwareLayer/Devices/AVR/avrFuse.cpp \
    widgets/fuseSettings/picFuseSettings.cpp \
    HardwareLayer/Devices/PIC/readXMLPIC.cpp \
    Libs/xmlFunctions.cpp \
    Libs/databaseLayer.cpp \
    HardwareLayer/Devices/fuseByte.cpp \
    HardwareLayer/Devices/AVR/readXMLAVR.cpp \
    HardwareLayer/Devices/PIC/picInfo.cpp
#! [0]
RESOURCES     = MultiProg.qrc \
    MultiProg.qrc
#! [0]


FORMS += \
    mainwindow.ui \
    widgets/hexEditor/hexEditor.ui \
    widgets/fuseSettings/fuseSettings.ui \
    widgets/fuseSettings/avrFuseSettings.ui \
    widgets/deviceListWidget.ui \
    Forms/convert.ui \
    Forms/ioCheck.ui \
    Forms/icd.ui \
    Forms/dis_asmble.ui \
    widgets/fuseSettings/picFuseSettings.ui

win32:LIBS += -lsetupapi -ladvapi32 -lws2_32

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/gen/org/qtproject/MultiProg/BuildConfig.java \
    android/gen/org/qtproject/MultiProg/R.java

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/org/qtproject/MultiProg/AndroidUSBTools.java \
    android/res/xml/device_filter.xml


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ExternalLibs/USB/armeabiV7a/release/ -lusb1
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ExternalLibs/USB/armeabiV7a/debug/ -lusb1
#else:unix: LIBS += -lusb1


#INCLUDEPATH += $$PWD/ExternalLibs/USB/libusb  \
#            $$PWD/ExternalLibs/USB

#DEPENDPATH += $$PWD/ExternalLibs/USB/libusb \
#            $$PWD/ExternalLibs/USB

#contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
#    ANDROID_EXTRA_LIBS = \
#        $$PWD/../../Android/USB/libusb-master/android/libs-1/armeabi-v7a/libusb1.0.so
#}

