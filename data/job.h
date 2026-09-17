#ifndef JOB_H
#define JOB_H

#include "shift.h"
#include "service.h"


class Job
{
public:
    Job();
    Job(std::string i, std::vector<std::vector<Shift*>> tab,std::vector<QTime> df, std::vector<int> dt, std::vector<std::vector<Service*>> s) :ID(i), shifts(tab), services(s), day_types(dt), default_frequencies(df) {}
    std::string writeInfo() const {return ID + ";" + writeShiftsAndConnections();}
    std::string getID() const {return ID;}
    Service* getCurrentService() const {return services[day_types[current_day]][current_service];}
    void resetCurrentService() {current_service = 0; current_shift = 0;}
    Shift* getCurrentShift() const {return shifts[day_types[current_day]][current_shift];}
    const std::vector<std::vector<Service*>>* getServices() const {return &services;}
    const std::vector<std::vector<Shift*>>* getShifts() const {return &shifts;}
    std::string getLine();
    QPoint getPosition(QTime t, int day_type, int prev_dt);
    std::vector<int> getDayTypes() const {return day_types;}
    std::vector<QTime> getDefaultFrequencies() const {return default_frequencies;}
    void setCurrentDay(int dt, int pdt,QTime t);
    int checkServiceCorrectness(int dt, int pdt, QTime t, std::string s);


private:
    std::string ID;
    std::vector<std::vector<Shift*>> shifts;
    std::vector<std::vector<Service*>> services;
    std::vector<int> day_types;
    std::vector<QTime> default_frequencies;
    int current_service = 0;
    int current_shift = 0;
    int current_day = 0;

    inline std::string writeShiftsAndConnections() const
    {
        std::string res;
        for(const auto& el : shifts)
            for(const auto& pos : el)
                res += pos->getLetter() + " ";
        res.pop_back();
        res += ";";
        for(const auto& el : default_frequencies)
            res+=el.toString("hh mm ss").toStdString()+" ";
        res.pop_back();
        res += ";";
        for(const auto& el : day_types)
            res+=std::to_string(el)+" ";
        res.pop_back();
        res += ";";
        for(const auto& el : services)
            for(const auto& pos : el)
                res += pos->getID() + " ";
        if(services.size())
            res.pop_back();
        return res;
    }
};

#endif // JOB_H
