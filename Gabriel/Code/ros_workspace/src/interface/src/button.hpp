
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <string>
#include "gui.hpp"

#include "std_msgs/String.h"
#include "std_msgs/Int8.h"


/**
*Button is the class which handles what happens when a button is pressed in the GUI.
*
*/
class Buttons //: public GUI
{	
public:

	Buttons();
	Buttons(GUI & , std::string,int);
	Buttons(GUI & , std::string,int,bool);
	Buttons(GUI & , std::string,int,int);
	virtual ~Buttons();

	void on_button_clicked();
	//void print_to_log(std::string);
	void chatterCallback(const std_msgs::String::ConstPtr& msg);

	Gtk::Button * my_button;
	std::string name;
	std::string message;
	
	GUI* my_gui;

	bool has_publisher = false;
	ros::Publisher button_pub;
	std_msgs::Int8 int_message;
	bool isToggle = false;
	bool hasDirection = false;


	std_msgs::Int16 direction_choice;
	ros::Publisher pub_direction_choice;


};


#endif