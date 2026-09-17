
#include "job.h"
#include <regex>

Job::Job()
{

}

std::string Job::getLine()
{
    std::regex reg("^([^/]{1,5})/.+$");
    std::smatch base_match;
    std::regex_match(Job::ID, base_match, reg);
    auto sub_match = base_match[1];
    std::string res = sub_match.str();
    return res;
}

QPoint Job::getPosition(QTime t, int day_type, int prev_dt)
{
    auto ttbl = day_types[day_type];
    auto prev_ttbl = day_types[prev_dt];
    if(ttbl != prev_ttbl)
    {
        auto current_day_ttbl = day_types[current_day];
        if(current_day_ttbl != -1)
        {
            if(current_service == services[current_day_ttbl].size() && current_day != day_type)
            {
                current_service = 0;
                current_day = day_type;
            }
        }
    }
    else if(ttbl != -1)
    {
        if(current_service == services[ttbl].size() && t > services[ttbl][0]->getDepartureTime() && t < services[ttbl][0]->getArrivalTime())
            current_service = 0;
        current_day = day_type;
    }
    QPoint point {-1,-1};
    ttbl = day_types[current_day];
    if(ttbl != -1 && current_service < services[ttbl].size())
    {
        QPoint too_late_point {-1,-1}, too_early_point {-2,-2};
        for(int i =0;i<services[ttbl].size(); ++i)
        {
            point = services[ttbl][current_service]->getPosition(t);
            if(point == too_late_point && point != too_early_point)
            {
                if(current_service < services[ttbl].size()-1)
                    current_service++;
                else
                    current_service = 0;
            }
            else
                break;
        }
    }
    else
        current_service = 0;
    if(ttbl != -1)
    {
        auto finish_time = shifts[ttbl][current_shift]->getFinishTime();
        if(t.hour()==finish_time.hour() && t.minute() == finish_time.minute() && t.second()==finish_time.second())
        {
            if(current_shift < shifts[ttbl].size()-1)
                current_shift++;
            else
                current_shift = 0;
        }
    }
    return point;
}

void Job::setCurrentDay(int dt, int pdt, QTime t)
{
    current_day = dt;
    auto prev_ttbl = day_types[pdt];
    bool found = false;
    if(prev_ttbl != -1)
    {
        auto first_dep_time = services[prev_ttbl][0]->getDepartureTime();
        auto max_dep_time = first_dep_time;
        if(t < first_dep_time)
            for(int i = 0; i < services[prev_ttbl].size();++i)
            {
                auto dep_time = services[prev_ttbl][i]->getDepartureTime();
                auto arr_time = services[prev_ttbl][i]->getArrivalTime();
                if(arr_time < dep_time || dep_time < max_dep_time)
                {
                    found = true;
                }
                if (found && t < arr_time)
                {
                    current_day = pdt;
                    current_service = i;
                    break;
                }
            }
    }
    auto ttbl = day_types[current_day];
    if(ttbl != -1)
    {
        for(int i = 0; i < shifts[ttbl].size(); ++i)
        {
            auto start_time = shifts[ttbl][i]->getStartTime();
            auto end_time = shifts[ttbl][i]->getFinishTime();
            if(start_time < end_time)
            {
                if(t > start_time && t < end_time)
                    current_shift = i;
            }
            else
            {
                if(t > start_time || t < end_time)
                    current_shift = i;
            }
        }

    }

}

int Job::checkServiceCorrectness(int dt, int pdt, QTime t, std::string s)
{
    setCurrentDay(dt,pdt,t);
    int found = 0;
    if(current_day == pdt && day_types[pdt] != -1)
    {
        for(int i = current_service; i < services[day_types[current_day]].size(); ++i)
        {
            if(services[day_types[current_day]][i]->getID() == s)
            {
                found = 1;
            }
        }       
    }
    if(!found && day_types[dt] != -1)
    {
        auto first_time = services[day_types[current_day]][0]->getDepartureTime();
        for(int i = 0; i < services[day_types[dt]].size(); ++i)
        {
            if(services[day_types[dt]][i]->getID() == s)
            {
                if(services[day_types[current_day]][i]->getArrivalTime() > first_time)
                {
                    found = 1;
                }
                else
                {
                    found = 2;
                }
            }
        }
    }
    return found;
}

