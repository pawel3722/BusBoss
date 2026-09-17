
#include "data.h"
#include <QFileDialog>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

Data::Data(QString path)
{
    readData(path);
}

Data::~Data()
{

}

void Data::readData(QString path)
{
    if(path.size())
        dir = path.toStdString();
    else
        dir = QFileDialog::getExistingDirectory(nullptr, "Otwórz zapis",
                                                    QDir::currentPath() + "/profiles",
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks).toStdString();
    readIntersections();
    readBusStops();
    readRoads();
    readConnections();
    readRoutes();
    readLines();
    readBuses();
    readWorkers();
    readShifts();
    readServices();
    readJobs();
    readSpecialDays();
}

void Data::writeData(QString path)
{
    auto copy = dir;
    if(path.size())
    {
        auto path_std = path.toStdString();
        std::filesystem::copy_file(dir+"\\map.jpg",path_std+"\\map.jpg");
        std::filesystem::copy_file(dir+"\\special_days.txt",path_std+"\\special_days.txt");
        std::filesystem::copy_file(dir+"\\special_days_const.txt",path_std+"\\special_days_const.txt");
        dir = path_std;
    }
    writeLines();
    writeRoutes();
    writeConnections();
    writeRoads();
    writeBusStops();
    writeIntersections();
    writeBuses();
    writeWorkers();
    writeShifts();
    writeJobs();
    writeServices();
    dir = copy;
}

Connection *Data::getConnectionFromStopsNames(std::string s1,std::string s2)
{
    BusStop* bs1 = nullptr; BusStop* bs2 = nullptr;
    for(const auto& el:bus_stops)
    {
        if(el.second->getName()==s1)
            bs1 = el.second.get();
        if (el.second->getName()==s2)
            bs2 = el.second.get();
        else if(bs1 and bs2)
            break;
    }
    return connections[bs1->getID()][bs2->getID()].get();
}

std::pair<int, std::string> Data::getDayType(QDate date)
{
    std::pair<int,std::string> res;
    std::vector<int> day_types {0,0,0,0,0,2,3};
    int pattern = date.day()*100 + date.month();
    if(const_special_days.find(pattern) != const_special_days.end())
    {
        res.second = const_special_days.at(pattern).second;
        res.first = const_special_days.at(pattern).first;
    }
    else if (special_days.find(date) != special_days.end())
    {
        res.second = special_days.at(date).second;
        res.first  = special_days.at(date).first;
    }
    else if((date.month() == 7 || date.month() == 8) && date.dayOfWeek()>0 && date.dayOfWeek()<6)
    {
        res.first = 1;
        res.second = "Wakacje";
    }
    else
        res.first  = day_types[date.dayOfWeek()-1];

    return res;
}

void Data::readIntersections()
{
    std::string file_name = dir + "\\intersections.txt";
    std::ifstream file(file_name);
    if(file)
    {
        std::string ID, line;
        while(getline(file,ID,';')&&getline(file,line))
        {   
            int x,y;
            std::stringstream ss;  
            ss<<line;
            ss>>x;
            ss>>y;
            QPoint point{x,y};
            auto in = std::make_shared<Intersection>(ID,point);
            intersections[ID] = in;
        }
    }
}

void Data::readBusStops()
{
    std::string file_name = dir + "\\stops.txt";
    std::ifstream file(file_name);
    if(file)
    {
        std::string ID, name, line;
        while(getline(file,ID,';')&&getline(file,name,';')&&getline(file,line))
        {
            int x,y,num;
            std::vector<int> numbers;
            std::stringstream ss;
            ss<<line;
            ss>>x;
            ss>>y;
            while(ss>>num)
                numbers.push_back(num);
            QPoint point{x,y};
            auto bs = std::make_shared<BusStop>(ID,point,name,numbers);
            bus_stops[ID] = bs;
            intersections[ID] = bs;
        }
    }
}

void Data::readRoads()
{
    std::string file_name = dir + "\\roads.txt";
    std::ifstream file(file_name);
    if(file)
    {
        std::string ID, i1, i2;
        while(getline(file,ID,';') && getline(file,i1,';') && getline(file,i2))
        {


            auto p1 = intersections.at(i1).get();
            auto p2 = intersections.at(i2).get();
            auto rd = std::make_shared<Road>(ID,p1,p2);
            roads[ID] = rd;
        }
    }
}

void Data::readConnections()
{
    std::string file_name = dir + "\\connections.txt";
    std::ifstream file(file_name);
    if(file)
    {
       std::string ID1, ID2, line1, line2;
        while(getline(file,ID1,';') && getline(file,ID2,';') && getline(file,line1,';') && getline(file,line2))
        {
            double distance;
            int min, sec;
            std::string road;
            std::list<Road*> rds;
            std::vector<QTime> timescales;
            std::stringstream ss, ss2;   
            ss<<line1;
            ss>>distance;
            while(ss>>min&&ss>>sec)
            {
                timescales.push_back({0,min,sec});
            }
            ss2<<line2;
            while(ss2>>road)
                rds.push_back(this->roads[road].get());
            auto con = std::make_shared<Connection>(bus_stops[ID1].get(),bus_stops[ID2].get(),distance,timescales,rds);
            connections[ID1][ID2] = con;
        }
    }
}

void Data::readRoutes()
{
    std::string file_name = dir + "\\routes.txt";
    std::ifstream file(file_name);
    if(file)
    {
        std::string ID, direction, stop_ID, line1, line2;
        while(getline(file,ID,';') && getline(file,direction,';') && getline(file,line1,';')&& getline(file,line2))
        {
            std::vector<std::string> route_stops;
            std::list<Connection*> route_connections;
            std::vector<std::vector<int>> timescales;
            std::vector<int> timescale;
            std::stringstream ss1, ss2;
            int number;
            std::list<int> numbers;
            ss1<<line1;
            ss2<<line2;
            while(ss1>>stop_ID && ss1>>number)
            {
                route_stops.push_back(stop_ID);
                numbers.push_back(number);
            }
            for(int i=1;i<route_stops.size();++i)
            {
                route_connections.push_back(connections[route_stops[i-1]][route_stops[i]].get());
            }
            int cnt = 1;
            while(ss2>>number)
            {
                if(cnt < route_connections.size())
                    timescale.push_back(number);
                else
                {
                    timescale.push_back(number);
                    timescales.push_back(timescale);
                    timescale.clear();
                    cnt = 0;
                }
                cnt++;
            }
            //timescales.push_back(timescale);
            auto rt = std::make_shared<Route>(ID,direction,route_connections,numbers,timescales);
            routes[ID]=rt;
        }
    }
}

void Data::readLines()
{
    std::string file_name = dir + "\\lines.txt";
    std::ifstream file(file_name);
    if(file)
    {
        std::string ID, name, route_ID, line;
        while(getline(file,ID,';') && getline(file,name,';') && getline(file,line))
        {
           std::vector<Route*> local_routes;
           std::stringstream ss;
           ss<<line;
           while(ss>>route_ID)
                local_routes.push_back(routes[ID+"/"+route_ID].get());
           auto li = std::make_shared<Line>(ID,name,local_routes);
           lines[ID]=li;
        }
    }
}

void Data::readBuses()
{
    std::string file_name = dir + "\\buses.txt";
    std::ifstream file(file_name);
    if(file)
    {
        std::string ID, name, line;
        int type;
        while(getline(file,ID,';') && getline(file,line,';') && getline(file,name))
        {
           type = std::stoi(line);
           auto bus = std::make_shared<Bus>(ID,name,type);
           buses[ID] = bus;
        }
    }
}

void Data::readWorkers()
{
    std::string file_name = dir + "\\workers.txt";
    std::ifstream file(file_name);
    if(file)
    {
        std::string ID, name;
        while(getline(file,ID,';') && getline(file,name))
        {
           auto wrk = std::make_shared<Worker>(ID,name);
           workers[ID] = wrk;
        }
    }
}

void Data::readShifts()
{
    std::string file_name = dir + "\\shifts.txt";
    std::ifstream file(file_name);
    if(file)
    {
        std::string ID, line, bus, worker;
        while(getline(file,ID,';') && getline(file,line,';') && getline(file,bus,';') && getline(file,worker))
        {
           std::stringstream ss;
           ss<<line;
           int hour,minute,second;
           ss>>hour; ss>>minute; ss>>second;
           QTime start {hour,minute,second};
           ss>>hour; ss>>minute, ss>>second;
           QTime finish {hour,minute,second};
           auto sft = std::make_shared<Shift>(ID,buses[bus].get(),workers[worker].get(),start,finish);
           shifts[ID] = sft;
        }
    }
}

void Data::readServices()
{
    std::string file_name = dir + "\\services.txt";
    std::ifstream file(file_name);
    if(file)
    {
        std::string ID, h, m, s, ts;
        while(getline(file,ID,';') && getline(file,h,':')&& getline(file,m,':')&& getline(file,s,';') && getline(file,ts))
        {
           std::string route_ID;

           std::regex reg("^(\\d{5}/\\S+)/\\S+/\\S+$");
           std::smatch base_match;
           std::regex_match(ID, base_match, reg);
           auto sub_match = base_match[1];
           route_ID = sub_match.str();
           int timescale = std::stoi(ts);

           QTime departure_time = {std::stoi(h),std::stoi(m),std::stoi(s)};
           auto serv = std::make_shared<Service>(ID,routes[route_ID].get(),departure_time,timescale);
           services[ID] = serv;
        }
    }
}

void Data::readSpecialDays()
{
    std::string file_name = dir + "\\special_days.txt";
    std::ifstream file(file_name);
    if(file)
    {
        std::string day,month,year,type,desc;
        while(getline(file,day,'.') && getline(file,month,'.') && getline(file,year,';')&& getline(file,type,';')&& getline(file,desc))
        {
           QDate date {std::stoi(year),std::stoi(month),std::stoi(day)};
           special_days[date] = std::make_pair(std::stoi(type),desc);
        }
    }
    std::string file_name2 = dir + "\\special_days_const.txt";
    std::ifstream file2(file_name2);
    if(file2)
    {
        std::string day,month,type,desc;
        while(getline(file2,day,'.') && getline(file2,month,';')&& getline(file2,type,';')&& getline(file2,desc))
        {
           const_special_days[std::stoi(day)*100 + std::stoi(month)] = std::make_pair(std::stoi(type),desc);
        }
    }
}

void Data::readJobs()
{
    std::string file_name = dir + "\\jobs.txt";
    std::ifstream file(file_name);
    if(file)
    {
        std::string ID, ID_copy, line1, line2, line3, line4;
        while(getline(file,ID,';') && getline(file,line1,';') && getline(file,line2,';') && getline(file,line3,';') && getline(file,line4))
        {
           std::string shift_ID, service_ID;
           std::vector<std::vector<Shift*>> local_shifts;
           std::vector<std::vector<Service*>> local_services;
           std::vector<QTime> def_freq;
           std::vector<int> day_types;
           std::stringstream ss1, ss2, ss3, ss4;
           ss1 << line1;
           std::vector<Shift*> shift_vec;
           std::vector<Service*> services_vec;
           int i = -1;
           ID_copy = ID;
           while(ss1>>shift_ID)
           {
                if(shift_ID == "A")
                {
                    local_shifts.push_back(shift_vec);
                    local_services.push_back(services_vec);
                    ++i;
                    if(i > 0)
                        ID[ID.length()-2] += 2;
                }
                local_shifts[i].push_back(shifts[ID + "/" + shift_ID].get());
           }
           ID = ID_copy;
           ss2 << line2;
           for(int i = 0; i < local_shifts.size();++i)
           {
                int h,m,s;
                ss2>>h; ss2>>m; ss2>>s;
                def_freq.push_back(QTime{h,m,s});
           }
           ss3 << line3;
           for(int i = 0; i < 5; ++i)
           {
                int dt;
                ss3>>dt;
                day_types.push_back(dt);
           }
           ss4<<line4;
           while(ss4>>service_ID)
           {
                if(service_ID[9]=='0')
                    local_services[0].push_back(services[service_ID].get());
                else if(service_ID[9]=='2')
                    local_services[1].push_back(services[service_ID].get());
                else if(service_ID[9]=='4')
                    local_services[2].push_back(services[service_ID].get());
                else if(service_ID[9]=='6')
                    local_services[3].push_back(services[service_ID].get());
                else if(service_ID[9]=='8')
                    local_services[4].push_back(services[service_ID].get());
           }
           auto job = std::make_shared<Job>(ID,local_shifts,def_freq,day_types,local_services);
           jobs[ID] = job;
        }
    }
}



void Data::writeLines()
{
    std::string file_name = dir + "\\lines.txt";
    std::ofstream file(file_name);
    if(file)
    {
        for(const auto& el: lines)
        {
            file<<el.second->writeInfo()<<std::endl;
        }
    }
}

void Data::writeRoutes()
{
    std::string file_name = dir + "\\routes.txt";
    std::ofstream file(file_name);
    if(file)
    {
        for(const auto& el: routes)
        {
            file<<el.second->writeInfo()<<std::endl;
        }
    }
}

void Data::writeConnections()
{
    std::string file_name = dir + "\\connections.txt";
    std::ofstream file(file_name);
    if(file)
    {
        for(const auto& el: connections)
        {
           for(const auto& el2 : el.second)
                file<<el2.second->writeInfo()<<std::endl;
        }
    }
}

void Data::writeRoads()
{
    std::string file_name = dir + "\\roads.txt";
    std::ofstream file(file_name);
    if(file)
    {
        for(const auto& el: roads)
        {
           file<<el.second->writeInfo()<<std::endl;
        }
    }
}

void Data::writeBusStops()
{
    std::string file_name = dir + "\\stops.txt";
    std::ofstream file(file_name);
    if(file)
    {
        std::vector<std::string> IDs;
        for(const auto& el: bus_stops)
        {
           file<<el.second->writeInfo()<<std::endl;
           IDs.push_back(el.first);
        }
        for(const auto& el: IDs)
        {
           intersections.erase(el);
        }
    }
}

void Data::writeIntersections()
{
    std::string file_name = dir + "\\intersections.txt";
    std::ofstream file(file_name);
    if(file)
    {
        for(const auto& el: intersections)
        {
           file<<el.second->writeInfo()<<std::endl;
        }
    }
}

void Data::writeBuses()
{
    std::string file_name = dir + "\\buses.txt";
    std::ofstream file(file_name);
    if(file)
    {
        for(const auto& el: buses)
        {
           file<<el.second->writeInfo()<<std::endl;
        }
    }
}

void Data::writeWorkers()
{
    std::string file_name = dir + "\\workers.txt";
    std::ofstream file(file_name);
    if(file)
    {
        for(const auto& el: workers)
        {
           file<<el.second->writeInfo()<<std::endl;
        }
    }
}

void Data::writeShifts()
{
    std::string file_name = dir + "\\shifts.txt";
    std::ofstream file(file_name);
    if(file)
    {
        for(const auto& el: shifts)
        {
           file<<el.second->writeInfo()<<std::endl;
        }
    }
}

void Data::writeJobs()
{
    std::string file_name = dir + "\\jobs.txt";
    std::ofstream file(file_name);
    if(file)
    {
        for(const auto& el: jobs)
        {
           file<<el.second->writeInfo()<<std::endl;
        }
    }
}

void Data::writeServices()
{
    std::string file_name = dir + "\\services.txt";
    std::ofstream file(file_name);
    if(file)
    {
        for(const auto& el: services)
        {
           file<<el.second->writeInfo()<<std::endl;
        }
    }
}


