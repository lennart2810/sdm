#!/usr/bin/env python3

import rospy
from std_msgs.msg import Int32


def callback(adc_value):
    #rospy.loginfo(rospy.get_caller_id() + " I heard adc_value: %s", rx_data.data)
    pub.publish(adc_to_pwm(adc_value.data))


def adc_to_pwm(adc_value):
    pwm_value = (235/675) * adc_value # adc_max = 675; pwm_max = 255
    return  int(pwm_value)



def main():

    rospy.init_node('adc_to_pwm', anonymous=True)

    rospy.Subscriber("adc", Int32, callback)

    global pub # durch Klasse ersetzen (self.pub)
    pub = rospy.Publisher('pwm', Int32, queue_size=10)

    rospy.spin()


if __name__ == '__main__':
    main()