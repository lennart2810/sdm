/*
  roscore
  rosrun rosserial_arduino serial_node.py /dev/ttyACM0

  oder launch file:
  roslaunch sdm controllino.launch (serial node wird als arduino_node gestartet)
*/

#include <ros.h>
#include <std_msgs/Int32.h> // geht bestimmt auch kleiner
ros::NodeHandle nh;

#include <Encoder.h>
#define ENC 2000            // pulses per revolution (with Encoder.h)
Encoder encoder(2, 3);      // best performance: both pins have interrrupt capability
std_msgs::Int32 enc_msg;
ros::Publisher enc_pub("enc", &enc_msg);


void setup()
{
  nh.initNode();
  nh.advertise(enc_pub);
}

void loop()
{
  enc_msg.data = encoder.read(); // man könnte noch funktion schreiben, die das ganze auf 0-360° umwandelt bzw. 0-2000
  enc_pub.publish( &enc_msg );
  nh.spinOnce();
}
