/* 
 * Button Example for Rosserial
 */

#include <ros.h>
#include <std_msgs/Bool.h>

//NodeHandle
ros::NodeHandle nh;

//Boolean message for Push button
std_msgs::Bool sw1_state;

//Defining Publisher in a topic called pushed
ros::Publisher pub_sw1_state("sw1_state", &sw1_state);


void messageCb( const std_msgs::Bool& msg){
  digitalWrite(13, msg.data);   // set the pin state to the message data
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, msg.data);
  delay(500);

}
//Defining Subscriber in a topic called aaa
ros::Subscriber<std_msgs::Bool> sub("/arduino/led", &messageCb );

//LED and Push button pi definitions
const short sw1_pin = 7;

void setup()
{
  nh.initNode();
  nh.advertise(pub_sw1_state);
  nh.subscribe(sub);
  
  //initialize an LED output pin 
  //and a input pin for our push button
  pinMode(sw1_pin, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop()
{
  bool reading_sw1 = ! digitalRead(sw1_pin);
  
  sw1_state.data = reading_sw1;
  
  pub_sw1_state.publish(&sw1_state);
  
  nh.spinOnce();
  
  delay(100);
}
