
#ifndef STOPWIDGET_H
#define STOPWIDGET_H


#include "data/data.h"
#include "qcombobox.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qspinbox.h"
#include <QWidget>


class StopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StopWidget(Data* data,QWidget *parent = nullptr);
    void updateStops();


private:
    Data* data;
    QWidget* parent;
    QPushButton* ret_button;

    //choose_section
    QLabel* choose_stop_label;
    QComboBox* got_stop;
    QPushButton* confirm_stop;
    BusStop* chosen_stop;
    QLabel* chosen_stop_label;

    //add_section
    QLabel* add_msg_label;
    QLabel* add_info_label;
    QLabel* add_desc_label;
    QSpinBox* got_number;
    QPushButton* add_ok_button;
    std::map<std::string,std::string> stop_names;

    //del_section
    QLabel* del_msg_label;
    QLabel* del_info_label;
    QLabel* del_desc_label;
    QComboBox* got_platform;
    QPushButton* del_ok_button;

    void updatePlatforms();
    void activateBoxes();
    void deactivateBoxes();
    void addPlatform();
    void delPlatform();
    void resetAddInfoLine();
    void resetAddBoxes();
    void resetDelInfoLine();

};

#endif // STOPWIDGET_H
