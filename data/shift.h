#ifndef SHIFT_H
#define SHIFT_H

#include "qdatetime.h"
#include <iostream>
#include "bus.h"
#include "worker.h"


class Shift
{
public:
    Shift();
    Shift(std::string i,Bus* b, Worker* w, QTime s, QTime f) :name(i), bus(b), worker(w), start(s), finish(f){}
    std::string writeInfo() const { return name + ";" + writeTimes() + ";" + bus->getID() + ";" + worker->getID(); }
    std::string getLetter() const {return {*name.rbegin()};}
    std::string getID() const {return name;}
    std::string getWorkerID() const {return worker->getID();}
    std::string getBusID() const {return bus->getID();}
    void setBus(Bus* b) {bus=b;}
    const Worker* getWorker() const {return worker;}
    void setWorker(Worker* w) {worker = w;}
    const Bus* getBus() const {return bus;}
    QTime getStartTime() const {return start;}
    QTime getFinishTime() const {return finish;}


private:
    std::string name;
    Bus* bus;
    Worker* worker;
    QTime start, finish;

    std::string writeTimes() const;
};

#endif // SHIFT_H
