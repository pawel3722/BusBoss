#ifndef ROUTE_H
#define ROUTE_H

#include "connection.h"
#include <list>

class Route
{
public:
    Route();
    Route(std::string i,std::string d, std::list<Connection*>& c, std::list<int> p, std::vector<std::vector<int>> ts) :ID(i),direction(d),connections(c),platforms(p),timescales(ts)
    {
        auto cit = connections.rbegin();
        auto pit = platforms.rbegin();
        ++pit;
        (*cit)->setPlatformsAndRoutes(ID,*pit,*platforms.rbegin());
        ++cit;
        ++pit;
        while(cit != connections.rend())
        {
            (*cit)->setPlatformsAndRoutes(ID,*pit);
            ++cit;
            ++pit;
        }
        calculateTimescalesLengths();
    };
    ~Route();
    std::string writeInfo() const {return ID+";"+direction+";"+writeConnectionsandPlatforms()+";"+writeTimescales();}
    std::string getNumber() const;
    std::string getLineID() const;
    std::string getID() const {return ID;}
    std::string getDirection() const {return direction;}
    void setDirection(std::string d) {direction = d;}
    QTime setPlaftormsAndDepartures(std::string service_ID, QTime time, int ts);
    QPoint getPosition(QTime t, int ts) const;
    const std::list<Connection*>* getConnections() const {return &connections;}
    const std::vector<std::vector<int>>* getTimescales() const {return &timescales;}
    void setTimescales(std::vector<std::vector<int>> m) {timescales = m; calculateTimescalesLengths();};
    void calculateTimescalesLengths();
    const std::vector<QTime>* getTimescalesLengths() const {return &timescales_lengths;}
    BusStop* getFirstStop() const {return (*connections.begin())->get1StopPointer();}
    void deleteService(std::string service_ID,QTime t, int ts);

private:
    std::string ID;
    std::string direction;
    std::list<Connection*> connections;
    std::list<int> platforms;
    std::vector<std::vector<int>> timescales;
    std::vector<QTime> timescales_lengths;

    inline std::string writeConnectionsandPlatforms() const
    {
        std::string res;
        auto p1 = platforms.begin();
        res+= (*connections.begin())->get1Stop() + " " + std::to_string(*p1);
        for(const auto& el: connections)
        {
            ++p1;
            res += " " + el->get2Stop() + " " + std::to_string(*p1);
        }
        return res;
    }
    inline std::string writeTimescales() const
    {
        std::string res;
        for(const auto& ts : timescales)
            for(const auto& el : ts)
                res+=std::to_string(el)+" ";
        return res;
    }
};

#endif // ROUTE_H
