
#include "route.h"
#include <regex>

Route::Route()
{

}

Route::~Route()
{
    for(auto& el : connections)
    {
        el->deleteRoute(ID);
    }
}

std::string Route::getNumber() const
{
    std::regex reg("^\\d{5}/(\\d{2})$");
    std::smatch base_match;
    std::regex_match(Route::ID, base_match, reg);
    auto sub_match = base_match[1];
    std::string res = sub_match.str();
    return res;
}

std::string Route::getLineID() const
{
    std::regex reg("^(\\d{5})/\\d{2}$");
    std::smatch base_match;
    std::regex_match(Route::ID, base_match, reg);
    auto sub_match = base_match[1];
    std::string res = sub_match.str();
    return res;
}

QTime Route::setPlaftormsAndDepartures(std::string service_ID, QTime time, int ts)
{
    auto pit = platforms.begin();
    auto last_platform = *platforms.rbegin();
    int i = (int) connections.size()-1;
    auto tit = timescales[ts].begin();
    for(const auto& el : connections)
    {
        if(i)
            time = el->setPlatformsAndDepartures(service_ID,*pit,time,*tit);
        else
            time = el->setPlatformsAndDepartures(service_ID,*pit,time,*tit,last_platform);
        --i;
        ++pit;
        ++tit;
    }
    return time;
}

QPoint Route::getPosition(QTime t, int ts) const
{
    QPoint point;
    QTime end_time {0,0}, start_time {0,0};
    auto tit = timescales[ts].begin();
    for(const auto& el : connections)
    {
        end_time = Connection::addTime(end_time,el->getTime(*tit));
        if(end_time > t)
        {
            auto new_time = Connection::subTime(t,start_time);
            point = el->getPosition(new_time,*tit);
            break;
        }
        start_time = end_time;
        ++tit;
    }
    return point;
}

void Route::calculateTimescalesLengths()
{
    timescales_lengths.clear();
    for(const auto& el: timescales)
    {
        QTime res {0,0};
        auto it = connections.begin();
        for(const auto& pos : el)
        {
            res = Connection::addTime(res,(*it)->getTime(pos));
            ++it;
        }
        timescales_lengths.push_back(res);
    }
}

void Route::deleteService(std::string service_ID, QTime t, int ts)
{
    auto tit = timescales[ts].begin();
    for(const auto& el : connections)
    {
        t = el->deleteService(service_ID,t,*tit);
        ++tit;
    }
}

