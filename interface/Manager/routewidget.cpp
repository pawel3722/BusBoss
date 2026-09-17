
#include "routewidget.h"
#include "interface/Manager/manager.h"
#include "qinputdialog.h"
#include "qmessagebox.h"

RouteWidget::RouteWidget(Data* data, QWidget *parent)
    : QWidget{parent}
{
    this->data = data;
    this->parent = parent;
    ret_button = new QPushButton("Powrót");
        connect(ret_button,&QPushButton::clicked,static_cast<Manager*>(parent),&Manager::showMenu);
        connect(ret_button,&QPushButton::clicked,this,&RouteWidget::deactivateBoxes);

    choose_route_label = new QLabel("Wybierz trasę:");
    got_route = new QComboBox;
    connect(got_route,&QComboBox::currentIndexChanged,this,&RouteWidget::deactivateBoxes);

    confirm_route = new QPushButton("Wybierz");
        connect(confirm_route,&QPushButton::clicked,this,&RouteWidget::activateBoxes);

    chosen_route_label = new QLabel;
    time_msg_label = new QLabel;
    add_time_button = new QPushButton ("Dodaj czas");
        connect(add_time_button,&QPushButton::clicked,this,&RouteWidget::addTime);
        add_time_button->setEnabled(false);
    del_time_button = new QPushButton ("Usuń czas");
        connect(del_time_button,&QPushButton::clicked,this,&RouteWidget::delTime);
        del_time_button->setEnabled(false);
    confirm_changes = new QPushButton ("Zapisz czasy");
        connect(confirm_changes,&QPushButton::clicked,this,&RouteWidget::confirmChanges);
        confirm_changes->setEnabled(false);
    change_direction = new QPushButton ("Zmień kierunek");
        connect(change_direction,&QPushButton::clicked,this,&RouteWidget::changeDirection);
        change_direction->setEnabled(false);
    del_route_button = new QPushButton ("Usuń trasę");
        connect(del_route_button,&QPushButton::clicked,this,&RouteWidget::delRoute);
        del_route_button->setEnabled(false);

    layout = new QGridLayout(this);
    layout->setColumnMinimumWidth(1,150);
    layout->setColumnMinimumWidth(4,60);
    layout->setRowMinimumHeight(5,25);
    layout->addWidget(ret_button,1,1,1,1);
    layout->addWidget(choose_route_label,2,1,1,2);
    layout->addWidget(got_route,3,1,1,2);
    layout->addWidget(confirm_route,4,1,1,2);
    layout->addWidget(chosen_route_label,5,1,1,2);
    layout->addWidget(time_msg_label,6,1,1,2);
    layout->addWidget(add_time_button,7,1,1,2);
    layout->addWidget(del_time_button,8,1,1,2);
    layout->addWidget(confirm_changes,9,1,1,2);
    layout->addWidget(change_direction,10,1,1,2);
    layout->addWidget(del_route_button,11,1,1,2);

    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Minimum),0,50);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding),50,0);
    layout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred),50,50);
}

void RouteWidget::updateRoutes()
{
    got_route->clear();
    for(const auto& el : *(data->getRoutes()))
        got_route->addItem(QString::fromStdString(el.second->getID() + " " + el.second->getDirection()));
}

void RouteWidget::activateBoxes()
{
    auto routes = *(data->getRoutes());
    auto name = got_route->currentText().toStdString().substr(0,8);
    if(name.size())
    {
        chosen_route = routes[name].get();
        chosen_route_label->setText("Wybrana trasa: " + QString::fromStdString(chosen_route->getID()));
        auto connections = *(chosen_route->getConnections());
        const std::vector<std::vector<int>> timescales = *(chosen_route->getTimescales());
        auto cit = connections.begin();

        deactivateBoxes();
        add_time_button->setEnabled(true);
        del_time_button->setEnabled(true);
        confirm_changes->setEnabled(true);
        change_direction->setEnabled(true);
        del_route_button->setEnabled(true);
        auto stop_1_label = new QLabel("1. " + QString::fromStdString((*cit)->get1StopPointer()->getName()));
        stop_labels.push_back(stop_1_label);
        layout->addWidget(stop_1_label,2,3);

        for(int i = 0; i < connections.size(); ++i)
        {
            auto stop_i_label = new QLabel(QString::number(i+2)+". "+QString::fromStdString((*cit)->get2StopPointer()->getName()));
            stop_labels.push_back(stop_i_label);
            ++cit;
        }

        for(int i = 0; i < stop_labels.size(); ++i)
        {
            layout->addWidget(stop_labels[i],2 + ((2 * i)%30) ,3 + 7 * (i/15));
        }

        for(int current_timescale = 0; current_timescale < timescales.size(); ++current_timescale)
        {
            addColumn(current_timescale);
        }
    }
}

void RouteWidget::deactivateBoxes()
{
    for(int i = 0; i < stop_labels.size(); ++i)
    {
        layout->removeItem(layout->itemAtPosition(2 + ((2 * i)%30) ,3 + 7 * (i/15)));
        delete stop_labels[i];
        stop_labels[i] = nullptr;
    }
    for(int i = 0; i < timescale_labels.size(); ++i)
    {
        layout->removeItem(layout->itemAtPosition(1,4+i));
        delete timescale_labels[i];
        timescale_labels[i] = nullptr;
    }
    for(int i = 0; i < time_labels.size(); ++i)
    {
        for(int j = 0; j < time_labels[i].size(); ++j)
        {
            layout->removeItem(layout->itemAtPosition(3 + (2*j) % 28,4+ i + 7 * ((2*j)/28)));
            delete time_labels[i][j];
            time_labels[i][j] = nullptr;
        }
    }
    for(int i = 0; i < time_boxes.size(); ++i)
    {
        for(int j = 0; j < time_boxes[i].size(); ++j)
        {
            layout->removeItem(layout->itemAtPosition(3 + (2*j) % 28,4+ i + 7 * ((2*j)/28)));
            delete time_boxes[i][j];
            time_boxes[i][j] = nullptr;
        }
    }
    stop_labels.clear();
    timescale_labels.clear();
    time_labels.clear();
    time_boxes.clear();
    chosen_route_label->setText("Wybrana trasa: ");
    time_msg_label->setText("");
    add_time_button->setEnabled(false);
    del_time_button->setEnabled(false);
    confirm_changes->setEnabled(false);
    change_direction->setEnabled(false);
    del_route_button->setEnabled(false);
}

void RouteWidget::updateTimeLabels()
{
    for(int i = 0; i < time_boxes.size(); ++i)
    {
        for(int j = 0; j < time_boxes[i].size(); ++j)
        {
            time_labels[i][j+1]->setText(Connection::addTime(QTime::fromString(time_labels[i][j]->text(),"hh:mm:ss"),QTime::fromString(time_boxes[i][j]->currentText(),"mm:ss")).toString("hh:mm:ss"));
        }
    }
}

void RouteWidget::addColumn(int current_timescale)
{

    auto connections = *(chosen_route->getConnections());
    const std::vector<std::vector<int>> timescales = *(chosen_route->getTimescales());

    auto nl = new QLabel ("Czas " + QString::number(current_timescale) + ":");
    timescale_labels.push_back(nl);
    layout->addWidget(nl,1,4+current_timescale);

    std::vector<QLabel*> time_lab_1;
    std::vector<QComboBox*> time_box_1;
    auto time_1_lab = new QLabel("00:00:00");
    time_lab_1.push_back(time_1_lab);
    layout->addWidget(time_1_lab,2,4+current_timescale);
    auto cit = connections.begin();

    for(int i = 0 ; i < 2*connections.size() ; ++i)
    {
        if(i % 2)
        {
            auto lab = new QLabel;
            time_lab_1.push_back(lab);
            layout->addWidget(lab,1 + (i+2) % 30, 4 + current_timescale + 7 * ((i+1)/30));
        }
        else
        {
            auto cb = new QComboBox;
            cb->addItem((*cit)->getTime(0).toString("mm:ss"));
            cb->addItem((*cit)->getTime(1).toString("mm:ss"));
            cb->addItem((*cit)->getTime(2).toString("mm:ss"));
            time_box_1.push_back(cb);

            if(current_timescale < timescales.size())
                cb->setCurrentIndex(timescales[current_timescale][i/2]);
            else
                cb->setCurrentIndex(timescales[0][i/2]);
            layout->addWidget(cb,3 + i % 30, 4 + current_timescale + 7 * (i/30));
            connect(cb,&QComboBox::currentIndexChanged,this,&RouteWidget::updateTimeLabels);
            ++cit;
        }

    }
    time_labels.push_back(time_lab_1);
    time_boxes.push_back(time_box_1);
    updateTimeLabels();
}

void RouteWidget::delColumn()
{
    int col = (int) timescale_labels.size()-1;
    layout->removeItem(layout->itemAtPosition(1,4+col));
    delete timescale_labels[col];
    timescale_labels.pop_back();

    for(int j = 0; j < time_labels[col].size(); ++j)
    {
        layout->removeItem(layout->itemAtPosition((2*j+2) % 40, 4 + 10 * (col/40)));
        delete time_labels[col][j];
    }
    time_labels.pop_back();
    for(int j = 0; j < time_boxes[col].size(); ++j)
    {
        layout->removeItem(layout->itemAtPosition((2*j+3) % 40, 4 + 10 * (col/40)));
        delete time_boxes[col][j];
    }
    time_boxes.pop_back();
}

void RouteWidget::addTime()
{
    int size = (int) timescale_labels.size();
    if(size < 6)
    {
        addColumn(size);
        del_time_button->setEnabled(true);
        if(size == 5)
            add_time_button->setEnabled(false);
    }
}

void RouteWidget::delTime()
{
    auto size = timescale_labels.size();
    if(size > 1)
    {
        bool found = false;
        auto services = *data->getServices();
        for(const auto& el : services)
        {
            if(el.second->getTimescale()==size-1)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            delColumn();
            add_time_button->setEnabled(true);
            if(size == 2)
                del_time_button->setEnabled(false);
        }
        else
        {
            QMessageBox dial(QMessageBox::Warning,"Nie można usunąć czasu","Nie można usunąć czasu " + QString::number(size - 1) + ", ponieważ istnieją kursy korzystające z niego.", QMessageBox::Ok);
            dial.exec();
        }
    }
}

void RouteWidget::confirmChanges()
{
    std::vector<std::vector<int>> matrix;
    std::vector<int> vec;
    for(int i = 0; i < time_boxes.size(); ++i)
    {
        for(int j = 0; j < time_boxes[i].size(); ++j)
        {
            vec.push_back(time_boxes[i][j]->currentIndex());
        }
        matrix.push_back(vec);
        vec.clear();
    }
    chosen_route->setTimescales(matrix);
    time_msg_label->setText("Zapisano!");
}

void RouteWidget::changeDirection()
{
    bool ok;
    auto new_direction = QInputDialog::getText(this,"Zmiana kierunku","Podaj nowy kierunek:",QLineEdit::Normal,QString::fromStdString(chosen_route->getDirection()),&ok);
    if (ok && !new_direction.isEmpty())
    {
        chosen_route->setDirection(new_direction.toStdString());
        updateRoutes();
    }
}

void RouteWidget::delRoute()
{
    auto services = *(data->getServices());
    bool found = false;
    auto ID = chosen_route->getID();
    for(const auto& el : services)
    {
        if(el.second->getRoute()==chosen_route)
        {
            found = true;
            break;
        }
    }
    if(!found)
    {
        QMessageBox dial(QMessageBox::Question,"Usuwanie trasy","Czy na pewno chcesz usunąć trasę " + QString::fromStdString(ID) +"?", QMessageBox::Ok | QMessageBox::Cancel);
        int result = dial.exec();
        if(result == QMessageBox::Ok)
        {
            (*data->getLines())[chosen_route->getLineID()]->delRoute(chosen_route);
            data->getRoutes()->erase(ID);
            chosen_route=nullptr;
            deactivateBoxes();
            updateRoutes();
        }
    }
    else
    {
        QMessageBox dial(QMessageBox::Warning,"Nie można usunąć trasy","Nie można usunąć trasy " + QString::fromStdString(ID) + ", ponieważ istnieją kursy korzystające z niej.", QMessageBox::Ok);
        dial.exec();
    }
}


