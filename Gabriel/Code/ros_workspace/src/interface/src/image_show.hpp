#ifndef IMAGE_SHOW_HPP
#define IMAGE_SHOW_HPP
#include "gui.hpp"
//#include <gtkmm.h>
#include <gtkmm/image.h>
#include <gdkmm/pixbuf.h>
#include <glibmm/dispatcher.h>

#include <string>
#include <iostream>
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <mutex>
#include <thread>

class Image_show //(: public GUI
{

public:
	Image_show();
	Image_show(GUI&,std::string);
	virtual ~Image_show();

	GUI * my_gui;
	Gtk::Image *drawingImage;
	Glib::RefPtr<Gdk::Pixbuf> _image;
	Glib::RefPtr<Gdk::Pixbuf> _old_image;

	Glib::Dispatcher dispatcher;
	std::mutex myMutex;
	std::thread myThread;
	cv::Mat image_0;
	cv::Mat image_1;

	bool filter = false;
	int image_id = 0;
	int counter = 0;
	

	void set_image(std::string);
	void callback(const std_msgs::String::ConstPtr& msg);
	void imageCallback(const sensor_msgs::ImageConstPtr& msg);
	void LidarCallback(const sensor_msgs::LaserScan::ConstPtr& msg);

	bool on_timeout(int);

	void dispatch_notification();
	bool switchable = true;



};

#endif