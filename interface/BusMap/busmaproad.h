#ifndef BUSMAPROAD_H
#define BUSMAPROAD_H
#include "data/road.h"
#include "interface/BusMap/busmapintersection.h"

class BusMapRoad
{
public:
    BusMapRoad(std::string i,BusMapIntersection* ia, BusMapIntersection* ib,Road* p) :ID(i),i1(ia),i2(ib),ptr(p) {};
    //BusMapRoad(Road* r) {ID=r->getID(); i1 = r->getPoint1();i2 = r->getPoint2();};
    std::pair<const QPoint*,const QPoint*> getPoints() {return {i1->getPoint(),i2->getPoint()};}
    std::string getIDs() {return i1->getID()+" "+i2->getID();}
    std::string getID() const {return ID;}
    Road* getRoadPointer() const {return ptr;}

private:
    std::string ID;
    BusMapIntersection* i1;
    BusMapIntersection* i2;
    Road* ptr;
};

#endif // BUSMAPROAD_H
