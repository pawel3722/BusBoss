
#include "setshiftsdialog.h"
#include "data/connection.h"
#include <ranges>

SetShiftsDialog::SetShiftsDialog(std::vector<std::vector<QTime>> drive_times, std::vector<std::vector<QTime>> break_times, std::vector<QTime> dep_time)
{
    this->drive_times = drive_times;
    this->break_times = break_times;
    this->dep_time = dep_time;

    layout = new QGridLayout;
    lab1 = new QLabel ("Zmiana:");
    lab2 = new QLabel ("Początek:");
    lab3 = new QLabel ("Koniec:");
    lab4 = new QLabel ("Całkowity czas:");
    lab5 = new QLabel ("Czas jazdy:");
    lab6 = new QLabel ("Czas przerwy:");
    lab7 = new QLabel ("Poprawność czasu:");
    lab8 = new QLabel ("Poprawność przerw:");
    lab9 = new QLabel ("Przerwa 15 min:");

    layout->addWidget(lab1,0,2);
    layout->addWidget(lab2,0,3);
    layout->addWidget(lab3,0,4);
    layout->addWidget(lab4,0,5);
    layout->addWidget(lab5,0,6);
    layout->addWidget(lab6,0,7);
    layout->addWidget(lab7,0,8);
    layout->addWidget(lab8,0,9);
    layout->addWidget(lab9,0,10);
    int i = 0;
    for( ; i < (int) dep_time.size(); ++i)
    {
        time_labels.emplace_back(new QLabel("Czas "+QString::number(i)+":"));
        layout->addWidget(time_labels[i],1 + 4 * i,0,Qt::AlignHCenter);

        auto cb2 = new QCheckBox;
            connect(cb2,&QCheckBox::stateChanged,this,&SetShiftsDialog::changeD2TBState);
            connect(cb2,&QCheckBox::stateChanged,this,&SetShiftsDialog::changeCB3State);
        auto cb3 = new QCheckBox;
            connect(cb3,&QCheckBox::stateChanged,this,&SetShiftsDialog::changeD3TBState);
            connect(cb3,&QCheckBox::stateChanged,this,&SetShiftsDialog::changeCB2State);
        cb3->setEnabled(false);

        layout->addWidget(cb2,2+4*i,1,Qt::AlignHCenter);
        layout->addWidget(cb3,3+4*i,1,Qt::AlignHCenter);
        check_boxes.emplace_back(std::vector<QCheckBox*>{cb2,cb3});

        shift_name_labels.emplace_back(std::vector<QLabel*> {new QLabel("A"),new QLabel("B"),new QLabel("C")});
        layout->addWidget(shift_name_labels[i][0],1+4*i,2,Qt::AlignHCenter);
        layout->addWidget(shift_name_labels[i][1],2+4*i,2,Qt::AlignHCenter);
        layout->addWidget(shift_name_labels[i][2],3+4*i,2,Qt::AlignHCenter);

        dep_1_time_labels.emplace_back(new QLabel(dep_time[i].toString("hh:mm:ss")));
        layout->addWidget(dep_1_time_labels[i],1+4*i,3,Qt::AlignHCenter);
        dep_2_time_boxes.emplace_back(new QTimeEdit);
        dep_2_time_boxes[i]->setEnabled(false);
        dep_2_time_boxes[i]->setDisplayFormat("hh:mm:ss");
            connect(dep_2_time_boxes[i],&QDateTimeEdit::timeChanged,this,&SetShiftsDialog::timeChanged);
        layout->addWidget(dep_2_time_boxes[i],2+4*i,3,Qt::AlignHCenter);
        dep_3_time_boxes.emplace_back(new QTimeEdit);
        dep_3_time_boxes[i]->setEnabled(false);
        dep_3_time_boxes[i]->setDisplayFormat("hh:mm:ss");
            connect(dep_3_time_boxes[i],&QDateTimeEdit::timeChanged,this,&SetShiftsDialog::timeChanged);
        layout->addWidget(dep_3_time_boxes[i],3+4*i,3,Qt::AlignHCenter);

        drive_time_sum.emplace_back(QTime{0,0});
        break_time_sum.emplace_back(QTime{0,0});
        for(const auto& el : drive_times[i])
            drive_time_sum[i] = Connection::addTime(drive_time_sum[i],el);
        for(const auto& el : break_times[i])
            break_time_sum[i] = Connection::addTime(break_time_sum[i],el);

        QTime default_time = Connection::addTime(drive_time_sum[i],break_time_sum[i]);
        arr_times.emplace_back(std::vector<QLabel*>{new QLabel(Connection::addTime(dep_time[i],default_time).toString("hh:mm:ss")),new QLabel,new QLabel });
        layout->addWidget(arr_times[i][0],1+4*i,4,Qt::AlignHCenter);
        layout->addWidget(arr_times[i][1],2+4*i,4,Qt::AlignHCenter);
        layout->addWidget(arr_times[i][2],3+4*i,4,Qt::AlignHCenter);

        total_times.emplace_back(std::vector<QLabel*>{new QLabel(default_time.toString("hh:mm:ss")),new QLabel,new QLabel });
        layout->addWidget(total_times[i][0],1+4*i,5,Qt::AlignHCenter);
        layout->addWidget(total_times[i][1],2+4*i,5,Qt::AlignHCenter);
        layout->addWidget(total_times[i][2],3+4*i,5,Qt::AlignHCenter);

        drive_times_labels.emplace_back(std::vector<QLabel*>{new QLabel(drive_time_sum[i].toString("hh:mm:ss")),new QLabel,new QLabel });
        layout->addWidget(drive_times_labels[i][0],1+4*i,6,Qt::AlignHCenter);
        layout->addWidget(drive_times_labels[i][1],2+4*i,6,Qt::AlignHCenter);
        layout->addWidget(drive_times_labels[i][2],3+4*i,6,Qt::AlignHCenter);

        break_times_labels.emplace_back(std::vector<QLabel*>{new QLabel(break_time_sum[i].toString("hh:mm:ss")),new QLabel,new QLabel });
        layout->addWidget(break_times_labels[i][0],1+4*i,7,Qt::AlignHCenter);
        layout->addWidget(break_times_labels[i][1],2+4*i,7,Qt::AlignHCenter);
        layout->addWidget(break_times_labels[i][2],3+4*i,7,Qt::AlignHCenter);

        time_boxes.emplace_back(std::vector<QCheckBox*>{new QCheckBox, new QCheckBox, new QCheckBox});
        time_boxes[i][0]->setEnabled(false);
        time_boxes[i][1]->setEnabled(false);
        time_boxes[i][2]->setEnabled(false);
        layout->addWidget(time_boxes[i][0],1+4*i,8,Qt::AlignHCenter);
        layout->addWidget(time_boxes[i][1],2+4*i,8,Qt::AlignHCenter);
        layout->addWidget(time_boxes[i][2],3+4*i,8,Qt::AlignHCenter);

        break_boxes.emplace_back(std::vector<QCheckBox*>{new QCheckBox, new QCheckBox, new QCheckBox});
        break_boxes[i][0]->setEnabled(false);
        break_boxes[i][1]->setEnabled(false);
        break_boxes[i][2]->setEnabled(false);
        layout->addWidget(break_boxes[i][0],1+4*i,9,Qt::AlignHCenter);
        layout->addWidget(break_boxes[i][1],2+4*i,9,Qt::AlignHCenter);
        layout->addWidget(break_boxes[i][2],3+4*i,9,Qt::AlignHCenter);

        br_15_boxes.emplace_back(std::vector<QCheckBox*>{new QCheckBox, new QCheckBox, new QCheckBox});
        br_15_boxes[i][0]->setEnabled(false);
        br_15_boxes[i][1]->setEnabled(false);
        br_15_boxes[i][2]->setEnabled(false);
        layout->addWidget(br_15_boxes[i][0],1+4*i,10,Qt::AlignHCenter);
        layout->addWidget(br_15_boxes[i][1],2+4*i,10,Qt::AlignHCenter);
        layout->addWidget(br_15_boxes[i][2],3+4*i,10,Qt::AlignHCenter);

    }
    ok_button = new QPushButton("Ok");
        connect(ok_button,&QPushButton::clicked,this,&QDialog::accept);
    cancel_button = new QPushButton("Anuluj");
        connect(cancel_button,&QPushButton::clicked,this,&QDialog::reject);

    layout->addWidget(ok_button,1 + 4 * i,7,1,2);
    layout->addWidget(cancel_button,1 + 4 * i,9,1,2);

    this->setLayout(layout);
    this->setWindowTitle("Ustaw podział na zmiany");
    checkShiftsLaw();
    checkCorrectness();
}

void SetShiftsDialog::setDefaultShifts(std::map<int, QTime> sh1, std::map<int, QTime> sh2)
{
    for(const auto& el : sh1)
    {
        check_boxes[el.first][0]->setChecked(true);
        dep_2_time_boxes[el.first]->setTime(el.second);
    }
    for(const auto& el : sh2)
    {
        check_boxes[el.first][1]->setChecked(true);
        dep_3_time_boxes[el.first]->setTime(el.second);
    }
}

std::vector<std::vector<QTime> > SetShiftsDialog::getShiftTimes()
{
    std::vector<std::vector<QTime>> vec;    
    for(int i = 0; i < check_boxes.size(); ++i)
    {
        std::vector<QTime> v;
        if(check_boxes[i][0]->isChecked())
        {
            v.push_back(dep_2_time_boxes[i]->time());
            if(check_boxes[i][1]->isChecked())
                v.push_back(dep_3_time_boxes[i]->time());
        }
        vec.push_back(v);
    }
    return vec;
}

void SetShiftsDialog::changeCB3State()
{
    auto sen = static_cast<QCheckBox*>(sender());
    int index = 0;
    for(int i = 0; i < check_boxes.size(); ++i)
        if(check_boxes[i][0]==sen)
        {
            index = i;
            break;
        }
    sen->isChecked() ? check_boxes[index][1]->setEnabled(true) : check_boxes[index][1]->setEnabled(false);
}

void SetShiftsDialog::changeCB2State()
{
    auto sen = static_cast<QCheckBox*>(sender());
    int index = 0;
    for(int i = 0; i < check_boxes.size(); ++i)
        if(check_boxes[i][1]==sen)
        {
            index = i;
            break;
        }
    sen->isChecked() ? check_boxes[index][0]->setEnabled(false) : check_boxes[index][0]->setEnabled(true);
}

void SetShiftsDialog::changeD2TBState()
{
    auto sen = static_cast<QCheckBox*>(sender());
    int index = 0;
    for(int i = 0; i < check_boxes.size(); ++i)
        if(check_boxes[i][0]==sen)
        {
            index = i;
            break;
        }
    if(sen->isChecked())
    {
        dep_2_time_boxes[index]->setEnabled(true);
        QTime sum = Connection::addTime(drive_time_sum[index],break_time_sum[index]);
        sum.setHMS(sum.hour()/2,sum.minute()/2,sum.second()/2);
        QTime time = Connection::addTime(dep_time[index],sum);
        dep_2_time_boxes[index]->setTime(time);
    }
    else
    {
        dep_2_time_boxes[index]->setTime(QTime{0,0});
        dep_2_time_boxes[index]->setEnabled(false);
    }
}

void SetShiftsDialog::changeD3TBState()
{
    auto sen = static_cast<QCheckBox*>(sender());
    int index = 0;
    for(int i = 0; i < check_boxes.size(); ++i)
        if(check_boxes[i][1]==sen)
        {
            index = i;
            break;
        }
    if(sen->isChecked())
    {
        dep_3_time_boxes[index]->setEnabled(true);
        QTime sum = Connection::addTime(drive_time_sum[index],break_time_sum[index]);
        sum.setHMS(2*sum.hour()/3,2*sum.minute()/3,2*sum.second()/3);
        QTime time = Connection::addTime(dep_time[index],sum);
        dep_3_time_boxes[index]->setTime(time);
    }
    else
    {
        dep_3_time_boxes[index]->setTime(QTime{0,0});
        dep_3_time_boxes[index]->setEnabled(false);
    }
}

void SetShiftsDialog::checkShiftsLaw()
{
    for(int i = 0; i < dep_time.size();++i)
    {
        int it = 0;
        QTime next_drive_time{0,0};
        QTime next_break_time{0,0};
        bool break_first=false;
        for(int j = 0; j < 3; ++j)
        {
            if(j > 0 && !check_boxes[i][j-1]->isChecked())
            {
                drive_times_labels[i][j]->setText("");
                break_times_labels[i][j]->setText("");
                break_boxes[i][j]->setChecked(false);
                time_boxes[i][j]->setChecked(false);
                br_15_boxes[i][j]->setChecked(false);
                break;
            }
            QTime drive_sum = Connection::addTime(QTime{0,0},next_drive_time);
            QTime break_sum = Connection::addTime(QTime{0,0}, next_break_time);
            QTime sum{0,0};
            QTime end_time = QTime::fromString(arr_times[i][j]->text(),"hh:mm:ss");
            if(j > 0)
                end_time = Connection::subTime(end_time,QTime::fromString(arr_times[i][j-1]->text(),"hh:mm:ss"));
            else
                end_time = Connection::subTime(end_time,dep_time[i]);

            next_break_time={0,0};
            next_drive_time={0,0};
            bool count_services = true;
            if(end_time < drive_sum)
            {
                next_drive_time = Connection::subTime(drive_sum,end_time);
                drive_sum = end_time;
                count_services = false;
            }
            if(end_time < break_sum)
            {
                next_break_time = Connection::subTime(break_sum,end_time);
                break_sum = end_time;
               count_services = false;
            }

            bool br_15 = false;
            bool br_30 = true;
            bool br_45 = true;
            bool finish = false;

            if(count_services)
            {
                for( ; it < drive_times[i].size(); ++it)
                {
                    if(!break_first)
                    {
                        drive_sum = Connection::addTime(drive_sum,drive_times[i][it]);
                        sum = Connection::addTime(drive_sum,break_sum);
                        if(sum >= end_time)
                        {
                            if(sum>end_time)
                            {
                                next_drive_time = Connection::subTime(sum,end_time);
                                drive_sum = Connection::subTime(drive_sum,next_drive_time);
                            }
                            break_first = true;
                            finish = true;
                        }
                        if(drive_sum>QTime{6,0} && break_sum < QTime{0,30})
                            br_30 = false;
                        if(drive_sum>QTime{8,0} && break_sum < QTime{0,45})
                            br_45 = false;
                        if(finish)
                            break;
                    }
                    break_first = false;
                    if(it < drive_times[i].size()-1)
                    {
                        if(break_times[i][it] >= QTime{0,15} && drive_sum < QTime{ 6,0 })
                            br_15 = true;
                        break_sum = Connection::addTime(break_sum,break_times[i][it]);
                        sum = Connection::addTime(drive_sum,break_sum);
                        if(sum >= end_time)
                        {
                            next_break_time = Connection::subTime(sum,end_time);
                            break_sum = Connection::subTime(break_sum,next_break_time);
                            ++it;
                            break;
                        }
                    }
                }
            }
            if(drive_sum > QTime{10,0} || sum > QTime{13,0})
                time_boxes[i][j]->setChecked(false);
            else
                time_boxes[i][j]->setChecked(true);
            if(br_45 && br_30)
                break_boxes[i][j]->setChecked(true);
            else
                break_boxes[i][j]->setChecked(false);
            if(br_15 || drive_sum < QTime{6,0} )
                br_15_boxes[i][j]->setChecked(true);
            else
                br_15_boxes[i][j]->setChecked(false);
            drive_times_labels[i][j]->setText(drive_sum.toString("hh:mm:ss"));
            break_times_labels[i][j]->setText(break_sum.toString("hh:mm:ss"));
        }
    }
}

void SetShiftsDialog::timeChanged()
{
    for(int i = 0; i < dep_time.size(); ++i)
    {
        QTime sum = Connection::addTime(drive_time_sum[i],break_time_sum[i]);
        sum = Connection::addTime(sum,dep_time[i]);
        if(check_boxes[i][0]->isChecked())
        {
            arr_times[i][0]->setText(dep_2_time_boxes[i]->text());
            total_times[i][0]->setText(Connection::subTime(QTime::fromString(arr_times[i][0]->text(),"hh:mm:ss"),QTime::fromString(dep_1_time_labels[i]->text(),"hh:mm:ss")).toString("hh:mm:ss"));
            if(check_boxes[i][1]->isChecked())
            {
                arr_times[i][1]->setText(dep_3_time_boxes[i]->text());
                total_times[i][1]->setText(Connection::subTime(QTime::fromString(arr_times[i][1]->text(),"hh:mm:ss"),dep_2_time_boxes[i]->time()).toString("hh:mm:ss"));
                arr_times[i][2]->setText(sum.toString("hh:mm:ss"));
                total_times[i][2]->setText(Connection::subTime(QTime::fromString(arr_times[i][2]->text(),"hh:mm:ss"),dep_3_time_boxes[i]->time()).toString("hh:mm:ss"));
            }
            else
            {
                arr_times[i][1]->setText(sum.toString("hh:mm:ss"));
                total_times[i][1]->setText(Connection::subTime(QTime::fromString(arr_times[i][1]->text(),"hh:mm:ss"),dep_2_time_boxes[i]->time()).toString("hh:mm:ss"));
                arr_times[i][2]->setText("");
                total_times[i][2]->setText("");
            }
        }
        else
        {
            arr_times[i][0]->setText(sum.toString("hh:mm:ss"));
            total_times[i][0]->setText(Connection::subTime(QTime::fromString(arr_times[i][0]->text(),"hh:mm:ss"),QTime::fromString(dep_1_time_labels[i]->text(),"hh:mm:ss")).toString("hh:mm:ss"));
            arr_times[i][1]->setText("");
            total_times[i][1]->setText("");
            arr_times[i][2]->setText("");
            total_times[i][2]->setText("");
        }
    }
    checkShiftsLaw();
    checkCorrectness();
}

void SetShiftsDialog::checkCorrectness()
{
    auto size = dep_time.size();
    std::vector<bool> time_correct (size);
    std::vector<bool> ticks_correct (size);
    for(int i = 0; i < size; ++i)
    {
        time_correct[i] = false;
        ticks_correct[i] = true;
        QTime sum = QTime::fromString(total_times[i][0]->text(),"hh:mm:ss");
        if(!(time_boxes[i][0]->isChecked() && break_boxes[i][0]->isChecked() && br_15_boxes[i][0]->isChecked()))
            ticks_correct[i] = false;
        if(check_boxes[i][0]->isChecked())
        {
            auto t1 = QTime::fromString(total_times[i][1]->text(),"hh:mm:ss");
            sum = Connection::addTime(sum,t1,false);
            if(!(time_boxes[i][1]->isChecked() && break_boxes[i][1]->isChecked() && br_15_boxes[i][1]->isChecked()))
                ticks_correct[i] = false;
            if(check_boxes[i][1]->isChecked())
            {
                auto t2 = QTime::fromString(total_times[i][2]->text(),"hh:mm:ss");
                sum = Connection::addTime(sum,t2,false);
                if(!(time_boxes[i][2]->isChecked() && break_boxes[i][2]->isChecked() && br_15_boxes[i][2]->isChecked()))
                    ticks_correct[i] = false;
            }
        }
        if( sum  == Connection::addTime(drive_time_sum[i],break_time_sum[i]) && sum.isValid())
            time_correct[i] = true;
    }
    auto view1 = std::views::all(time_correct);
    auto view2 = std::views::all(ticks_correct);
    if(std::ranges::find(view1,false)==view1.end() && std::ranges::find(view2,false)==view2.end())
        ok_button->setEnabled(true);
    else
        ok_button->setEnabled(false);
}

