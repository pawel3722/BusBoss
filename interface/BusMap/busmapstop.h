#ifndef BUSMAPSTOP_H
#define BUSMAPSTOP_H
#include "data/busstop.h"
#include "interface/BusMap/busmapintersection.h"

class BusMapStop : public BusMapIntersection
{
public:
    BusMapStop(std::string i,QPoint p,BusStop* bs, QWidget* ptr= nullptr) :BusMapIntersection(i,p,bs,ptr)
    {
        bus_stop = bs;
        this->hide();
        QImage img;
        img.load("S:\\Qt_projects\\BusBoss\\icons\\BusIcon.jpg");
        this->setStyleSheet("color: white; background-color: rgb(0,22,137)");
        this->setText(QString::fromStdString(i));
    }
    BusMapStop(BusStop* b,QWidget* ptr= nullptr) :BusMapIntersection(b,ptr)
    {
        bus_stop=b;
        this->hide();
        QImage img;
        img.load("S:\\Qt_projects\\BusBoss\\icons\\BusIcon.jpg");
        this->setStyleSheet("color: white; background-color: rgb(0,22,137)");
        this->setText(QString::fromStdString(b->getID()));
    }
    void showPoint() override
    {
        this->setGeometry(BusMapIntersection::point.x()-WIDTH/2,BusMapIntersection::point.y()-WIDTH/2,WIDTH,WIDTH);
        this->show();
    } 
    Intersection* getIntersectionPointer() const override  {return bus_stop;}
    BusStop* getBusStopPointer() const override {return bus_stop;}

private:
    BusStop* bus_stop;
    const int WIDTH = 32;
};

#endif // BUSMAPSTOP_H
