
#include "interface/BusMap/busmapbus.h"

BusMapBus::BusMapBus()
{

}

void BusMapBus::setColor()
{
    this->setStyleSheet("background-color: rgb(255,221,0);");
    this->setText(QString::fromStdString(job->getLine()));
}

void BusMapBus::setPosition(QTime t, Data* data, int day_type, int prev_dt)
{
    QPoint point = job->getPosition(t, day_type, prev_dt);
    QPoint too_early_point {-2,-2}, too_late_point {-1,-1};
    if(point != too_early_point && point != too_late_point)
    {
        if(!this->isVisible())
        {
            auto name = (*(data->getLines()))[job->getCurrentService()->getLineID()]->getName();
            this->setText(QString::fromStdString(name));
            this->show();
        }
        this->setGeometry(point.x()-WIDTH/2,point.y()-WIDTH/2,WIDTH,WIDTH);
    }
    else
        this->hide();
}

