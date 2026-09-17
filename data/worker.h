#ifndef WORKER_H
#define WORKER_H
#include <iostream>




class Worker
{
public:
    Worker();
    Worker(std::string i,std::string n) :ID(i),name(n){};
    std::string writeInfo() const {return ID + ";" + name;}
    std::string getID() const {return ID;}
    std::string getName() const {return name;}

private:
    std::string ID;
    std::string name;

};

#endif // WORKER_H
