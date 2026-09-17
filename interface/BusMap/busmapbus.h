
#ifndef BUSMAPBUS_H
#define BUSMAPBUS_H
#include "data/data.h"
#include "data/job.h"
#include "qpushbutton.h"


class BusMapBus : public QPushButton
{
public:
    BusMapBus();
    BusMapBus(std::string i, Job* j, QWidget* ptr) :QPushButton(ptr) {ID = i, job = j; this->hide(); setColor();}
    void setColor();
    void setPosition(QTime t, Data* data, int day_type, int prev_dt);
    Job* getJobPointer() const {return job;}
    void resetCurrentService() {job->resetCurrentService();}
    void setCurrentDay(int dt, int pdt, QTime t) {job->setCurrentDay(dt,pdt,t);}

private:
    std::string ID;
    Job* job;
    const int WIDTH = 28;
};

#endif // BUSMAPBUS_H
