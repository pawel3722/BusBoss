
#include "linewidget.h"
#include "interface/Manager/manager.h"
#include "qgridlayout.h"
#include "qlineedit.h"
#include "qmessagebox.h"

LineWidget::LineWidget(Data* data,QWidget *parent)
    : QWidget{parent}
{
    this->data = data;
    this->parent = parent;

    ret_button = new QPushButton("Powrót");
        connect(ret_button,&QPushButton::clicked,static_cast<Manager*>(parent),&Manager::showMenu);
        connect(ret_button,&QPushButton::clicked,this,&LineWidget::resetAddBoxes);
        connect(ret_button,&QPushButton::clicked,this,&LineWidget::resetAddInfoLine);
        connect(ret_button,&QPushButton::clicked,this,&LineWidget::resetDelBoxes);
        connect(ret_button,&QPushButton::clicked,this,&LineWidget::resetDelInfoLine);

    add_msg_label = new QLabel;
    add_info_label = new QLabel("<b>Dodaj linię:</b>");
    add_desc1_label = new QLabel("Podaj ID linii:");
    add_desc2_label = new QLabel("Podaj nazwę linii:");
    got_number = new QSpinBox;
    got_number->setMaximum(99999);
        connect(got_number,&QSpinBox::textChanged,this,&LineWidget::resetAddInfoLine);
    got_name = new QLineEdit;
        connect(got_name,&QLineEdit::textChanged,this,&LineWidget::resetAddInfoLine);
    got_name->setMaxLength(4);
    add_ok_button = new QPushButton("Dodaj");
        connect(add_ok_button,&QPushButton::clicked,this,&LineWidget::addLine);

    del_msg_label = new QLabel;
            del_info_label = new QLabel("<b>Usuń linię:</b>");
                               del_desc_label = new QLabel("Wybierz linię:");
    choose_line = new QComboBox;
    connect(choose_line,&QComboBox::currentIndexChanged,this,&LineWidget::resetDelInfoLine);
    choose_line->addItem("(wybierz linię)");
    updateLines();

    del_ok_button = new QPushButton("Usuń");
        connect(del_ok_button,&QPushButton::clicked,this,&LineWidget::delLine);

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
    layout->addWidget(choose_line,5,4,1,2);
    layout->addWidget(del_ok_button,6,4,1,2);

    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,6);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding),9,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred),9,6);
    this->setLayout(layout);
    resetAddBoxes();
    resetDelBoxes();
}

void LineWidget::updateLines()
{
    choose_line->clear();
    choose_line->addItem("(wybierz linię)");
    auto lines = data->getLines();
    for(const auto& el: *lines)
    {
        auto name = el.second->getName();
        choose_line->addItem(QString::fromStdString(name));
        line_names[name] = el.second->getID();
    }
}


void LineWidget::addLine()
{
    auto lines = data->getLines();
    auto name = got_name->text().toStdString();
    std::stringstream ss;
    ss<<std::setfill('0')<<std::setw(5)<<got_number->value();
    std::string ID;
    ss >> ID;
    bool found = false;
    for(const auto& el : *lines)
        if (el.second->getName() == name)
        {
            found = true;
            break;
        }

    if(lines->find(ID) == lines->end())
    {
        if(!found)
        {
            auto line = std::make_shared<Line>(ID,name);
            (*lines)[ID] = line;
            resetAddBoxes();
            add_msg_label->setText("Dodano linię!");
            updateLines();
        }
        else
            add_msg_label->setText("Podana linia istnieje!");
    }
    else
        add_msg_label->setText("Linia o podanym ID istnieje!");
}

void LineWidget::delLine()
{
    auto line_name = choose_line->currentText();
    if(line_name != "(wybierz linię)")
    {
        auto lines = data->getLines();
        auto ID = line_names[line_name.toStdString()];
        QString bad_brigades;
        std::vector<QString> bad_brigades_vec;
        for(const auto& el: *data->getJobs())
        {
            for(const auto& pos : *el.second->getServices())
                for(const auto& ser : pos)
                    if(ser->getLineID()==ID)
                    {
                        auto txt = QString::fromStdString(el.second->getID()+", ");
                        if(std::find(bad_brigades_vec.begin(),bad_brigades_vec.end(),txt)==bad_brigades_vec.end())
                        {
                            bad_brigades += txt;
                            bad_brigades_vec.push_back(txt);
                        }
                        break;
                    }
        }
        if(!bad_brigades.size())
        {
            QMessageBox dial(QMessageBox::Question,"Usuń linię","Czy na pewno chcesz usunąć linię " + line_name + "?\n Spowoduje to usunięcie wszystkich jej wariantów.", QMessageBox::Ok | QMessageBox::Cancel);
            int result = dial.exec();
            if(result == QMessageBox::Ok)
            {
                auto routes = data->getRoutes();
                auto services = data->getServices();
                auto this_line_routes = (*lines)[ID]->getRoutes();
                std::vector<std::string> bad_services;
                for(const auto& el: *services)
                    if(std::find(this_line_routes.begin(),this_line_routes.end(),el.second->getRouteID())!=this_line_routes.end())
                        bad_services.push_back(el.second->getID());
                for(const auto& el: bad_services)
                    services->erase(el);
                for(const auto& el: this_line_routes)
                    routes->erase(el);
                lines->erase(ID);
                updateLines();
                del_msg_label->setText("Usunięto linię!");
            }
        }
        else
        {
            bad_brigades[bad_brigades.size()-2] = '.';
            QMessageBox dialog(QMessageBox::Warning,"Nie można usunąć linii","Nie można usunąć linii " + line_name + " z powodu istniejących kursów. Najpierw usuń kursy tej linii z następujących brygad:\n" + bad_brigades, QMessageBox::Ok);
            dialog.exec();
        }
    }
}

void LineWidget::resetAddInfoLine()
{
    add_msg_label->setText("");
}

void LineWidget::resetAddBoxes()
{
    got_number->clear();
    got_name->clear();
}

void LineWidget::resetDelInfoLine()
{
    del_msg_label->setText("");
}

void LineWidget::resetDelBoxes()
{
    resetDelInfoLine();
    choose_line->setCurrentIndex(0);
}
