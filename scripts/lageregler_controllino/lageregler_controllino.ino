
#include <Controllino.h> 
#include <ros.h>
#include <std_msgs/Int32.h>

ros::NodeHandle nh;


// PID
#include <PID_v1.h>
volatile double Setpoint;
double Input, Output;
double Kp=10, Ki=0, Kd=0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

volatile int enc = 0;
std_msgs::Int32 set_msg;

void setCB( const std_msgs::Int32 &set_msg) {Setpoint = double(set_msg.data);}
ros::Subscriber<std_msgs::Int32> set_sub("setpoint", setCB );

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
std_msgs::Int32 enc_msg;

void encCB( const std_msgs::Int32 &enc_msg) {enc = enc_msg.data;} // get enc-value from ros-topic "enc" (arduino uno)
ros::Subscriber<std_msgs::Int32> enc_sub("enc", encCB );


void setup()
{
  nh.initNode();
  //nh.advertise(adc_pub);
  //nh.subscribe(enc_sub);
  nh.subscribe(set_sub);

  // PID
  Input = analogRead(adc_pin);
  Setpoint = 100;
  myPID.SetMode(AUTOMATIC);

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
  Input = analogRead(adc_pin);
  myPID.Compute();
 
  drive_motor(Output);

  nh.spinOnce();
}

void drive_motor(int pwm)
{
    if (pwm >= 0) {
    digitalWrite(motor_in1_pin, LOW);
    digitalWrite(motor_in2_pin, HIGH);
  }

  else {
    digitalWrite(motor_in1_pin, HIGH);
    digitalWrite(motor_in2_pin, LOW);
  }

  analogWrite(pwm_pin, int(pwm));
}
