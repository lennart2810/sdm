/*
  roscore
  rosrun rosserial_arduino serial_node.py /dev/ttyACM0

  oder launch file:
  roslaunch sdm controllino.launch (serial node wird als arduino_node gestartet)
*/

#include <ros.h>
#include <std_msgs/Int32.h> // geht bestimmt auch kleiner
#include <Controllino.h>
ros::NodeHandle nh;

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

void setup()
{
  pinMode(interruptPin0, INPUT);
  pinMode(interruptPin1, INPUT);
  pinMode(digitalIn0, INPUT);
  pinMode(digitalIn1, INPUT);
  Serial.begin(9600);
  //nh.initNode();
  //nh.advertise(enc_pub);
}

void loop()
{
  //Serial.println(digitalRead(interruptPin0));
  Serial.print(encoder0.read());
  Serial.print("\t");
  Serial.println(encoder1.read());
  //enc_msg.data = encoder.read(); // man könnte noch funktion schreiben, die das ganze auf 0-360° umwandelt bzw. 0-2000
  //enc_pub.publish( &enc_msg );
  //nh.spinOnce();
  delay(10);
}
