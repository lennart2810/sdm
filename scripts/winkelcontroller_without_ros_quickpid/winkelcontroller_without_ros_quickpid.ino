#include <Controllino.h>

// Encoder
#include <Encoder.h>
#define ENC 2000   // pendel senkrecht nach oben
#define TOLROT 250 // ENC -+ 100
const byte interruptPin0 = CONTROLLINO_IN0;
const byte interruptPin1 = CONTROLLINO_IN1;
const byte digitalIn0 = CONTROLLINO_DI0;
const byte digitalIn1 = CONTROLLINO_DI1;
Encoder encoder_rot(interruptPin1, interruptPin0);

// Setpoint_rot
float Setpoint_rot = ENC / 2;
float Input_rot, Output_rot = 0;

// PIDs
#include "QuickPID.h"
float Kp_rot = 3.5, Ki_rot = 0.5, Kd_rot = 0.3;
QuickPID PID_rot(&Input_rot, &Output_rot, &Setpoint_rot);

// Motor
#define PWM_MIN 0
#define PWM_MAX 75 // 50
const int pwm_pin = CONTROLLINO_AO0;
const int motor_in1_pin = CONTROLLINO_DO1;
const int motor_in2_pin = CONTROLLINO_DO2;

// REFERENCE
const int ind_right_pin = CONTROLLINO_DI2;
const int ind_left_pin = CONTROLLINO_DI3;

// FREIGABE
int pwm_global;

void setup()
{
  Serial.begin(115200);

  // encoder
  pinMode(interruptPin0, INPUT);
  pinMode(interruptPin1, INPUT);
  pinMode(digitalIn0, INPUT);
  pinMode(digitalIn1, INPUT);

  // induktion
  pinMode(ind_right_pin, INPUT);
  pinMode(ind_left_pin, INPUT);

  // motor
  pinMode(pwm_pin, OUTPUT);
  pinMode(motor_in1_pin, OUTPUT);
  pinMode(motor_in2_pin, OUTPUT);
  // motor aktivieren ?!
  digitalWrite(motor_in1_pin, HIGH);
  digitalWrite(motor_in2_pin, LOW);

  // pid
  PID_rot.SetMode(PID_rot.Control::automatic);
  PID_rot.SetOutputLimits(-PWM_MAX, PWM_MAX);
  PID_rot.SetTunings(Kp_rot, Ki_rot, Kd_rot);
  PID_rot.SetSampleTimeUs(5000); // SampleTime auf 50ms
  delay(1000);
  encoder_rot.write(0);
}

void loop()
{
  int encoder_mod = int(abs(encoder_rot.read())) % ENC;
  bool left_pin = digitalRead(ind_left_pin);
  bool right_pin = digitalRead(ind_right_pin);

  if (left_pin == false && right_pin == false)
  {
    encoder_rot.write(0);
  }

  Input_rot = encoder_mod;
  PID_rot.Compute();
  printEncoder();
  // wenn das Pendel im regelbaren Bereich liegt..
  if (controllable() == true)
  {
    int drv = int(Output_rot);
    drive_motor(drv);
  }
  else
  {
    drive_motor(0);
  }
}

// Motor wird über Geschwindikeit angesteuert (PWM signal)
void drive_motor(int pwm)
{
  // Richtung setzen
  if (pwm > 0)
  {
    // der Motor reagiert erst ab PWM>8
    if (pwm < 10)
    {
      pwm = 10;
    }
    digitalWrite(motor_in1_pin, HIGH);
    digitalWrite(motor_in2_pin, LOW);
  }
  else
  {
    if (pwm > -10)
    {
      pwm = -10;
    }
    digitalWrite(motor_in1_pin, LOW);
    digitalWrite(motor_in2_pin, HIGH);
  }
  pwm_global = pwm;
  int abs_int = abs(pwm);
  analogWrite(pwm_pin, int(abs_int));
}

void printEncoder()
{
  if (!Serial)
  {
    return;
  }
  Serial.print(Input_rot);
  Serial.print("\t");
  Serial.println(pwm_global);
}

bool controllable()
{
  // true wenn Pendel im regelbaren Bereich liegt (ENC+-TOLROT)
  int grenze1 = ENC / 2 - TOLROT;
  int grenze2 = ENC / 2 + TOLROT;
  if (Input_rot >= grenze1 && Input_rot <= grenze2)
  {
    return true;
  }
  else
  {
    return false;
  }
}
