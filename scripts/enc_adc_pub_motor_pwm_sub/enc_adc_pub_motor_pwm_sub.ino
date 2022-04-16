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


const int motorPWM = 11;
const int IN3 = 12;
const int IN4 = 13;


//const int analogOutPin = 11;
void pwmCB( const std_msgs::Int32 &pwm_msg) {
  analogWrite(motorPWM, pwm_msg.data);
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
  //pinMode(analogOutPin, INPUT);

  pinMode(motorPWM, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop()
{

  int enc_temp = encoder.read();
  enc_valuate(enc_temp);
  enc_msg.data = enc_temp;
  enc_pub.publish( &enc_msg );

  adc_msg.data = analogRead(analogInPin);;
  adc_pub.publish( &adc_msg );

  // map it to the range of the analog out:
  //outputValue = map(sensorValue, 0, 1023, 0, 255);

  nh.spinOnce();
  //delay(1);
}

void enc_valuate(int enc)
{
  if (enc >= 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }

  else {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
}
