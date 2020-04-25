#include "main.h"

static LEDController controller;

Application::Application(cppcms::service &srv) :
    cppcms::application(srv)
{

    dispatcher().assign("/test", &Application::test, this);
    mapper().assign("/test");
};

std::string Application::BoolToString(bool b)
{
  return b ? "true" : "false";
}

void Application::test()
{
    bool SERV_OK = controller.check();
    json_out(BoolToString(SERV_OK));
};

void Application::json_out(std::string message){
    response().out() << "Hello, world!";
  //'{"status":"ok", "message": "' << message << '"}';
};

int main(int argc,char ** argv)
{
    controller.state = false;
    controller.ready = true;
    try {
        cppcms::service app(argc,argv);
        app.applications_pool().mount(cppcms::applications_factory<Application>());
        app.run();
    }
    catch(std::exception const &e) {
        std::cerr<<e.what()<<std::endl;
    }
}
