
#ifndef SETSHIFTSDIALOG_H
#define SETSHIFTSDIALOG_H

#include "qcheckbox.h"
#include "qdatetimeedit.h"
#include "qdialog.h"
#include "qgridlayout.h"
#include "qlabel.h"
#include "qpushbutton.h"

class SetShiftsDialog : public QDialog
{
public:
    SetShiftsDialog(std::vector<std::vector<QTime>> drive_times,std::vector<std::vector<QTime>> break_times, std::vector<QTime> dep_time);
    void setDefaultShifts(std::map<int,QTime> sh1, std::map<int,QTime> sh2);
    std::vector<std::vector<QTime>> getShiftTimes();
private:

    std::vector<std::vector<QTime>> drive_times;
    std::vector<std::vector<QTime>> break_times;
    std::vector<QTime> dep_time;
    QGridLayout* layout;
    QLabel* lab1;
    QLabel* lab2;
    QLabel* lab3;
    QLabel* lab4;
    QLabel* lab5;
    QLabel* lab6;
    QLabel* lab7;
    QLabel* lab8;
    QLabel* lab9;
    std::vector<QLabel*> time_labels;
    std::vector<std::vector<QCheckBox*>> check_boxes;
    std::vector<std::vector<QLabel*>> shift_name_labels;
    std::vector<QLabel*> dep_1_time_labels;
    std::vector<QTimeEdit*> dep_2_time_boxes;
    std::vector<QTimeEdit*> dep_3_time_boxes;
    std::vector<std::vector<QLabel*>> arr_times;
    std::vector<std::vector<QLabel*>> total_times;
    std::vector<std::vector<QLabel*>> drive_times_labels;
    std::vector<std::vector<QLabel*>> break_times_labels;
    std::vector<std::vector<QCheckBox*>> time_boxes;
    std::vector<std::vector<QCheckBox*>> break_boxes;
    std::vector<std::vector<QCheckBox*>> br_15_boxes;
    QPushButton* ok_button;
    QPushButton* cancel_button;

    std::vector<QTime> drive_time_sum;
    std::vector<QTime> break_time_sum;

    void changeCB3State();
    void changeCB2State();
    void changeD2TBState();
    void changeD3TBState();
    void checkShiftsLaw();
    void timeChanged();
    void checkCorrectness();
};

#endif // SETSHIFTSDIALOG_H
