#include "busstop.h"

std::map<int, const std::vector<std::string> *> BusStop::getRoutes()
{
    std::map<int,const std::vector<std::string>*>res;
    for(const auto& el: platforms)
        res[el.first] = el.second->getRoutes();
    return res;
}

std::map<int, const std::map<QTime, std::vector<std::string> > *> BusStop::getTimetable()
{
    std::map<int, const std::map<QTime, std::vector<std::string>>*> res;
    for(const auto& el:platforms)
        res[el.first] = el.second->getTimetable();
    return res;
}

BusStop::Platform::Platform()
{

}

void BusStop::Platform::deleteRoute(std::string r)
{
    std::vector<std::pair<std::map<QTime,std::vector<std::string>>::iterator,std::vector<std::string>::iterator>> pairs;
    auto it = std::find(routes.begin(),routes.end(),r);
    if(it!=routes.end())
        Platform::routes.erase(it);
    for(auto it = timetable.begin();it!=timetable.end();++it)
        for(auto ir = (*it).second.begin(); ir != (*it).second.end(); ++ir)
            if(ir->substr(0,8) == r)
                pairs.push_back({it,ir});
    for(auto& el : pairs)
    {
        (*el.first).second.erase(el.second);
    }
}

void BusStop::Platform::deleteService(std::string s, QTime t)
{

    auto it = std::find(timetable[t].begin(),timetable[t].end(),s);
    if(it != timetable[t].end())
        timetable[t].erase(it);
}
