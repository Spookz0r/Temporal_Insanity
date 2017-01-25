#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError


import cv2, sys
import numpy as np


rospy.init_node('camera_node')

publisher = rospy.Publisher("~topic",String,queue_size=1)

def callback(event):
	msg = String()
	msg.data = "hello"
	publisher.publish(msg)
	ret, frame = cap.read()
	
	image_message = CvBridge.cv2_to_imgmsg(frame, encoding="bgr8")
	



print ("Started camera node")
cap = cv2.VideoCapture(0)

pub_period = rospy.get_param("~pub_period",1.0)

rospy.Timer(rospy.Duration.from_sec(pub_period), callback)

rospy.spin()


if __name_- == '__main__':
