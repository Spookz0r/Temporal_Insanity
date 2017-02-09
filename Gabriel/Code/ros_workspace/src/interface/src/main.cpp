#include <gtkmm.h>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <sstream>
#include "button.hpp"
#include "gui.hpp"
#include "image_show.hpp"
#include "icon.hpp"

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Int16.h"
#include "std_msgs/UInt16.h"

#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"

#include "std_msgs/Int32MultiArray.h"
#include <sensor_msgs/LaserScan.h>




//g++ -std=c++11 *pp  `pkg-config gtkmm-3.0 --cflags --libs`

Gtk::Window *pWindow = nullptr;
Glib::RefPtr<Gdk::Pixbuf> image;

bool on_button_press(GdkEventButton* event){

	std::cerr << "hello" << std::endl;
}



int main(int argc, char *argv[])
{	

	auto app = 
		Gtk::Application::create(argc, argv, "org.gtkmm.example");
	GUI gui;
	//ROS
	// Setup subscribers and their functions
	ros::init(argc, argv, "gui");
	ros::NodeHandle n;

	//ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
	ros::Subscriber sub = n.subscribe("to_log", 1000, &GUI::logCallback, &gui);

	//ros::spinOnce();
	//Init stuff

	// Access the window widget from the xml file.
	//gui.refBuilder->get_widget("window1",gui.pWindow);

	//To log window:
	

	// ############## Buttons ###############
	Buttons accelerate(gui,"accelerate_button",5,0);
	Buttons reverse(gui,"reverse_button",-5);
	Buttons left(gui,"left_button",-10,-1);
	Buttons right(gui,"right_button",10,1);
	Buttons stop(gui,"stop_button",0);
	Buttons button_manual_control(gui,"button_manual_control",0,true);
	
	// ############# Images ###################
	// Creates an object for each image and then subscribe each image to a ROS publisher.
	Image_show left_image(gui,"camera_image");
	left_image.switchable = false;
	
	image_transport::ImageTransport it(n);

	//image_transport::Subscriber img_sub = it.subscribe("/image_compressed", 1, &Image_show::imageCallback,&left_image,
		//image_transport::TransportHints("compressed"));
	image_transport::Subscriber img_sub = it.subscribe("image", 1, &Image_show::imageCallback,&left_image);


	//####################### ICON ###################
	Icon icon_camera(gui,"status_camera");
	Icon icon_wifi(gui,"status_wifi");
	std::vector<Icon> icon_vector ={icon_camera,icon_wifi};
	Icon icons(gui, icon_vector);


	//######################## TIMER FOR UPDATING STATUS#################
	int m_timer_number = 1;
	int timeout_value = 1000;
	//sigc::slot<bool> my_slot = sigc::bind(sigc::ptr_fun(&on_timeout),image);
	//sigc::slot<bool> my_slot = sigc::mem_fun(left_image, &Image_show::on_timeout);
	sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(icons, &Icon::on_timeout), m_timer_number);

  	// This is where we connect the slot to the Glib::signal_timeout()
 	sigc::connection conn = Glib::signal_timeout().connect(my_slot, timeout_value);

 	sigc::slot<bool> my_slot2 = sigc::bind(sigc::mem_fun(gui, &GUI::on_timeout), 1);
 	sigc::connection conn2 = Glib::signal_timeout().connect(my_slot2, timeout_value);

 	//################ BUTTON PUBLISHERS ########################
 	accelerate.button_pub = n.advertise<std_msgs::Int8 >("user_motor_control", 1);
 	accelerate.has_publisher = true;
 	accelerate.pub_direction_choice = n.advertise<std_msgs::Int16 >("direction_choice", 1);

 	reverse.button_pub = n.advertise<std_msgs::Int8 >("user_motor_control", 1);
 	reverse.has_publisher = true;

 	left.button_pub = n.advertise<std_msgs::Int8 >("user_servo_control", 1);
 	left.has_publisher = true;
 	left.pub_direction_choice = n.advertise<std_msgs::Int16>("direction_choice",1);


 	right.button_pub = n.advertise<std_msgs::Int8 >("user_servo_control", 1);
 	right.has_publisher = true;

 	right.pub_direction_choice = n.advertise<std_msgs::Int16>("direction_choice",1);


 	button_manual_control.button_pub = n.advertise<std_msgs::Int8 >("user_manual_control",1);
 	button_manual_control.has_publisher = true;
	gui.pub_manual_control = n.advertise<std_msgs::Int8 >("user_manual_control",1);

 	stop.button_pub = n.advertise<std_msgs::Int8 >("user_stop_control", 1);
 	stop.has_publisher = true;


	gui.pub_manual_steering = n.advertise<std_msgs::UInt16 >("/servo",1);
	gui.pub_manual_acceleration = n.advertise<std_msgs::Int16 >("/motor",1);
	

	Gtk::MenuItem * menu = nullptr;

	ros::AsyncSpinner spinner(4); // Use 4 threads
	spinner.start();

	gui.pWindow->signal_key_press_event().connect( sigc::mem_fun(gui, &GUI::on_key_press_event), false );
	
	app->run(*gui.pWindow);
	delete pWindow;	

	return 0;
}

