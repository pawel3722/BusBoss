
#include "stopwidget.h"
#include "interface/Manager/manager.h"
#include "qgridlayout.h"
#include "qlayoutitem.h"
#include "qmessagebox.h"

StopWidget::StopWidget(Data* data, QWidget *parent)
    : QWidget{parent}
{
    this->data = data;
    this->parent = parent;
    ret_button = new QPushButton("Powrót");
        connect(ret_button,&QPushButton::clicked,static_cast<Manager*>(parent),&Manager::showMenu);
        connect(ret_button,&QPushButton::clicked,this,&StopWidget::resetAddBoxes);
        connect(ret_button,&QPushButton::clicked,this,&StopWidget::resetAddInfoLine);
        connect(ret_button,&QPushButton::clicked,this,&StopWidget::resetDelInfoLine);
        connect(ret_button,&QPushButton::clicked,this,&StopWidget::deactivateBoxes);

    choose_stop_label = new QLabel("Wybierz przystanek:");
    got_stop = new QComboBox;
    chosen_stop_label = new QLabel;

    confirm_stop = new QPushButton("Wybierz");
        connect(confirm_stop,&QPushButton::clicked,this,&StopWidget::activateBoxes);
    add_msg_label = new QLabel;
    add_info_label = new QLabel("<b>Dodaj stanowisko:</b>");
    add_desc_label = new QLabel("Podaj numer stanowiska:");
    got_number = new QSpinBox;
    got_number->setMinimum(1);
    got_number->setMaximum(99);
        connect(got_number,&QSpinBox::textChanged,this,&StopWidget::resetAddInfoLine);
    add_ok_button = new QPushButton("Dodaj");
        connect(add_ok_button,&QPushButton::clicked,this,&StopWidget::addPlatform);

    del_msg_label = new QLabel;
    del_info_label = new QLabel("<b>Usuń stanowisko:</b>");
    del_desc_label = new QLabel("  Wybierz stanowisko:  ");
    got_platform = new QComboBox;
    connect(got_platform,&QComboBox::currentIndexChanged,this,&StopWidget::resetDelInfoLine);

    del_ok_button = new QPushButton("Usuń");
        connect(del_ok_button,&QPushButton::clicked,this,&StopWidget::delPlatform);

       auto layout = new QGridLayout(this);
            layout->addWidget(ret_button,1,1,1,1);
            layout->addWidget(choose_stop_label,2,1,1,2);
            layout->addWidget(got_stop,3,1,1,2);
            layout->addWidget(confirm_stop,4,1,1,2);
            layout->addWidget(chosen_stop_label,5,1,1,2);
            layout->addWidget(add_msg_label,6,1,1,1);
            layout->addWidget(del_msg_label,6,2,1,1);
            layout->addWidget(add_info_label,7,1,1,1);
            layout->addWidget(del_info_label,7,2,1,1);
            layout->addWidget(add_desc_label,8,1,1,1);
            layout->addWidget(del_desc_label,8,2,1,1);
            layout->addWidget(got_number,9,1,1,1);
            layout->addWidget(got_platform,9,2,1,1);
            layout->addWidget(add_ok_button,10,1,1,1);
            layout->addWidget(del_ok_button,10,2,1,1);

       layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,0);
       layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,6);
       layout->addItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding),11,0);
       layout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred),11,6);
       this->setLayout(layout);
       deactivateBoxes();
}

void StopWidget::updateStops()
{
    got_stop->clear();
    for(const auto& el: *(data->getBusStops()))
    {
       auto name = el.second->getName();
       got_stop->addItem(QString::fromStdString(name));
       stop_names[name] = el.second->getID();
    }
}

void StopWidget::updatePlatforms()
{
    got_platform->clear();
    got_platform->addItem("(wybierz)");
    for(const auto& el : *chosen_stop->getPlatforms())
        got_platform->addItem(QString::number(el));
}

void StopWidget::activateBoxes()
{
    auto name = got_stop->currentText().toStdString();
    if(name.size())
    {
        chosen_stop = (*data->getBusStops())[stop_names[name]].get();
        updatePlatforms();
        chosen_stop_label->setText("Wybrany przystanek:\n"+QString::fromStdString(chosen_stop->getName()));
        got_platform->setEnabled(true);
        got_number->setEnabled(true);
        add_ok_button->setEnabled(true);
        del_ok_button->setEnabled(true);
    }
}

void StopWidget::deactivateBoxes()
{
   got_number->setEnabled(false);
   add_ok_button->setEnabled(false);
   got_platform->setEnabled(false);
   del_ok_button->setEnabled(false);
   chosen_stop_label->setText("Wybrany przystanek:\n ");
}

void StopWidget::addPlatform()
{
   auto new_platform = got_number->value();
   auto local_platforms = *(chosen_stop->getPlatforms());
   if(std::find(local_platforms.begin(),local_platforms.end(),new_platform) == local_platforms.end())
   {
        chosen_stop->addPlatform(new_platform);
        add_msg_label->setText("Dodano stanowisko!");
   }
   else
        add_msg_label->setText("Stanowisko " + QString::number(new_platform) + " już istnieje!");
   updatePlatforms();
}

void StopWidget::delPlatform()
{
   if(got_platform->currentText()!="(wybierz)")
   {
        auto del_platform = got_platform->currentText().toInt();
        auto local_platforms = chosen_stop->getRoutes();
        QString bad_routes;
        for(const auto& el : *local_platforms[del_platform])
            bad_routes += QString::fromStdString(el+", ");
        if(!bad_routes.size())
        {
            chosen_stop->deletePlatform(del_platform);
            updatePlatforms();
            del_msg_label->setText("Usunięto stanowisko!");
        }
        else
        {
            bad_routes[bad_routes.size()-2] = '.';
            QMessageBox dialog(QMessageBox::Warning,"Nie można usunąć stanowiska","Nie można usunąć tego stanowiska z powodu tras korzystających z niego. Najpierw usuń następujące trasy:\n"+bad_routes, QMessageBox::Ok );
            dialog.exec();
        }
   }
}

void StopWidget::resetAddInfoLine()
{
   add_msg_label->setText("");
}

void StopWidget::resetAddBoxes()
{
   got_number->setValue(1);
}

void StopWidget::resetDelInfoLine()
{
   del_msg_label->setText("");
}
