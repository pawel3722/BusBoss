
#include "interface/Manager/manager.h"

Manager::Manager(Data* data,QWidget *parent)
    : QStackedWidget{parent}
{
    this->data = data;
    createWidgets();
    showMenu();
}

void Manager::createWidgets()
{
    menu_widget = new MenuWidget(this);
    line_widget = new LineWidget(data,this);
    stop_widget = new StopWidget(data,this);
    connection_widget = new ConnectionWidget(data,this);
    route_widget = new RouteWidget(data,this);
    job_widget = new JobWidget(data,this);
    shift_widget = new ShiftWidget(data,this);
    bus_widget = new BusWidget(data,this);
    worker_widget = new WorkerWidget(data,this);
    this->addWidget(menu_widget);
    this->addWidget(line_widget);
    this->addWidget(stop_widget);
    this->addWidget(connection_widget);
    this->addWidget(route_widget);
    this->addWidget(job_widget);
    this->addWidget(shift_widget);
    this->addWidget(bus_widget);
    this->addWidget(worker_widget);
}
