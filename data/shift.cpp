
#include "shift.h"

Shift::Shift()
{

}

std::string Shift::writeTimes() const
{
    std::string res;
    res+=start.toString("hh mm ss").toStdString();
    res+=" "+finish.toString("hh mm ss").toStdString();
    return res;
}

