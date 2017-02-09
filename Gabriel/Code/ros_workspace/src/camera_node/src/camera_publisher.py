#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import CompressedImage


import cv2, sys
import numpy as np


def main(args):
    rospy.init_node('camera_publisher', anonymous=True)
    
    image_pub = rospy.Publisher("image",Image, queue_size=1)
    image_pub_compressed = rospy.Publisher("image/compressed",CompressedImage,queue_size=1)
    
    bridge = CvBridge()
    cap = cv2.VideoCapture(0)
    cap.set(3,320);
    cap.set(4,240);

    while(True):
        ret, frame = cap.read()
        
        cv2.imshow('frame',frame)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        
        

        image_pub.publish(bridge.cv2_to_imgmsg(frame, encoding="bgr8"))
        
        ## Create compressed image message
        msg = CompressedImage()
        msg.header.stamp = rospy.Time.now()
        msg.format = "jpeg"
        msg.data = np.array(cv2.imencode('.jpg', frame)[1]).tostring()
        
        
        ## publish compressed image
        image_pub_compressed.publish(msg)
        
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
