/*
  roscore
  rosrun rosserial_arduino serial_node.py /dev/ttyACM0

  oder einfach: roslaunch sdm controllino.launch 
*/

#include <Controllino.h> 
#include <ros.h>
#include <std_msgs/Int32.h>

ros::NodeHandle nh;

// Motor
#define PWM_MIN 0
#define PWM_MAX 255
//bool dir = true; // true --> cw
const int pwm_pin = CONTROLLINO_AO0;
const int motor_in1_pin = CONTROLLINO_DO1;
const int motor_in2_pin = CONTROLLINO_DO2;

// Näherungsschalter
const int ind_pin = CONTROLLINO_DI0;

// Seilzug
#define ADC_MAX 700
#define ADC_MIN 0
const int adc_pin = CONTROLLINO_A12;
std_msgs::Int32 adc_msg;
ros::Publisher adc_pub("adc", &adc_msg);

// Drehgeber
volatile int enc = 0;
std_msgs::Int32 enc_msg;

void encCB( const std_msgs::Int32 &enc_msg) {enc = enc_msg.data;} // get enc-value from ros-topic "enc" (arduino uno)
ros::Subscriber<std_msgs::Int32> enc_sub("enc", encCB );


void setup()
{
  nh.initNode();
  nh.advertise(adc_pub);
  nh.subscribe(enc_sub);

  // Motor
  pinMode(pwm_pin, OUTPUT);
  pinMode(motor_in1_pin, OUTPUT);
  pinMode(motor_in2_pin, OUTPUT);

  // Seilzug
  pinMode(adc_pin, INPUT);

  // Näherungsschalter
  pinMode(ind_pin, INPUT);
}

void loop()
{
  int adc = analogRead(adc_pin);

  adc_msg.data = adc;;
  adc_pub.publish( &adc_msg );
 
  drive_motor(map(adc, ADC_MIN,ADC_MAX, PWM_MIN,PWM_MAX), set_dir());

  nh.spinOnce();
}

bool set_dir()
{
 if (digitalRead(ind_pin)){return true;}
 else {return false;}  
}

void drive_motor(int pwm, bool dir)
{
    if (dir) {
    digitalWrite(motor_in1_pin, LOW);
    digitalWrite(motor_in2_pin, HIGH);
  }

  else {
    digitalWrite(motor_in1_pin, HIGH);
    digitalWrite(motor_in2_pin, LOW);
  }

  analogWrite(pwm_pin, pwm);
}
