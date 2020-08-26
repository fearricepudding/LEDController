
class controller: public cppcms::application{
	public:
		bool state; 							// State of the LEDS 0 - off / 1 - on
		std::string message = "Starting up";	// Status message of program
		int status = 1;							// Status int of program if != 0 not ready
		controller(cppcms::service &srv);
	private:
		void initLEDS();
		void toggle();
		void serveStatus();
		void idleRainbow();
		void setBrightness();
		void white();
};
