#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError


import cv2, sys
import numpy as np

class image_converter:
	
	def __init__(self):
		self.image_pub = rospy.Publisher("image_topic_2",Image,queue_size=1)
		
		self.bridge = CvBridge()
		self.image_sub = rospy.Subscriber("image_topic",Image,self.callback)
		print("Created image converter")

	def callback(self, data):
		try:
			cv_image = self.bridge.imgmsg_to_cv2(data,"bgr8")
		except CvBridgeError as e:
			print(e)
			
		(rows,cols,channels) = cv_image.shape
		if cols > 60 and rows > 60 :
			cv2.circle(cv_image, (rows/2,cols/2), 40, 255)
		cv2.imshow("Image window", cv_image)
		cv2.waitKey(3)
		
		try:
			self.image_pub.publish(self.bridge.cv2_to_imgmsg(cv_image, "bgr8"))
		except CvBridgeError as e:
			print(e)





def main(args):
	ic = image_converter()
	rospy.init_node('image_converter', anonymous=True)
	try:
		rospy.spin()
	except KeyboardInterrupt:
		print("Shutting down")
	cv2.destroyAllWindows()

if __name__ == '__main__':
	main(sys.argv)