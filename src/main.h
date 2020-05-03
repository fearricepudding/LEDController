#include "libs.h"

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

class Application: public cppcms::application {
    public:
        Application(cppcms::service &srv);
        std::string BoolToString(bool b);
        void test();
        void json_out(std::string message);
        void status();
        void toggle();
        void static log(std::string message);
};

#endif // MAIN_H_INCLUDED
