// Gleichstrommotor 1

int GSM1 = 44;
int in1 = 45;
int in2 = 46;

// Poti
int poti_pin = A0;


void setup()
{
  Serial.begin(115200);
  pinMode(GSM1, OUTPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(poti_pin, INPUT);

}
void loop()
{

  
  int poti_value = get_poti_value();
  Serial.println(poti_value);
  drive_motor(poti_value);

}

void drive_motor(int pwm)

{
  if (pwm > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(GSM1, abs(pwm));
  }
  else if (pwm < 0)
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(GSM1, abs(pwm));
  }
  else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(GSM1, 0);
  }
}

int get_poti_value() {
  int adc = analogRead(poti_pin);
  int test = map(adc, 0, 1023, -255, 255);
  return test;
}
