#include "ros/ros.h"

int main(int argc, char **argv){
	ros::init(argc, argv, "camera_pub_cpp");
	
	ros::spin();
}
