QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20
CONFIG += console
CONFIG += static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    interface/BusMap/busmapbus.cpp \
    data/intersection.cpp \
    data/road.cpp \
    interface/BusMap/busmap.cpp \
    interface/BusMap/busmapstop.cpp \
    interface/Manager/buswidget.cpp \
    interface/Manager/connectionwidget.cpp \
    interface/Manager/jobwidget.cpp \
    interface/Manager/linewidget.cpp \
    interface/Manager/routewidget.cpp \
    interface/Manager/setshiftsdialog.cpp \
    interface/Manager/shiftwidget.cpp \
    interface/Manager/stopwidget.cpp \
    interface/Manager/workerwidget.cpp \
    interface/changetimedialog.cpp \
    interface/mainwindow.cpp \
    data/busstop.cpp \
    data/connection.cpp \
    data/data.cpp \
    data/job.cpp \
    data/line.cpp \
    data/main.cpp \
    data/route.cpp \
    data/service.cpp \
    data/shift.cpp \
    interface/Manager/manager.cpp \
    interface/Manager/menuwidget.cpp

HEADERS += \
    interface/BusMap/busmapbus.h \
    data/intersection.h \
    data/road.h \
    interface/BusMap/busmap.h \
    interface/BusMap/busmapintersection.h \
    interface/BusMap/busmaproad.h \
    interface/BusMap/busmapstop.h \
    interface/Manager/buswidget.h \
    interface/Manager/connectionwidget.h \
    interface/Manager/jobwidget.h \
    interface/Manager/linewidget.h \
    interface/Manager/routewidget.h \
    interface/Manager/setshiftsdialog.h \
    interface/Manager/shiftwidget.h \
    interface/Manager/stopwidget.h \
    interface/Manager/workerwidget.h \
    interface/changetimedialog.h \
    interface/mainwindow.h \
    data/bus.h \
    data/busstop.h \
    data/connection.h \
    data/data.h \
    data/job.h \
    data/line.h \
    data/route.h \
    data/service.h \
    data/shift.h \
    data/worker.h \
    interface/Manager/manager.h \
    interface/Manager/menuwidget.h
RESOURCES += resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
