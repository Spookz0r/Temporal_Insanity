#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import CompressedImage


import cv2, sys
import numpy as np

class image_converter:
	
	def __init__(self):
		
		
		self.bridge = CvBridge()
		self.image_sub = rospy.Subscriber("image_compressed",CompressedImage,self.callback, queue_size=1)
		print("Created image converter")

	def callback(self, data):
		##Cv bridge does not support compressed image in python...
# 		try:
# 			cv_image = self.bridge.imgmsg_to_cv2(data,"bgr8")
# 		except CvBridgeError as e:
# 			print(e)
		np_arr = np.fromstring(data.data, np.uint8)
		image_np = cv2.imdecode(np_arr, cv2.IMREAD_COLOR) 
		
		
		
		
		
		
		cv2.imshow("Image window", image_np)
		cv2.waitKey(2)





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