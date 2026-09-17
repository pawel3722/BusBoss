
#ifndef BUSWIDGET_H
#define BUSWIDGET_H


#include "data/data.h"
#include "qcheckbox.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "qspinbox.h"
#include <QWidget>
#include <QComboBox>


class BusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BusWidget(Data* data,QWidget *parent = nullptr);
    void updateBuses();

private:
    Data* data;
    QWidget* parent;
    QPushButton* ret_button;

    //add_section
    QLabel* add_msg_label;
    QLabel* add_info_label;
    QLabel* add_desc1_label;
    QLabel* add_desc2_label;
    QLabel* add_desc3_label;
    QLabel* add_desc4_label;
    QLabel* add_desc5_label;
    QSpinBox* company_number;
    QSpinBox* bus_number;
    QLineEdit* got_name;
    QComboBox* got_type;
    QCheckBox* add_multiple;
    QSpinBox* got_quantity;
    QPushButton* add_ok_button;

    //del_section
    QLabel* del_msg_label;
    QLabel* del_info_label;
    QLabel* del_desc_label;
    QComboBox* choose_bus;
    QPushButton* del_ok_button;
    std::map<std::string,std::string> bus_names;


    void addBus();
    void delBus();
    void resetAddInfoLine();
    void resetAddBoxes();
    void resetDelInfoLine();
    void resetDelBoxes();
    void enableQuantityLine() {add_multiple->isChecked() ? got_quantity->setEnabled(true) : got_quantity->setEnabled(false);}

};

#endif // BUSWIDGET_H
