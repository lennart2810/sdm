#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>

ros::NodeHandle  nh;

// SETPOINT
volatile int setpoint = 0;
const int setpoint_led = 7;
void setpointCb(const std_msgs::Int32& setpoint_msg) { setpoint = setpoint_msg.data; }
ros::Subscriber<std_msgs::Int32> setpoint_sub("setpoint", setpointCb);

// ADC
std_msgs::Int32 adc_msg;
ros::Publisher adc_pub("adc", &adc_msg);

// PIDs 
volatile double kp, ki, kd = 0.0;
const int kp_led = 6;
const int ki_led = 5;
const int kd_led = 4;
//void pidsCb(const std_msgs::Float32MultiArray &pids_msg) { kp = pids_msg.data[0]; ki = pids_msg.data[1]; kd = pids_msg.data[2]; }
//ros::Subscriber<std_msgs::Float32MultiArray> pids_sub("pids", pidsCb);
void pidsCb(const std_msgs::Int32MultiArray &pids_msg) { 
  kp = pids_msg.data[0]; 
  ki = pids_msg.data[1]; 
  kd = pids_msg.data[2]; }
ros::Subscriber<std_msgs::Int32MultiArray> pids_sub("pids", pidsCb);

// REFERENCE
void messageCb( const std_msgs::Empty& toggle_msg){
  setpoint = 0;
  kp = 0.0;
  ki = 0.0;
  kd = 0.0;  
}
ros::Subscriber<std_msgs::Empty> ref_sub("toggle_led", &messageCb );

void setup()
{
  nh.initNode();
  nh.advertise(adc_pub);
  nh.subscribe(setpoint_sub);
  nh.subscribe(pids_sub);
  nh.subscribe(ref_sub);
}

void loop()
{
  analogWrite(setpoint_led, setpoint);
  analogWrite(kp_led, kp/100); 
  analogWrite(ki_led, ki/10); 
  analogWrite(kd_led, kd); 

  adc_msg.data = setpoint + 10;
  adc_pub.publish( &adc_msg );
  
  nh.spinOnce();
}
