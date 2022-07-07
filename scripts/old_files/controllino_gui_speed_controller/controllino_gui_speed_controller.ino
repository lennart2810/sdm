#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>

#include <Controllino.h>

ros::NodeHandle  nh;

//Left- & Right-Position
#define LEFTPOS 10000
#define RIGHTPOS 2500
#define MAXSPEED 45

// SETPOINT
volatile double Setpoint = 0;
double Input, Output = 0;
bool referenced = false;
//volatile int setpoint = 0;
//const int setpoint_led = 7;

volatile int speed = 0;
void setpointCb(const std_msgs::Int32& setpoint_msg) {
  //Setpoint = setpoint_msg.data;
  speed = setpoint_msg.data;
  //drive_motor(setpoint_msg.data/100); // setpooint als motorgeschwindigkeit verwenden!!!
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

// Encoder
#include <Encoder.h>
#define ENC 500            // pulses per revolution (with Encoder.h)
const byte interruptPin0 = CONTROLLINO_IN0;
const byte interruptPin1 = CONTROLLINO_IN1;
const byte digitalIn0 = CONTROLLINO_DI0;
const byte digitalIn1 = CONTROLLINO_DI1;
Encoder encoder0(interruptPin0, digitalIn0);
Encoder encoder1(interruptPin1, digitalIn1);
std_msgs::Int32 enc_msg;
ros::Publisher enc_pub("enc", &enc_msg);

// REFERENCE
const int ind_pin = CONTROLLINO_DI2;

void referenceCb( const std_msgs::Empty& reference_msg) {
  reference();
}
ros::Subscriber<std_msgs::Empty> ref_sub("reference", &referenceCb );

void setup()
{
  pinMode(interruptPin0, INPUT);
  pinMode(interruptPin1, INPUT);
  pinMode(digitalIn0, INPUT);
  pinMode(digitalIn1, INPUT);

  pinMode(ind_pin, INPUT);

  pinMode(pwm_pin, OUTPUT);
  pinMode(motor_in1_pin, OUTPUT);
  pinMode(motor_in2_pin, OUTPUT);

  //myPID.SetOutputLimits(-PWM_MAX, PWM_MAX);
  //myPID.SetMode(AUTOMATIC);

  //delay(500);

  nh.initNode();
  nh.advertise(enc_pub);
  nh.subscribe(setpoint_sub);
  nh.subscribe(pids_sub);
  nh.subscribe(ref_sub);

  // motor aktivieren ?!
  digitalWrite(motor_in1_pin, HIGH);
  digitalWrite(motor_in2_pin, LOW);

  //delay(2000);

  //reference();
}

void loop()
{
  Input = encoder0.read();
  //Input = analogRead(adc_pin);
  //myPID.Compute();
  drive_motor(speed);

  enc_msg.data = Input;
  enc_pub.publish( &enc_msg );

  nh.spinOnce();
}

bool allowMovement(int pwm)
{
  if (digitalRead(ind_pin) == false && pwm > 0) {
    return false;
  }

  //Positionsüberprüfung erst ausführen, wenn referenziert wurde
  if (referenced == true) {
    int currentPosition = Input;
    //wenn linker Anschlag und Fahrtrichtung nach links
    if (currentPosition > LEFTPOS && pwm < 0) {
      return false;
    }
    //wenn rechter Anschlag und Fahrtrichtung nach rechts
    if (currentPosition < RIGHTPOS && pwm > 0) {
      return false;
    }
  }
  return true;
}

void drive_motor(int pwm)
{
  if (allowMovement(pwm) == true) {

    if (pwm >= 0) {
      digitalWrite(motor_in1_pin, HIGH);
      digitalWrite(motor_in2_pin, LOW);
    }

    else {
      digitalWrite(motor_in1_pin, LOW);
      digitalWrite(motor_in2_pin, HIGH);
    }

    analogWrite(pwm_pin, int(abs(pwm)));
  } else {
    speed = 0;
    digitalWrite(motor_in1_pin, LOW);
    digitalWrite(motor_in2_pin, LOW);
    analogWrite(pwm_pin, int(0));
  }
}

void reference()
{
  // nach links fahren bis induction aktiviert wird
  while (digitalRead(ind_pin) == true)
  {
    drive_motor(10);
  }

  drive_motor(0);
  delay(1000);
  encoder0.write(0);
  referenced = true;
}
