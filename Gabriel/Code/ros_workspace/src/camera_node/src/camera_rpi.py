#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import CompressedImage

from picamera.array import PiRGBArray
from picamera import PiCamera
import time, cv2, sys
import numpy as np



def main(args):
    rospy.init_node('camera_rpi', anonymous=True)

    image_pub = rospy.Publisher("image_topic",Image, queue_size=1)
    image_pub_compressed = rospy.Publisher("image_compressed",CompressedImage,queue_size=1)
    
    bridge = CvBridge()
    print("Hello")
    img_width = 320
    img_height = 240
    # initialize camera and grab reference to raw camera capture
    camera = PiCamera()
    camera.resolution = (img_width, img_height)
    camera.framerate = 30
    rawCapture = PiRGBArray(camera, size=(img_width, img_height))

    #allow camera to warm up
    time.sleep(0.1)

    
    for frame in camera.capture_continuous(rawCapture, format='bgr', use_video_port=True):
        image = frame.array

        
        image_pub.publish(bridge.cv2_to_imgmsg(frame.array, encoding="bgr8"))
        
        ## Create compressed image message
        msg = CompressedImage()
        msg.header.stamp = rospy.Time.now()
        msg.format = "jpeg"
        msg.data = np.array(cv2.imencode('.jpg', frame.array)[1]).tostring()
        
        
        ## publish compressed image
        image_pub_compressed.publish(msg)

        
        #cv2.imshow("Frame", image)
        #key = cv2.waitKey(1) & 0xFF

        rawCapture.truncate(0)
        rawCapture.seek(0)
        
        #if key == ord("q"):
          #  break


    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
    cv2.destroyAllWindows()


if __name__ == '__main__':
    main(sys.argv)
