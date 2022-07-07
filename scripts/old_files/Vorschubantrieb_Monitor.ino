
// MTS2 Projekt
// Lennart Fuhrig 
// 201726546

#include <AFMotor.h>    // Ardafruit Motor-Shield (China Kopie)
AF_DCMotor motor(4);    // Gleichstrommotor an Platz 4 angeschlossen

#include <PIDController.h>
PIDController PID;

int Input, Setpoint, Output;
const float kp = 50.0;
const float ki = 0.0;
const float kd = 0.0;

#define ENCODER 690  // 700Encoderschritte pro Umdrehung der Motorwelle
#define STEIGUNG 5   // Steigung der Kugelumlaufspindel
#define MAX_MM 200   // max. Weg der Achse
#define MIN_SPEED 150  // min. PWM Wert (Betrag)

const int encoderPin1 = 20;      // Interrupt-Pin
const int encoderPin2 = 21;
volatile long int encoderValue = 0;

const int referencePin = 42;     // Anschlag-Pin
bool referenced = false;

char inputChar[20];   // serieller Input-Buffer
int serialCounter = 0;

void setup() {

  Serial.begin(9600);

  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  pinMode(referencePin, INPUT);

  PID.begin();
  PID.tune(kp, ki, kd);
  PID.limit(-255, 255); // min. und max. PWM-Signal --> Drehzahl n

  Serial.println("MTS2 Projekt!\n");
  Serial.println("Eingabe '0' zum Referenzieren! \n");
}

void loop() { }

void serialEvent() {  // liest Daten von seriellen Monitor ein

  while (Serial.available()) {

    char inChar = (char)Serial.read();
    inputChar[serialCounter] += inChar;
    serialCounter++;

    if (inChar == '\n')  // "Enter" --> Eingabe ist komplett
    {
      serialCounter = 0;
      serialData(inputChar);

      for (int i = 0; i < sizeof(inputChar); i++) // Eingabe-Array muss zurückgesetzt werden
      {
        inputChar[i] = (char)0;
      }
    }
  }
}


void serialData(char input[]) {

  switch (input[0]) {  // 1. Zeichen des seriellen Inputs ist entscheident für Ausführung eines bestimmtes Codes!

    case '0': reference(); break;

    case 's':

      if (referenced == true) {
        char mm[4];  // Array für Erkennung des Sollwertes (max Wert 999\n)
        for (int c = 0; c < 3; c++) {
          mm[c] = input[c + 1];
        }

        if (atoi(mm) <= MAX_MM) {  // Vorschub wird nur gefahren, wenn Setpoint <= max Verfahrweg
          feed(atoi(mm));  // atoi: char[] --> int
        }

        else {  // Default Ausgabe
          Serial.print("Setpoint ist außerhalb der Begrenzung (0 - ");
          Serial.print(MAX_MM);
          Serial.println(" mm)!");
        }

        for (int c = 0; c < sizeof(mm); c++) {  // Array für Speicherung der Sollpostion auf '0000' zurücksetzen
          mm[c] = (char)0;
        }
      }

      else {
        Serial.println("Eingabe '0' zum Referenzieren! \n");
      } break;

    default: break;
  }
}


void reference() {

  Serial.println("Referenzierung der Linearachse...\n");

  while (digitalRead(referencePin) == HIGH) {  // Motor fährt zurück, bis Endposition (Anschlag-Pin --> LOW) erreicht ist
    motor.setSpeed(250);
    motor.run(FORWARD);
  }

  motor.setSpeed(0); // Stoppt den Motor
  delay(10);
  encoderValue = 0;
  referenced = true;

  Serial.print("Position: "); Serial.print("\t"); Serial.print(encoderValue * STEIGUNG / ENCODER); Serial.println(" mm");
  Serial.print("Encoder: "); Serial.print("\t"); Serial.println(encoderValue);

  Serial.println("\nSollposition in [mm] eingeben! (s200 --> Sollwert = 200 mm):");
}


void feed(int stp) {

  Serial.print("\nSollwertvorgabe:  "); Serial.print(stp); Serial.println(" mm");
  Serial.print("Sollwertvorgabe:  "); Serial.print(stp / STEIGUNG * ENCODER); Serial.println(" Schritte\n");

  Setpoint = stp / STEIGUNG * ENCODER; // setpoint [Encoderschritte] --> Setpoint [mm]
  PID.setpoint(Setpoint);

  attachInterrupt(digitalPinToInterrupt(encoderPin1), updateEncoder, RISING); // Interrupt aktivieren

  while (abs(Setpoint - encoderValue) != 0) {  // stationäre Regelabweichung von max. 1 Encoderschritt

    Input = encoderValue;
    Output = PID.compute(Input);
    moveMotor(Output);
    delayMicroseconds(500);
  }

  motor.setSpeed(0); // stoppt den Motor
  delay(10);
  Serial.print("Encoder: "); Serial.print("\t"); Serial.println(encoderValue);
  Serial.print("Position: "); Serial.print("\t"); Serial.print(encoderValue * STEIGUNG / ENCODER); Serial.println(" mm\n");

  referenced = false; // muss vor dem nächsten feed() neu referenziert werden!

  detachInterrupt(digitalPinToInterrupt(encoderPin1)); // Interrupt deaktivieren
}


void updateEncoder() {

  if (digitalRead(encoderPin2) == HIGH) {
    encoderValue++;
  }

  else {
    encoderValue--;
  }
}


void moveMotor(int n) {   // Drehzahl n wird als PWM-Signal übergeben

  n = n * (-1);  // muss Richtung an das Motor-Shield und Motor anpassen

  if (abs(n) >= MIN_SPEED) {  // Werte < MIN_SPEED reichen nicht aus, um den Motor zu bewegen (HIGH Signal in PWM zu kurz)

    if (n > 0) {  // + PWM-Signal --> vorwärts
      motor.setSpeed(n);
      motor.run(FORWARD);
    }

    else if (n < 0) {  // - PWM-Signal --> rückwärts
      motor.setSpeed(abs(n));
      motor.run(BACKWARD);
    }
  }

  else {
    motor.setSpeed(MIN_SPEED);
  }
}
