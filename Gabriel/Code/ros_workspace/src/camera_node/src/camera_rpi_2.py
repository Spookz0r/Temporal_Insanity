#!/usr/bin/env python
from __future__ import print_function
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import CompressedImage


from imutils.video.pivideostream import PiVideoStream
from imutils.video import FPS
import argparse
import imutils
from picamera.array import PiRGBArray
from picamera import PiCamera
import time, cv2, sys
import numpy as np
from threading import Thread


    
ap = argparse.ArgumentParser()
ap.add_argument("-n", "--num-frames", type=int, default=100,
	help="# of frames to loop over for FPS test")
ap.add_argument("-d", "--display", type=int, default=-1,
	help="Whether or not frames should be displayed")
args = vars(ap.parse_args())



class PiVideoStream:
	def __init__(self, resolution=(320, 240), framerate=60):
		self.camera = PiCamera()
		self.camera.resolution = resolution
		self.camera.framerate = framerate
		self.rawCapture = PiRGBArray(self.camera, size=resolution)
		self.stream = self.camera.capture_continuous(self.rawCapture,
			format="bgr", use_video_port=True)

		self.frame = None
		self.stopped = False
		
		

	def start(self):
		Thread(target=self.update, args=()).start()
		return self

	def update(self):
		for f in self.stream:
				self.frame = f.array
				self.rawCapture.truncate(0)

				if self.stopped:
					self.stream.close()
					self.rawCapture.close()
					self.camera.close()
					return
	
	def read(self):
		return self.frame

	def stop(self):
		self.stopped = True


def main(args):
    rospy.init_node('camera_rpi', anonymous=True)

    image_pub = rospy.Publisher("image_topic",Image, queue_size=1)
    image_pub_compressed = rospy.Publisher("image_compressed",CompressedImage,queue_size=1)
    
    bridge = CvBridge()
    print("Starting Camera Node")

    camera_stream = PiVideoStream().start()
    time.sleep(2.0)    
    fps = FPS().start()

    r = rospy.Rate(60)


    while not rospy.is_shutdown():
		start_time = time.time()
		
		frame = camera_stream.read()
		#frame = imutils.resize(frame, width=400)
		
		#print("--- %s camera capture seconds ---" % (time.time() - start_time))
		ros_start_time = time.time()
		image_pub.publish(bridge.cv2_to_imgmsg(frame, encoding="bgr8"))
		
		## Create compressed image message
		msg = CompressedImage()
		msg.header.stamp = rospy.Time.now()
		msg.format = "jpeg"
		msg.data = np.array(cv2.imencode('.jpg', frame)[1]).tostring()
		
		
		## publish compressed image
		image_pub_compressed.publish(msg)
		#print("--- %s ros seconds ---" % (time.time() - ros_start_time))
		

		r.sleep()

	

if __name__ == '__main__':
    main(sys.argv)
