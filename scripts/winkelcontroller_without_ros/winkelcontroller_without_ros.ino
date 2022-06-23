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
Encoder encoder_lin(interruptPin0, digitalIn0);
Encoder encoder_rot(interruptPin1, digitalIn1);

// Setpoint_rot
double Setpoint_rot = ENC / 2;
double Input_rot, Output_rot = 0;

// Setpoint_lin
double Setpoint_lin = (LEFTPOS - RIGHTPOS) / 2 + RIGHTPOS; //Mitte der Linearführung
double Input_lin, Output_lin = 0;

// PIDs
#include <PID_v1.h>
double Kp_rot = 0.3, Ki_rot = 0, Kd_rot = 0.1;
PID PID_rot(&Input_rot, &Output_rot, &Setpoint_rot, Kp_rot, Ki_rot, Kd_rot, DIRECT);

// Motor
#define PWM_MIN 0
#define PWM_MAX 50
const int pwm_pin = CONTROLLINO_AO0;
const int motor_in1_pin = CONTROLLINO_DO1;
const int motor_in2_pin = CONTROLLINO_DO2;

// REFERENCE
const int ind_pin = CONTROLLINO_DI2;
bool referenced = false;

void setup()
{
  Serial.begin(9600);

  // encoder
  pinMode(interruptPin0, INPUT);
  pinMode(interruptPin1, INPUT);
  pinMode(digitalIn0, INPUT);
  pinMode(digitalIn1, INPUT);

  // induktion
  pinMode(ind_pin, INPUT);

  // motor
  pinMode(pwm_pin, OUTPUT);
  pinMode(motor_in1_pin, OUTPUT);
  pinMode(motor_in2_pin, OUTPUT);
  // motor aktivieren ?!
  digitalWrite(motor_in1_pin, HIGH);
  digitalWrite(motor_in2_pin, LOW);

  // pid
  PID_rot.SetMode(AUTOMATIC);
  PID_rot.SetOutputLimits(-PWM_MAX, PWM_MAX);
  PID_rot.SetSampleTime(50); //SampleTime auf 50ms

  delay(1000);
  encoder_rot.write(0);
}

void loop()
{
  long encoder_read = encoder_rot.read();
  long encoder_abs = abs(encoder_read);
  int encoder_int = int(encoder_abs);
  int encoder_mod = encoder_int%ENC;
  Input_rot = encoder_mod;
  printEncoder();

  if (controlable() == true)
  {
    PID_rot.Compute();
    int drv = int(Output_rot);
    drive_motor(drv);
  } else {
    drive_motor(0);
  }

  if (digitalRead(ind_pin) == false) {
    encoder_rot.write(0);
  }

}

bool allowMovement(int pwm)
{
  return true;
  if (digitalRead(ind_pin) == false && pwm > 0) {
    return false;
  }

  //Positionsüberprüfung erst ausführen, wenn referenziert wurde
  if (referenced == true) {
    int currentPosition = encoder_lin.read();
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

// motor wird über geschwindikeit angesteuert (pwm signal)
void drive_motor(int pwm)
{
  //falls Schlitten bereits an einer Grenze steht, soll der Motor deaktiviert werden
  //oder falls PWM=0
  if (allowMovement(pwm) == false || pwm == 0) {
    digitalWrite(motor_in1_pin, LOW);
    digitalWrite(motor_in2_pin, LOW);
    analogWrite(pwm_pin, int(0));
    return;
  }
  //Richtung setzen
  if (pwm > 0) {
    digitalWrite(motor_in1_pin, HIGH);
    digitalWrite(motor_in2_pin, LOW);
  }
  else {
    digitalWrite(motor_in1_pin, LOW);
    digitalWrite(motor_in2_pin, HIGH);
  }

  int abs_int = abs(pwm);
  analogWrite(pwm_pin, int(abs_int));
}

void reference()
{
  referenced = false;
  // nach links fahren bis induction aktiviert wird
  while (digitalRead(ind_pin) == true)
  {
    drive_motor(8);
  }

  drive_motor(0);
  delay(1000);

  // nullpunkt der linearführung
  encoder_lin.write(0);
  referenced = true;

  delay(2500);

  // zur mitte fahren
  while (encoder_lin.read() < (LEFTPOS-RIGHTPOS) / 2 + RIGHTPOS)  
  {
    drive_motor(-8);
    printEncoder();
  }

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
      Serial.println(Output_rot);
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
