
#ifndef SHIFTWIDGET_H
#define SHIFTWIDGET_H


#include "data/data.h"
#include "qcombobox.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include <QWidget>


class ShiftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShiftWidget(Data* data,QWidget *parent = nullptr);
    void updateShifts();


private:
    Data* data;
    QWidget* parent;
    QPushButton* ret_button;

    //choose_section
    QLabel* choose_shift_label;
    QComboBox* got_shift;
    QPushButton* confirm_shift;
    Shift* chosen_shift;
    QLabel* chosen_shift_label;

    //add_section
    QLabel* msg_label;
    QLabel* choose_bus_label;
    QLabel* choose_worker_label;
    QComboBox* buses_box;
    QComboBox* workers_box;
    QPushButton* confirm_button;
    std::map<std::string,std::string> buses_names;
    std::map<std::string,std::string> workers_names;

    void updateBuses();
    void updateWorkers();
    void activateBoxes();
    void deactivateBoxes();
    void setValues();
    void resetInfoLine();

};

#endif // SHIFTWIDGET_H
