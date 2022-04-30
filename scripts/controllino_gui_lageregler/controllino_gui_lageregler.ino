#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>

#include <Controllino.h>

ros::NodeHandle  nh;

// SETPOINT
volatile double Setpoint = 0;
double Input, Output = 0;
//volatile int setpoint = 0;
//const int setpoint_led = 7;
void setpointCb(const std_msgs::Int32& setpoint_msg) {
  Setpoint = setpoint_msg.data;
}
ros::Subscriber<std_msgs::Int32> setpoint_sub("setpoint", setpointCb);

// PIDs
#include <PID_v1.h>
double Kp = 0.1, Ki = 0, Kd = 0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void pidsCb(const std_msgs::Int32MultiArray &pids_msg) {
  Kp = pids_msg.data[0];
  Ki = pids_msg.data[1];
  Kd = pids_msg.data[2];
}
ros::Subscriber<std_msgs::Int32MultiArray> pids_sub("pids", pidsCb);

// Motor
#define PWM_MIN 0
#define PWM_MAX 155
//bool dir = true; // true --> cw
const int pwm_pin = CONTROLLINO_AO0;
const int motor_in1_pin = CONTROLLINO_DO1;
const int motor_in2_pin = CONTROLLINO_DO2;

// ADC
#define ADC_MAX 700
#define ADC_MIN 0
const int adc_pin = CONTROLLINO_A12;
std_msgs::Int32 adc_msg;
ros::Publisher adc_pub("adc", &adc_msg);

// REFERENCE
const int ind_pin = CONTROLLINO_DI0;

void referenceCb( const std_msgs::Empty& reference_msg) { reference(); }
ros::Subscriber<std_msgs::Empty> ref_sub("reference", &referenceCb );

void setup()
{
  pinMode(adc_pin, INPUT);
  pinMode(ind_pin, INPUT);

  pinMode(pwm_pin, OUTPUT);
  pinMode(motor_in1_pin, OUTPUT);
  pinMode(motor_in2_pin, OUTPUT);

  myPID.SetOutputLimits(-PWM_MAX, PWM_MAX); 
  myPID.SetMode(AUTOMATIC);

  delay(500);

  nh.initNode();
  nh.advertise(adc_pub);
  nh.subscribe(setpoint_sub);
  nh.subscribe(pids_sub);
  nh.subscribe(ref_sub);
  
  reference();
}

void loop()
{
  Input = analogRead(adc_pin);
  myPID.Compute();

  drive_motor(Output);

  adc_msg.data = Input;
  adc_pub.publish( &adc_msg );

  nh.spinOnce();
}

void drive_motor(int pwm)
{
  if (pwm >= 0) {
    digitalWrite(motor_in1_pin, HIGH);
    digitalWrite(motor_in2_pin, LOW);
  }

  else {
    digitalWrite(motor_in1_pin, LOW);
    digitalWrite(motor_in2_pin, HIGH);
  }

  analogWrite(pwm_pin, int(pwm));
}

void reference()
{
  // nach links fahren bis induction aktiviert wird
  while (digitalRead(ind_pin))
  {
    drive_motor(-30);
  }

  // Input = 100 gemittelte Messwerte
  int sum = 0;
  for (int i = 0; i < 100; i++)
  {
    sum = sum + analogRead(adc_pin);
  }

  Input = sum / 100;
}
