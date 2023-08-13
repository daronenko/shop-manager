QT += core charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/chartwindow.cpp \
    src/editwindow.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/table.cpp \
    src/tablewidget.cpp

HEADERS += \
    src/chartwindow.h \
    src/editwindow.h \
    src/mainwindow.h \
    src/table.h \
    src/tablewidget.h

FORMS += \
    ui/chartwindow.ui \
    ui/editwindow.ui \
    ui/mainwindow.ui \
    ui/tablewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
