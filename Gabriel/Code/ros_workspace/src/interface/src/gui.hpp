#ifndef GUI_HPP
#define GUI_HPP

#include <gtkmm.h>
//#include <gtkmm/window.h>
//#include <gtkmm/textview.h>
//#include <gtkmm/textbuffer.h>
//#include <glibmm/refptr.h>
//#include <gtkmm/builder.h>

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Int16.h"
#include "std_msgs/UInt16.h"
#include <chrono>
#include <fstream>

#include "ros/ros.h"

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Float32MultiArray.h"
#include <sensor_msgs/LaserScan.h>
#include <mutex>
#include <thread>
#include <glibmm/dispatcher.h>






class GUI : public Gtk::Window
{
public:
	GUI();
	virtual ~GUI();

	void print_to_log(std::string);
	void logCallback(const std_msgs::String::ConstPtr& msg);
	void systemstatusCallback(const std_msgs::Int32MultiArray::ConstPtr& msg);
	void sensordataCallback(const std_msgs::Int32MultiArray::ConstPtr& msg);
	void dispatch_notification();
	
	bool on_timeout(int);

	void createLogFile();

	ros::Publisher pub_manual_control;
	ros::Publisher pub_manual_steering;
	ros::Publisher pub_manual_acceleration;

	std_msgs::UInt16 _steering;
	std_msgs::Int16 _acceleration;

	Glib::Dispatcher dispatcher;
	std::mutex myMutex;
	std::thread myThread;

	
	Gtk::Window *pWindow = nullptr;
	Gtk::TextView* ptext_view = nullptr;
	Glib::RefPtr<Gtk::TextBuffer> buffer;
	Glib::RefPtr<Gtk::Builder> refBuilder;
	//						motor, servo, camera, lidar, enc1-4
	int status[8] = {0,0,0,0,0,0,0,0};
	std_msgs::Int8 battery_test_mode;

	std_msgs::Int8 _motor;
	std_msgs::Int8 _servo;
	std_msgs::Int8 manual_control_msg;
	std_msgs::Int8 lidar_switch_message;
	std_msgs::Int8 lidar_choice_message;

	int max_grade = 100;
	int min_grade = 1;
	int grade_mul = 10;
	//button events
	std_msgs::Int32MultiArray pre_points;
	std_msgs::Float32MultiArray pid_settings;
	std_msgs::Float32MultiArray bird_eye_settings;
	std_msgs::Int16 motor_speed;

	//Sensor data
	float battery_level= 0;
	float car_speed = 0;


	std::vector<Glib::RefPtr<Gdk::Pixbuf>> images;
	int image_to_show = 0;
	Gtk::Label * right_image_label;
	std::vector<std::string> image_labels;

	//Log file
	//ofstream myfile;
	std::ofstream log_file;
	std::string latest_message;
	std::string file_name;


	bool on_key_press_event(GdkEventKey* event) override;


};


#endif