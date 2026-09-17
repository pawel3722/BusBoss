
#include "line.h"

Line::Line()
{

}

const std::vector<std::string> Line::getRoutes() const
{
    std::vector<std::string> res;
    for(const auto& el: routes)
        res.push_back(el->getID());
    return res;
}

void Line::delRoute(Route *r)
{
    auto it = std::find(routes.begin(),routes.end(),r);
    if(it!=routes.end())
        routes.erase(it);
}

