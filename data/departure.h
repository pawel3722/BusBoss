#ifndef DEPARTURE_H
#define DEPARTURE_H
#include <iostream>
#include "busstop.h"
#include "qdatetime.h"


class Departure
{
public:
    Departure();
private:
    std::string ID;
    BusStop* stop;
    QTime time;

};

#endif // DEPARTURE_H
