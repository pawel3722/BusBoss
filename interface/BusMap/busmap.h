#ifndef BUSMAP_H
#define BUSMAP_H

#include "data/data.h"
#include "interface/BusMap/busmapbus.h"
#include <QWidget>
#include <QPaintEvent>
#include <QFileDialog>
#include <QPushButton>
#include <QTimer>
#include <vector>
#include <memory>
#include <interface/BusMap/busmaproad.h>
#include <interface/BusMap/busmapstop.h>


class BusMap : public QWidget
{
    Q_OBJECT
public:
    explicit BusMap(Data*, QWidget *parent = nullptr);
    ~BusMap();
    bool openMap(const QString&);
    void setSimMode();
    void setEditMode();
    void addBusStop();
    void addIntersection();
    void addRoad();
    void addRoute(std::string line);
    void deleteIntersection();
    void deleteRoad();
    void divideRoadwithIntersection();
    void divideRoadwithBusStop();
    void setTime(QTime t);
    QTime getTime() const {return simulation_time;}
    void setTimeSpeed(double d) {time_speed = d;}
    double getTimeSpeed() const {return time_speed;}
    void setDate(QDate d, int dt);
    QDate getDate() const {return simulation_date;}
    int getMapHSize() const {return displayed_map.width();}
    int getMapVSize() const {return displayed_map.height();}

protected:
    void mousePressEvent(QMouseEvent *event)override;
    void paintEvent(QPaintEvent*) override;
private slots:
    void intersectionClicked();
    void busStopClicked();
    void finishButtonClicked();
    void busClicked();
private:
    Data* data;
    QString map_name;
    QImage displayed_map;
    enum class BusMapAction {empty,add_stop,add_intersect,add_road,add_route,delete_intersect,delete_road,delete_route,div_rd_int,div_rd_stop};
    BusMapAction action;
    bool edit_mode = 0;

    BusMapIntersection* memorised_bus_stop = nullptr;
    BusMapIntersection* memorised_intersection = nullptr;
    BusMapRoad* memorised_road = nullptr;
    bool is_memorised_road_one_way = false;
    std::string adding_line;
    std::list<Road*> creating_connection;
    std::list<int> creating_route_platforms;
    std::list<Connection*> creating_route;

    std::map<std::string,std::shared_ptr<BusMapRoad>> roads;
    std::map<BusMapIntersection*,std::map<BusMapIntersection*,BusMapRoad*>> roads_graph;
    std::map<std::string,std::shared_ptr<BusMapIntersection>> intersections;
    std::map<std::string,std::shared_ptr<BusMapStop>> bus_stops;
    std::map<std::string,std::shared_ptr<BusMapBus>> buses;

    std::map<std::string,std::map<std::string,std::shared_ptr<Connection>>>* connections;
    std::map<std::string,std::shared_ptr<Route>>* routes;
    std::map<std::string,std::shared_ptr<Line>>* lines;

    QTimer *timer = new QTimer(this);
    QTime simulation_time;
    double time_speed = 1;
    QPushButton *finish_button;
    int prev_day_type;
    int day_type;
    int next_day_type;
    QDate previous_sim_date;
    QDate simulation_date;

    void drawMap();
    void readMapItems();
    void createBusStop(const QPoint&);
    void createIntersection(const QPoint&);
    void createRoad(BusMapIntersection*);
    void createRoute(BusMapIntersection*);
    void removeBusStop(BusMapIntersection* const);
    void removeIntersection(BusMapIntersection* const, bool checked=false);
    void removeRoad(BusMapIntersection* const);
    void divideRoad(BusMapIntersection* clicked_intersection,const QPoint& point, bool bus_stop);
    void doRoadDivision(bool if_second_direction=false);
    bool getPlatformCount(BusMapIntersection* const);
    void showBusStopInfo(BusMapStop*clicked_bus_stop);
    void addTime();
    void setPreviousDate() {previous_sim_date = simulation_date.addDays(-1); prev_day_type = data->getDayType(previous_sim_date).first; next_day_type = data->getDayType(simulation_date.addDays(1)).first;}

};

#endif // BUSMAP_H
