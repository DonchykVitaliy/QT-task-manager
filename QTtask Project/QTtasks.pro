QT       += core gui

RESOURCES += resources.qrc
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basket_widget.cpp \
    basket_window.cpp \
    create_folder_widnow.cpp \
    create_window.cpp \
    edit_task_window.cpp \
    folder_widget.cpp \
    folders_widget.cpp \
    folders_window.cpp \
    generate_name.cpp \
    main.cpp \
    mainwindow.cpp \
    open_folder_window.cpp \
    settings_window.cpp \
    stat_widget.cpp \
    stat_window.cpp \
    task_widget.cpp

HEADERS += \
    basket_widget.h \
    basket_window.h \
    create_folder_widnow.h \
    create_window.h \
    edit_task_window.h \
    folder_widget.h \
    folders_widget.h \
    folders_window.h \
    generate_name.h \
    mainwindow.h \
    open_folder_window.h \
    settings_window.h \
    stat_widget.h \
    stat_window.h \
    task_widget.h

FORMS += \
    basket_window.ui \
    create_folder_widnow.ui \
    create_window.ui \
    edit_task_window.ui \
    folders_window.ui \
    mainwindow.ui \
    open_folder_window.ui \
    settings_window.ui \
    stat_window.ui

TRANSLATIONS += \
    QTtasks_uk_UA.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
