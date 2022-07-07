#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>

#include <Controllino.h>

// P 0.22
// I 0.1

ros::NodeHandle  nh;

//Max- & Min-Position
#define MAXPOS 400
#define MINPOS 200
//#define MAXSPEED 45




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
double Kp = 0, Ki = 0, Kd = 0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void pidsCb(const std_msgs::Int32MultiArray &pids_msg) {
  Kp = float(pids_msg.data[0] / 100.0);
  Ki = float(pids_msg.data[1] / 100.0);
  Kd = float(pids_msg.data[2] / 100.0);
  myPID.SetTunings(Kp, Ki, Kd);
}
ros::Subscriber<std_msgs::Int32MultiArray> pids_sub("pids", pidsCb);

// Motor
#define PWM_MIN -120
#define PWM_MAX 120
//bool dir = true; // true --> cw
const int pwm_pin = CONTROLLINO_AO0;
const int motor_in1_pin = CONTROLLINO_DO1;
const int motor_in2_pin = CONTROLLINO_DO2;

// ADC raus
#define ADC_MAX 700
#define ADC_MIN 0
const int adc_pin = CONTROLLINO_A12;
std_msgs::Int32 adc_msg;
ros::Publisher adc_pub("adc", &adc_msg);

// Encoder
#include <Encoder.h>
#define ENC 500            // pulses per revolution (with Encoder.h)

const byte interruptPin0 = CONTROLLINO_IN0;
const byte interruptPin1 = CONTROLLINO_IN1;
const byte digitalIn0 = CONTROLLINO_DI0;
const byte digitalIn1 = CONTROLLINO_DI1;

Encoder encoder0(interruptPin0, digitalIn0);
Encoder encoder1(interruptPin1, digitalIn1);


// REFERENCE
const int ind_pin = CONTROLLINO_DI0;

void referenceCb( const std_msgs::Empty& reference_msg) {
  reference();
}
ros::Subscriber<std_msgs::Empty> ref_sub("reference", &referenceCb );

void setup()
{

  Serial.begin(115200);
  //pinMode(adc_pin, INPUT);
  pinMode(ind_pin, INPUT);

  pinMode(interruptPin0, INPUT);
  pinMode(interruptPin1, INPUT);
  pinMode(digitalIn0, INPUT);
  pinMode(digitalIn1, INPUT);

  pinMode(pwm_pin, OUTPUT);
  pinMode(motor_in1_pin, OUTPUT);
  pinMode(motor_in2_pin, OUTPUT);

  myPID.SetOutputLimits(PWM_MIN, PWM_MAX);
  myPID.SetMode(AUTOMATIC);

  //delay(500);

  //  nh.initNode();
  //  nh.advertise(adc_pub);
  //  nh.subscribe(setpoint_sub);
  //  nh.subscribe(pids_sub);
  //  nh.subscribe(ref_sub);

  digitalWrite(motor_in1_pin, HIGH);
  digitalWrite(motor_in2_pin, LOW);

  //reference();
}

void loop()
{
  //Serial.println(analogRead(adc_pin));
  //delay(10);
  Input = encoder0.read();
  //Input = analogRead(adc_pin);
  myPID.Compute();

  drive_motor(Output);

  //adc_msg.data = Input;
  //adc_pub.publish( &adc_msg );

  nh.spinOnce();
}

bool allowMovement(int pwm)
{
  return true;
  int currentPosition = Input;
  if (currentPosition > MINPOS && currentPosition < MAXPOS) {
    return true;
  } else {
    if (pwm > 0 && currentPosition < MAXPOS) {
      return true;
    } else if (pwm < 0 && currentPosition > MINPOS) {
      return true;
    }
  }
  return false;
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
    analogWrite(pwm_pin, int(0));
  }
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
