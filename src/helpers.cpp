
#include "helpers.h"
#include <sstream>

namespace LEDC{
    std::string helpers::stringify(std::map<std::string, std::string> map){
        std::stringstream output;
        output << "{";
        int count;
        for(auto i = map.begin(); i != map.end(); ++i){
            if(count > 0){
                output << ",";
            };
            output << "\"" << i->first << "\":\"" << i->second << "\"";
            ++count;
        };
        output << "}";
        return output.str();
    }
}