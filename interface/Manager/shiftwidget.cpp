
#include "shiftwidget.h"
#include "interface/Manager/manager.h"

ShiftWidget::ShiftWidget(Data* data, QWidget *parent)
    : QWidget{parent}
{
    this->data = data;
    this->parent = parent;
    ret_button = new QPushButton("Powrót");
        connect(ret_button,&QPushButton::clicked,static_cast<Manager*>(parent),&Manager::showMenu);
        connect(ret_button,&QPushButton::clicked,this,&::ShiftWidget::resetInfoLine);
        connect(ret_button,&QPushButton::clicked,this,&ShiftWidget::deactivateBoxes);

    choose_shift_label = new QLabel("Wybierz zmianę:");
    got_shift = new QComboBox;
        connect(got_shift,&QComboBox::currentIndexChanged,this,&ShiftWidget::deactivateBoxes);
        connect(got_shift,&QComboBox::currentIndexChanged,this,&::ShiftWidget::resetInfoLine);
    chosen_shift_label = new QLabel;

    confirm_shift = new QPushButton("Wybierz");
        connect(confirm_shift,&QPushButton::clicked,this,&ShiftWidget::activateBoxes);

    msg_label = new QLabel;
    choose_bus_label = new QLabel ("Wybierz autobus:");
    choose_worker_label = new QLabel("Wybierz kierowcę:");

    confirm_button = new QPushButton("Potwierdź");
        connect(confirm_button,&QPushButton::clicked,this,&ShiftWidget::setValues);

    buses_box = new QComboBox;
    workers_box = new QComboBox;

    auto layout = new QGridLayout(this);
    layout->addWidget(ret_button,1,1,1,1);
    layout->addWidget(choose_shift_label,2,1,1,2);
    layout->addWidget(got_shift,3,1,1,2);
    layout->addWidget(confirm_shift,4,1,1,2);
    layout->addWidget(chosen_shift_label,5,1,1,2);
    layout->addWidget(msg_label,6,1,1,2);
    layout->addWidget(choose_bus_label,7,1,1,2);
    layout->addWidget(buses_box,8,1,1,2);
    layout->addWidget(choose_worker_label,9,1,1,2);
    layout->addWidget(workers_box,10,1,1,2);
    layout->addWidget(confirm_button,11,1,1,2);
    layout->setColumnMinimumWidth(1,125);
    layout->setColumnMinimumWidth(2,125);

    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,6);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding),12,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred),12,6);
    this->setLayout(layout);
    deactivateBoxes();
}

void ShiftWidget::updateShifts()
{
    got_shift->clear();
    for(const auto& el: *(data->getShifts()))
    {
       auto name = el.second->getID();
        got_shift->addItem(QString::fromStdString(name));
    }
}

void ShiftWidget::updateBuses()
{
    auto shifts = *data->getShifts();
    int index;
    buses_box->clear();
    int i = 0;
    for(const auto& bus : *data->getBuses())
    {
        bool bad = false;
        if(bus.second.get() == chosen_shift->getBus())
            index = i;
        else
            for(const auto& el : shifts)
            {
                if(el.second->getBus() == bus.second.get())
                {
                    auto el_ID = el.second->getID(); el_ID.pop_back();
                    auto ch_ID = chosen_shift->getID(); ch_ID.pop_back();

                    if(el_ID == ch_ID)
                        bad = false;
                    else
                    {
                        auto el_st = el.second->getStartTime();
                        auto el_fin = el.second->getFinishTime();
                        auto ch_st = chosen_shift->getStartTime();
                        auto ch_fin = chosen_shift->getFinishTime();
                        el_ID[el_ID.size()-3] -= 2*((el_ID[el_ID.size()-3]-48)/2);
                        ch_ID[ch_ID.size()-3] -= 2*((ch_ID[ch_ID.size()-3]-48)/2);
                        if(el_ID == ch_ID)
                        {
                            bad = false;
                            if(el_fin < el_st && ch_fin > ch_st && (el_fin > ch_st))
                                bad = true;
                            else if(el_fin > el_st && ch_fin < ch_st && (ch_fin > el_st))
                                bad = true;
                            else if(el_fin > el_st && ch_fin < ch_st && (el_fin > ch_st || ch_fin > el_st))
                                bad = true;
                        }
                        else
                        {
                            if(((el_st < ch_fin && el_fin > ch_st) || (ch_st < el_fin && ch_fin > el_st)) && el_fin > el_st && ch_fin > ch_st)
                                bad = true;
                            else if(el_fin < el_st && ch_fin > ch_st && (el_st < ch_fin || el_fin > ch_st))
                                bad = true;
                            else if(el_fin > el_st && ch_fin < ch_st && (ch_st < el_fin || ch_fin > el_st))
                                bad = true;
                            else if(el_fin > el_st && ch_fin < ch_st)
                                bad = true;
                        }
                    }
                    break;
                }
            }
        if(!bad)
        {
            auto displayed_name = bus.second->getID() + " " + bus.second->getModel();
            buses_box->addItem(QString::fromStdString(displayed_name));
            buses_names[displayed_name] = bus.second->getID();
            i++;
        }
    }
    buses_box->setCurrentIndex(index);
}

void ShiftWidget::updateWorkers()
{
    auto shifts = *data->getShifts();
    int index;
    workers_box->clear();
    int i = 0;
    for(const auto& wrk : *data->getWorkers())
    {
        bool found = false;
        if(wrk.second.get() == chosen_shift->getWorker())
            index = i;
        else
            for(const auto& el : shifts)
            {
                if(el.second->getWorker() == wrk.second.get())
                {
                    found = true;
                    break;
                }
            }
        if(!found)
        {
            auto displayed_name = wrk.second->getID() + " " + wrk.second->getName();
            workers_box->addItem(QString::fromStdString(displayed_name));
            workers_names[displayed_name] = wrk.second->getID();
            i++;
        }
    }
    workers_box->setCurrentIndex(index);
}

void ShiftWidget::activateBoxes()
{
    if(got_shift->currentText().size())
    {
        chosen_shift = (*data->getShifts())[got_shift->currentText().toStdString()].get();
        updateBuses();
        updateWorkers();
        chosen_shift_label->setText("Wybrana zmiana:\n"+QString::fromStdString(chosen_shift->getID()));
        buses_box->setEnabled(true);
        workers_box->setEnabled(true);
        confirm_button->setEnabled(true);
    }
}

void ShiftWidget::deactivateBoxes()
{
    buses_box->setEnabled(false);
    workers_box->setEnabled(false);
    confirm_button->setEnabled(false);
    chosen_shift_label->setText("Wybrana zmiana:\n ");
}

void ShiftWidget::setValues()
{
    data->getShifts()->at(got_shift->currentText().toStdString())->setBus(data->getBuses()->at(buses_names.at(buses_box->currentText().toStdString())).get());
    data->getShifts()->at(got_shift->currentText().toStdString())->setWorker(data->getWorkers()->at(workers_names.at(workers_box->currentText().toStdString())).get());
    msg_label->setText("Zapisano!");
    deactivateBoxes();
}

void ShiftWidget::resetInfoLine()
{
   msg_label->setText("");
}

