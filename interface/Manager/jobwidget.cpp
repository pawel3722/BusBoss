
#include "jobwidget.h"
#include "interface/Manager/manager.h"
#include "interface/Manager/setshiftsdialog.h"
#include "qdialog.h"
#include "qdialogbuttonbox.h"
#include "qmessagebox.h"
#include <QProgressDialog>
#include <QProgressBar>
#include <random>

JobWidget::JobWidget(Data* data, QWidget *parent)
    : QWidget{parent}
{
    this->data = data;
    this->parent = parent;

    ret_button = new QPushButton("Powrót");
        connect(ret_button,&QPushButton::clicked,static_cast<Manager*>(parent),&Manager::showMenu);
        connect(ret_button,&QPushButton::clicked,this,&JobWidget::deactivateButtons);
    choose_label = new QLabel("Wybierz brygadę:");
    got_job = new QComboBox;
        connect(got_job,&QComboBox::currentIndexChanged,this,&JobWidget::deactivateButtons);
    confirm_job_but = new QPushButton("Wybierz");
        connect(confirm_job_but,&QPushButton::clicked,this,&JobWidget::chooseJobButtonClicked);
    chosen_job_label = new QLabel;
    add_job_but = new QPushButton("Dodaj brygadę");
        connect(add_job_but,&QPushButton::clicked,this,&JobWidget::addJobButtonClicked);
    info_msg_label = new QLabel;
    set_weekdays_but = new QPushButton("Ustaw dni kursowania");
        connect(set_weekdays_but,&QPushButton::clicked,this,&JobWidget::setDayTypesButtonClicked);
    add_timetable_but = new QPushButton("Dodaj rozkład");
        connect(add_timetable_but,&QPushButton::clicked,this,&JobWidget::addTimetable);
    del_timetable_but = new QPushButton("Usuń rozkład");
        connect(del_timetable_but,&QPushButton::clicked,this,&JobWidget::delTimetable);
    set_def_freq_but = new QPushButton("Ustaw domyślną częstotliwość");
        connect(set_def_freq_but,&QPushButton::clicked,this,&JobWidget::setDefFrequenciesButtonClicked);
    save_but = new QPushButton("Zapisz");
        connect(save_but,&QPushButton::clicked,this,&JobWidget::saveButtonClicked);
    del_job_but = new QPushButton("Usuń brygadę");
        connect(del_job_but,&QPushButton::clicked,this,&JobWidget::delButtonClicked);

    int row = 1;

    layout = new QGridLayout(this);
    layout->addWidget(ret_button,row++,1,1,1);
    layout->addWidget(choose_label,row++,1,1,2);
    layout->addWidget(got_job,row++,1,1,2);
    layout->addWidget(confirm_job_but,row++,1,1,2);
    layout->addWidget(add_job_but,row++,1,1,2);
    layout->addWidget(chosen_job_label,row++,1,1,2);
    layout->addWidget(info_msg_label,row++,1,1,2);
    layout->addWidget(set_weekdays_but,row++,1,1,2);
    layout->addWidget(add_timetable_but,row++,1,1,2);
    layout->addWidget(del_timetable_but,row++,1,1,2);
    layout->addWidget(set_def_freq_but,row++,1,1,2);
    layout->addWidget(save_but,row++,1,1,2);
    layout->addWidget(del_job_but,row++,1,1,2);

    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,4);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding),row,4);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred),row,4);
    layout->setColumnMinimumWidth(3,1500);

    this->setLayout(layout);
    deactivateButtons();
}

void JobWidget::updateJobs()
{
    got_job->clear();
    for(const auto& el : *data->getJobs())
        got_job->addItem(QString::fromStdString(el.second->getID()));
}

void JobWidget::setColumnHeader(int i)
{
    std::vector<QLabel*> local_service_labels;
    std::vector<QTimeEdit*> local_dep_times_edits;
    std::vector<QLabel*> local_dep_stops_labels;
    std::vector<QComboBox*> local_routes_boxes;
    std::vector<QComboBox*> local_timescales_boxes;
    std::vector<QLabel*> local_arr_times_labels;
    std::vector<QLabel*> local_breaks_labels;
    service_labels.push_back(local_service_labels);
    dep_times_edits.push_back(local_dep_times_edits);
    dep_stops_labels.push_back(local_dep_stops_labels);
    routes_boxes.push_back(local_routes_boxes);
    timescales_boxes.push_back(local_timescales_boxes);
    arr_times_labels.push_back(local_arr_times_labels);
    breaks_labels.push_back(local_breaks_labels);
    std::vector<QLabel*> local_top_labels;
    local_top_labels.push_back(new QLabel("Kurs:"));
    local_top_labels.push_back(new QLabel("Odjazd:"));
    local_top_labels.push_back(new QLabel("Początek:"));
    local_top_labels.push_back(new QLabel("Trasa:"));
    local_top_labels.push_back(new QLabel("Czas:"));
    local_top_labels.push_back(new QLabel("Przyjazd:"));
    local_top_labels.push_back(new QLabel("Przerwa:"));
    top_labels.push_back(local_top_labels);

    day_labels.push_back(new QLabel ("Kursuje w: "));
    add_service_buttons.push_back(new QPushButton("Dodaj kurs"));
        connect(add_service_buttons[i],&QPushButton::clicked,this,&JobWidget::addService);
    del_service_buttons.push_back(new QPushButton("Usuń kurs"));
        connect(del_service_buttons[i],&QPushButton::clicked,this,&JobWidget::delService);

    auto size = (int) top_labels[i].size();

    scroll_layout->addWidget(day_labels[i],1,size * i + 1,1,5);
    scroll_layout->addWidget(add_service_buttons[i],1,size * i + 6,1,1);
    scroll_layout->addWidget(del_service_buttons[i],1,size * i + 7,1,1);

    for(int j = 0; j < size; ++j)
    {
        scroll_layout->addWidget(top_labels[i][j],2,size * i + j + 1);
    }
}

void JobWidget::setDisplay()
{
    scroll_layout = new QGridLayout;
    scroll_widget = new QWidget;
    scroll_area = new QScrollArea;
    auto services = *chosen_job->getServices();
    int counter = 1;
    int current_step = 0;
    QProgressDialog progress_dialog ("Ładowanie...","Anuluj",0,counter,this);
    progress_dialog.setMinimumDuration(200);
    progress_dialog.setWindowModality(Qt::WindowModal);
    for(int i = 0; i < (int) services.size(); i++)
    {
        setColumnHeader(i);
        if(services[i].size()==1)
            del_service_buttons[i]->setEnabled(false);
        for(int j = 0; j < (int)services[i].size(); j++)
        {
            progress_dialog.setValue(current_step);
            current_step++;
            auto ser_lab = new QLabel (QString::number(j+1));
            service_labels[i].push_back(ser_lab);
            scroll_layout->addWidget(ser_lab,j+3,7*i+1);

            auto dep_time = services[i][j]->getDepartureTime();
            auto dep_edit = new QTimeEdit;
            dep_edit->setTime(dep_time);
            dep_edit->setDisplayFormat("hh:mm:ss");
            scroll_layout->addWidget(dep_edit,j+3,7*i+2);
            dep_times_edits[i].push_back(dep_edit);
                connect(dep_edit,&QDateTimeEdit::timeChanged,this,&JobWidget::updateTimeLabels);

            if(j > 0)
            {
                auto t2 = QTime::fromString(arr_times_labels[i][j-1]->text(),"hh:mm:ss");
                breaks_labels[i][j-1]->setText(Connection::subTime(dep_edit->time(),t2).toString("hh:mm:ss"));
            }

            auto dep_stop = new QLabel;
            scroll_layout->addWidget(dep_stop,j+3,7*i+3);
            dep_stops_labels[i].push_back(dep_stop);

            auto rt_box = new QComboBox;
            scroll_layout->addWidget(rt_box,j+3,7*i+4);
            routes_boxes[i].push_back(rt_box);
            int index = 0;
            for(const auto& el : chosen_job_routes)
            {
                rt_box->addItem(el);
                if(el.toStdString().substr(0,8) == services[i][j]->getRouteID())
                {
                    rt_box->setCurrentIndex(index);
                    dep_stop->setText(QString::fromStdString(services[i][j]->getRoute()->getFirstStop()->getName()));
                }
                ++index;
            }
            connect(rt_box,&QComboBox::currentIndexChanged,this,&JobWidget::updateRouteLabels);

            auto ts_box = new QComboBox;
            scroll_layout->addWidget(ts_box,j+3,7*i+5);
            timescales_boxes[i].push_back(ts_box);
            auto rt_ID = rt_box->currentText().toStdString().substr(0,8);
            for(const auto& el : mapped_times.at(rt_ID))
            {
                ts_box->addItem(el);
            }
            ts_box->setCurrentIndex(services[i][j]->getTimescale());
                connect(ts_box,&QComboBox::currentIndexChanged,this,&JobWidget::updateTimeLabels);
            QTime arr_time = Connection::addTime(dep_edit->time(),QTime::fromString(ts_box->currentText(),"hh:mm:ss"));
            auto arr_lab = new QLabel(arr_time.toString());
            scroll_layout->addWidget(arr_lab,j+3,7*i+6);
            arr_times_labels[i].push_back(arr_lab);

            auto br_lab = new QLabel;
            breaks_labels[i].push_back(br_lab);
            scroll_layout->addWidget(br_lab,j+3,7*i+7);
        }
        if(progress_dialog.wasCanceled())
        {
            resetDisplay();
            break;
        }
        dep_times_edits[i].size() == 1 ? del_service_buttons[i]->setEnabled(false) : del_service_buttons[i]->setEnabled(true);
    }
    updateDayTypeDesriptions();

    scroll_widget->setLayout(scroll_layout);
    scroll_area->setWidget(scroll_widget);
    layout->addWidget(scroll_area,1,3,20,1);
    if(services.size())
        pixels = scroll_widget->width()/(int) services.size();
    progress_dialog.setValue(counter);

}

void JobWidget::resetDisplay(int col)
{
    for(int i = (int) dep_times_edits.size()-1; i >= col; i--)
    {
        scroll_layout->removeItem(scroll_layout->itemAtPosition(1,7*i+1));
        delete day_labels[i];
        day_labels.pop_back();
        scroll_layout->removeItem(scroll_layout->itemAtPosition(1,7*i+6));
        delete add_service_buttons[i];
        add_service_buttons.pop_back();
        scroll_layout->removeItem(scroll_layout->itemAtPosition(1,7*i+7));
        delete del_service_buttons[i];
        del_service_buttons.pop_back();

        for(int j = 0; j < (int)top_labels[i].size(); ++j)
        {
            scroll_layout->removeItem(scroll_layout->itemAtPosition(2,7*i+j+2));
            delete top_labels[i][j];
        }
        top_labels.pop_back();

        auto d_size = (int)dep_times_edits[i].size();
        for(int j = d_size-1; j >= 0; j--)
        {
            removeLine(i,j);
        }
        service_labels.pop_back();
        dep_times_edits.pop_back();
        dep_stops_labels.pop_back();
        routes_boxes.pop_back();
        timescales_boxes.pop_back();
        arr_times_labels.pop_back();
        breaks_labels.pop_back();
    }
}

void JobWidget::activateButtons()
{
    auto size = dep_times_edits.size();
    set_weekdays_but->setEnabled(true);
    size < 5 ? add_timetable_but->setEnabled(true) : add_timetable_but->setEnabled(false);
    size > 1 ? del_timetable_but->setEnabled(true) : del_timetable_but->setEnabled(false);
    set_def_freq_but->setEnabled(true);
    save_but->setEnabled(true);
    del_job_but->setEnabled(true);
    info_msg_label->setText("");
}

void JobWidget::deactivateButtons()
{
    set_weekdays_but->setEnabled(false);
    add_timetable_but->setEnabled(false);
    del_timetable_but->setEnabled(false);
    set_def_freq_but->setEnabled(false);
    save_but->setEnabled(false);
    del_job_but->setEnabled(false);
    chosen_job_label->setText("Wybrana brygada:");
    if(chosen_job != nullptr)
        resetDisplay();
    chosen_job = nullptr;
    default_buses.clear();
    default_drivers.clear();
}

void JobWidget::chooseJobButtonClicked()
{
    auto copy = chosen_job;
    auto text = got_job->currentText();
    if(text.size())
        chosen_job = data->getJobs()->at(text.toStdString()).get();
    if(copy != chosen_job)
    {
        chosen_job_routes.clear();
        mapped_times.clear();
        chosen_job_label->setText("Wybrana brygada: " + text);
        day_types = chosen_job->getDayTypes();
        default_frequencies = chosen_job->getDefaultFrequencies();
        auto line = chosen_job->getLine();
        auto lines = *data->getLines();
        for(const auto& el : lines)
            if(el.second->getName()==line)
            {
                for(const auto& pos : (el.second->getRoutes()))
                {
                    chosen_job_routes.push_back(QString::fromStdString(pos + " " + data->getRoutes()->at(pos)->getDirection()));
                    std::vector<QString> vs;
                    for(const auto& ts : *data->getRoutes()->at(pos)->getTimescalesLengths())
                    {
                        vs.push_back(ts.toString("hh:mm:ss"));
                    }
                    mapped_times[pos] = vs;
                }
                break;
            }  
        setDisplay();
        activateButtons();
    }
}

void JobWidget::addJobButtonClicked()
{
    auto def_driver = chooseDefaultDriver();
    auto def_bus = chooseDefaultBus();
    auto jobs = data->getJobs();
    if(data->getLines()->empty())
    {
        QMessageBox msg_box(QMessageBox::Warning,"Brak linii","Stwórz linię, aby dodać brygadę!",QMessageBox::Ok);
        msg_box.exec();
        return;
    }
    QString failure;
    if(def_driver.size()==0)
        failure+="kierowców, ";
    if(def_bus.size()==0)
        failure+="autobusów, ";
    if(failure.size())
    {
        QMessageBox msg_box(QMessageBox::Warning,"Niewystarczająca liczba","Niewystarczająca liczba "+failure+"aby dodać brygadę!",QMessageBox::Ok);
        msg_box.exec();
    }
    else
    {
        QDialog dial;
        QVBoxLayout layout;
        QLabel lab1("Wybierz linię:");
        QComboBox line_box;
        for(const auto& el : *data->getLines())
        {
            auto name = el.second->getName();
            if(el.second->getRoutes().size())
                line_box.addItem(QString::fromStdString(name));
        }
        if(!line_box.count())
        {
            QMessageBox msg_box(QMessageBox::Warning,"Brak tras","Stwórz trasę, aby dodać brygadę!",QMessageBox::Ok);
            msg_box.exec();
            return;
        }
        QLabel lab2("Podaj numer brygady:");
        QSpinBox job_num; job_num.setMinimum(1); job_num.setMaximum(19);
        QDialogButtonBox button_box (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        connect(&button_box, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
        connect(&button_box, &QDialogButtonBox::rejected, &dial, &QDialog::reject);
        layout.addWidget(&lab1);
        layout.addWidget(&line_box);
        layout.addWidget(&lab2);
        layout.addWidget(&job_num);
        layout.addWidget(&button_box);
        dial.setLayout(&layout);
        dial.setWindowTitle("Dodaj brygadę");
        std::string new_job_name;
        do
        {
            dial.exec();
            new_job_name = line_box.currentText().toStdString();
            auto val = job_num.value();
            val < 10 ? new_job_name += "/0" + std::to_string(val) : new_job_name += "/" + std::to_string(val);
            lab1.setText("<b>Brygada o tym numerze istnieje!</b></p><p>Wybierz linię");
        }while(dial.result() && jobs->find(new_job_name) != jobs->end());

        if(dial.result())
        {
            auto new_shift_name = new_job_name+"/A";
            QTime s {0,0};
            QTime f {23,59};
            auto new_shift = std::make_shared<Shift>(new_shift_name,data->getBuses()->at(def_bus).get(),data->getWorkers()->at(def_driver).get(),s,f);
            (*data->getShifts())[new_shift_name] = new_shift;
            std::vector<std::vector<Shift*>> new_shifts{1};
            new_shifts[0].push_back(new_shift.get());
            std::string line_ID;
            for(const auto& el : *data->getLines())
                if(el.second->getName() == line_box.currentText().toStdString())
                {
                    line_ID = el.second->getID();
                    break;
                }
            auto route_ID = data->getLines()->at(line_ID)->getRoutes()[0];
            std::string tail = {new_job_name[new_job_name.size()-2],new_job_name[new_job_name.size()-1]};
            auto service_ID = route_ID+"/"+tail+"/"+"01";
            std::vector<std::vector<Service*>> new_services;
            auto new_service = std::make_shared<Service>(service_ID,data->getRoutes()->at(route_ID).get(),QTime{0,0},0);
            (*data->getServices())[service_ID] = new_service;
            new_services.push_back({std::vector<Service*>{new_service.get()}});
            std::vector<QTime> d_f = std::vector<QTime>{QTime{1,0}};
            std::vector<int> d_t {-1,-1,-1,-1,-1};
            auto new_job = std::make_shared<Job>(new_job_name,new_shifts,d_f,d_t,new_services);
            (*data->getJobs())[new_job_name] = new_job;
            updateJobs();
        }
    }
}

std::string JobWidget::chooseDefaultDriver()
{
    std::string res;
    std::list<std::string> bad_workers;
    auto shifts = *data->getShifts();
    auto workers = *data->getWorkers();
    if(workers.size()>shifts.size())
    {
        for(const auto& el : shifts)
            bad_workers.push_back(el.second->getWorkerID());
        for(const auto& el : workers)
            if(std::find(bad_workers.begin(),bad_workers.end(),el.second->getID())==bad_workers.end())
            {
                res = el.second->getID();
                break;
            }
    }
    return res;
}

std::string JobWidget::chooseDefaultBus()
{
    std::string res;
    std::list<std::string> bad_buses;
    auto shifts = *data->getShifts();
    auto buses = *data->getBuses();
    for(const auto& el : shifts)
        bad_buses.push_back(el.second->getBusID());
    for(const auto& el : buses)
        if(std::find(bad_buses.begin(),bad_buses.end(),el.second->getID())==bad_buses.end())
        {
            res = el.second->getID();
            break;
        }
    return res;
}

void JobWidget::setDayTypesButtonClicked()
{
    QDialog dial;
    QGridLayout layout;
    QLabel lab1("Typ dnia:");
    QLabel lab2("Rozkład:");
    QLabel labR("Dni robocze szkolne:");
    QLabel labW("Dni robocze nieszkolne:");
    QLabel labS("Soboty:");
    QLabel labN("Niedziele:");
    QLabel labX("Święta Specjalne:");
    std::vector<QComboBox> boxes (5);
    for(int i = 0; i < 5; ++i)
    {
        boxes[i].addItem("Nie kursuje");
        for(int j = 0; j < dep_times_edits.size(); ++j)
            boxes[i].addItem(QString::number(j));
        boxes[i].setCurrentIndex(day_types[i]+1);
    }

    QDialogButtonBox button_box (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(&button_box, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
    connect(&button_box, &QDialogButtonBox::rejected, &dial, &QDialog::reject);
    layout.addWidget(&lab1,0,0);
    layout.addWidget(&lab2,0,1);
    layout.addWidget(&labR,1,0);
    layout.addWidget(&boxes[0],1,1);
    layout.addWidget(&labW,2,0);
    layout.addWidget(&boxes[1],2,1);
    layout.addWidget(&labS,3,0);
    layout.addWidget(&boxes[2],3,1);
    layout.addWidget(&labN,4,0);
    layout.addWidget(&boxes[3],4,1);
    layout.addWidget(&labX,5,0);
    layout.addWidget(&boxes[4],5,1);
    layout.addWidget(&button_box,6,0,1,2);
    dial.setLayout(&layout);
    dial.setWindowTitle("Dni kursowania");
    dial.exec();
    if(dial.result())
    {
        for(int i = 0; i < 5; ++i)
        {
            day_types[i]=boxes[i].currentIndex()-1;
        }
    }
    updateDayTypeDesriptions();
}

void JobWidget::setDefFrequenciesButtonClicked()
{
    QDialog dial;
    QGridLayout layout;
    QLabel lab1 ("Rozkład:");
    QLabel lab2 ("Częstotliwość:");
    layout.addWidget(&lab1,0,0);
    layout.addWidget(&lab2,0,1,1,3);

    std::vector<QSpinBox*> hrs;
    std::vector<QSpinBox*> mins;
    std::vector<QSpinBox*> secs;

    int size = (int)default_frequencies.size();
    for(int i = 0; i < size; ++i)
    {
        auto nlab = new QLabel(QString::number(i));
        layout.addWidget(nlab,i+1,0);
        auto hr = new QSpinBox;
        hr->setMaximum(23);
        hr->setSuffix(" h");
        hr->setValue(default_frequencies[i].hour());
        layout.addWidget(hr,i+1,1);
        hrs.push_back(hr);
        auto min = new QSpinBox;
        min->setMaximum(59);
        min->setSuffix(" m");
        min->setValue(default_frequencies[i].minute());
        layout.addWidget(min,i+1,2);
        mins.push_back(min);
        auto sec = new QSpinBox;
        sec->setMaximum(59);
        sec->setSuffix(" s");
        sec->setValue(default_frequencies[i].second());
        layout.addWidget(sec,i+1,3);
        secs.push_back(sec);
    }
    QDialogButtonBox button_box (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(&button_box, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
    connect(&button_box, &QDialogButtonBox::rejected, &dial, &QDialog::reject);
    layout.addWidget(&button_box,size+1,0,1,4);
    dial.setLayout(&layout);
    dial.setWindowTitle("Częstotliwość kursowania");
    dial.exec();
    if(dial.result())
    {
        for(int i = 0; i < size; ++i)
        {
            default_frequencies[i] = QTime(hrs[i]->value(),mins[i]->value(),secs[i]->value());
            delete hrs[i];
            delete mins[i];
            delete secs[i];
        }
    }
}

void JobWidget::addTimetable()
{
    auto size = (int) top_labels.size();
    default_frequencies.push_back({1,0});
    setColumnHeader(size);   
    del_timetable_but->setEnabled(true);
    if(size == 4)
        add_timetable_but->setEnabled(false);
    addService(size);
}

void JobWidget::delTimetable()
{
    auto size = (int) top_labels.size();
    default_frequencies.pop_back();
    resetDisplay(size-1);
    add_timetable_but->setEnabled(true);
    if(size == 2)
        del_timetable_but->setEnabled(false);
    for(auto& el : day_types)
        if(el == size - 1)
            el = -1;
    changeSize();
}

void JobWidget::updateDayTypeDesriptions()
{
    QString info;
    for(auto& el : day_labels)
        el->setText("Kursuje w: ");
    for(int i = 0; i < 5; ++i)
    {
        switch(i)
        {
        case 0:
            info = "Dni robocze szkolne, ";
            break;
        case 1:
            info = "Dni robocze nieszkolne, ";
            break;
        case 2:
            info = "Soboty, ";
            break;
        case 3:
            info = "Niedziele, ";
            break;
        case 4:
            info = "Święta specjalne, ";
            break;
        }
        if(day_types[i]>=0)
            day_labels[day_types[i]]->setText(day_labels[day_types[i]]->text()+info);
    }
}

void JobWidget::updateTimeLabels()
{   auto sen = static_cast<QWidget*>(sender());
    int i = 0;
    int j = 0;
    bool found = false;
    for(int a = 0; a < (int) dep_times_edits.size() and !found; ++a)
        for(int b = 0; b < (int) dep_times_edits[a].size() and !found;++b)
        {
            if(dep_times_edits[a][b]==sen or timescales_boxes[a][b]==sen)
            {
                i = a;
                j = b;
                found = true;
            }
        }
    if(j>0)
    {
        auto t2 = QTime::fromString(arr_times_labels[i][j-1]->text(),"hh:mm:ss");
        breaks_labels[i][j-1]->setText(Connection::subTime(dep_times_edits[i][j]->time(),t2).toString("hh:mm:ss"));
    }
    arr_times_labels[i][j]->setText(Connection::addTime(dep_times_edits[i][j]->time(),QTime::fromString(timescales_boxes[i][j]->currentText(),"hh:mm:ss")).toString("hh:mm:ss"));
    if(j<dep_times_edits[i].size()-1)
    {
        auto t2 = QTime::fromString(arr_times_labels[i][j]->text(),"hh:mm:ss");
        breaks_labels[i][j]->setText(Connection::subTime(dep_times_edits[i][j+1]->time(),t2).toString("hh:mm:ss"));
    }
}

void JobWidget::updateRouteLabels()
{
    auto rt_box = static_cast<QComboBox*>(sender());
    int i = 0;
    int j = 0;
    bool found = false;
    for(int a = 0; a < (int) dep_times_edits.size() and !found; ++a)
        for(int b = 0; b < (int) dep_times_edits[a].size() and !found;++b)
        {
            if(routes_boxes[a][b]==rt_box)
            {
                i = a;
                j = b;
                found = true;
            }
        }
    auto ts_box = timescales_boxes[i][j];
    dep_stops_labels[i][j]->setText(QString::fromStdString(data->getRoutes()->at(rt_box->currentText().toStdString().substr(0,8))->getFirstStop()->getName()));
    ts_box->clear();
    auto rt_ID = rt_box->currentText().toStdString().substr(0,8);
    for(const auto& el : mapped_times.at(rt_ID))
    {
        ts_box->addItem(el);
    }
}

void JobWidget::addService(int column)
{
    int i = column;
    if(column == 0)
    {
        auto sen = static_cast<QPushButton*>(sender());
        for(int x = 0; x < add_service_buttons.size(); ++x)
            if(add_service_buttons[x]==sen)
            {
                i = x;
                break;
            }
        if(dep_times_edits[i].size() == 98)
            sen->setEnabled(false);
    }
    int j = (int) dep_times_edits[i].size();

    auto ser_lab = new QLabel (QString::number(j+1));
    service_labels[i].push_back(ser_lab);
    scroll_layout->addWidget(ser_lab,j+3,7*i+1);

    QTime dep_time;
    j < 2 ? ( j == 0 ? dep_time = QTime{0,0} : dep_time = QTime::fromString(arr_times_labels[i][0]->text(),"hh:mm:ss")) : dep_time = Connection::addTime(dep_times_edits[i][j-2]->time(),default_frequencies[i]);
    auto dep_edit = new QTimeEdit;
    dep_edit->setTime(dep_time);
    dep_edit->setDisplayFormat("hh:mm:ss");
    scroll_layout->addWidget(dep_edit,j+3,7*i+2);
    dep_times_edits[i].push_back(dep_edit);
    connect(dep_edit,&QDateTimeEdit::timeChanged,this,&JobWidget::updateTimeLabels);

    if(j > 0)
    {
        auto t2 = QTime::fromString(arr_times_labels[i][j-1]->text(),"hh:mm:ss");
        breaks_labels[i][j-1]->setText(Connection::subTime(dep_edit->time(),t2).toString("hh:mm:ss"));
    }

    auto dep_stop = new QLabel;
    scroll_layout->addWidget(dep_stop,j+3,7*i+3);
    dep_stops_labels[i].push_back(dep_stop);

    auto rt_box = new QComboBox();

    scroll_layout->addWidget(rt_box,j+3,7*i+4);
    routes_boxes[i].push_back(rt_box);
    int index = 0;
    QString new_ID;
    if(j > 0)
    {
        auto el = routes_boxes[i][j-1]->currentText();
        int num = el.sliced(6,2).toInt();
        if(num % 2)
            ++num < 10 ? new_ID = el.sliced(0,6) + "0" + QString::number(num) : new_ID = el.sliced(0,6)  + QString::number(num);
        else
            --num < 10 ? new_ID = el.sliced(0,6) + "0" + QString::number(num) : new_ID = el.sliced(0,6)  + QString::number(num);
    }
    else
        new_ID = chosen_job_routes.begin()->sliced(0,8);
    for(const auto& el : chosen_job_routes)
    {
        rt_box->addItem(el);
        if(el.sliced(0,8) == new_ID)
        {
            rt_box->setCurrentIndex(index);
            if(data->getRoutes()->find(new_ID.toStdString())!=data->getRoutes()->end())
                dep_stop->setText(QString::fromStdString(data->getRoutes()->at(new_ID.toStdString())->getFirstStop()->getName()));
        }
        ++index;
    }

    auto ts_box = new QComboBox;
    scroll_layout->addWidget(ts_box,j+3,7*i+5);
    timescales_boxes[i].push_back(ts_box);
    auto rt_ID = rt_box->currentText().toStdString().substr(0,8);
    for(const auto& el : mapped_times.at(rt_ID))
    {
        ts_box->addItem(el);
    }
    auto current_index = 0;
    if(j>=2)
        current_index = timescales_boxes[i][j-2]->currentIndex();
    else
        current_index = timescales_boxes[i][0]->currentIndex();
    j < 2 && current_index <= ts_box->count() ? ts_box->setCurrentIndex(0) : ts_box->setCurrentIndex(current_index);

    QTime arr_time = Connection::addTime(dep_edit->time(),QTime::fromString(ts_box->currentText(),"hh:mm:ss"));
    auto arr_lab = new QLabel(arr_time.toString());
    scroll_layout->addWidget(arr_lab,j+3,7*i+6);
    arr_times_labels[i].push_back(arr_lab);

    auto br_lab = new QLabel;
    breaks_labels[i].push_back(br_lab);
    scroll_layout->addWidget(br_lab,j+3,7*i+7);
    del_service_buttons[i]->setEnabled(true);   
    changeSize();
    connect(rt_box,&QComboBox::currentIndexChanged,this,&JobWidget::updateRouteLabels);
    connect(ts_box,&QComboBox::currentIndexChanged,this,&JobWidget::updateTimeLabels);
}

void JobWidget::delService()
{
    auto sen = static_cast<QPushButton*>(sender());
    int ttbl = 0;
    for(int i = 0; i < del_service_buttons.size(); ++i)
        if(del_service_buttons[i]==sen)
        {
            ttbl = i;
            break;
        }
    removeLine(ttbl,(int) dep_times_edits[ttbl].size()-1);
    if(dep_times_edits[ttbl].size() == 1)
        sen->setEnabled(false);
    changeSize();
}

void JobWidget::removeLine(int i, int j)
{
    scroll_layout->removeItem(scroll_layout->itemAtPosition(j+3,7*i+2));
    delete service_labels[i][j];
    service_labels[i].pop_back();

    scroll_layout->removeItem(scroll_layout->itemAtPosition(j+3,7*i+3));
    delete dep_times_edits[i][j];
    dep_times_edits[i].pop_back();

    scroll_layout->removeItem(scroll_layout->itemAtPosition(j+3,7*i+4));
    delete dep_stops_labels[i][j];
    dep_stops_labels[i].pop_back();

    scroll_layout->removeItem(scroll_layout->itemAtPosition(j+3,7*i+5));
    delete routes_boxes[i][j];
    routes_boxes[i].pop_back();

    scroll_layout->removeItem(scroll_layout->itemAtPosition(j+3,7*i+6));
    delete timescales_boxes[i][j];
    timescales_boxes[i].pop_back();

    scroll_layout->removeItem(scroll_layout->itemAtPosition(j+3,7*i+7));
    delete arr_times_labels[i][j];
    arr_times_labels[i].pop_back();

    scroll_layout->removeItem(scroll_layout->itemAtPosition(j+3,7*i+8));
    delete breaks_labels[i][j];
    breaks_labels[i].pop_back();
    if(j>0)
        breaks_labels[i][j-1]->setText("");
}

void JobWidget::changeSize()
{
    auto size = (int) dep_times_edits.size();
    int max = 0;
    for(const auto& el : dep_times_edits)
        if(el.size()>max)
            max = (int) el.size();
    scroll_widget->resize(size*pixels,(max+2)*28);
    scroll_widget->update();
}

void JobWidget::saveButtonClicked()
{
    std::vector<QTime> sum;
    std::vector<std::vector<QTime>> drive_times, break_times;
    for(int i = 0; i < timescales_boxes.size(); ++i)
    {
        sum.push_back({0,0});
        std::vector<QTime> vec;
        for(const auto& el : timescales_boxes[i])
        {
            auto time = QTime::fromString(el->currentText(),"hh:mm:ss");
            vec.push_back(time);
            sum[i] = Connection::addTime(sum[i],time,false);
        }
        drive_times.push_back(vec);
    }
    for(int i = 0; i < breaks_labels.size(); ++i)
    {
        std::vector<QTime> vec;
        int j = 0;
        for( ; j <  ((int) breaks_labels[i].size())-1; j++)
        {
            auto time = QTime::fromString(breaks_labels[i][j]->text(),"hh:mm:ss");
            vec.push_back(time);
            sum[i] = Connection::addTime(sum[i],time,false);
        }
        break_times.push_back(vec);
    }
    bool sums_valid = true;
    for(const auto& el: sum)
        if(!el.isValid())
        {
            sums_valid = false;
            break;
        }
    if(sums_valid)
    {
        std::vector<QTime> dep_times;
        for(const auto& el : dep_times_edits)
            if(el.size())
                dep_times.push_back(el[0]->time());
        if(dep_times.size())
        {
            SetShiftsDialog dial(drive_times,break_times,dep_times);
            auto shifts = *chosen_job->getShifts();
            std::map<int,QTime> sh1,sh2;
            for(int i = 0; i < std::min(dep_times_edits.size(),shifts.size()); ++i)
            {
                std::vector<std::string> local_def_drivers;
                std::vector<std::string> local_def_buses;
                local_def_buses.push_back(shifts[i][0]->getBusID());
                local_def_drivers.push_back(shifts[i][0]->getWorkerID());
                if(shifts[i].size()>1)
                {
                    local_def_buses.push_back(shifts[i][1]->getBusID());
                    local_def_drivers.push_back(shifts[i][1]->getWorkerID());
                    sh1[i]=shifts[i][1]->getStartTime();
                    if(shifts[i].size()>2)
                    {
                        local_def_buses.push_back(shifts[i][2]->getBusID());
                        local_def_drivers.push_back(shifts[i][2]->getWorkerID());
                        sh2[i]=shifts[i][2]->getStartTime();
                    }
                }
                default_buses.push_back(local_def_buses);
                default_drivers.push_back(local_def_drivers);
            }
            dial.setDefaultShifts(sh1,sh2);
            dial.exec();
            if(dial.result())
            {
                saveJob(dial.getShiftTimes());
            }
        }
    }
    else
    {
        QMessageBox msg_box(QMessageBox::Warning,"Zbyt długa brygada", "Czas trwania brygady nie może przekroczyć doby!",QMessageBox::Ok);
        msg_box.exec();
    }

}

void JobWidget::saveJob(std::vector<std::vector<QTime> > shift_times)
{
    delJob();
    std::vector<std::vector<Shift*>> new_shifts;
    std::vector<std::vector<Service*>> new_services;
    auto new_job_ID = got_job->currentText().toStdString();

    for(int i = 0; i < shift_times.size(); ++i)
    {
        auto next_dep_time = dep_times_edits[i][0]->time();
        QTime end_time;
        Worker* worker;
        Bus* bus;
        std::vector<Shift*> local_new_shifts;
        for(int j = 0; j < shift_times[i].size()+1;++j)
        {
            if(j < shift_times[i].size())
                end_time = shift_times[i][j];
            else
                end_time = QTime::fromString(arr_times_labels[i][arr_times_labels[i].size()-1]->text(),"hh:mm:ss");
            if(default_buses.size() > i && default_buses[i].size() > j)
            {
                bus = data->getBuses()->at(default_buses[i][j]).get();
                worker = data->getWorkers()->at(default_drivers[i][j]).get();
            }
            else
            {
                bus = createRandomBus();
                worker = createRandomWorker();
            }
            std::string letter = "/";
            std::string new_shift_ID = new_job_ID;
            letter.push_back( 65 + j );
            new_shift_ID[new_shift_ID.size()-2] += 2 * i;
            new_shift_ID += letter;
            auto ns = std::make_shared<Shift>(new_shift_ID,bus,worker,next_dep_time,end_time);
            (*data->getShifts())[new_shift_ID] = ns;
            local_new_shifts.push_back(ns.get());
            next_dep_time = end_time;
        }
        new_shifts.push_back(local_new_shifts);
    }
    for(int i = 0; i < dep_times_edits.size(); ++i)
    {
        std::vector<Service*> local_new_services;
        for(int j = 0; j < dep_times_edits[i].size(); ++j)
        {
            int pos = (int) new_job_ID.find("/");
            std::string route_ID = routes_boxes[i][j]->currentText().toStdString().substr(0,8);
            std::string ID_3 = new_job_ID.substr(++pos,2)+"/";
            ID_3[0] += 2 * i;
            std::string ID_4 = service_labels[i][j]->text().toStdString();
            if(ID_4.size()==1)
                ID_4.insert(0,"0");
            std::string final_ID = route_ID+"/"+ID_3+ID_4;
            auto dep_time = dep_times_edits[i][j]->time();
            int timescale = timescales_boxes[i][j]->currentIndex();
            auto ns = std::make_shared<Service>(final_ID,data->getRoutes()->at(route_ID).get(),dep_time,timescale);
            (*data->getServices())[final_ID] = ns;
            local_new_services.push_back(ns.get());
        }
        new_services.push_back(local_new_services);
    }
    auto nj = std::make_shared<Job>(new_job_ID,new_shifts,default_frequencies,day_types,new_services);
    (*data->getJobs())[new_job_ID] = nj;
    info_msg_label->setText("Zapisano!");
    deactivateButtons();
}

void JobWidget::delButtonClicked()
{
    QMessageBox msg_box (QMessageBox::Question,"Usuwanie brygady","Czy na pewno chcesz usunąć brygadę?",QMessageBox::Ok | QMessageBox::Cancel);
    int result = msg_box.exec();
    if(result == QMessageBox::Ok)
    {
        delJob();
        deactivateButtons();
        updateJobs();
    }
}

void JobWidget::delJob()
{
    auto services = *chosen_job->getServices();
    auto shifts = *chosen_job->getShifts();
    std::vector<std::string> services_IDs;
    std::vector<std::string> shifts_IDs;
    for(const auto& el : services)
        for(const auto ser : el)
            services_IDs.push_back(ser->getID());
    for(const auto& el : shifts)
        for(const auto sh : el)
            shifts_IDs.push_back(sh->getID());
    for(const auto& el : services_IDs)
        data->getServices()->erase(el);
    for(const auto& el : shifts_IDs)
        data->getShifts()->erase(el);
    data->getJobs()->erase(chosen_job->getID());
}

Worker *JobWidget::createRandomWorker()
{
    auto workers = *data->getWorkers();
    std::string letters = "0123456789";
    std::random_device rd;
    std::mt19937 g(rd());
    std::string ID;
    do
    {
        std::shuffle(letters.begin(),letters.end(),g);
        ID = letters.substr(0,3) + "/" + letters.substr(3,3);
    }
    while (workers.find(ID)!=workers.end());
    auto nw = std::make_shared<Worker>(ID,"Random Driver");
    (*data->getWorkers())[ID] = nw;
    return nw.get();
}

Bus *JobWidget::createRandomBus()
{
    auto buses = *data->getBuses();
    std::string letters = "0123456789";
    std::random_device rd;
    std::mt19937 g(rd());
    std::string ID;
    do
    {
        std::shuffle(letters.begin(),letters.end(),g);
        ID = letters.substr(0,3) + "/" + letters.substr(3,3);
    }
    while (buses.find(ID)!=buses.end());
    auto nw = std::make_shared<Bus>(ID,"Random Bus",0);
    (*data->getBuses())[ID] = nw;
    return nw.get();
}
