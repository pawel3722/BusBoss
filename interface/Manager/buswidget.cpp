
#include "buswidget.h"
#include "interface/Manager/manager.h"
#include "qmessagebox.h"

BusWidget::BusWidget(Data* data,QWidget *parent)
    : QWidget{parent}
{
    this->data = data;
    this->parent = parent;

    ret_button = new QPushButton("Powrót");
        connect(ret_button,&QPushButton::clicked,static_cast<Manager*>(parent),&Manager::showMenu);
        connect(ret_button,&QPushButton::clicked,this,&BusWidget::resetAddBoxes);
        connect(ret_button,&QPushButton::clicked,this,&BusWidget::resetAddInfoLine);
        connect(ret_button,&QPushButton::clicked,this,&BusWidget::resetDelBoxes);
        connect(ret_button,&QPushButton::clicked,this,&BusWidget::resetDelInfoLine);

    add_msg_label = new QLabel;
    add_info_label = new QLabel("<b>Dodaj autobus:</b>");
    add_desc1_label = new QLabel("Podaj numer przewoźnika:");
    add_desc2_label = new QLabel("Podaj numer autobusu:");
    add_desc3_label = new QLabel("Podaj model autobusu:");
    add_desc4_label = new QLabel("Wybierz typ autobusu:");

    company_number = new QSpinBox;
    company_number->setMaximum(999);
        connect(company_number,&QSpinBox::textChanged,this,&BusWidget::resetAddInfoLine);
    bus_number = new QSpinBox;
    bus_number->setMaximum(9999);
        connect(bus_number,&QSpinBox::textChanged,this,&BusWidget::resetAddInfoLine);
    got_name = new QLineEdit;
        connect(got_name,&QLineEdit::textChanged,this,&BusWidget::resetAddInfoLine);
    got_type = new QComboBox;
        got_type->addItem("MINI");
        got_type->addItem("MIDI");
        got_type->addItem("MAXI");
        got_type->addItem("MEGA");

    add_multiple = new QCheckBox("Dodaj wiele autobusów");
            connect(add_multiple,&QCheckBox::stateChanged,this,&BusWidget::enableQuantityLine);
    add_desc5_label = new QLabel("Podaj ilość:");
    got_quantity = new QSpinBox;
        got_quantity->setEnabled(false);
        got_quantity->setMinimum(2);
        got_quantity->setMaximum(100);

    add_ok_button = new QPushButton("Dodaj");
    connect(add_ok_button,&QPushButton::clicked,this,&BusWidget::addBus);

    del_msg_label = new QLabel;
    del_info_label = new QLabel("<b>Usuń autobus:</b>");
    del_desc_label = new QLabel("Wybierz autobus:");
    choose_bus = new QComboBox;
    connect(choose_bus,&QComboBox::currentIndexChanged,this,&BusWidget::resetDelInfoLine);
    choose_bus->addItem("(wybierz autobus)");
    updateBuses();

    del_ok_button = new QPushButton("Usuń");
        connect(del_ok_button,&QPushButton::clicked,this,&BusWidget::delBus);

    int row = 0;
    auto layout = new QGridLayout(this);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),row,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),row++,6);
    layout->addWidget(ret_button,row++,1,1,1);
    layout->addWidget(add_msg_label,row++,1,1,2);
    layout->addWidget(add_info_label,row++,1,1,2);
    layout->addWidget(add_desc1_label,row++,1,1,2);
    layout->addWidget(company_number,row++,1,1,2);
    layout->addWidget(add_desc2_label,row++,1,1,2);
    layout->addWidget(bus_number,row++,1,1,2);
    layout->addWidget(add_desc3_label,row++,1,1,2);
    layout->addWidget(got_name,row++,1,1,2);
    layout->addWidget(add_desc4_label,row++,1,1,2);
    layout->addWidget(got_type,row++,1,1,2);
    layout->addWidget(add_multiple,row++,1,1,2);
    layout->addWidget(add_desc5_label,row++,1,1,2);
    layout->addWidget(got_quantity,row++,1,1,2);
    layout->addWidget(add_ok_button,row++,1,1,2);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding),row,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred),row,6);
    row = 2;
    layout->addWidget(del_msg_label,row++,4,1,2);
    layout->addWidget(del_info_label,row++,4,1,2);
    layout->addWidget(del_desc_label,row++,4,1,2);
    layout->addWidget(choose_bus,row++,4,1,2);
    layout->addWidget(del_ok_button,row++,4,1,2);
    layout->setRowMinimumHeight(2,40);
    layout->setColumnMinimumWidth(1,180);

    this->setLayout(layout);
    resetAddBoxes();
    resetDelBoxes();
}

void BusWidget::updateBuses()
{
    choose_bus->clear();
    choose_bus->addItem("(wybierz autobus)");
    auto lines = data->getBuses();
    for(const auto& el: *lines)
    {
        auto name = el.second->getID() + " " + el.second->getModel();
        choose_bus->addItem(QString::fromStdString(name));
        bus_names[name] = el.second->getID();
    }
}


void BusWidget::addBus()
{
    int types [] {7,9,12,18};
    auto buses = data->getBuses();
    std::vector<std::shared_ptr<Bus>> local_buses;

    auto ID = company_number->text().toStdString() + "/";
    auto name = got_name->text().toStdString();
    if(name.size()==0 || ID.size() == 1)
        return;
    auto type = types[got_type->currentIndex()];
    int quantity = 1;
    if(add_multiple->isChecked())
        quantity = got_quantity->value();

    bool found = false;
    int num = bus_number->text().toInt();
    for(int i = 0; i < quantity; ++i)
    {
        auto final_ID = ID + std::to_string(num);
        if(buses->find(final_ID) != buses->end())
        {
            found = true;
            break;
        }
        auto bus = std::make_shared<Bus>(final_ID,name,type);
        local_buses.push_back(bus);
        ++num;
    }

    if(!found)
    {
        for(const auto& el : local_buses)
        {
            (*buses)[el->getID()] = el;
        }
        resetAddBoxes();
        if(local_buses.size()==1)
            add_msg_label->setText("Dodano autobus!");
        else
            add_msg_label->setText("Dodano autobusy!");
        updateBuses();
    }
    else
        add_msg_label->setText("Anulowano operację!\nAutobus o podanym ID istnieje!");
    got_quantity->setEnabled(false);
}

void BusWidget::delBus()
{
    auto b_name = choose_bus->currentText();
    if(b_name != "(wybierz autobus)")
    {
        auto buses = data->getBuses();
        auto ID = bus_names[b_name.toStdString()];
        QString bad_brigades;
        for(const auto& el: *data->getShifts())
        {
            if(el.second->getBusID()==ID)
            {
                bad_brigades += QString::fromStdString(el.second->getID()+", ");
            }
        }
        if(!bad_brigades.size())
        {
            QMessageBox dial(QMessageBox::Question,"Usuń autobus","Czy na pewno chcesz usunąć autobus " + b_name + "?", QMessageBox::Ok | QMessageBox::Cancel);
            int result = dial.exec();
            if(result == QMessageBox::Ok)
            {
                buses->erase(ID);
                updateBuses();
                del_msg_label->setText("Usunięto autobus!");
            }
        }
        else
        {
            QMessageBox dialog(QMessageBox::Warning,"Nie można usunąć autobusu","Nie można usunąć autobusu " + b_name + " z powodu istniejących przydziałów. Najpierw usuń przydziały tego autobusu do następujących zmian:\n" + bad_brigades, QMessageBox::Ok);
            dialog.exec();
        }
    }
}

void BusWidget::resetAddInfoLine()
{
    add_msg_label->setText("");
}

void BusWidget::resetAddBoxes()
{
    company_number->clear();
    bus_number->clear();
    got_name->clear();
}

void BusWidget::resetDelInfoLine()
{
    del_msg_label->setText("");
}

void BusWidget::resetDelBoxes()
{
    resetDelInfoLine();
    choose_bus->setCurrentIndex(0);
}
