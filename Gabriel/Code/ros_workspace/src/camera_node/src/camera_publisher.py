#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError


import cv2, sys
import numpy as np


def main(args):
    rospy.init_node('camera_publisher', anonymous=True)
    
    image_pub = rospy.Publisher("image_topic",Image, queue_size=1)
    bridge = CvBridge()
    cap = cv2.VideoCapture(0)

    while(True):
        ret, frame = cap.read()
        
        cv2.imshow('frame',frame)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        
        

        image_pub.publish(bridge.cv2_to_imgmsg(frame, encoding="bgr8"))
        #rospy.spin()
        
    cap.release()
    cv2.destroyAllWindows()
    
    
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)