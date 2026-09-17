#include <QVBoxLayout>
#include "menuwidget.h"
#include "interface/Manager/manager.h"


MenuWidget::MenuWidget(QWidget *parent)
    : QWidget{parent}
{
    this->parent = parent;
    auto local_parent = dynamic_cast<Manager*>(parent);
    const int SIZE = 100;
    QString ss = "font-size:20px;";
    manage_lines_but = new QPushButton("Zarządzanie liniami");
        manage_lines_but->setMinimumHeight(SIZE);
        manage_lines_but->setStyleSheet(ss);
        connect(manage_lines_but,&QPushButton::clicked,local_parent,&Manager::manageLines);
    manage_stops_but = new QPushButton("Zarządzanie przystankami");
        manage_stops_but->setMinimumHeight(SIZE);
        manage_stops_but->setStyleSheet(ss);
        connect(manage_stops_but,&QPushButton::clicked,local_parent,&Manager::manageStops);
    manage_connections_but = new QPushButton("Zarządzanie połączeniami");
        manage_connections_but->setMinimumHeight(SIZE);
        manage_connections_but->setStyleSheet(ss);
        connect(manage_connections_but,&QPushButton::clicked,local_parent,&Manager::manageConnections);
    manage_routes_but = new QPushButton("Zarządzanie trasami");
        manage_routes_but->setMinimumHeight(SIZE);
        manage_routes_but->setStyleSheet(ss);
        connect(manage_routes_but,&QPushButton::clicked,local_parent,&Manager::manageRoutes);
    manage_jobs_but = new QPushButton("Zarządzanie brygadami");
        manage_jobs_but->setMinimumHeight(SIZE);
        manage_jobs_but->setStyleSheet(ss);
        connect(manage_jobs_but,&QPushButton::clicked,local_parent,&Manager::manageJobs);
    manage_shifts_but = new QPushButton("Zarządzanie zmianami");
        manage_shifts_but->setMinimumHeight(SIZE);
        manage_shifts_but->setStyleSheet(ss);
        connect(manage_shifts_but,&QPushButton::clicked,local_parent,&Manager::manageShifts);
    manage_buses_but = new QPushButton("Zarządzanie autobusami");
        manage_buses_but->setMinimumHeight(SIZE);
        manage_buses_but->setStyleSheet(ss);
        connect(manage_buses_but,&QPushButton::clicked,local_parent,&Manager::manageBuses);
    manage_workers_but = new QPushButton("Zarządzanie kadrą");
        manage_workers_but->setMinimumHeight(SIZE);
        manage_workers_but->setStyleSheet(ss);
        connect(manage_workers_but,&QPushButton::clicked,local_parent,&::Manager::manageWorkers);
    int row = 0;
    auto layout = new QGridLayout;
    layout->addItem(new QSpacerItem(500,1,QSizePolicy::Preferred,QSizePolicy::Preferred),row,0);
    layout->addItem(new QSpacerItem(500,1,QSizePolicy::Preferred,QSizePolicy::Preferred),row++,2);
    layout->addWidget(manage_lines_but,row++,1);
    layout->addWidget(manage_stops_but,row++,1);
    layout->addWidget(manage_connections_but,row++,1);
    layout->addWidget(manage_routes_but,row++,1);
    layout->addWidget(manage_jobs_but,row++,1);
    layout->addWidget(manage_shifts_but,row++,1);
    layout->addWidget(manage_buses_but,row++,1);
    layout->addWidget(manage_workers_but,row++,1);
    layout->addItem(new QSpacerItem(500,1,QSizePolicy::Preferred,QSizePolicy::Preferred),row,0);
    layout->addItem(new QSpacerItem(500,1,QSizePolicy::Preferred,QSizePolicy::Preferred),row,2);

    setLayout(layout);

}

