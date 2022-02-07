#include <Arduino.h>
#include "ros.h"
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Float64.h"
#include "string.h"

float my_float[10];

void marrayCallback(const std_msgs::Float64MultiArray &comingMarray);

ros::NodeHandle nh;
ros::Subscriber<std_msgs::Float64MultiArray>marray_sub("multiarray_topic", &marrayCallback);

std_msgs::Float64 my_msgg;
ros :: Publisher debugpub ("debug_topic",&my_msgg);
HardwareSerial Serial3(PB11,PB10);

int get_abs (float my_float);
float map_data(float x);
String direction_funct(float y);
String message_creator(float z);

void setup() {
nh.initNode();
nh.subscribe(marray_sub);
nh.advertise(debugpub);
Serial3.begin(9600);
}

void loop() {
  nh.spinOnce();
  delay(1);
  my_msgg.data =my_float[1];
  debugpub.publish(&my_msgg);

}

int get_abs(float my_float){
  if(my_float<=0) {
    return -my_float;
  }
  else if(my_float>0) { return my_float;}
}

float map_data(float x){
  return 255*x;
}

String direction_funct(float y){
  static int direction;
  if(y<=0){
    direction =0;
  }
  else if(y >0){
    direction =1;}
  return String(direction);
}

String message_creator ( float z){
  String processed_string = String ( get_abs ( map_data (z))) ;
  while(processed_string.length()<3){
  processed_string = "0" + processed_string;
  }
  return "S" + direction_funct (z) + processed_string + "F";
}


void marrayCallback(const std_msgs::Float64MultiArray &comingMarray ){
for (int i =0; i <8; i ++) 
  {
    my_float[i] = comingMarray.data[i];
  }
Serial3.println(message_creator(my_float[1]));

}