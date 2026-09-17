
#include "changetimedialog.h"
#include "qdatetime.h"
#include <algorithm>

ChangeTimeDialog::ChangeTimeDialog(QDate date, QTime time, double speed, Data* data)
{
    this->data = data;
    layout = new QGridLayout;
    lab1 = new QLabel ("Podaj datę:");
    lab2 = new QLabel;
    lab3 = new QLabel ("Podaj godzinę:");
    lab4 = new QLabel ("Podaj prędkość czasu:");
    date_edit = new QDateEdit;
    date_edit->setDate(date);
    date_edit->setDisplayFormat("dd.MM.yyyy");
        connect(date_edit,&QDateTimeEdit::dateChanged,this,&ChangeTimeDialog::dateChanged);
    new_hour = new QSpinBox;
    new_min = new QSpinBox;
    new_sec = new QSpinBox;
    new_speed = new QDoubleSpinBox;
    button_box = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    new_hour->setValue(time.hour());
    new_hour->setMaximum(23);
    new_hour->setSuffix(" h");
    new_min->setValue(time.minute());
    new_min->setMaximum(59);
    new_min->setSuffix(" m");
    new_sec->setValue(time.second());
    new_sec->setMaximum(59);
    new_sec->setSuffix(" s");
    new_speed->setValue(speed);
    new_speed->setMaximum(60);
    new_speed->setSuffix("x");

    layout->addWidget(lab1,0,0,1,3);
    layout->addWidget(date_edit,1,0,1,3);
    layout->addWidget(lab2,2,0,1,3);
    layout->addWidget(lab3,3,0,1,3);
    layout->addWidget(new_hour,4,0,1,1);
    layout->addWidget(new_min,4,1,1,1);
    layout->addWidget(new_sec,4,2,1,1);
    layout->addWidget(lab4,5,0,1,3);
    layout->addWidget(new_speed,6,0,1,3);
    layout->addWidget(button_box,7,0,1,3);
    dateChanged();

    this->setLayout(layout);
    this->setWindowTitle("Zmień datę i czas");

}

void ChangeTimeDialog::dateChanged()
{
        std::vector<QString> day_names {"Poniedziałek","Wtorek","Środa","Czwartek","Piątek","Sobota","Niedziela"};
        QDate date = date_edit->date();
        QString txt = day_names[date.dayOfWeek()-1];
        auto res = data->getDayType(date);
        day_type = res.first;
        if(res.second.size())
            txt += QString::fromStdString(" - " + res.second);
        lab2->setText(txt);
}

