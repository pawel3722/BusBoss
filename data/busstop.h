#ifndef BUSSTOP_H
#define BUSSTOP_H
#include "intersection.h"
#include "qdatetime.h"
#include <iostream>
#include <vector>
#include <map>
#include <QWidget>

inline std::string getNums(const std::vector<int>& v)
{
    std::string res;
    for(const auto& el: v)
        res +=' '+std::to_string(el);
    return res;
}

class BusStop : public Intersection
{
public:
    BusStop(){};
    BusStop(std::string i,QPoint p, std::string n, std::vector<int> num) :Intersection(i,p){
        nums=num;name=n;
        platforms[0] =  std::make_shared<Platform>(n+" 0");
        for(const auto& el : num)
        {
            auto pl = std::make_shared<Platform>(n+" "+std::to_string(el));
            platforms[el]=pl;
        }
    };
    ~BusStop() override {};
    std::string writeInfo() override {return ID+";"+name+"; "+std::to_string(point.x())+" "+std::to_string(point.y())+getNums(nums);}
    const std::vector<int>* getPlatforms() const override {return &nums;}
    std::string getName() {return name;}
    void addPlatform(int n) { auto pl = std::make_shared<Platform>(name+" "+std::to_string(n));platforms[n]=pl;nums.push_back(n);}
    void addRoute(std::string route, int platform = 0) override {platforms[platform]->addRoute(route);}
    void addDeparture(std::string service,QTime time, int platform) {platforms[platform]->addDeparture(service,time);}
    std::map<int,const std::vector<std::string>*> getRoutes() override;
    std::map<int, const std::map<QTime, std::vector<std::string> > *> getTimetable();
    void deleteRoute(std::string r) override
    {
        for(auto& el: platforms)
            el.second->deleteRoute(r);
    }
    void deleteService(std::string r, QTime t)
    {
        for(auto& el : platforms)
            el.second->deleteService(r,t);
    }
    void deletePlatform(int n){platforms.erase(n);nums.erase(std::find(nums.begin(),nums.end(),n));};

private:
    class Platform
    {
    public:
        Platform();
        Platform(std::string n) :name(n){};
        void addRoute(std::string r) {routes.push_back(r);};
        void addDeparture(std::string r, QTime t) {timetable[t].push_back(r);}
        void deleteRoute(std::string r);
        void deleteService(std::string s, QTime t);
        const std::vector<std::string>* getRoutes() const {return &routes;}
        const std::map<QTime, std::vector<std::string>>* getTimetable() const {return &timetable;}
    private:
        std::string name;
        std::vector<std::string> routes;
        std::map<QTime, std::vector<std::string>> timetable;
    };
    std::map<int,std::shared_ptr<Platform>> platforms;
    std::vector<int> nums;
    std::string name;
};

#endif // BUSSTOP_H
