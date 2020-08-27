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
	dispatcher().assign("/brightness", &controller::setBrightness, this);
	dispatcher().assign("/white", &controller::white, this);
	dispatcher().assign("/rainbow", &controller::idleRainbow, this);
	dispatcher().assign("/status", &controller::stripStatus, this);
	status = 0;
	message = "Ready";
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
//				printf("Decreasing: %f\n", brightness);
				n->setBrightness(brightness);
				n->show();
				usleep(10000);
			}
			state = false;
		}else{
			float brightness = .1f;
			n->setBrightness(.1f);
			n->show();
			while(brightness < 1.f){
				brightness += .01f;
//				printf("Increasing... %f\n", brightness);
				n->setBrightness(brightness);
				n->show();
				usleep(10000);
			};
			state = true;
		};
		stripStatus();
	}else{
		response().status(403);
		response().out() << "403 - Method Not Allowd";
	};
}

/**
* Set brightness of the strip
*
* @return void
*/
void controller::setBrightness(){
	if(request().request_method() == "POST"){
//		std::string postData = request().post((std::string)"data");

		std::pair<void *,size_t> data = request().raw_post_data();
		std::string postdata(reinterpret_cast<char *>(data.first),data.second);
		std::cout << postdata << std::endl;
	}else{
		response().status(403);
		response().out() << "403 - Method Not Allowd";
	}
}	

/**
* Reset all LEDs to full white
*
* @return void
*/
void controller::white(){
	int i;
	for(i=0;i<PIXELS;i++){
		n->setPixelColor(i, 255, 255, 255);
	}
	n->setBrightness(1.f);
	n->show();
	state = true;
	stripStatus();
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
void controller::idleRainbow(){
	n->setBrightness(.3f);
	state = true;
	while(animate){
		n->rainbowCycle(5);
	}
}

void shutdown(){
	printf("Shutting down");
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
    
    shutdown();
}
