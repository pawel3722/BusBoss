
#ifndef ROAD_H
#define ROAD_H

#include "intersection.h"
#include <memory>


class Road
{
public:
    Road();
    Road(std::string i,Intersection* a, Intersection* b) :ID(i),i1(a),i2(b){}
    std::string writeInfo() const {return ID + ";" + i1->getID() + ";" + i2->getID();}
    std::string getID() const {return ID;}
    std::string getPoint1ID() const {return i1->getID();}
    std::string getPoint2ID() const {return i2->getID();}
    Intersection* getPoint1() const {return i1;}
    Intersection* getPoint2() const {return i2;}
private:
    std::string ID;
    Intersection* i1;
    Intersection* i2;
};

#endif // ROAD_H
