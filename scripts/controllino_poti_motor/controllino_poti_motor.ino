#include <Controllino.h> 
/* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

// the setup routine runs once when you press reset:
void setup() {
    // initialize necessary pin as input pin
    pinMode(CONTROLLINO_A12, INPUT);
    pinMode(CONTROLLINO_AO0, OUTPUT);
    pinMode(CONTROLLINO_DO1, OUTPUT);
    pinMode(CONTROLLINO_DO2, OUTPUT);
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);

    digitalWrite(CONTROLLINO_DO1, HIGH);
}

void loop() {

    int test = analogRead(CONTROLLINO_A12);
    //Serial.println(test);
    
    analogWrite(CONTROLLINO_AO0, test);
    //Serial.println(analogRead(CONTROLLINO_A12));
    delay(10); // delay in between reads for stability
}
