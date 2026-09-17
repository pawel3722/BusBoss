
#ifndef LINEWIDGET_H
#define LINEWIDGET_H


#include "data/data.h"
#include "qcombobox.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qspinbox.h"
#include <QWidget>


class LineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LineWidget(Data* data,QWidget *parent = nullptr);


private:
    Data* data;
    QWidget* parent;
    QPushButton* ret_button;

    //add_section
    QLabel* add_msg_label;
    QLabel* add_info_label;
    QLabel* add_desc1_label;
    QLabel* add_desc2_label;
    QSpinBox* got_number;
    QLineEdit* got_name;
    QPushButton* add_ok_button;

    //del_section
    QLabel* del_msg_label;
    QLabel* del_info_label;
    QLabel* del_desc_label;
    QComboBox* choose_line;
    QPushButton* del_ok_button;
    std::map<std::string,std::string> line_names;

    void updateLines();
    void addLine();
    void delLine();
    void resetAddInfoLine();
    void resetAddBoxes();
    void resetDelInfoLine();
    void resetDelBoxes();
};

#endif // LINEWIDGET_H
