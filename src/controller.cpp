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
	dispatcher().assign("/toggle", &controller::toggle, this);
	status = 0;
	message = "Ready";
}

void controller::serveStatus(){
	json dat;
	dat["version"] = "0.1";
	dat["status"] = status;
	dat["message"] = message;
	response().out() << dat.dump();
}

void controller::toggle(){
	if(request().request_method() == "POST"){
	if(state){
		float brightness = n->getBrightness();
		printf("Brightness: %f", brightness);
		while(brightness > 0){
			brightness -= .1f;
			n->setBrightness(brightness);
			n->show();
		}
		state = false;
	}else{
		float brightness = n->getBrightness();
		printf("brightness: %f", brightness);
		int i;
		
		for(i=0;i<PIXELS;i++){
			n->setPixelColor(i, 255,255,255);
		}
		n->setBrightness(0.1f);
		n->show();
		while(brightness < .9f){
			printf("Increasing... %f", brightness);
			brightness += .1f;
			n->setBrightness(brightness);
			n->show();
		}
		state = true;
	}	
}
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
	while(true){
		n->rainbowCycle(10);
	}
    try{
        cppcms::service app(argc,argv);
        app.applications_pool().mount(cppcms::applications_factory<controller>());
        app.run();
    }catch(std::exception const &e){
        std::cerr<<e.what()<<std::endl;
    };
}
