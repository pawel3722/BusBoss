
#include "workerwidget.h"
#include "interface/Manager/manager.h"
#include "qgridlayout.h"
#include "qlineedit.h"
#include "qmessagebox.h"

WorkerWidget::WorkerWidget(Data* data,QWidget *parent)
    : QWidget{parent}
{
    this->data = data;
    this->parent = parent;

    ret_button = new QPushButton("Powrót");
        connect(ret_button,&QPushButton::clicked,static_cast<Manager*>(parent),&Manager::showMenu);
        connect(ret_button,&QPushButton::clicked,this,&WorkerWidget::resetAddBoxes);
        connect(ret_button,&QPushButton::clicked,this,&WorkerWidget::resetAddInfoLine);
        connect(ret_button,&QPushButton::clicked,this,&WorkerWidget::resetDelBoxes);
        connect(ret_button,&QPushButton::clicked,this,&WorkerWidget::resetDelInfoLine);

    add_msg_label = new QLabel;
    add_info_label = new QLabel("<b>Dodaj pracownika:</b>");
    add_desc1_label = new QLabel("Podaj ID pracownika:");
    add_desc2_label = new QLabel("Podaj dane pracownika:");
    got_number = new QLineEdit;
        connect(got_number,&QLineEdit::textChanged,this,&WorkerWidget::resetAddInfoLine);
    got_name = new QLineEdit;
        connect(got_name,&QLineEdit::textChanged,this,&WorkerWidget::resetAddInfoLine);
    got_number->setMaxLength(7);
    add_ok_button = new QPushButton("Dodaj");
        connect(add_ok_button,&QPushButton::clicked,this,&WorkerWidget::addWorker);

    del_msg_label = new QLabel;
            del_info_label = new QLabel("<b>Usuń pracownika:</b>");
                               del_desc_label = new QLabel("Wybierz pracownika:");
            choose_worker = new QComboBox;
                               connect(choose_worker,&QComboBox::currentIndexChanged,this,&WorkerWidget::resetDelInfoLine);
            choose_worker->addItem("(wybierz pracownika)");
    updateWorkers();

    del_ok_button = new QPushButton("Usuń");
        connect(del_ok_button,&QPushButton::clicked,this,&WorkerWidget::delWorker);

    auto layout = new QGridLayout(this);
    layout->addWidget(ret_button,1,1,1,1);
    layout->addWidget(add_msg_label,2,1,1,2);
    layout->addWidget(add_info_label,3,1,1,2);
    layout->addWidget(add_desc1_label,4,1,1,2);
    layout->addWidget(got_number,5,1,1,2);
    layout->addWidget(add_desc2_label,6,1,1,2);
    layout->addWidget(got_name,7,1,1,2);
    layout->addWidget(add_ok_button,8,1,1,2);

    layout->addWidget(del_msg_label,2,4,1,2);
    layout->addWidget(del_info_label,3,4,1,2);
    layout->addWidget(del_desc_label,4,4,1,2);
    layout->addWidget(choose_worker,5,4,1,2);
    layout->addWidget(del_ok_button,6,4,1,2);

    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,6);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding),9,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred),9,6);
    this->setLayout(layout);
    resetAddBoxes();
    resetDelBoxes();
}

void WorkerWidget::updateWorkers()
{
    choose_worker->clear();
    choose_worker->addItem("(wybierz pracownika)");
    auto lines = data->getWorkers();
    for(const auto& el: *lines)
    {
        auto name = el.second->getID() + " " + el.second->getName();
        choose_worker->addItem(QString::fromStdString(name));
        worker_names[name] = el.second->getID();
    }
}


void WorkerWidget::addWorker()
{
    auto workers = data->getWorkers();
    auto name = got_name->text().toStdString();
    auto ID = got_number->text().toStdString();

    if(workers->find(ID) == workers->end())
    {
        auto worker = std::make_shared<Worker>(ID,name);
        (*workers)[ID] = worker;
        resetAddBoxes();
        add_msg_label->setText("Dodano pracownika!");
        updateWorkers();
    }
    else
        add_msg_label->setText("Pracownik o podanym ID istnieje!");
}

void WorkerWidget::delWorker()
{
    auto w_name = choose_worker->currentText();
    if(w_name != "(wybierz pracownika)")
    {
        auto workers = data->getWorkers();
        auto ID = worker_names[w_name.toStdString()];
        QString bad_brigades;
        for(const auto& el: *data->getShifts())
        {
            if(el.second->getWorkerID()==ID)
            {
                bad_brigades += QString::fromStdString(el.second->getID()+", ");
            }
        }
        if(!bad_brigades.size())
        {
            QMessageBox dial(QMessageBox::Warning,"Usuń pracownika","Czy na pewno chcesz usunąć pracownika " + w_name + "?", QMessageBox::Ok | QMessageBox::Cancel);
            int result = dial.exec();
            if(result == QMessageBox::Ok)
            {
                workers->erase(ID);
                updateWorkers();
                del_msg_label->setText("Usunięto pracownika!");
            }
        }
        else
        {
            QMessageBox dialog(QMessageBox::Warning,"Nie można usunąć pracownika","Nie można usunąć pracownika " + w_name + " z powodu istniejących przydziałów. Najpierw usuń przydziały tego pracownika do następujących zmian:\n" + bad_brigades, QMessageBox::Ok);
            dialog.exec();
        }
    }
}

void WorkerWidget::resetAddInfoLine()
{
    add_msg_label->setText("");
}

void WorkerWidget::resetAddBoxes()
{
    got_number->clear();
    got_name->clear();
}

void WorkerWidget::resetDelInfoLine()
{
    del_msg_label->setText("");
}

void WorkerWidget::resetDelBoxes()
{
    resetDelInfoLine();
    choose_worker->setCurrentIndex(0);
}
