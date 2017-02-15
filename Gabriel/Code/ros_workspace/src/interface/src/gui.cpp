#include "gui.hpp"
#include <boost/filesystem.hpp>

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
	_steering.data = 50;

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

	record_slot = sigc::bind(sigc::mem_fun(*this,&GUI::record_timer), record_timer_number); 	
	


	pWindow->add_events(Gdk::KEY_PRESS_MASK);
	pWindow->add_events(Gdk::KEY_RELEASE_MASK);


	//this->signal_key_press_event().connect( 
	//	sigc::mem_fun( *this, &GUI::onKeyPress), false);


}

GUI::~GUI(){
	log_file.close();
}

bool GUI::on_key_press_event(GdkEventKey* event){
	//std::cerr << " Hello " << std::endl;
	//std::cerr << event->keyval << ' ' << event->hardware_keycode << ' ' << event->state << std::endl;
	/*  left arrow  = 65361 113 16
		right arrow = 65363 114 16
		up arrow    = 65362 111 16
		down arrow  = 65364 116 16
	*/
	//if(event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Up 
	//								|| event->keyval == GDK_KEY_Down
	//								|| event->keyval == GDK_KEY_Left
	//								|| event->keyval == GDK_KEY_Right){
		//pub_manual_acceleration.publish(_acceleration);
		//pub_manual_steering.publish(_steering);
	//}
	if(event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Up){

		//std::cerr << "up arrow" << std::endl;
		//if(_acceleration.data != 255){
			//if(_acceleration.data < 255) _acceleration.data +=5;
			_acceleration.data = 255;
			//pub_manual_acceleration.publish(_acceleration);
		//}

	}else if(event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Right){

		//std::cerr << "right arrow" << std::endl;
		//if(_steering.data != 100){
			//if(_steering.data < 100) _steering.data += 5;
			_steering.data = 100;
			//pub_manual_steering.publish(_steering);
		//}

	}else if(event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Left){

		//std::cerr << "left arrow" << std::endl;
		//if(_steering.data != 0){
			//if(_steering.data > 0) _steering.data -=5;
			_steering.data = 0;
			//pub_manual_steering.publish(_steering);
		//}

	}else if(event->type == GDK_KEY_PRESS && event->keyval == GDK_KEY_Down){
		//if(_acceleration.data != -255){
			//if(_acceleration.data > -255) _acceleration.data -=5;
			//std::cerr << "down arrow" << std::endl;
			_acceleration.data = -255;
			//pub_manual_acceleration.publish(_acceleration);
		//}

	}else if(event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_Up){
			//std::cerr << "released up arrow" << std::endl;
			_acceleration.data = 0;
			//pub_manual_acceleration.publish(_acceleration);
	}else if(event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_Down){
			//std::cerr << "released down arrow" << std::endl;
			_acceleration.data = 0;
			//pub_manual_acceleration.publish(_acceleration);
	

	}else if(event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_Left){
			//std::cerr << "released left arrow" << std::endl;
			_steering.data = 50;
			//pub_manual_steering.publish(_steering);
	}else if(event->type == GDK_KEY_RELEASE && event->keyval == GDK_KEY_Right){
			//std::cerr << "released right arrow" << std::endl;
			_steering.data = 50;
			//pub_manual_steering.publish(_steering);
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

	log_buffer.push_back(latest_message);
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
	
	return;

	
	


}

void GUI::logCallback(const std_msgs::String::ConstPtr& msg){
	//std::cerr << msg->data.c_str() << std::endl;
	print_to_log(msg->data.c_str());
}

bool GUI::record_timer(int timer_number){
	if(recording == false){
		//Do once
		recording = true;

		print_to_log("Started Recording");
		
		std::string folder_path = "Recorded_data/dataset";
		
		int folder_number = 0;
		//Loop through existing folder and then create a new unique one to store data in.
		while(true){
			std::ostringstream os_folder;
			os_folder << folder_path << folder_number;
			folder_number++;
			if( boost::filesystem::is_directory( os_folder.str() ) ){
				std::cerr << "Folder exists" << std::endl;

			} else{
				std::cerr << "folder does not exist" << std::endl;
				current_folder = os_folder.str();
				break;
			}
		}
			
		boost::filesystem::path dir(current_folder );
		boost::filesystem::create_directory(dir);
		print_to_log("Storing data at: " + current_folder);
		
	}

	//Store latest camera image together with the latest motor speed and steering value
	std::ostringstream os;
	std::ostringstream img_name;
	std::string log_text;
	img_name << "img";
	os << current_folder << "/";
	if(frame_counter < 10) img_name << "000" << frame_counter;
	else if(frame_counter < 100) img_name << "00" << frame_counter;
	else if(frame_counter < 1000) img_name << "0" << frame_counter;
	else if(frame_counter < 10000) img_name << frame_counter;
	img_name << ".jpg";
	os << img_name.str();

	cv::imwrite(os.str(),camera_image);
	frame_counter++;

	data_log.open(current_folder + "/data.txt", std::ios_base::app);
	data_log << img_name.str() << " " << _acceleration.data << " " << _steering.data << std::endl;
	data_log.close();




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

	// Record stuff

	Gtk::Switch * temp_switch = nullptr;

	refBuilder->get_widget("record_button",temp_switch);
	int old_record_on_off = record_on_off;
	record_on_off = temp_switch->get_active();
	if(record_on_off == 1 && old_record_on_off != record_on_off){
		
		record_conn = Glib::signal_timeout().connect(record_slot, record_timeout_value);
	}
	if(record_on_off == 0 && old_record_on_off != record_on_off){
		print_to_log("Stopped Recording");

		record_conn.disconnect();
		recording = false;
		frame_counter = 0;
	}


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


void GUI::dispatch_notification(){
	
		myMutex.lock();
		for(auto msg : log_buffer){

			buffer->insert(buffer->end(),msg);
			Gtk::ScrolledWindow * scrolled_window = nullptr;
			refBuilder->get_widget("Scroll_Log",scrolled_window);
			scrolled_window->get_vadjustment()->set_value(scrolled_window->get_vadjustment()->get_upper());
		}
		log_buffer.clear();

		myMutex.unlock();
}

void GUI::timer_manual_control(const ros::TimerEvent& event){
	pub_manual_acceleration.publish(_acceleration);
	pub_manual_steering.publish(_steering);

}