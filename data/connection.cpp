
#include "connection.h"
#include <algorithm>
#include <cmath>

Connection::Connection()
{

}

void Connection::setRoadLengths()
{
    auto previous_point = (*roads_IDs.begin())->getPoint1()->getPoint();
    for(const auto& el : roads_IDs)
    {
        auto point = el->getPoint2()->getPoint();
        double len = sqrt(pow( point->x() - previous_point->x(),2) + pow( point->y() - previous_point -> y(),2 ));
        roads_lengths.push_back(len);
        previous_point = point;
    }
}

void Connection::setPlatformsAndRoutes(std::string route,int platform1, int platform2)
{
    auto it = roads_IDs.begin();
    (*it)->getPoint1()->addRoute(route,platform1);
    ++it;
    for(;it!=roads_IDs.end();++it)
    {
        (*it)->getPoint1()->addRoute(route,0);
    }
    if(platform2 != 0)
    {
        --it;
        (*it)->getPoint2()->addRoute(route,platform2);
    }
}

void Connection::replaceRoads(Road *old, Road *new1, Road *new2)
{
    auto it = std::find(roads_IDs.begin(),roads_IDs.end(),old);

    if(it==roads_IDs.begin())
    {
        roads_IDs.erase(it);
        roads_IDs.push_front(new2);
        roads_IDs.push_front(new1);
    }
    else
    {
        roads_IDs.insert(it,new1);
        roads_IDs.insert(it,new2);
        roads_IDs.erase(it);
    }

}

QTime Connection::setPlatformsAndDepartures(std::string service, int platform1, QTime time1, int timescale, int platform2)
{
    stop1->addDeparture(service,time1,platform1);
    auto time2 = addTime(time1,timescales[timescale]);
    if(platform2 != 0)
        stop2->addDeparture(service,time2,platform2);
    return time2;
}

QPoint Connection::getPosition(QTime t, int ts) const
{
    QPoint result {0,0};
    auto connection_time = timescales[ts];
    unsigned int all_milliseconds = connection_time.msec() + 1000 * connection_time.second() + 60'000 * connection_time.minute() + 3'600'000 * connection_time.hour() - 10'000;
    unsigned int dem_milliseconds = t.msec() + 1000 * t.second() + 60'000 * t.minute() + 3'600'000 * t.hour();
    if(dem_milliseconds>all_milliseconds)
        dem_milliseconds = all_milliseconds;
    double percentage = 1.0 * dem_milliseconds / all_milliseconds;
    double sum_of_distances = std::accumulate(roads_lengths.begin(),roads_lengths.end(),0);
    double driven_distance = percentage * sum_of_distances;
    double sum = 0.0, previous_sum = 0.0;
    auto road_it = roads_IDs.begin();
    for(const auto& el : roads_lengths)
    {
        sum += el;
        if(sum/sum_of_distances >= percentage)
        {
            auto section = driven_distance - previous_sum;
            auto road_begin = (*road_it)->getPoint1()->getPoint();
            auto road_end = (*road_it)->getPoint2()->getPoint();
            std::pair<int,int> main_vec = {road_end->x()-road_begin->x(),road_end->y()-road_begin->y()};
            auto factor = section / el;
            main_vec.first *= factor;
            main_vec.second *= factor;
            result  = {road_begin->x() + main_vec.first, road_begin->y() + main_vec.second};
            break;
        }
        previous_sum = sum;
        road_it++;
    }

    return result;
}

void Connection::deleteRoute(std::string r)
{
    (*roads_IDs.begin())->getPoint1()->deleteRoute(r);
    for(auto& el: roads_IDs)
    {
        el->getPoint2()->deleteRoute(r);
    }
}

QTime Connection::deleteService(std::string s, QTime t, int ts)
{
    QTime res = addTime(t,timescales[ts]);
    stop1->deleteService(s,t);
    stop2->deleteService(s,res);
    return res;
}

QTime Connection::addTime(QTime t1, QTime t2, bool hour_carry)
{
    int t_mili = t1.msec()+t2.msec();
    int t_seconds = t1.second()+t2.second();
    int t_minutes = t1.minute()+t2.minute();
    int t_hours = t1.hour() + t2.hour();
    if(t_mili>999)
    {
        t_mili -= 1000;
        t_seconds ++;
    }
    if(t_seconds>59)
    {
        t_minutes ++;
        t_seconds -= 60;
    }
    if(t_minutes>59)
    {
        t_hours ++;
        t_minutes -= 60;
    }
    if(hour_carry && t_hours>23)
        t_hours -= 24;

    QTime time_res {t_hours,t_minutes,t_seconds,t_mili};
    return time_res;
}

QTime Connection::subTime(QTime t1, QTime t2)
{
    auto new_msec = t1.msec() - t2.msec();
    auto new_second = t1.second() - t2.second();
    auto new_minute = t1.minute() - t2.minute();
    auto new_hour = t1.hour() - t2.hour();
    if(new_msec < 0)
    {
        new_msec+=1000;
        new_second--;
    }
    if(new_second < 0)
    {
        new_second += 60;
        new_minute--;
    }
    if(new_minute < 0)
    {
        new_minute +=60;
        new_hour--;
    }
    if(new_hour < 0)
    {
        new_hour += 24;
    }
    QTime time_res {new_hour,new_minute,new_second, new_msec};
    return time_res;
}

