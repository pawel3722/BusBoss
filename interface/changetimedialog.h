
#ifndef CHANGETIMEDIALOG_H
#define CHANGETIMEDIALOG_H

#include "data/data.h"
#include "qdatetimeedit.h"
#include "qdialog.h"
#include "qdialogbuttonbox.h"
#include "qgridlayout.h"
#include "qlabel.h"
#include "qspinbox.h"


class ChangeTimeDialog : public QDialog
{
public:
    ChangeTimeDialog(QDate date, QTime time, double speed, Data* data);
    QDate getDate() const {return date_edit->date();}
    QTime getTime() const {return QTime{new_hour->value(),new_min->value(),new_sec->value()};}
    double getSpeed() const {return new_speed->value();}
    int getDayType() const {return day_type;}
private:
    Data* data;
    QGridLayout* layout;
    int day_type;

    void dateChanged();
    QLabel* lab1;
    QLabel* lab2;
    QLabel* lab3;
    QLabel* lab4;
    QDateEdit* date_edit;
    QSpinBox* new_hour;
    QSpinBox* new_min;
    QSpinBox* new_sec;
    QDoubleSpinBox* new_speed;
    QDialogButtonBox* button_box;
};

#endif // CHANGETIMEDIALOG_H
