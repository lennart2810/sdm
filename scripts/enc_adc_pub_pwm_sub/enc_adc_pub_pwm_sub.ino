/*
  roscore
  rosrun rosserial_arduino serial_node.py /dev/ttyACM0

  oder einfach: roslaunch sdm enc_adc_pub_pwm_sub.launch
*/

#include <ros.h>
#include <std_msgs/Int32.h>

ros::NodeHandle nh;


#include <Encoder.h>
#define ENC 2000            // pulses per revolution (with Encoder.h)
Encoder encoder(2, 4);      // best performance: both pins have interrrupt capability
std_msgs::Int32 enc_msg;
ros::Publisher enc_pub("enc", &enc_msg);

std_msgs::Int32 adc_msg;
ros::Publisher adc_pub("adc", &adc_msg);
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to


const int analogOutPin = 11;
void pwmCB( const std_msgs::Int32 &pwm_msg){
  analogWrite(analogOutPin, pwm_msg.data);
}

ros::Subscriber<std_msgs::Int32> pwm_sub("pwm", pwmCB );


void setup()
{
  //nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.advertise(enc_pub);
  nh.advertise(adc_pub);
  nh.subscribe(pwm_sub);

  pinMode(analogInPin, INPUT);
  pinMode(analogOutPin, INPUT);
}

void loop()
{
  enc_msg.data = encoder.read();
  enc_pub.publish( &enc_msg );

  adc_msg.data = analogRead(analogInPin);;
  adc_pub.publish( &adc_msg );

  // map it to the range of the analog out:
  //outputValue = map(sensorValue, 0, 1023, 0, 255);

  nh.spinOnce();
  //delay(1);
}
