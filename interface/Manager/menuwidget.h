#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include "qpushbutton.h"
#include <QWidget>


class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);

private:
    QWidget* parent;

    QPushButton* manage_lines_but;
    QPushButton* manage_stops_but;
    QPushButton* manage_connections_but;
    QPushButton* manage_routes_but;
    QPushButton* manage_jobs_but;
    QPushButton* manage_shifts_but;
    QPushButton* manage_buses_but;
    QPushButton* manage_workers_but;
};

#endif // MENUWIDGET_H
