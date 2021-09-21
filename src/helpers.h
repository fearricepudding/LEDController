#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#include <stdio.h> 
#include <iostream>
#include <map>

namespace LEDC{

class helpers{
public: 
    static std::string stringify(std::map<std::string, std::string> map);
private:
};

};

#endif