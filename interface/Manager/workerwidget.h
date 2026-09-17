
#ifndef WORKERWIDGET_H
#define WORKERWIDGET_H


#include "data/data.h"
#include "qcombobox.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include <QWidget>


class WorkerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkerWidget(Data* data,QWidget *parent = nullptr);
    void updateWorkers();

private:
    Data* data;
    QWidget* parent;
    QPushButton* ret_button;

    //add_section
    QLabel* add_msg_label;
    QLabel* add_info_label;
    QLabel* add_desc1_label;
    QLabel* add_desc2_label;
    QLineEdit* got_number;
    QLineEdit* got_name;
    QPushButton* add_ok_button;

    //del_section
    QLabel* del_msg_label;
    QLabel* del_info_label;
    QLabel* del_desc_label;
    QComboBox* choose_worker;
    QPushButton* del_ok_button;
    std::map<std::string,std::string> worker_names;


    void addWorker();
    void delWorker();
    void resetAddInfoLine();
    void resetAddBoxes();
    void resetDelInfoLine();
    void resetDelBoxes();

};

#endif // WORKERWIDGET_H
