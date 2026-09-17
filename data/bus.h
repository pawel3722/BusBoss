#ifndef BUS_H
#define BUS_H
#include <iostream>
#include <string>



class Bus
{
public:
    Bus();
    Bus(std::string i,std::string m, int t):ID(i),model(m),type(static_cast<BusType>(t)){};
    std::string writeInfo() const {return ID +";"+std::to_string(static_cast<int>(type)) + ";" + model; }
    std::string getID() const {return ID;}
    std::string getModel() const {return model;}
    std::string getType() const
    {
        std::string res;
        switch(type)
        {
        case BusType::mini:
            res = "MINI";
            break;
        case BusType::midi:
            res = "MIDI";
            break;
        case BusType::maxi:
            res = "MAXI";
            break;
        case BusType::mega:
            res = "MEGA";
            break;
        }
        return res;
    }

private:
    std::string ID;
    std::string model;
    enum class BusType {mini=7,midi=9,maxi=12,mega=18};
    BusType type;

};

#endif // BUS_H
