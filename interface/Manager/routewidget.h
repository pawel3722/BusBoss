
#ifndef ROUTEWIDGET_H
#define ROUTEWIDGET_H


#include "data/data.h"
#include "qcombobox.h"
#include "qgridlayout.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include <QWidget>


class RouteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RouteWidget(Data* data, QWidget *parent = nullptr);
    void updateRoutes();

private:
    Data* data;
    QWidget* parent;
    QPushButton* ret_button;
    QGridLayout* layout;
    QPushButton* add_time_button;
    QPushButton* del_time_button;
    QPushButton* confirm_changes;
    QPushButton* change_direction;
    QPushButton* del_route_button;

    //choose_section
    QLabel* choose_route_label;
    QComboBox* got_route;
    QPushButton* confirm_route;
    Route* chosen_route;
    QLabel* chosen_route_label;
    QLabel* time_msg_label;

    //route_section
    std::vector<QLabel*> stop_labels;
    std::vector<QLabel*> timescale_labels;
    std::vector<std::vector<QLabel*>> time_labels;
    std::vector<std::vector<QComboBox*>> time_boxes;

    void activateBoxes();
    void deactivateBoxes();
    void updateTimeLabels();
    void addColumn(int current_timescale);
    void delColumn();
    void addTime();
    void delTime();
    void confirmChanges();
    void changeDirection();
    void delRoute();

};

#endif // ROUTEWIDGET_H
