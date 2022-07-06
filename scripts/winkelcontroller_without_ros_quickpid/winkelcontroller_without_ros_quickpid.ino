#include <Controllino.h>

//Left- & Right-Position
#define LEFTPOS 10000   // max. enc steps track
#define RIGHTPOS 2500   // min. enc steps track

// Encoder
#include <Encoder.h>
#define ENC 2000 // pendel senkrecht nach oben 
#define TOLROT 250 // ENC -+ 100
const byte interruptPin0 = CONTROLLINO_IN0;
const byte interruptPin1 = CONTROLLINO_IN1;
const byte digitalIn0 = CONTROLLINO_DI0;
const byte digitalIn1 = CONTROLLINO_DI1;
//Encoder encoder_lin(interruptPin0, digitalIn0);
Encoder encoder_rot(interruptPin1, interruptPin0);

// Setpoint_rot
float Setpoint_rot = ENC / 2;
float Input_rot, Output_rot = 0;

// Setpoint_lin
double Setpoint_lin = (LEFTPOS - RIGHTPOS) / 2 + RIGHTPOS; //Mitte der Linearführung
double Input_lin, Output_lin = 0;

// PIDs
#include "QuickPID.h"
float Kp_rot = 3.5, Ki_rot = 0.5, Kd_rot = 0.3; //krass bescheuert Junge!
//float Kp_rot = 3.5, Ki_rot = 0.5, Kd_rot = 0.3; //krass gut Junge!
//float Kp_rot = 5.5, Ki_rot = 0.0, Kd_rot = 0.1;
//float Kp_rot = 3.5, Ki_rot = 0.0, Kd_rot = 0.06; //gut
//float Kp_rot = 5.5, Ki_rot = 0.001, Kd_rot = 0.001; 
//float Kp_rot = 3.5, Ki_rot = 0.00002, Kd_rot = 0.06; 
//float Kp_rot = 3.5, Ki_rot = 0.0, Kd_rot = 0.06; 
//float Kp_rot = 3.0, Ki_rot = 0.0, Kd_rot = 0.06; wird nicht schlechter
//float Kp_rot = 1.4, Ki_rot = 0.0, Kd_rot = 0.03; //sehr gut!
//float Kp_rot = 1.4, Ki_rot = 0.0, Kd_rot = 0.02; //besser
//float Kp_rot = 1, Ki_rot = 0.0, Kd_rot = 0.02; //gut
//float Kp_rot = 1, Ki_rot = 0.0, Kd_rot = 0.0;
QuickPID PID_rot(&Input_rot, &Output_rot, &Setpoint_rot);
// Motor
#define PWM_MIN 0
#define PWM_MAX 75 //50
const int pwm_pin = CONTROLLINO_AO0;
const int motor_in1_pin = CONTROLLINO_DO1;
const int motor_in2_pin = CONTROLLINO_DO2;

// REFERENCE
const int ind_right_pin = CONTROLLINO_DI2;
const int ind_left_pin = CONTROLLINO_DI3;
bool referenced = false;

// FREIGABE
bool freigegeben = true;
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
  PID_rot.SetSampleTimeUs(5000); //SampleTime auf 50ms
  delay(1000);
  encoder_rot.write(0);
}

void loop()
{
  long encoder_read = encoder_rot.read();
  long encoder_abs = abs(encoder_read);
  int encoder_int = int(encoder_abs);
  //int encoder_mod = encoder_int%ENC;
  int encoder_mod = int(abs(encoder_rot.read()))%ENC;
  bool left_pin = digitalRead(ind_left_pin);
  bool right_pin = digitalRead(ind_right_pin);
  
  if ( left_pin == false && right_pin == false) {
    encoder_rot.write(0);
    freigegeben = true;
  } 
  
  Input_rot = encoder_mod;
  PID_rot.Compute();
  printEncoder();
  
  if (controlable() == true && freigegeben == true)
  {
    int drv = int(Output_rot);
    drive_motor(drv);
  } else {
    drive_motor(0);
  }

}

//bool allowMovement(int pwm)
//{
//  return true;
//  if (digitalRead(ind_pin) == false && pwm > 0) {
//    return false;
//  }
//
//  //Positionsüberprüfung erst ausführen, wenn referenziert wurde
//  if (referenced == true) {
//    int currentPosition = encoder_lin.read();
//    //wenn linker Anschlag und Fahrtrichtung nach links
//    if (currentPosition > LEFTPOS && pwm < 0) {
//      return false;
//    }
//    //wenn rechter Anschlag und Fahrtrichtung nach rechts
//    if (currentPosition < RIGHTPOS && pwm > 0) {
//      return false;
//    }
//  }
//  return true;
//}

// motor wird über geschwindikeit angesteuert (pwm signal)
void drive_motor(int pwm)
{
  //falls Schlitten bereits an einer Grenze steht, soll der Motor deaktiviert werden
  //oder falls PWM=0
//  if (allowMovement(pwm) == false || pwm == 0) {
//    digitalWrite(motor_in1_pin, LOW);
//    digitalWrite(motor_in2_pin, LOW);
//    analogWrite(pwm_pin, int(0));
//    return;
//  }

  //Richtung setzen
  if (pwm > 0) {
    if (pwm < 10) {
      pwm = 10;
    }
    digitalWrite(motor_in1_pin, HIGH);
    digitalWrite(motor_in2_pin, LOW);
  }
  else {
    if (pwm > -10) {
      pwm = -10;
    }
    digitalWrite(motor_in1_pin, LOW);
    digitalWrite(motor_in2_pin, HIGH);
  }
  pwm_global = pwm;
  int abs_int = abs(pwm);
  analogWrite(pwm_pin, int(abs_int));
}

void reference()
{
  referenced = false;
  // nach links fahren bis induction aktiviert wird
  while (digitalRead(ind_right_pin) == true)
  {
    drive_motor(8);
  }

  drive_motor(0);
  delay(1000);

  // nullpunkt der linearführung
  //encoder_lin.write(0);
  referenced = true;

  delay(2500);

//  // zur mitte fahren
//  while (encoder_lin.read() < (LEFTPOS-RIGHTPOS) / 2 + RIGHTPOS)  
//  {
//    drive_motor(-8);
//    printEncoder();
//  }

  drive_motor(0);
  delay(2500);
}

void printEncoder() 
{
    if (!Serial) {
      return;
    }
    //Serial.print(encoder_lin.read());
    //Serial.print("\t");
    //Serial.print(encoder_rot.read());
    //Serial.print("\t");
    Serial.print(Input_rot);
    Serial.print("\t");
    //if (Output_rot != 0) {
      //Serial.println(Output_rot);
      Serial.println(pwm_global);
      //Serial.print("\t");
    //}
    //Serial.println(controlable());
}

bool controlable()
{
  // true wenn Pendel im regelbaren Bereich liegt (ENC+-TOLROT)
  int grenze1 = ENC/2 - TOLROT;
  int grenze2 = ENC/2 + TOLROT;
  if (Input_rot >= grenze1 && Input_rot <= grenze2) {
    return true;
  } else {
    return false; 
  }
}
