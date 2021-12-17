#-------------------------------------------------
#
# Project created by QtCreator 2021-06-15T11:03:54
#
#-------------------------------------------------

QT       += core gui svg    # 包含的模块

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rpminstaller       # 应用程序名称
TEMPLATE = app              # 模板类型

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

# 所有的源文件
SOURCES += \
    common/installthread.cpp \
    main.cpp \
    pages/mainwindow.cpp \
    common/dragdropfilearea.cpp \
    pages/selectpage.cpp \
    pages/pkgdetaildialog.cpp

# 所有的头文件
HEADERS += \
    common/datastructs.h \
    common/installthread.h \
    pages/mainwindow.h \
    common/dragdropfilearea.h \
    pages/selectpage.h \
    pages/pkgdetaildialog.h

# UI 设计文件
FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/imageassets.qrc
