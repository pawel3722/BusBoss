
#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H


#include "data/data.h"
#include "qcombobox.h"
#include "qgridlayout.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qspinbox.h"
#include <QWidget>


class ConnectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionWidget(Data* data, QWidget *parent = nullptr);
    void updateConnections();


private:
    Data* data;
    QWidget* parent;
    QPushButton* ret_button;
    QGridLayout* layout;

    //choose_section
    QLabel* choose_connection_label;
    QComboBox* got_connection;
    QPushButton* confirm_connection;
    Connection* chosen_connection;
    QLabel* chosen_connection_label;
    QLabel* time_msg_label;
    QLabel* distance_msg_label;

    //time1 section
    QLabel* time1_label;
    QSpinBox* time1_min;
    QSpinBox* time1_sec;

    //time2 section
    QLabel* time2_label;
    QSpinBox* time2_min;
    QSpinBox* time2_sec;

    //time3 section
    QLabel* time3_label;
    QSpinBox* time3_min;
    QSpinBox* time3_sec;

    QPushButton* confirm_times;

    QLabel* distance_label;
    QDoubleSpinBox* got_distance;
    QPushButton* confirm_distance;

    void activateBoxes();
    void setDefaultValues();
    void deactivateBoxes();
    void setTimes();
    void setDistance();
    void resetTimeMsg(){time_msg_label->setText("");}
    void resetDistanceMsg(){distance_msg_label->setText("");}

};

#endif // CONNECTIONWIDGET_H
