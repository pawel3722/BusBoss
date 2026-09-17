#include "interface/BusMap/busmap.h"
#include "qscrollarea.h"
#include <QMenu>
#include <QPainter>
#include <QMessageBox>
#include <QInputDialog>
#include <QGroupBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <set>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSpinBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <set>

BusMap::BusMap(Data* d,QWidget *parent)
    : QWidget{parent}
{
    data = d;
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&BusMap::update));
    finish_button = new QPushButton {"Koniec",this};
    finish_button->setStyleSheet("color: white; background-color: red");
    finish_button->hide();
    finish_button->setGeometry(0,0,50,20);
        connect(finish_button,SIGNAL(clicked()),this,SLOT(finishButtonClicked()));
    simulation_time = QTime::currentTime();
    simulation_date = QDate::currentDate();
    day_type = data->getDayType(simulation_date).first;
    setPreviousDate();

    timer->start(16);
       connect(timer,&QTimer::timeout,this,QOverload<>::of(&BusMap::update));
       connect(timer,&QTimer::timeout,this,&BusMap::addTime);
    setAttribute(Qt::WA_StaticContents);
    map_name = QString::fromStdString(data->getDir()) + "/map.jpg";
    if(openMap(map_name))
    {
       readMapItems();
       drawMap();
    }
}

BusMap::~BusMap()
{

}

bool BusMap::openMap(const QString& fileName)
{
    QImage loadedImage;
        if (!loadedImage.load(fileName))
            return false;

        displayed_map = loadedImage;

        update();
        return true;
}

void BusMap::setSimMode()
{
    if(edit_mode)
    {
        for(const auto& el: intersections)
            el.second->hide();
        edit_mode=0;      
    }
    readMapItems();
    memorised_intersection = nullptr;
    memorised_bus_stop = nullptr;
    memorised_road = nullptr;
    action=BusMapAction::empty;
    update();
}
void BusMap::setEditMode()
{
    for(const auto& el: intersections)
        el.second->hide();
    for(const auto& el : buses)
        el.second->hide();
    edit_mode=1;
    memorised_intersection = nullptr;
    memorised_bus_stop = nullptr;
    memorised_road = nullptr;
    action=BusMapAction::empty;
    update();
}

void BusMap::addBusStop()
{
   setEditMode();
   action = BusMapAction::add_stop;
}

void BusMap::addIntersection()
{
   setEditMode();
   action = BusMapAction::add_intersect;
}

void BusMap::addRoad()
{
    setEditMode();
    action = BusMapAction::add_road;
}

void BusMap::addRoute(std::string line)
{
    adding_line = line;
    finish_button->show();
    setEditMode();
    action = BusMapAction::add_route;
    update();
}

void BusMap::deleteIntersection()
{
   setEditMode();
   action = BusMapAction::delete_intersect;
}

void BusMap::deleteRoad()
{
    setEditMode();
   action = BusMapAction::delete_road;
}

void BusMap::divideRoadwithIntersection()
{
   setEditMode();
   action = BusMapAction::div_rd_int;
}

void BusMap::divideRoadwithBusStop()
{
   setEditMode();
   action = BusMapAction::div_rd_stop;
}

void BusMap::setTime(QTime t)
{
    if(t < simulation_time)
    {
        for(auto& el : buses)
            el.second->resetCurrentService();
    }
    simulation_time = t;
}

void BusMap::setDate(QDate d, int dt)
{
    simulation_date = d;
    day_type = dt;
    setPreviousDate();
    for(auto& el : buses)
        el.second->setCurrentDay(dt,prev_day_type,simulation_time);
}

void BusMap::drawMap()
{
    QPainter painter(&displayed_map);
    painter.setRenderHint(QPainter::Antialiasing);
    QColor kolor (0,0,0);
    painter.setPen(QPen(kolor, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for(const auto& el: roads)
    {
        auto pair = el.second->getPoints();
         painter.drawLine(*pair.first,*pair.second);
    }
}

void BusMap::readMapItems()
{
    auto int_ptr = data->getIntersections();
    for(const auto& el: *int_ptr)
    {
         auto ID = el.second->getID();
         auto BMI = std::make_shared<BusMapIntersection>(el.second.get(),this);
         connect(BMI.get(),SIGNAL(clicked()),this,SLOT(intersectionClicked()));
         intersections[ID] = BMI;
    }
    auto stop_ptr = data->getBusStops();
    for(const auto& el: *stop_ptr)
    {
         auto ID = el.second->getID();
         auto BMS = std::make_shared<BusMapStop>(el.second.get(),this);
         connect(BMS.get(),SIGNAL(clicked()),this,SLOT(intersectionClicked()));
         connect(BMS.get(),SIGNAL(clicked()),this,SLOT(busStopClicked()));
         bus_stops[ID] = BMS;
         intersections[ID] = BMS;
    }
    auto road_ptr = data->getRoads();
    for(const auto& el: *road_ptr)
    {
         auto ID = el.second->getID();
         auto pID1 = el.second->getPoint1ID();
         auto pID2 = el.second->getPoint2ID();
         auto p1 = intersections.at(pID1).get();
         auto p2 = intersections.at(pID2).get();
         auto BMR = std::make_shared<BusMapRoad>(ID,p1,p2,el.second.get());
         roads[ID] = BMR;
         roads_graph[p1][p2]=BMR.get();
    }
    auto job_ptr = data->getJobs();
    for(const auto& el : *job_ptr)
    {
         auto ID = el.second->getID();
         auto BMB = std::make_shared<BusMapBus>(ID,el.second.get(),this);
         connect(BMB.get(),SIGNAL(clicked()),this,SLOT(busClicked()));
         buses[ID] = BMB;
         BMB->setCurrentDay(day_type,prev_day_type,simulation_time);
    }
    lines = data->getLines();
    routes = data->getRoutes();
    connections = data->getConnections();
}

void BusMap::paintEvent(QPaintEvent *event)
{
    if(!edit_mode)
         for(const auto& el : buses)
           el.second->setPosition(simulation_time,data,day_type,prev_day_type);
    if(action == BusMapAction::add_route && memorised_intersection == nullptr)
         for(const auto& el:bus_stops)
            el.second->showPoint();
    if(action != BusMapAction::add_route)
    {
        if(edit_mode)
            for(const auto& el:intersections)
            {
                el.second->showPoint();
            }
        else
            for(const auto& el:bus_stops)
            {
                el.second->showPoint();
            }
    }
    drawMap();
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, displayed_map, dirtyRect);
}

void BusMap::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && edit_mode)
    {
        auto point = event->position().toPoint();
        switch(action)
        {
        case BusMapAction::add_stop:
            createBusStop(point);
            break;
        case BusMapAction::add_intersect:
            createIntersection(point);
            break;
        case BusMapAction::div_rd_int:
            divideRoad(memorised_intersection,point,0);
            break;
        case BusMapAction::div_rd_stop:
            divideRoad(memorised_intersection,point,1);
            break;
        default:
            break;
        }
       update();
    }
}

void BusMap::intersectionClicked()
{
    BusMapIntersection *clicked_intersection = static_cast<BusMapIntersection*>(sender());
    if(edit_mode)
    {
        switch(action)
        {
        case BusMapAction::add_road:
            createRoad(clicked_intersection);
            break;
        case BusMapAction::add_route:
            createRoute(clicked_intersection);
            break;
        case BusMapAction::delete_intersect:
            if(dynamic_cast<BusMapStop*>(clicked_intersection))
                removeBusStop(clicked_intersection);
            else
                removeIntersection(clicked_intersection);
            action = BusMapAction::empty;
            break;
        case BusMapAction::delete_road:
            removeRoad(clicked_intersection);
            break;
        case BusMapAction::div_rd_int:
            divideRoad(clicked_intersection,{0,0},0);
            break;
        case BusMapAction::div_rd_stop:
            divideRoad(clicked_intersection,{0,0},1);
            break;
        default:
            break;
        }
        update();
    }
}

void BusMap::busStopClicked()
{
    BusMapStop *clicked_bus_stop = static_cast<BusMapStop*>(sender());
    if(!edit_mode)
    {
        showBusStopInfo(clicked_bus_stop);
    }
}
void BusMap::finishButtonClicked()
{
    if(creating_route.size() > 0)
    {
        std::string ID = adding_line, direction;

        QDialog dialog;
        QVBoxLayout layout;
        QLabel label1 ("Podaj numer wariantu:"), label2 ("Podaj kierunek:");
        QSpinBox got_number; got_number.setMinimum(1); got_number.setMaximum(98); got_number.setValue( (int) data->getLines()->at(adding_line)->getRoutes().size() + 1);
        QLineEdit got_direction (QString::fromStdString((*(creating_route.rbegin()))->get2StopPointer()->getName()));
        QDialogButtonBox button_box (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        connect(&button_box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&button_box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        layout.addWidget(&label1);
        layout.addWidget(&got_number);
        layout.addWidget(&label2);
        layout.addWidget(&got_direction);
        layout.addWidget(&button_box);
        dialog.setLayout(&layout);
        dialog.setWindowTitle("Wariant");

        do
        {
            dialog.exec();
            auto val = got_number.value();
            val < 10 ? ID += "/0" + std::to_string(val) : ID += "/" + std::to_string(val);
            direction = got_direction.text().toStdString();
            label1.setText("<b>NUMER ZAJĘTY</b></p><p>Podaj numer wariantu:");

        } while(dialog.result() && routes->find(ID) != routes->end());

        if(dialog.result() && ID != adding_line + "/")
        {
            if(direction.size()==0)
            {
                direction = (*(creating_route.rbegin()))->get2StopPointer()->getName();
            }
            std::vector<int> ts (creating_route_platforms.size()-1);
            std::vector<std::vector<int>> timescales {ts};
            auto rt = std::make_shared<Route>(ID,direction,creating_route,creating_route_platforms,timescales);
            (*routes)[ID] = rt;
            (*lines)[adding_line]->addRoute(rt.get());
        }
    }
    creating_route.clear();
    creating_connection.clear();
    creating_route_platforms.clear();
    memorised_intersection=nullptr;
    action=BusMapAction::empty;
    finish_button->hide();
    displayed_map.load(map_name);
    drawMap();
    update();
}

void BusMap::busClicked()
{
    BusMapBus *clicked_bus = static_cast<BusMapBus*>(sender());
    if(!edit_mode)
    {
        auto job = clicked_bus->getJobPointer();
        auto service = job->getCurrentService();
        auto shift = job->getCurrentShift();

        QDialog dialog;
        QVBoxLayout layout;
        QLabel line_label ("Linia: " + QString::fromStdString(lines->at(service->getLineID())->getName()));
        QLabel direction_label ("Kierunek: " + QString::fromStdString(service->getRoute()->getDirection()));
        QLabel bus_num_label ("Numer pojazdu: " + QString::fromStdString(shift->getBus()->getID()));
        QLabel bus_model_label ("Model pojazdu: " + QString::fromStdString(shift->getBus()->getModel()));
        QLabel bus_type_label ("Typ pojazdu: " + QString::fromStdString(shift->getBus()->getType()));
        QLabel worker_label ("Kierowca: " + QString::fromStdString(shift->getWorker()->getID() + " " + shift->getWorker()->getName()));
        QLabel delay_label ("Opóźnienie: 0 min");

        QDialogButtonBox button_box (QDialogButtonBox::Ok);
        connect(&button_box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

        layout.addWidget(&line_label);
        layout.addWidget(&direction_label);
        layout.addWidget(&bus_num_label);
        layout.addWidget(&bus_model_label);
        layout.addWidget(&bus_type_label);
        layout.addWidget(&worker_label);
        layout.addWidget(&delay_label);
        layout.addWidget(&button_box);
        dialog.setLayout(&layout);
        dialog.setWindowTitle(QString::fromStdString(service->getID()));
        dialog.exec();
    }
}

void BusMap::createBusStop(const QPoint &point)
{
    std::string ID,name;
    int nums;
    std::vector<int> platforms;

    QDialog dialog;
    QVBoxLayout layout;
    QLabel label1 ("Podaj ID przystanku:"), label2 ("Podaj nazwę przystanku:"), label3("Podaj ilość stanowisk:");
    QLineEdit got_ID, got_name;
    got_ID.setMaxLength(4);
    QSpinBox got_number; got_number.setMinimum(1);
    QDialogButtonBox button_box (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(&button_box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&button_box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    layout.addWidget(&label1);
    layout.addWidget(&got_ID);
    layout.addWidget(&label2);
    layout.addWidget(&got_name);
    layout.addWidget(&label3);
    layout.addWidget(&got_number);
    layout.addWidget(&button_box);
    dialog.setLayout(&layout);
    dialog.setWindowTitle("Przystanek");

    do
    {
        dialog.exec();
        ID = got_ID.text().toStdString();
        name = got_name.text().toStdString();
        nums = got_number.value();
        label1.setText("<b>NAZWA ID ZAJĘTA</b></p><p>Podaj ID przystanku:");

    } while(dialog.result() && intersections.find(ID) != intersections.end());


    if(dialog.result() && ID.size()!=0)
    {
        for(int i = 1; i<=nums;++i)
            platforms.push_back(i);

        auto BS = std::make_shared<BusStop>(ID,point,name,platforms);
        auto BMS = std::make_shared<BusMapStop>(BS.get(),this);
        connect(BMS.get(),SIGNAL(clicked()),this,SLOT(intersectionClicked()));
        connect(BMS.get(),SIGNAL(clicked()),this,SLOT(busStopClicked()));

        data->addIntersection(ID,BS);
        data->addBusStop(ID,BS);

        intersections[ID] = BMS;
        bus_stops[ID] = BMS;
        memorised_intersection = BMS.get();
    }
}

void BusMap::createIntersection(const QPoint& point)
{
    QInputDialog dial;
    std::string ID;
    ID = dial.getText(this,"Skrzyżowanie","Podaj nazwę skrzyżowania:").toStdString();
    while(intersections.find(ID)!=intersections.end())
        ID = dial.getText(this,"Skrzyżowanie","<b>NAZWA ZAJĘTA</b></p><p>Podaj nazwę skrzyżowania:").toStdString();
    if(ID.size()!=0)
    {
        auto BI = std::make_shared<Intersection>(ID,point);
        auto BMI = std::make_shared<BusMapIntersection>(BI.get(),this);
        connect(BMI.get(),SIGNAL(clicked()),this,SLOT(intersectionClicked()));
        data->addIntersection(ID,BI);
        intersections[ID] = BMI;
        memorised_intersection = BMI.get();
    }
}

void BusMap::createRoad(BusMapIntersection* clicked_intersection)
{

    if(memorised_intersection == nullptr || memorised_intersection == clicked_intersection)
    {
        memorised_intersection = clicked_intersection;
    }
    else
    {
        std::string ID,ID1,ID2;
        bool one_way_road = false;

        QString lab = "Podaj nazwę drogi pomiędzy \""+QString::fromStdString(memorised_intersection->getID()+"\" a \""+clicked_intersection->getID()+"\":");
        QDialog dialog;
        QVBoxLayout layout;
        QLabel label (lab);
        QLineEdit line_edit;
        QCheckBox check_box ("Droga jednokierunkowa");
        QDialogButtonBox button_box (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        connect(&button_box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&button_box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        layout.addWidget(&label);
        layout.addWidget(&line_edit);
        layout.addWidget(&check_box);
        layout.addWidget(&button_box);
        dialog.setLayout(&layout);
        dialog.setWindowTitle("Droga");

        do
        {
            dialog.exec();
            one_way_road = check_box.isChecked();
            ID = line_edit.text().toStdString();
            ID1=ID+"A";
            label.setText("<b>NAZWA ZAJĘTA</b></p><p>Podaj nazwę drogi");
        } while(dialog.result() && roads.find(ID1)!=roads.end());

        if(dialog.result() && ID.size() != 0)
        {
            ID2=ID+"B";
            auto BR1 = std::make_shared<Road>(ID1,memorised_intersection->getIntersectionPointer(),clicked_intersection->getIntersectionPointer());
            auto BMR1 = std::make_shared<BusMapRoad>(ID1,memorised_intersection,clicked_intersection,BR1.get());
            data->addRoad(ID1,BR1);
            roads[ID1] = BMR1;
            roads_graph[memorised_intersection][clicked_intersection]=BMR1.get();
            if(!one_way_road)
            {
                auto BR2 = std::make_shared<Road>(ID2,clicked_intersection->getIntersectionPointer(),memorised_intersection->getIntersectionPointer());
                auto BMR2 = std::make_shared<BusMapRoad>(ID2,clicked_intersection,memorised_intersection,BR2.get());
                data->addRoad(ID2,BR2);
                roads[ID2] = BMR2;
                roads_graph[clicked_intersection][memorised_intersection]=BMR2.get();
            }
            memorised_intersection=nullptr;
        }
    }
}

void BusMap::createRoute(BusMapIntersection* clicked_intersection)
{
    std::vector<std::string> route_tracer;

    for(const auto& el: intersections)
        el.second->hide();
    for(const auto& el: roads_graph[clicked_intersection])
        el.first->showPoint();

    if(dynamic_cast<BusMapStop*>(clicked_intersection))     //wybrane skrzyzowanie jest przystankiem
    {
        if(memorised_intersection == nullptr || (memorised_intersection == clicked_intersection && memorised_bus_stop != clicked_intersection))
        {
            if(getPlatformCount(clicked_intersection))
            {
                memorised_bus_stop = clicked_intersection;
                if(connections->find(clicked_intersection->getID()) != connections->end())
                {
                    for(const auto& el: connections->at(clicked_intersection->getID()))
                    {
                        intersections[el.first]->showPoint();
                    }
                }
            }
            else
                finishButtonClicked();
        }
        else
        {
            auto mem_int_ID = memorised_intersection->getID();
            auto mem_stop_ID = memorised_bus_stop->getID();
            auto cli_int_ID = clicked_intersection->getID();
            route_tracer.push_back(mem_int_ID);
            if(connections->find(mem_stop_ID) != connections->end() &&
               connections->at(mem_stop_ID).find(cli_int_ID) != connections->at(mem_stop_ID).end())       //wybrane polaczenie istnieje
            {
                auto con = connections -> at(mem_stop_ID).at(cli_int_ID).get();
                auto rds = con -> getRoads();
                if(getPlatformCount(clicked_intersection))
                {
                    if(connections->find(clicked_intersection->getID()) != connections->end())
                    {
                        for(const auto& el: connections->at(clicked_intersection->getID()))
                        {
                            intersections[el.first]->showPoint();
                        }
                    }
                    creating_route.push_back(con);
                    memorised_bus_stop = clicked_intersection;
                    creating_connection.clear();
                }
                for(const auto& el : *rds)
                {
                    route_tracer.push_back(el->getPoint2ID());
                }
            }
            else                                                                                       //wybrane polaczenie NIE istnieje
            {
                creating_connection.push_back(roads_graph[memorised_intersection][clicked_intersection]->getRoadPointer());
                double distance;
                std::vector<QTime> times;
                QDialog dialog;
                QGridLayout layout;
                QLabel label_p ("Wybierz stanowisko:");
                std::list<QRadioButton*> platforms_list;
                QDialogButtonBox button_box (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
                connect(&button_box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
                connect(&button_box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
                QLabel label ("Podaj długość połączenia "+QString::fromStdString(memorised_bus_stop->getID()+" - "+clicked_intersection->getID() +":")), label2("Podaj czas przejazdu:");
                QDoubleSpinBox got_distance; got_distance.setMinimum(0); got_distance.setSuffix(" km"), got_distance.setSingleStep(0.05);
                QSpinBox got_min,got_sec; got_min.setMinimum(0); got_sec.setMinimum(0); got_sec.setMaximum(59); got_min.setSuffix(" m"); got_sec.setSuffix(" s");

                layout.addWidget(&label,0,0,1,2);
                layout.addWidget(&got_distance,1,0,1,2);
                layout.addWidget(&label2,2,0,1,2);
                layout.addWidget(&got_min,3,0);
                layout.addWidget(&got_sec,3,1);
                layout.addWidget(&label_p,4,0,1,2);
                int i = 5;
                for(const auto& el : *clicked_intersection->getIntersectionPointer()->getPlatforms())
                {
                    auto platform = QString::number(el);
                    QRadioButton *action = new QRadioButton(platform, this);
                    layout.addWidget(action,i,0,1,2);
                    platforms_list.push_back(action);
                    if(i==5)
                    {
                        action->setChecked(true);
                    }
                    i++;
                }
                layout.addWidget(&button_box,i,0,1,2);

                dialog.setLayout(&layout);
                dialog.setWindowTitle("Połączenie");

                dialog.exec();
                distance = got_distance.value();
                times.push_back({0,got_min.value(),got_sec.value()});
                times.push_back({0,0,0});
                times.push_back({0,0,0});

                if(dialog.result())
                {
                    if(connections->find(clicked_intersection->getID()) != connections->end())
                        for(const auto& el: connections->at(clicked_intersection->getID()))
                        {
                            intersections[el.first]->showPoint();
                        }
                    route_tracer.push_back(cli_int_ID);
                    auto con = std::make_shared<Connection>(bus_stops[mem_stop_ID]->getBusStopPointer(),bus_stops[cli_int_ID]->getBusStopPointer(),distance,times,creating_connection);
                    (*connections)[mem_stop_ID][cli_int_ID] = con;
                    for(const auto& el : platforms_list)
                    {
                        if(el->isChecked())
                        {
                            creating_route_platforms.push_back(std::stoi(el->text().toStdString()));
                            break;
                        }
                    }
                    creating_connection.clear();
                    creating_route.push_back(con.get());
                    memorised_bus_stop = clicked_intersection;
                }
                else
                {
                    route_tracer.push_back(memorised_intersection->getID());
                    route_tracer.push_back(clicked_intersection->getID());
                }
            }
        }
    }
    else                                                    //wybrane skrzyzowanie NIE jest przystankiem
    {
        if(memorised_intersection != clicked_intersection)
        {
            route_tracer.push_back(memorised_intersection->getID());
            route_tracer.push_back(clicked_intersection->getID());
            creating_connection.push_back(roads_graph[memorised_intersection][clicked_intersection]->getRoadPointer());
        }
    }
    if(memorised_intersection!=nullptr)
    {
        QPainter painter(&displayed_map);
        painter.setRenderHint(QPainter::Antialiasing);
        QColor kolor (255,0,0);
        painter.setPen(QPen(kolor, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for(int i = 1; i < route_tracer.size(); ++i)
            painter.drawLine(*(intersections[route_tracer[i-1]]->getPoint()),*(intersections[route_tracer[i]]->getPoint()));
    }

    update();
    memorised_intersection = clicked_intersection;
}

bool BusMap::getPlatformCount(BusMapIntersection * const clicked_intersection)
{
    QDialog dialog;
    QGridLayout layout;
    QLabel label_p ("Wybierz stanowisko:");
    std::list<QRadioButton*> platforms_list;
    QDialogButtonBox button_box (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(&button_box, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&button_box, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout.addWidget(&label_p,0,0,1,2);
    int i = 1;
    for(const auto& el : *clicked_intersection->getIntersectionPointer()->getPlatforms())
    {
        auto platform = QString::number(el);
        QRadioButton *action = new QRadioButton(platform, this);
        layout.addWidget(action,i,0);
        platforms_list.push_back(action);
        if(i==1)
        {
            action->setChecked(true);
        }
        i++;
    }
    layout.addWidget(&button_box,i,0,1,2);
    dialog.setLayout(&layout);
    dialog.setWindowTitle(QString::fromStdString("Przystanek " + clicked_intersection->getID()));
    std::vector<bool> visible_bus_stops;
      for(const auto& el: bus_stops)
      {
            visible_bus_stops.push_back(el.second->isVisible());
      }
    dialog.exec();
      int ix = 0;
      for(const auto& el: bus_stops)
      {
        if(visible_bus_stops[ix])
            el.second->showPoint();
        else
            el.second->hide();
        ix++;
      }
    if(dialog.result())
    {
        for(const auto& el : platforms_list)
        {
            if(el->isChecked())
            {
                creating_route_platforms.push_back(std::stoi(el->text().toStdString()));
                break;
            }
        }
    }
    return dialog.result();
}

void BusMap::showBusStopInfo(BusMapStop *clicked_bus_stop)
{
    auto bus_stop = clicked_bus_stop->getBusStopPointer();
    auto timetable = bus_stop->getTimetable();
    QDialog dial;
    QScrollArea scroll_area;
    QWidget scroll_widget;
    QLabel label (QString::fromStdString(bus_stop->getName()));
    QVBoxLayout layout, scroll_layout;
    QDialogButtonBox button_box (QDialogButtonBox::Ok);
    connect(&button_box, &QDialogButtonBox::accepted, &dial, &QDialog::accept);

    layout.addWidget(&label);
    for(const auto& el: timetable)
    {
        if(el.first != 0)
        {
            QString info = "Stanowisko " + QString::number(el.first)+":\n";
            for(const auto& pos : *(el.second))
            {
                for(const std::string& dep : pos.second)
                {
                    auto rt = (*routes)[dep.substr(0,8)];
                    auto line = (*lines)[rt->getLineID()]->getName();
                    auto job_tail =  dep.substr(8,3);
                    char letter = 2*((job_tail[1]-48)/2);
                    job_tail[1] -= letter;
                    auto job_ID = line + job_tail;
                    auto job = (*data->getJobs()).at(job_ID).get();
                    auto res = job->checkServiceCorrectness(day_type,prev_day_type,simulation_time,dep);
                    switch(res)
                    {
                        case 1:
                            if(pos.first > simulation_time)
                                info += pos.first.toString("hh:mm") + " - " + QString::fromStdString(line + " " + rt->getDirection()) +"\n";
                            break;
                        case 2:
                            if(pos.first > simulation_time && pos.first > QTime(12,0,0))
                                info += pos.first.toString("hh:mm") + " - " + QString::fromStdString(line + " " + rt->getDirection()) +"\n";
                            break;
                    }
                }
            }
            QLabel* lab = new QLabel (info);
            scroll_layout.addWidget(lab);
        }
    }
    scroll_area.setMinimumWidth(300);
    scroll_widget.setLayout(&scroll_layout);
    scroll_area.setWidget(&scroll_widget);
    layout.addWidget(&scroll_area);
    layout.addWidget(&button_box);

    dial.setLayout(&layout);
    dial.setWindowTitle(QString::fromStdString("Przystanek " + bus_stop->getID()));
    dial.exec();
    update();
}

void BusMap::addTime()
{
    auto copy = simulation_time;
    simulation_time = simulation_time.addMSecs(time_speed * 16);
    if(copy.hour() == 23 && simulation_time.hour() == 0)
    {
        previous_sim_date = simulation_date;
        prev_day_type = day_type;
        simulation_date = simulation_date.addDays(1);
        day_type = data->getDayType(simulation_date).first;
        next_day_type = data->getDayType(simulation_date.addDays(1)).first;
    }
}

void BusMap::removeBusStop(BusMapIntersection * const clicked_intersection)
{
    auto platforms = clicked_intersection->getIntersectionPointer()->getRoutes();
    std::string blocking_routes;
    for(const auto& rt : platforms)
    {
        for(const auto& el: *rt.second)
            blocking_routes += el + ", ";
    }
    if(!blocking_routes.size())
    {
        auto ID = clicked_intersection->getID();
        bus_stops.erase(ID);
        data->deleteBusStop(ID);
        removeIntersection(clicked_intersection,true);
    }
    else
    {
        QMessageBox msg_box (QMessageBox::Warning,"Nie można usunąć przystanku","Aby usunąć ten przystanek, najpierw usuń następujące trasy:\n" + QString::fromStdString(blocking_routes),QMessageBox::Ok);
        msg_box.exec();
    }
}

void BusMap::removeIntersection(BusMapIntersection* const clicked_intersection, bool checked)
{
    auto platforms = clicked_intersection->getIntersectionPointer()->getRoutes();
    std::string blocking_routes;
    for(const auto& el : *platforms.begin()->second)
        blocking_routes += el + ", ";
    if(checked || !(*platforms.begin()).second->size())
    {
        auto ID = clicked_intersection->getID();
        std::vector<std::string> tab;
        std::vector<std::pair<std::map<std::string,std::map<std::string,std::shared_ptr<Connection>>>::iterator,std::map<std::string,std::shared_ptr<Connection>>::iterator>> connections_iterators;
        for(auto it=roads.begin();it != roads.end();++it)
        {
            auto name = it->second->getIDs();
            if(name.find(ID) != std::string::npos)
                tab.push_back(it->first);
        }
        for(auto el:tab)
        {
            for(auto it1 = (*connections).begin(); it1 != (*connections).end(); ++it1)
            {
                for(auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
                {
                    auto rds = it2->second->getRoads();
                    if(std::find(rds->begin(),rds->end(),roads[el]->getRoadPointer()) != rds->end())
                        if(std::find(connections_iterators.begin(),connections_iterators.end(),std::make_pair(it1,it2)) == connections_iterators.end())
                            connections_iterators.push_back(std::make_pair(it1,it2));
                }
            }
            roads.erase(el);
            data->deleteRoad(el);
        }
        for(const auto& el: connections_iterators)
        {
            (*el.first).second.erase(el.second);
        }

        data->deleteIntersection(ID);
        intersections.erase(ID);
        displayed_map.load(map_name);
    }
    else
    {
        QMessageBox msg_box (QMessageBox::Warning,"Nie można usunąć skrzyżowania","Aby usunąć to skrzyżowanie, najpierw usuń następujące trasy:\n" + QString::fromStdString(blocking_routes),QMessageBox::Ok);
        msg_box.exec();
    }
}

void BusMap::removeRoad(BusMapIntersection* const clicked_intersection)
{
    if(memorised_intersection==nullptr || memorised_intersection == clicked_intersection)
    {
        memorised_intersection = clicked_intersection;
    }
    else
    {
        std::set<std::string> blocking_routes;
        auto platforms1 = memorised_intersection->getIntersectionPointer()->getRoutes();
        auto platforms2 = clicked_intersection->getIntersectionPointer()->getRoutes();
        std::string blocking_routes_info;

        for(const auto& rt : platforms1)
        {
            for(const auto& el: *rt.second)
                blocking_routes.insert(el);
        }
        for(const auto& rt : platforms2)
        {
            for(const auto& el: *rt.second)
                if(std::find(blocking_routes.begin(),blocking_routes.end(),el)!=blocking_routes.end())
                    blocking_routes_info += el + ", ";
        }
        if(!blocking_routes_info.size())
        {
            std::string ID;
            std::vector<std::pair<std::map<std::string,std::map<std::string,std::shared_ptr<Connection>>>::iterator,std::map<std::string,std::shared_ptr<Connection>>::iterator>> connections_iterators;
            if(roads_graph.find(clicked_intersection) != roads_graph.end() && roads_graph.at(clicked_intersection).find(memorised_intersection)!=roads_graph.at(clicked_intersection).end())
            {
                ID = roads_graph[clicked_intersection][memorised_intersection]->getID();
                for(auto it1 = (*connections).begin(); it1 != (*connections).end(); ++it1)
                {
                    for(auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
                    {
                        auto rds = it2->second->getRoads();
                        if(std::find(rds->begin(),rds->end(),roads[ID]->getRoadPointer()) != rds->end())
                            connections_iterators.push_back({it1,it2});
                    }
                }
                roads_graph[clicked_intersection].erase(memorised_intersection);
                roads.erase(ID);
                data->deleteRoad(ID);
            }
            if(roads_graph.find(memorised_intersection) != roads_graph.end() && roads_graph.at(memorised_intersection).find(clicked_intersection)!=roads_graph.at(memorised_intersection).end())
            {
                ID = roads_graph[memorised_intersection][clicked_intersection]->getID();
                for(auto it1 = (*connections).begin(); it1 != (*connections).end(); ++it1)
                {
                    for(auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
                    {
                        auto rds = it2->second->getRoads();
                        if(std::find(rds->begin(),rds->end(),roads[ID]->getRoadPointer()) != rds->end())
                            connections_iterators.push_back({it1,it2});
                    }
                }
                roads_graph[memorised_intersection].erase(clicked_intersection);
                roads.erase(ID);
                data->deleteRoad(ID);
            }
            for(const auto& el: connections_iterators)
            {
                (*el.first).second.erase(el.second);
            }
        }
        else
        {
            QMessageBox msg_box (QMessageBox::Warning,"Nie można usunąć drogi","Aby usunąć tą drogę, najpierw usuń następujące warianty:\n" + QString::fromStdString(blocking_routes_info),QMessageBox::Ok);
            msg_box.exec();
        }
        memorised_intersection = nullptr;
        displayed_map.load(map_name);
    }
}

void BusMap::divideRoad(BusMapIntersection* clicked_intersection, const QPoint &point, bool bus_stop)
{
    if(point != QPoint{0,0} && memorised_intersection==nullptr && memorised_road==nullptr)
        return;
    QPoint comp_point{0,0};
    if(point == comp_point && (memorised_intersection == nullptr || memorised_intersection == clicked_intersection))
    {
        memorised_intersection = clicked_intersection;
    }
    else if(memorised_road == nullptr)
    {
        bool v1 = false, v2 = false;
        BusMapRoad* copy;
        if(roads_graph.find(clicked_intersection) != roads_graph.end() && roads_graph.at(clicked_intersection).find(memorised_intersection)!=roads_graph.at(clicked_intersection).end())
        {
            copy = memorised_road=roads_graph[clicked_intersection][memorised_intersection];
            v1 = true;
        }
        if(roads_graph.find(memorised_intersection) != roads_graph.end() && roads_graph.at(memorised_intersection).find(clicked_intersection)!=roads_graph.at(memorised_intersection).end())
        {
            memorised_road=roads_graph[memorised_intersection][clicked_intersection];
            v2=true;
        }
        is_memorised_road_one_way = v1 xor v2;
        memorised_intersection = clicked_intersection;
        auto road_name = memorised_road->getID();
        if(!is_memorised_road_one_way && road_name[road_name.size()-1] == 'B')
            memorised_road = copy;
    }
    else
    {
        if(bus_stop)
        {
            createBusStop(point);
        }
        else
        {
            createIntersection(point);
        }
        if(memorised_intersection!= nullptr)
        {
            auto ID = memorised_road->getID();
            doRoadDivision();

            if(!is_memorised_road_one_way)
            {
                ID.pop_back();
                ID += "B";
                memorised_road = roads[ID].get();
                doRoadDivision(1);
            }
            displayed_map.load(map_name);
        }
        memorised_intersection = nullptr;
        memorised_road = nullptr;
    }
    update();
}

void BusMap::doRoadDivision(bool if_second_direction)
{
    auto ID = memorised_road->getID();
    std::string newID1,newID2;
    if(if_second_direction)
    {
        newID1 = "2" + ID;
        newID2 = "1" + ID;
    }
    else
    {
        newID1 = "1" + ID;
        newID2 = "2" + ID;
    }
    auto starting_point = memorised_road->getRoadPointer()->getPoint1();
    auto starting_point_ID = memorised_road->getRoadPointer()->getPoint1ID();
    auto finish_point = memorised_road->getRoadPointer()->getPoint2();
    auto finish_point_ID = memorised_road->getRoadPointer()->getPoint2ID();

    auto BR1 = std::make_shared<Road>(newID1,starting_point,memorised_intersection->getIntersectionPointer());
    auto BMR1 = std::make_shared<BusMapRoad>(newID1,intersections[starting_point_ID].get(),memorised_intersection,BR1.get());

    auto BR2 = std::make_shared<Road>(newID2,memorised_intersection->getIntersectionPointer(),finish_point);
    auto BMR2 = std::make_shared<BusMapRoad>(newID2,memorised_intersection,intersections[finish_point_ID].get(),BR2.get());

    data->addRoad(newID1,BR1);
    data->addRoad(newID2,BR2);
    roads[newID1] = BMR1;
    roads[newID2] = BMR2;
    roads_graph[intersections[starting_point_ID].get()][memorised_intersection]=BMR1.get();
    roads_graph[memorised_intersection][intersections[finish_point_ID].get()]=BMR2.get();

    for(const auto& con : *connections)
    {
        for(const auto& el : con.second)
        {
            auto rds = *(el.second->getRoads());
            if(std::find(rds.begin(),rds.end(),memorised_road->getRoadPointer())!=rds.end())
            {
                el.second->replaceRoads(memorised_road->getRoadPointer(),BR1.get(),BR2.get());
            }

        }
    }
    roads.erase(ID);
    roads_graph[intersections[starting_point_ID].get()].erase(intersections[finish_point_ID].get());
    data->deleteRoad(ID);

}


