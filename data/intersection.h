
#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <QPoint>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

class Intersection
{
public:
    Intersection();
    Intersection(std::string i, QPoint p) :ID(i),point(p) {}
    virtual ~Intersection(){}
    virtual std::string writeInfo() {return ID + "; " + std::to_string(point.x()) + " " + std::to_string(point.y());};
    std::string getID() {return ID;}
    virtual void showPoint() {}
    const QPoint* getPoint() const { return &point; }
    virtual const std::vector<int>* getPlatforms () const { return &vec;};
    virtual void addRoute(std::string route,int platform = 0) {routes.push_back(route); platform=0;}
    virtual std::map<int,const std::vector<std::string>*> getRoutes() {return {{0,&routes}};}
    virtual void deleteRoute(std::string r) { routes.erase(std::find(routes.begin(),routes.end(),r)); }



protected:
    std::string ID;
    QPoint point;
    std::vector<int> vec; //pusty wektor, zeby dzialala funkcja wirtualna
    std::vector<std::string> routes;
};

#endif // INTERSECTION_H
