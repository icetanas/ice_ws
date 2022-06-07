// code for dc motor with encoder
//////////////////////////////////////////////////////////////////
/////////////////////////ros plugin///////////////////////////////
//////////////////////////////////////////////////////////////////
#include <ros.h>
#include <std_msgs/Bool.h>
// platform.io require this 
#include <Arduino.h>
#include <ezButton.h>
ezButton sw_down_done(6);
ezButton sw_up_done(7);
//state switch
int state_sw;

// The aformention connections
int enA = 10;
int in1 = 8;
int in2 = 9;

//NodeHandle
ros::NodeHandle nh;

//Delare variable publisher
std_msgs::Bool done;
//Defining Publisher in a topic called done and id_down
ros::Publisher pub_lift_done("done", &done);

std_msgs::Bool sw2;
//Defining Publisher in a topic called done and id_down
ros::Publisher pub_sw2("sw2", &sw2);

std_msgs::Bool sw1;
//Defining Publisher in a topic called done and id_down
ros::Publisher pub_sw1("sw1", &sw1);

void lift_cb( const std_msgs::Bool& lift)
{
  if(lift.data){
    while(true)
     {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      analogWrite(enA, 255);
      done.data = false;
      pub_lift_done.publish(&done);
      
      sw_up_done.loop();
      state_sw = ! sw_up_done.getState();
      sw2.data = state_sw;
      pub_sw2.publish(&sw2);
      
      if(state_sw)
      {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        done.data = true;
        pub_lift_done.publish(&done);
        sw2.data = state_sw;
        pub_sw2.publish(&sw2);
        nh.spinOnce();
        break;
      }
     }
  }
  else
  {
    while(true)
     {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(enA, 255);
      done.data = false;
      pub_lift_done.publish(&done);

      sw_down_done.loop();
      state_sw = ! sw_down_done.getState();
      sw1.data = state_sw;
      pub_sw1.publish(&sw1);
      
      if(state_sw)
      {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        done.data = true;
        pub_lift_done.publish(&done);
        nh.spinOnce();
        break;
      }
     }
   }
   digitalWrite(13, LOW);
}

// this pin for arduino mega
//////////////////////////////////////////////////////////////////
///////////////////////Arduino and switch/////////////////////////
//////////////////////////////////////////////////////////////////

// Motor encoder output pulses per 360 degree revolution (measured manually)
#define ENC_COUNT_REV 2000 //pules per revolution
 
// Encoder output to Arduino Interrupt pin. Tracks the pulse count.
#define ENC_IN_RIGHT_A 2 // from encoder to Aruino
#define ENC_IN_RIGHT_B 3

// 1 = Forward; -1 = Reverse
int Direction_right = 1;
 
// Keep track of the number of right wheel pulses
volatile long right_wheel_pulse_count = 0;
 
// One-second interval for measurements
int interval = 1000; //ms
  
// Counters for milliseconds during interval
long previousMillis = 0;
long currentMillis = 0;
 
// Variable for RPM measuerment
float en_rpm_right = 0;
float motor_rpm_right = 0;
 
// Variable for angular velocity measurement
float ang_velocity_right = 0;
float ang_velocity_right_deg = 0;
 
const float rpm_to_radians = 0.10471975512;
const float rad_to_deg = 57.29578;


// Gear Ratio
// Small = 10, Large = 15
float gear_ratio = 15/10;

// Increment the number of pulses by 1
void right_wheel_pulse() {
  right_wheel_pulse_count += Direction_right;
  /* 
  // Read the value for the encoder for the right wheel
  int val = digitalRead(ENC_IN_RIGHT_B);

  Direction_right = true
  if(val == LOW) {
    Direction_right = false; // Reverse
  }
  else {
    Direction_right = true; // Forward
  }
   
  if (Direction_right) {
    right_wheel_pulse_count++;
  }
  else {
    right_wheel_pulse_count--;
  } */
}

void home_lift_walkie()
{
  while(true)
     {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      analogWrite(enA, 255);
      sw_down_done.loop();
      state_sw = ! sw_down_done.getState();
      if(state_sw)
      {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        break;
      }
     }
}
 
//Defining Subscriber in a topic called lift_up and lift_down
ros::Subscriber<std_msgs::Bool> lift_command("lift_command", &lift_cb);

void setup() {

  //////////////////////////////////////////////////////////////////
  /////////////////////////ros node init////////////////////////////
  //////////////////////////////////////////////////////////////////
  nh.initNode();
  nh.advertise(pub_lift_done);
  nh.advertise(pub_sw1);
  nh.advertise(pub_sw2);
  nh.subscribe(lift_command);
  //////////////////////////////////////////////////////////////////
  ///////////////////////Arduino and switch/////////////////////////
  //////////////////////////////////////////////////////////////////
  // Set used pins as output
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  //Serial.begin(9600); 
 
  // Set pin states of the encoder
  pinMode(ENC_IN_RIGHT_A, INPUT_PULLUP);
  pinMode(ENC_IN_RIGHT_B, INPUT);

  // debug led
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  sw_down_done.setDebounceTime(50); 
  sw_up_done.setDebounceTime(50);
  // Every time the pin goes high, this is a pulse
  attachInterrupt(digitalPinToInterrupt(ENC_IN_RIGHT_A), right_wheel_pulse, RISING);
  // Making sure I don't get my beard tangled 
  // in the motor when I'm hanging over it 
  // plugging it in AKA motor off
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  //publish
  done.data = true;
  pub_lift_done.publish(&done);
  nh.spinOnce();
  home_lift_walkie();
}


void loop() {

  //Serial.print(state1);
  /*
  if(state2 == 1){
    // turn it on
    //Serial.println(" not touch");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, 255);
  }
  if(state2 == 0){
    //Serial.println("touch");
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }*/
  
//  // turn it on
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, HIGH);
//
//  // analogWrite(enA, 100); // a little bit faster now
//  analogWrite(enA, 70); // a little bit faster now
//
//  // Come on now!
//  digitalWrite(in1, LOW);
//  digitalWrite(in2, LOW);
//  

  // Record the time
  currentMillis = millis();
  long delta_time = currentMillis - previousMillis;
  // If one second has passed, print the number of pulses
  if (currentMillis - previousMillis >= interval) {
 
    previousMillis = currentMillis;
    //Serial.print(delta_time);

 
    // Calculate revolutions per minute
    // Real RPM at 10.20 RPM, pulses 284 and RPM " 8.00"
    en_rpm_right = ((float)(right_wheel_pulse_count * 60.0 / (float)ENC_COUNT_REV))/ (float)(delta_time/1000.0);
    motor_rpm_right =  en_rpm_right * (float)gear_ratio;
    ang_velocity_right = en_rpm_right * rpm_to_radians;   
    ang_velocity_right_deg = ang_velocity_right * rad_to_deg;
    /*
    Serial.print(" Pulses: ");
    Serial.println(right_wheel_pulse_count);
    Serial.print(" Encoder Speed: ");
    Serial.print(en_rpm_right);
    Serial.println(" RPM");
    Serial.print(" Motor Speed: ");
    Serial.print(motor_rpm_right);
    Serial.println(" RPM");
    Serial.print(" Angular Velocity: ");
    Serial.print(ang_velocity_right);
    Serial.print(" rad per second");
    Serial.print("\t");
    Serial.print(ang_velocity_right_deg);
    Serial.println(" deg per second");
    Serial.println();
    */
    right_wheel_pulse_count = 0; 
  }


  //////////////////////////////////////////////////////////////////
  /////////////////////////ros node init////////////////////////////
  //////////////////////////////////////////////////////////////////
  //publish
  done.data = true;
  pub_lift_done.publish(&done);

  sw_down_done.loop();
  state_sw = ! sw_down_done.getState();
  sw1.data = state_sw;
  pub_sw1.publish(&sw1);
  
  sw_up_done.loop();
  state_sw = ! sw_up_done.getState();
  sw2.data = state_sw;
  pub_sw2.publish(&sw2);
  
  nh.spinOnce();



}
