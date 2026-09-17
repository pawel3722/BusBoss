#ifndef SERVICE_H
#define SERVICE_H
#include "route.h"
#include <list>


class Service
{
public:
    Service();
    ~Service() {route->deleteService(ID,departure_time,timescale);};
    Service(std::string i, Route* r, QTime t, int ts) :ID(i),route(r),departure_time(t),timescale(ts) {
        end_time = route->setPlaftormsAndDepartures(i,t,ts);
    }
    std::string writeInfo() const {return ID + ";" + departure_time.toString("hh:mm:ss").toStdString()+";"+std::to_string(timescale);}
    std::string getID() const {return ID;}
    std::string getRouteID() const {return route->getID();}
    const Route* getRoute() const {return route;}
    std::string getLineID() const {return route->getLineID();}
    QPoint getPosition (QTime t);
    int getTimescale() const {return timescale;}
    QTime getDepartureTime() const {return departure_time;}
    QTime getArrivalTime() const {return end_time;}

private:
    std::string ID;
    Route* route;
    QTime departure_time, end_time;
    int timescale;
};

#endif // SERVICE_H
