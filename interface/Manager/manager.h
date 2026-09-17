
#ifndef MANAGER_H
#define MANAGER_H


#include "data/data.h"
#include "interface/Manager/buswidget.h"
#include "interface/Manager/connectionwidget.h"
#include "interface/Manager/jobwidget.h"
#include "interface/Manager/linewidget.h"
#include "interface/Manager/menuwidget.h"
#include "interface/Manager/routewidget.h"
#include "interface/Manager/shiftwidget.h"
#include "interface/Manager/workerwidget.h"
#include "qstackedwidget.h"
#include "stopwidget.h"


class Manager : public QStackedWidget
{
    Q_OBJECT
public:
    explicit Manager(Data* data,QWidget *parent = nullptr);
    void showMenu(){setCurrentIndex(0);}
    void manageLines(){setCurrentIndex(1);}
    void manageStops(){setCurrentIndex(2); stop_widget->updateStops();}
    void manageConnections(){setCurrentIndex(3); connection_widget->updateConnections();}
    void manageRoutes(){setCurrentIndex(4);route_widget->updateRoutes();}
    void manageJobs(){setCurrentIndex(5); job_widget->updateJobs();}
    void manageShifts(){setCurrentIndex(6);shift_widget->updateShifts();}
    void manageBuses(){setCurrentIndex(7); bus_widget->updateBuses();}
    void manageWorkers(){setCurrentIndex(8); worker_widget->updateWorkers();}

private:

    void createWidgets();

    Data* data;
    MenuWidget* menu_widget;
    LineWidget* line_widget;
    StopWidget* stop_widget;
    ConnectionWidget* connection_widget;
    RouteWidget* route_widget;
    JobWidget* job_widget;
    ShiftWidget* shift_widget;
    BusWidget* bus_widget;
    WorkerWidget* worker_widget;

};

#endif // MANAGER_H
