#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from std_msgs.msg import UInt16
from std_msgs.msg import Int16
import pygame
import time
import sys


def remap(value, old_max, old_min, new_max, new_min):
    old_range = old_max-old_min
    new_range = new_max-new_min
    new_value = ( ( (value - old_min) * new_range) / old_range) + new_min
    return new_value

def main(args):
    rospy.init_node('joystick', anonymous=True)
    
    steering_pub = rospy.Publisher("servo",UInt16, queue_size=1)
    speed_pub = rospy.Publisher("motor",Int16, queue_size=1)

    pygame.init()
    print(int(remap(0.55, 0.88, -0.89,0,100)))
    rate = rospy.Rate(10)
    while(True):
        pygame.event.pump()
        joystick_count = pygame.joystick.get_count()
	
        for i in range(joystick_count):
            joystick = pygame.joystick.Joystick(i)
            joystick.init()

            name = joystick.get_name()
            #axes = joystick.get_numaxes()
            #hats = joystick.get_numhats()
            #button = joystick.get_numbuttons()

            speed = joystick.get_axis(0) # max forward = -0.85, max backward = 0.82
            steering = joystick.get_axis(1) # max left = 0.88, max right = -0.89
            steering_pub.publish( int(remap(steering, 0.88, -0.89,100,0)) )

            if(speed <= 0):
                tmp_speed = int(remap(speed, -0.89, 0,255,60))
                if(tmp_speed < 70): tmp_speed = 0 
                speed_pub.publish(tmp_speed)
            elif(speed >0):
                tmp_speed = int(remap(speed, 0.88, 0,-255,-60)) 
                if(tmp_speed > -70): tmp_speed = 0 
                speed_pub.publish(tmp_speed)


			
        rate.sleep()
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")

if __name__ == '__main__':
    main(sys.argv)
