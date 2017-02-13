#include "gui.hpp"


GUI::GUI(){
	refBuilder = Gtk::Builder::create();
	try
	{
		//refBuilder->add_from_file("src/gui/src/GUI_glade.glade");
		refBuilder->add_from_file("src/interface/src/GUI.glade");
	}
	catch(const Glib::FileError& ex)
	{
		std::cerr << "FileError: " << ex.what() << std::endl;
	}
	catch(const Glib::MarkupError& ex)		
	{
		std::cerr << "MarkupError: " << ex.what() << std::endl;
	}
	catch(const Gtk::BuilderError& ex)
	{
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		
	}
	std::cerr << "created gui" << std::endl;

	//std::cerr << "load ptextview" << std::endl;
	refBuilder->get_widget("Car_Log",ptext_view);
	//std::cerr << "get buffer" << std::endl;
	buffer = ptext_view->get_buffer();
	//std::cerr << "got buffer" << std::endl;

	createLogFile();

	refBuilder->get_widget("window1",this->pWindow);

	
	//Call print_to_log to print log message
	//std::cerr << "before print" << std::endl;
	print_to_log("Started Graphical User Interface for Temporal Insanity");

	_acceleration.data = 0;
	_steering.data = 0;

	_motor.data = 0;
	_servo.data = 0;
	pre_points.data.resize(9);
	pid_settings.data.resize(6);
	bird_eye_settings.data.resize(8);
	images.resize(3);

	// Status vector
	refBuilder->get_widget("label7",right_image_label);
	right_image_label->set_label("Original view");
	image_labels.push_back("Original view");
	image_labels.push_back("Lanes");
	image_labels.push_back("LIDAR"); 	


	dispatcher.connect(sigc::mem_fun(*this,&GUI::dispatch_notification));


	//pWindow->add_events(Gdk::KEY_PRESS_MASK);

	//this->signal_key_press_event().connect( 
	//	sigc::mem_fun( *this, &GUI::onKeyPress), false);


}

GUI::~GUI(){
	log_file.close();
}

bool GUI::on_key_press_event(GdkEventKey* event){
	//std::cerr << event->keyval << ' ' << event->hardware_keycode << ' ' << event->state << std::endl;
	/*  left arrow  = 65361 113 16
		right arrow = 65363 114 16
		up arrow    = 65362 111 16
		down arrow  = 65364 116 16
	*/
	if(event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Up){

		std::cerr << "up arrow" << std::endl;
		//if(_acceleration.data != 255){
			//if(_acceleration.data < 255) _acceleration.data +=5;
			_acceleration.data = 255;
			pub_manual_acceleration.publish(_acceleration);
		//}

	}else if(event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Right){

		std::cerr << "right arrow" << std::endl;
		//if(_steering.data != 100){
			//if(_steering.data < 100) _steering.data += 5;
			_steering.data = 100;
			pub_manual_steering.publish(_steering);
		//}

	}else if(event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Left){

		std::cerr << "left arrow" << std::endl;
		//if(_steering.data != 0){
			//if(_steering.data > 0) _steering.data -=5;
			_steering.data = 0;
			pub_manual_steering.publish(_steering);
		//}

	}else if(event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Down){
		//if(_acceleration.data != -255){
			//if(_acceleration.data > -255) _acceleration.data -=5;
			std::cerr << "down arrow" << std::endl;
			_acceleration.data = -255;
			pub_manual_acceleration.publish(_acceleration);
		//}

	}else{

		return Gtk::Window::on_key_press_event(event);
	}

}


void GUI::print_to_log(std::string message){
	std::ostringstream os;
	//get current time
	time_t rawtime;
	struct tm * timeinfo;
	char timebuffer[80];
	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(timebuffer,80,"%H:%M:%S >> ",timeinfo);
	std::string timestr(timebuffer);

	//compose log message: 
	os << timestr << message << std::endl;
	message = os.str();
	latest_message = message;
	//buffer->insert(buffer->end(),message);
	log_file.open("LogFiles/"+file_name, std::ios_base::app);
	log_file << message; //To log file
	log_file.close();
	//TODO - scroll to end.

    //GtkTextBuffer *buffer_2 = NULL;
    //GtkTextMark *mark = NULL;
	//GtkTextIter iter_start, iter_end;
	
	//Auto scroll after each message

	
	dispatcher.emit();

	
	


}

void GUI::logCallback(const std_msgs::String::ConstPtr& msg){
	//std::cerr << msg->data.c_str() << std::endl;
	print_to_log(msg->data.c_str());
}

bool GUI::on_timeout(int timer_number){
	//Publishes settings from the gui


	Gtk::Adjustment * lu1 = nullptr;
	Gtk::SpinButton * temp_button = nullptr;

	//Manual control
	
	Gtk::ToggleButton * temp_toggle_button = nullptr;
	refBuilder->get_widget("button_manual_control",temp_toggle_button);
	int old_manual_control = manual_control_msg.data;
	manual_control_msg.data = temp_toggle_button->get_active();
	//if(manual_control_msg.data != old_manual_control) pub_manual_control.publish(manual_control_msg);
	pub_manual_control.publish(manual_control_msg);

	//settings
	
	//std::cerr << std::endl;
	//return true if continue timer
	Gtk::Switch * temp_switch = nullptr;



	//Sensor update gui
	int max_battery = 16;
	int min_battery = 13; //To be updated
	Gtk::ProgressBar * temp_progress_bar = nullptr;
	refBuilder->get_widget("battery_bar",temp_progress_bar);
	
	std::stringstream ss;
	ss << "Battery: " << battery_level << "V";
	double fraction = (battery_level-min_battery)/(max_battery-min_battery);
	temp_progress_bar->set_text(ss.str());
	temp_progress_bar->set_fraction(fraction);

	Gtk::Label * temp_label = nullptr;
	refBuilder->get_widget("car_speed",temp_label);
	
	ss.str("");
	ss << std::fixed << std::setprecision( 2 ) << car_speed << " km/h";
	temp_label->set_text(ss.str());

	return true;

}


void GUI::systemstatusCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){
	//std::cerr << "Got system status update" << std::endl;
	 for(int i = 0; i < msg->data.size(); ++i){
	 	if(msg->data[i] == 1){
	 		//icons[i].set_icon(icon_ok);
	 		this->status[i] = true;
	 	}
	 	else{
	 		//icons[i].set_icon(icon_error);
	 		this->status[i] = false;
	 	}
	 }
}

void GUI::createLogFile(){

	//std::ifstream log_file;
	//std::string line;

	//std::ostringstream os;
	std::chrono::time_point<std::chrono::system_clock> current_time;
	current_time = std::chrono::system_clock::now();
	std::time_t print_time = std::chrono::system_clock::to_time_t(current_time);
	std::cerr << std::ctime(&print_time) << std::endl;

	file_name.append(std::ctime(&print_time));
	file_name.append(".txt");

	log_file.open("LogFiles/"+file_name);



	//os << timestr;


}

void GUI::sensordataCallback(const std_msgs::Int32MultiArray::ConstPtr& msg){

  //msg elements: AX AY AZ GX GY GZ MX MY MZ MpuTimeStamp CarSpeed CarSpeedTimeStamp BatteryLevel
  //               0  1  2  3  4  5 6  7  8    9               10        11              12
	
	//Manual control
	battery_level = float(msg->data[12])/10;
	car_speed = float(msg->data[10])*0.036; //cm/s to km/h


}

void GUI::dispatch_notification(){
	
		myMutex.lock();
		
		buffer->insert(buffer->end(),latest_message);

		Gtk::ScrolledWindow * scrolled_window = nullptr;
		refBuilder->get_widget("Scroll_Log",scrolled_window);
		scrolled_window->get_vadjustment()->set_value(scrolled_window->get_vadjustment()->get_upper());

		myMutex.unlock();
}
