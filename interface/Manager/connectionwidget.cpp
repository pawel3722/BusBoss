
#include "connectionwidget.h"
#include "interface/Manager/manager.h"
#include <regex>

ConnectionWidget::ConnectionWidget(Data* data,QWidget *parent)
    : QWidget{parent}
{
    this->data = data;
    this->parent = parent;
    ret_button = new QPushButton("Powrót");
        connect(ret_button,&QPushButton::clicked,static_cast<Manager*>(parent),&Manager::showMenu);
        connect(ret_button,&QPushButton::clicked,this,&ConnectionWidget::deactivateBoxes);

    choose_connection_label = new QLabel("Wybierz połączenie:");
    got_connection = new QComboBox;
    connect(got_connection,&QComboBox::currentIndexChanged,this,&ConnectionWidget::deactivateBoxes);
    confirm_connection = new QPushButton("Wybierz");
        connect(confirm_connection,&QPushButton::clicked,this,&ConnectionWidget::setDefaultValues);

    chosen_connection_label = new QLabel;
    time_msg_label = new QLabel;
    distance_msg_label = new QLabel;

    time1_label = new QLabel("Czas domyślny:");
    time1_min = new QSpinBox;  time1_min->setMaximum(59); time1_min->setSuffix(" min");
    time1_sec = new QSpinBox;  time1_sec->setMaximum(59); time1_sec->setSuffix(" s");
        connect(time1_min,&QSpinBox::valueChanged,this,&ConnectionWidget::resetTimeMsg);
        connect(time1_sec,&QSpinBox::valueChanged,this,&ConnectionWidget::resetTimeMsg);

    time2_label = new QLabel("Czas alternatywny:");
    time2_min = new QSpinBox;  time2_min->setMaximum(59); time2_min->setSuffix(" min");
    time2_sec = new QSpinBox;  time2_sec->setMaximum(59); time2_sec->setSuffix(" s");
        connect(time2_min,&QSpinBox::valueChanged,this,&ConnectionWidget::resetTimeMsg);
        connect(time2_sec,&QSpinBox::valueChanged,this,&ConnectionWidget::resetTimeMsg);


    time3_label = new QLabel("Czas opcjonalny:");
    time3_min = new QSpinBox;  time3_min->setMaximum(59); time3_min->setSuffix(" min");
    time3_sec = new QSpinBox;  time3_sec->setMaximum(59); time3_sec->setSuffix(" s");
        connect(time3_min,&QSpinBox::valueChanged,this,&ConnectionWidget::resetTimeMsg);
        connect(time3_sec,&QSpinBox::valueChanged,this,&ConnectionWidget::resetTimeMsg);

    confirm_times = new QPushButton("Ustaw");
        connect(confirm_times,&QPushButton::clicked,this,&ConnectionWidget::setTimes);

    distance_label = new QLabel ("Odległość:");
    got_distance = new QDoubleSpinBox;  got_distance->setSingleStep(0.05); got_distance->setSuffix(" km");
        connect(got_distance,&QDoubleSpinBox::valueChanged,this,&ConnectionWidget::resetDistanceMsg);
    confirm_distance = new QPushButton("Zapisz");
        connect(confirm_distance,&QPushButton::clicked,this,&ConnectionWidget::setDistance);

    layout = new QGridLayout;
    layout->addWidget(ret_button,1,1,1,1);
    layout->addWidget(choose_connection_label,2,1,1,2);
    layout->addWidget(got_connection,3,1,1,2);
    layout->addWidget(confirm_connection,4,1,1,2);
    layout->addWidget(chosen_connection_label,5,1,1,2);
    layout->addWidget(time_msg_label,6,1,1,2);
    layout->addWidget(time1_label,7,1,1,2);
    layout->addWidget(time1_min,8,1,1,1);
    layout->addWidget(time1_sec,8,2,1,1);
    layout->addWidget(time2_label,9,1,1,2);
    layout->addWidget(time2_min,10,1,1,1);
    layout->addWidget(time2_sec,10,2,1,1);
    layout->addWidget(time3_label,11,1,1,2);
    layout->addWidget(time3_min,12,1,1,1);
    layout->addWidget(time3_sec,12,2,1,1);
    layout->addWidget(confirm_times,13,1,1,2);
    layout->addWidget(distance_msg_label,14,1,1,2);
    layout->addWidget(distance_label,15,1,1,2);
    layout->addWidget(got_distance,16,1,1,2);
    layout->addWidget(confirm_distance,17,1,1,2);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,3);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding),18,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred),18,3);
    this->setLayout(layout);
    deactivateBoxes();
}

void ConnectionWidget::updateConnections()
{
    got_connection->clear();
    for(const auto& el: *(data->getConnections()))
    {
        for(const auto& pos : el.second)
        {
            auto name = pos.second->get1StopPointer()->getName() + " - " + pos.second->get2StopPointer()->getName();
            got_connection->addItem(QString::fromStdString(name));
        }
    }
}

void ConnectionWidget::activateBoxes()
{
    time1_min->setEnabled(true);
    time1_sec->setEnabled(true);
    time2_min->setEnabled(true);
    time2_sec->setEnabled(true);
    time3_min->setEnabled(true);
    time3_sec->setEnabled(true);
    confirm_times->setEnabled(true);
    got_distance->setEnabled(true);
    confirm_distance->setEnabled(true);
    time_msg_label->setText("");
    distance_msg_label->setText("");
}

void ConnectionWidget::setDefaultValues()
{
    auto txt = got_connection->currentText();
    if(txt.size())
    {
        activateBoxes();
        std::string name = txt.toStdString();
        std::regex reg("^(.+) - (.+)$");
        std::smatch base_match;
        std::regex_match(name, base_match, reg);
        auto sub_match1 = base_match[1];
        auto sub_match2 = base_match[2];
        std::string s1 = sub_match1.str();
        std::string s2 = sub_match2.str();
        chosen_connection = data->getConnectionFromStopsNames(s1,s2);
        chosen_connection_label->setText("Wybrane połączenie:\n" + txt);
        auto time1 = chosen_connection->getTime(0);
        auto time2 = chosen_connection->getTime(1);
        auto time3 = chosen_connection->getTime(2);
        time1_min->setValue(time1.minute());
        time1_sec->setValue(time1.second());
        time2_min->setValue(time2.minute());
        time2_sec->setValue(time2.second());
        time3_min->setValue(time3.minute());
        time3_sec->setValue(time3.second());
        got_distance->setValue(chosen_connection->getDistance());
    }
}

void ConnectionWidget::deactivateBoxes()
{
    time1_min->setEnabled(false);
    time1_sec->setEnabled(false);
    time2_min->setEnabled(false);
    time2_sec->setEnabled(false);
    time3_min->setEnabled(false);
    time3_sec->setEnabled(false);
    confirm_times->setEnabled(false);
    got_distance->setEnabled(false);
    confirm_distance->setEnabled(false);
    time1_min->setValue(0);
    time1_sec->setValue(0);
    time2_min->setValue(0);
    time2_sec->setValue(0);
    time3_min->setValue(0);
    time3_sec->setValue(0);
    got_distance->setValue(0);
    chosen_connection_label->setText("Wybrane połączenie:\n");
    time_msg_label->setText("");
    distance_msg_label->setText("");
}

void ConnectionWidget::setTimes()
{
    chosen_connection->setTime(0,{0,time1_min->value(),time1_sec->value()});
    chosen_connection->setTime(1,{0,time2_min->value(),time2_sec->value()});
    chosen_connection->setTime(2,{0,time3_min->value(),time3_sec->value()});
    time_msg_label->setText("Zmieniono czasy!");
}

void ConnectionWidget::setDistance()
{
    chosen_connection->setDistance(got_distance->value());
    distance_msg_label->setText("Zmieniono dystans!");
}

