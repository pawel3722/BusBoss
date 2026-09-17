#ifndef DATA_H
#define DATA_H

#include "data/job.h"
#include "line.h"
#include <map>
#include <memory>

class Data
{
public:
    Data(QString path = "");
    ~Data();
    void readData(QString path = "");
    void writeData(QString path = "");

    std::map<std::string,std::shared_ptr<BusStop>>* getBusStops() {return &bus_stops;}
    std::map<std::string,std::shared_ptr<Intersection>>* getIntersections() {return &intersections;}
    std::map<std::string,std::shared_ptr<Road>>* getRoads() {return &roads;}
    std::map<std::string,std::map<std::string,std::shared_ptr<Connection>>>* getConnections() {return &connections;}
    std::map<std::string,std::shared_ptr<Route>>* getRoutes() {return &routes;}
    std::map<std::string,std::shared_ptr<Line>>* getLines() {return &lines;}
    std::map<std::string,std::shared_ptr<Job>>* getJobs() {return &jobs;}
    std::map<std::string,std::shared_ptr<Service>>* getServices() {return &services;}
    std::map<std::string,std::shared_ptr<Shift>>* getShifts() {return &shifts;}
    std::map<std::string,std::shared_ptr<Bus>>* getBuses() {return &buses;}
    std::map<std::string, std::shared_ptr<Worker>>* getWorkers() {return &workers;}
    std::string getDir() {return dir;}
    Connection* getConnectionFromStopsNames(std::string s1,std::string s2);
    std::pair<int,std::string> getDayType(QDate date);
    std::map<int,std::pair<int,std::string>>* getConstSpecialDays() {return &const_special_days;}
    const std::map<QDate,std::pair<int,std::string>>* getSpecialDays() const {return &special_days;}

    void addIntersection(std::string ID, std::shared_ptr<Intersection> ptr) {intersections[ID]= ptr;}
    void addBusStop(std::string ID, std::shared_ptr<BusStop> ptr) {bus_stops[ID]= ptr;}
    void addRoad(std::string ID, std::shared_ptr<Road> ptr) {roads[ID]= ptr;}

    void deleteIntersection(std::string ID) {intersections.erase(ID);}
    void deleteBusStop(std::string ID ) {bus_stops.erase(ID);}
    void deleteRoad(std::string ID) {roads.erase(ID);}

private:

    void readIntersections();
    void readBusStops();
    void readRoads();
    void readConnections();
    void readRoutes();
    void readLines();
    void readBuses();
    void readWorkers();
    void readShifts();
    void readJobs();
    void readServices();
    void readSpecialDays();

    void writeLines();
    void writeRoutes();
    void writeConnections();
    void writeRoads();
    void writeBusStops();
    void writeIntersections();
    void writeBuses();
    void writeWorkers();
    void writeShifts();
    void writeJobs();
    void writeServices();

    std::string dir;
    std::map<std::string,std::shared_ptr<BusStop>> bus_stops;
    std::map<std::string,std::shared_ptr<Intersection>> intersections;
    std::map<std::string,std::shared_ptr<Road>> roads;
    std::map<std::string,std::map<std::string,std::shared_ptr<Connection>>> connections;
    std::map<std::string,std::shared_ptr<Route>> routes;
    std::map<std::string,std::shared_ptr<Line>> lines;
    std::map<std::string,std::shared_ptr<Bus>> buses;
    std::map<std::string,std::shared_ptr<Worker>> workers;
    std::map<std::string,std::shared_ptr<Shift>> shifts;
    std::map<std::string,std::shared_ptr<Job>> jobs;
    std::map<std::string,std::shared_ptr<Service>> services;

    std::map<int,std::pair<int,std::string>> const_special_days;
    std::map<QDate,std::pair<int,std::string>> special_days;
};

#endif // DATA_H
