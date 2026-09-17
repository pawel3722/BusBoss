#include "busmapintersection.h"

BusMapIntersection::BusMapIntersection(Intersection i, QWidget *ptr)
    : QPushButton(ptr) {
    ID = i.getID();
    point = i.getPoint();
    this->hide();
};