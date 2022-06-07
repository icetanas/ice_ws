// code for dc motor with encoder
// this pin for arduino mega
// platform.io require this 
#include <Arduino.h>
#include <ezButton.h>
ezButton sw_is_up(7);

// ros plugin
#include <ros.h>
#include <std_msgs/Bool.h>

//NodeHandle
ros::NodeHandle nh;
/////////////////////////////////////////////////////////////////
///////////////////////////pub///////////////////////////////////
/////////////////////////////////////////////////////////////////
//Boolean message for Push button
std_msgs::Bool is_up;
//Defining Publisher in a topic called pushed
ros::Publisher pub_is_up("is_up", &is_up);


/////////////////////////////////////////////////////////////////
////////////////////////////sub//////////////////////////////////
/////////////////////////////////////////////////////////////////
void lift_cm_up( const std_msgs::Bool& lift_cm_up)
{
  while(lift_cm_up.data)
  {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
    digitalWrite(13, HIGH);
    delay(500);
    sw_is_up.loop();
    is_up.data = ! sw_is_up.getState();
    if(is_up.data)
    {
      pub_is_up.publish(&is_up);
      digitalWrite(13, HIGH);
      nh.spinOnce();
      break;
    }
  }
}
//Defining Subscriber in a topic called lift_up_cm
ros::Subscriber<std_msgs::Bool> sub_lift_cm_up("lift_cm_up", &lift_cm_up);

void lift_cm_down( const std_msgs::Bool& lift_cm_down)
{
}
//Defining Subscriber in a topic called lift_up_cm
ros::Subscriber<std_msgs::Bool> sub_lift_cm_down("lift_cm_down", &lift_cm_down);

void setup()
{
  nh.initNode();
  nh.subscribe(sub_lift_cm_up);
  nh.subscribe(sub_lift_cm_down);
  nh.advertise(pub_is_up);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  pinMode(7, INPUT);

  sw_is_up.setDebounceTime(50);

}

void loop()
{
  nh.spinOnce();
}
