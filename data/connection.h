#ifndef CONNECTION_H
#define CONNECTION_H

#include "qdatetime.h"
#include "busstop.h"
#include "road.h"




class Connection
{
public:
    Connection();
    Connection(BusStop* i1,BusStop* i2, double d, std::vector<QTime>& t, std::list<Road*>& r) :stop1(i1),stop2(i2),distance(d),timescales(t), roads_IDs(r)
    {
        setRoadLengths();
    }
    void setRoadLengths();
    std::string writeInfo() const {return stop1->getID()+";"+stop2->getID()+"; "+std::to_string(distance)+writeTime()+";"+writeRoads();}
    std::string get1Stop() const {return stop1->getID();}
    std::string get2Stop() const {return stop2->getID();}
    BusStop* get1StopPointer() const {return stop1;}
    BusStop* get2StopPointer() const {return stop2;}
    const std::list<Road*>* getRoads() const {return &roads_IDs;}
    void setPlatformsAndRoutes(std::string route, int platform1, int platform2 = 0);
    void replaceRoads(Road* old, Road* new1, Road* new2);
    QTime setPlatformsAndDepartures(std::string service, int platform1, QTime time1, int timescale = 0, int platform2 = 0);
    QTime getTime(int ts) const {return timescales[ts];}
    void setTime(int ts,QTime t) {timescales[ts] = t;}
    QPoint getPosition (QTime t,int ts) const;
    void setDistance(double d) {distance = d;}
    double getDistance() {return distance;}
    void deleteRoute(std::string r);
    QTime deleteService(std::string s, QTime t, int ts);

    static QTime addTime(QTime t1, QTime t2, bool hour_carry = true);
    static QTime subTime(QTime t1, QTime t2);
private:
    BusStop* stop1;
    BusStop* stop2;
    double distance;
    std::vector<QTime> timescales;
    std::list<Road*> roads_IDs;
    std::vector<double> roads_lengths;

    inline std::string writeTime() const
    {
        std::string res;
        for(const auto& el: timescales)
            res+=" "+el.toString("mm ss").toStdString();
        return res;
    }
    inline std::string writeRoads() const
    {
        std::string res;
        for(const auto& el: roads_IDs)
            res+=" "+el->getID();
        return res;
    }
};

#endif // CONNECTION_H
