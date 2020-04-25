#include <cppcms/application.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/applications_pool.h>
#include <iostream>
#include <stdlib.h>
#include "LEDController.h"

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

class Application: public cppcms::application {
    public:
        Application(cppcms::service &srv);
        std::string BoolToString(bool b);
        void test();
        void json_out(std::string);
};

#endif // MAIN_H_INCLUDED
