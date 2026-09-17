#include "data/service.h"

Service::Service()
{

}

QPoint Service::getPosition(QTime t)
{
    QPoint point;
    if(end_time > departure_time)
    {
        if(t < departure_time)
            point = {-2,-2};
        else if(t > end_time)
            point = {-1,-1};
        else
        {
            auto new_time = Connection::subTime(t,departure_time);
            point = route->getPosition(new_time,timescale);
        }
    }
    else
    {
        if(t < departure_time && t > QTime{12,0,0})
            point = {-2,-2};
        else if(t > end_time && t < QTime{12,0,0})
            point = {-1,-1};
        else
        {
            auto new_time = Connection::subTime(t,departure_time);
            point = route->getPosition(new_time,timescale);
        }
    }
    return point;
}

