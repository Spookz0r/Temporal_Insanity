#include "ros/ros.h"

int main(int argc, char **argv){
	ros::init(argc, argv, "gpio_test");
	
	ros::spin();
}
