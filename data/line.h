
#ifndef LINE_H
#define LINE_H

#include "route.h"
#include <sstream>
#include <iomanip>

class Line
{
public:
    Line();
    Line(std::string i,std::string n) : ID(i),name(n) {}
    Line(std::string i,std::string n,std::vector<Route*>& r) :ID(i),name(n),routes(r){}
    std::string writeInfo() const {return ID+";"+name+";"+writeRoutes();}
    const std::vector<std::string> getRoutes() const;
    std::string getName() const {return name;}
    std::string getID() const {return ID;}
    void addRoute(Route* r) {routes.push_back(r);}
    void delRoute(Route* r);

private:
    std::string ID;
    std::string name;
    std::vector<Route*> routes;

    inline std::string writeRoutes() const
    {
        std::string res;
        for(const auto& el:routes)
            res += " " + el -> getNumber();
        return res;
    }
};

#endif // LINE_H
