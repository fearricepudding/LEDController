#include <cppcms/application.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/applications_pool.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string> 
#include "ws2812-rpi.h"
#include "controller.h"
#include <json/json.h>

using json = nlohmann::json;
#define PIXELS 191

NeoPixel *n=new NeoPixel(PIXELS);

controller::controller(cppcms::service &srv): cppcms::application(srv){
	dispatcher().assign("", &controller::serveStatus, this);
	dispatcher().assign("/toggle", &controller::toggle, this);
	dispatcher().assign("/brightness", &controller::setBrightness, this);
	dispatcher().assign("/white", &controller::white, this);
	dispatcher().assign("/rainbow", &controller::rainbow, this);
	dispatcher().assign("/status", &controller::stripStatus, this);
	dispatcher().assign("/newPixels", &controller::newPixels, this);
//	dispatcher().assign("/stop", &controller::stopAnimation, this);
	status = 0;
	message = "Ready";
}

void controller::newPixels(){
	if(request().request_method() == "POST"){
		std::string postdata = request().post((std::string)"pixels");
//		std::cout << postdata << std::endl;
		
		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse( postdata, root );
		if ( !parsingSuccessful ){
		    std::cout << "Error parsing the string" << std::endl;
   		}
   		Json::Value newPixels = root;

		for ( int index = 0; index < newPixels.size(); ++index ){
		    n->setPixelColor(index, newPixels[index]["r"].asInt(), newPixels[index]["g"].asInt(), newPixels[index]["b"].asInt());
		}
		
		n->show();
		
		response().out() << "End";
	}else{
		response().status(403);
		response().out() << "403 - Method Not Allowed";
	}
}

/**
* Show the status of the controller
*
* @return void
*/
void controller::serveStatus(){
	json dat;
	dat["version"] = "0.1";
	dat["status"] = status;
	dat["message"] = message;
	response().out() << dat.dump();
	printf("Status request \n");
}

/**
* Toggle the LEDs off and back onto their previous setting
*
* @return void
*/
void controller::toggle(){
	if(request().request_method() == "POST"){
		if(state){
			float brightness = n->getBrightness();
			while(brightness > .0f){
				brightness -= .01f;
				n->setBrightness(brightness);
				n->show();
				usleep(10000);
			}
			state = false;
		}else{
			float brightness = .1f;
			while(brightness < 1.f){
				brightness += .01f;
				n->setBrightness(brightness);
				n->show();
				usleep(10000);
			};
			state = true;
		};
		stripStatus();
	}else{
		response().status(403);
		response().out() << "403 - Method Not Allowed";
	};
}
	
/**
* Set brightness of the strip
*
* @return void
*/
void controller::setBrightness(){
	if(request().request_method() == "POST"){
		std::string postdata = request().post((std::string)"data");
		std::cout << postdata << std::endl;
		try{
			float subbright = std::stof(postdata);
			float setBrightness;
			if(subbright < 0){
				setBrightness = 0;
			}else if(subbright > 1){
				setBrightness = 1;
			}else{
				setBrightness = subbright;
			};
			std::cout << setBrightness << std::endl;
			n->setBrightness(setBrightness);
		}catch(std::invalid_argument e){
			
			std::cout << "Number error - Brightness submit not a number" << std::endl;
		}
		stripStatus();
	}else{
		response().status(403);
		response().out() << "403 - Method Not Allowed";
	};
}	

/**
* Reset all LEDs to full white
*
* @return void
*/
void controller::white(){
	if(request().request_method() == "POST"){
		int i;
		for(i=0;i<PIXELS;i++){
			n->setPixelColor(i, 255, 255, 255);
		}
		n->show();
		stripStatus();
	}else{
		response().status(403);
		response().out() << "403 - Method not Allowed";
	};
}

/**
* LEDStip Status
*
* @return void
*/
void controller::stripStatus(){
	json r;
	r["pixels"];
	std::vector<Color_t> updateBuffer = n->getPixels();
	int i;
	for(i=0;i<PIXELS;i++){
		r["pixels"][i]["r"] = updateBuffer[i].r;
		r["pixels"][i]["g"] = updateBuffer[i].g;
		r["pixels"][i]["b"] = updateBuffer[i].b;
	}
	r["state"] = state;
	r["brightness"] = n->getBrightness();
	r["animating"] = animate;
	response().out() << r.dump();
}

/**
* Cool idle rainbow
*
* @return void
*/
void controller::rainbow(){
	if(request().request_method() == "POST"){
		n->rainbow();
		n->show();
		stripStatus();
	}else{
		response().status(403);
		response().out() << "403 - Method Not Allowed";	
	}
}

void shutdown(){
	std::cout << "Shutdown" << std::endl;
	delete n;
}

int main(int argc,char ** argv){
	std::cout << "Starting controller" << std::endl;
    try{
        cppcms::service app(argc,argv);
        app.applications_pool().mount(cppcms::applications_factory<controller>());
        app.run();
    }catch(std::exception const &e){
        std::cerr<<e.what()<<std::endl;
    };
    
    shutdown();
}
