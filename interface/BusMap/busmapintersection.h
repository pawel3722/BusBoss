#ifndef BUSMAPINTERSECTION_H
#define BUSMAPINTERSECTION_H
#include "data/busstop.h"
#include "data/intersection.h"
#include <QPushButton>

class BusMapIntersection : public QPushButton
{
public:
    BusMapIntersection(){}
    BusMapIntersection(std::string i,QPoint p,Intersection* is,QWidget* ptr= nullptr) :QPushButton(ptr)
    {
        ID = i; point = p; intersection = is;
        this->hide(); this->setStyleSheet("color: white; background-color: black");
    }
    BusMapIntersection(Intersection* i,QWidget* ptr= nullptr) :QPushButton(ptr)
    {
        ID = i->getID(); point = *i->getPoint(); intersection = i;
        this->hide(); this->setStyleSheet("color: white; background-color: black");
    };
    std::string getInfo() const {return std::to_string(point.x())+" "+std::to_string(point.y());}
    std::string getID() const {return ID;}
    void setPoint(QPoint p){ point=p; }
    const QPoint* getPoint() const { return &point; }
    virtual Intersection* getIntersectionPointer() const {return intersection;}
    virtual BusStop* getBusStopPointer() const {return nullptr;}
    virtual void showPoint()
    {
        this->setGeometry(point.x()-WIDTH/2,point.y()-WIDTH/2,WIDTH,WIDTH);
        this->show();
    }
    bool operator!=(BusMapIntersection is)
    {
        return point != *is.getPoint();
    }

protected:
    std::string ID;
    QPoint point;
private:
    Intersection* intersection;
    const int WIDTH = 24;
};

#endif // BUSMAPINTERSECTION_H
