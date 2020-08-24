#include <cppcms/application.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/applications_pool.h>
#include <nlohmann/json.hpp>
#include "ws2812-rpi.h"
#include "controller.h"

using json = nlohmann::json;
#define PIXELS 191

NeoPixel *n=new NeoPixel(PIXELS);

controller::controller(cppcms::service &srv): cppcms::application(srv){
	dispatcher().assign("", &controller::serveStatus, this);
	status = 0;
	message = "Ready";
}

void controller::serveStatus(){
	json response;
	response["version"] = "0.1";
	response["status"] = status;
	response["message"] = message;
}

void rainbow(){
	n->setBrightness(.3f);
	while(true){
	    	n->rainbowCycle(5);
	}
}

void shutdown(){
	delete n;
}

int main(int argc,char ** argv){
    try{
        cppcms::service app(argc,argv);
        app.applications_pool().mount(cppcms::applications_factory<controller>());
        app.run();
    }catch(std::exception const &e){
        std::cerr<<e.what()<<std::endl;
    };
}
