
#ifndef JOBWIDGET_H
#define JOBWIDGET_H


#include "data/data.h"
#include <QComboBox>
#include "qdatetimeedit.h"
#include "qgridlayout.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include <QScrollArea>
#include <QWidget>


class JobWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JobWidget(Data* data,QWidget *parent = nullptr);
    void updateJobs();


private:
    Data* data;
    QWidget* parent;
    QPushButton* ret_button;
    QGridLayout* layout;

    //choose section:
    QLabel* choose_label;
    QComboBox* got_job;
    QPushButton* confirm_job_but;
    QPushButton* add_job_but;
    Job* chosen_job = nullptr;
    QLabel* chosen_job_label;

    //job section:
    QLabel* info_msg_label;
    QPushButton* set_weekdays_but;
    QPushButton* add_timetable_but;
    QPushButton* del_timetable_but;
    QPushButton* set_def_freq_but;
    QPushButton* save_but;
    QPushButton* del_job_but;

    //display section:
    QWidget* scroll_widget;
    QGridLayout* scroll_layout;
    QScrollArea* scroll_area;
    std::vector<QLabel*> day_labels;
    std::vector<std::vector<QLabel*>> top_labels;
    std::vector<QPushButton*> add_service_buttons;
    std::vector<QPushButton*> del_service_buttons;
    std::vector<std::vector<QLabel*>> service_labels;
    std::vector<std::vector<QTimeEdit*>> dep_times_edits;
    std::vector<std::vector<QLabel*>> dep_stops_labels;
    std::vector<std::vector<QComboBox*>> routes_boxes;
    std::vector<std::vector<QComboBox*>> timescales_boxes;
    std::vector<std::vector<QLabel*>> arr_times_labels;
    std::vector<std::vector<QLabel*>> breaks_labels;
    std::vector<QString> chosen_job_routes;
    std::map<std::string,std::vector<QString>> mapped_times;
    std::vector<int> day_types;
    std::vector<QTime> default_frequencies;
    std::vector<std::vector<std::string>> default_drivers;
    std::vector<std::vector<std::string>> default_buses;
    int pixels;

    void setColumnHeader(int i);
    void setDisplay();
    void resetDisplay(int col = 0);
    void activateButtons();
    void deactivateButtons();
    void chooseJobButtonClicked();
    void addJobButtonClicked();
    std::string chooseDefaultDriver();
    std::string chooseDefaultBus();
    void setDayTypesButtonClicked();
    void setDefFrequenciesButtonClicked();
    void addTimetable();
    void delTimetable();
    void updateDayTypeDesriptions();
    void updateTimeLabels();
    void updateRouteLabels();
    void addService(int column = 0);
    void delService();
    void removeLine(int i, int j);
    void changeSize();
    void saveButtonClicked();
    void saveJob(std::vector<std::vector<QTime>> shift_times);
    void delButtonClicked();
    void delJob();
    Worker* createRandomWorker();
    Bus* createRandomBus();
};

#endif // JOBWIDGET_H
