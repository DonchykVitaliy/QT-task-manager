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
    calendar_window.cpp \
    count_complited_note.cpp \
    create_folder_widnow.cpp \
    create_window.cpp \
    edit_task_window.cpp \
    folder_widget.cpp \
    folders_widget.cpp \
    folders_window.cpp \
    generate_name.cpp \
    main.cpp \
    mainwindow.cpp \
    notifications.cpp \
    open_folder_window.cpp \
    open_note.cpp \
    settings_window.cpp \
    stat_line_widget.cpp \
    stat_widget.cpp \
    stat_window.cpp \
    task_widget.cpp

HEADERS += \
    basket_widget.h \
    basket_window.h \
    calendar_window.h \
    count_complited_note.h \
    create_folder_widnow.h \
    create_window.h \
    edit_task_window.h \
    folder_widget.h \
    folders_widget.h \
    folders_window.h \
    generate_name.h \
    mainwindow.h \
    notifications.h \
    open_folder_window.h \
    open_note.h \
    settings_window.h \
    stat_line_widget.h \
    stat_widget.h \
    stat_window.h \
    task_widget.h

FORMS += \
    basket_window.ui \
    calendar_window.ui \
    create_folder_widnow.ui \
    create_window.ui \
    edit_task_window.ui \
    folders_window.ui \
    mainwindow.ui \
    open_folder_window.ui \
    open_note.ui \
    settings_window.ui \
    stat_window.ui

TRANSLATIONS += \
    QTtasks_uk_UA.ts \
    QTtasks_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

win32:RC_FILE = icon.rc

DISTFILES +=
