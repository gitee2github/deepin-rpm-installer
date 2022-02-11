#-------------------------------------------------
#
# Project created by QtCreator 2021-06-15T11:03:54
#
#-------------------------------------------------

QT       += core gui svg quickwidgets    # 包含的模块

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
    pages/rpmselectpage.cpp \
    pages/selectpage.cpp \
    pages/pkgdetaildialog.cpp \
    pages/singleinstallpage.cpp

# 所有的头文件
HEADERS += \
    common/consts.h \
    common/datastructs.h \
    common/installthread.h \
    common/rpminfo.h \
    pages/mainwindow.h \
    common/dragdropfilearea.h \
    pages/rpmselectpage.h \
    pages/selectpage.h \
    pages/pkgdetaildialog.h \
    pages/singleinstallpage.h

# UI 设计文件
FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml/qmls.qrc \
    resources/imageassets.qrc
