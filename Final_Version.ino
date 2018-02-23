#include <Wire.h>
#include <AFMotor.h>

#include <Servo.h>

AF_DCMotor left_motor(3, MOTOR12_1KHZ);
AF_DCMotor right_motor(4, MOTOR12_1KHZ);

#define S2 10
#define S3 13
#define sensorOut 11

#define uchar unsigned char
int pos;
int R = 0;
int G = 0;
int B = 0;
int r = 0;
int g = 0;
int b = 0;
int frequency = 0;
int magnetis ;
int plasticis; 

const int trigPin = 2;
const int echoPin = 3;

long duration;
int distance;
int turn_flag;
int counter = 0;
int middleR = 001;
int left = 011;
int right = 110;
int magneticstart = 000;
int middleL = 100;
int checkMagnet;
int sensorPin = A2;
int sensorValue = 0;
int finalmiddle = 111;
int magCounter = 0;

Servo myservo; 
uchar t;
uchar data[16]; // array which stores sensor readings
int where = 19812091092741;
int start = 00001111;
int check_tape = 00011111111;
int check_object = 0010111;
int go_forward = 0100111111;
int turn_back = 1000;
int go_back = 1001;
int creep_forward = 1010;
int turn_around = 1100;
int go_see = 1101;
int creep_backward = 1110;
int do_nothing = 1111110111;
int state = where;

void ReadSensorData() { // reads sensor data and stores in array

  Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
  while (Wire.available())   // slave may send less than requested
  {
    data[t] = Wire.read(); // receive a byte as character
    if (t < 15)
      t++;
    else
      t = 0;
  }
}
void initial_turn(){ // keeps on turning until its off black tape
  ReadSensorData();
  
  while( (data[6] <= 200) || (data[8] <= 200) ) {
    left_motor.setSpeed(70);
    left_motor.run(FORWARD);
    right_motor.setSpeed(70);
    right_motor.run(BACKWARD);
    ReadSensorData();
  }
}
void turn() {
  ReadSensorData();
  
  while( (data[6] >= 200) && (data[8] >= 200) ) {
    left_motor.setSpeed(70);
    left_motor.run(FORWARD);
    right_motor.setSpeed(70);
    right_motor.run(BACKWARD);
    ReadSensorData();
    turn_flag = 1;
  }

  if (turn_flag) 
    STOP();
}
void Magnet(){

sensorValue = analogRead(sensorPin); //read the value of A0
 
  Serial.print("Sensor Value "); // print label to serial monitor 
  Serial.println(sensorValue); //print the value of A0

   if(sensorValue<465 || sensorValue >469){


    checkMagnet = left;
    
      
    
  }

  else {

    checkMagnet = right;
    
    
  }

  
}
void DropLeft() {

 for (pos = 120; pos >=50; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
  
}
void DropRight(){


for (pos = 120; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);       

  
}

}
void middlefromleft(){

      while(pos <=120){
myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30); 
    pos++;
        
      }




  
}
void middlefromright (){

  while( pos >= 120){
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30); 
    pos--;

    
  }

        





  
}
void getDistance() {
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration*0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);
}
void makeColor() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("R= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  R = frequency;
  delay(100);
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  G = frequency;
  delay(100);
  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.println("  ");
  B = frequency;
  delay(100);


 if ( (R < G) && (R < B) ) {
  digitalWrite(A3, HIGH); //flash Red
  digitalWrite(A1, LOW);    
  digitalWrite(A0, LOW);

  state = magnetis;
 }

 else if ( (B < R) && (B < G) ) {
  digitalWrite(A3, LOW);   
  digitalWrite(A1, LOW);    
  digitalWrite(A0,HIGH); //flash Blue

  state = plasticis;
 }


 else if ( (G < R) && (G < B) ) {
  digitalWrite(A0,LOW);   
  digitalWrite(A1,HIGH); //flash Green   
  digitalWrite(A3,LOW);   

  state = turn_back; 
 }

 else {
  digitalWrite(A3,LOW);
  digitalWrite(A1,LOW);    
  digitalWrite(A0,LOW);
 }
  
}
void forward() {
  ReadSensorData(); 
  
  if ( (data[6] <= 200) && (data[8] <= 200) ) { // if the middle 2 sensors see black keep going forward at default speed 
    left_motor.setSpeed(160);
    left_motor.run(FORWARD);
    right_motor.setSpeed(170);
    right_motor.run(FORWARD);
  }

  else if ( (data[6] >= 200) || (data[10] <= 200) || (data[12] <= 200) || (data[14] <= 200) ) { // if any of the right sensors see black speed up the left wheel 
    left_motor.setSpeed(230);
    left_motor.run(FORWARD);
    right_motor.setSpeed(170);
    right_motor.run(FORWARD);
  }

  else if ( (data[8] >= 200) || (data[0] <= 200) || (data[2] <= 200) || (data[4] <= 200) ) { // if any of the left sensors see black speed up the right wheel  
    left_motor.setSpeed(160);
    left_motor.run(FORWARD);
    right_motor.setSpeed(230);
    right_motor.run(FORWARD);
  }
  
  else     
    // otherwise stop  
    STOP();
}
void STOP() {
    left_motor.run(RELEASE);
    right_motor.run(RELEASE);
    delay(1000);  
}
void creep() {
    left_motor.setSpeed(160);
    left_motor.run(FORWARD);
    right_motor.setSpeed(170);
    right_motor.run(FORWARD);
}
void creepBack() {
    left_motor.setSpeed(170);
    left_motor.run(BACKWARD);
    right_motor.setSpeed(160);
    right_motor.run(BACKWARD);
}
void setup(){
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  pinMode(A3, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);
  myservo.attach(10);
  
  Wire.begin();        // join i2c bus (address optional for master)
  t = 0;
}
void checkplace(){


  sensorValue = analogRead(sensorPin); //read the value of A0
  
if( sensorValue < 520 || sensorValue > 526){

  magnetis = left;
  plasticis = right;
}

else {

        magnetis = right;
        plasticis = left; 
}
}


void loop() {



  while( state == where){

checkplace();

state = check_object;



    
  }

  while ( state == start ) { // Following 2 while loops make the robot turn onto different tapes from center
    counter = 0;
    initial_turn();
    state = check_tape;
  }
  
  while ( state == check_tape ) {
    turn();
    state = check_object;
  }

  while ( state == check_object ) {
   
    for(int i = 0; i <= 5; i++){
    getDistance();
    
    if ( distance <= 100){
      state = go_forward;
          break;
      
    }
      
    else{
      state = start;
    }
       
    }

    
  }

  while ( state == go_forward ) {
    forward();
    getDistance();
   
    if ( distance <= 5 ) {
    STOP();
    state = go_see; 
    }
  }

  while ( state == go_see ) {
    for (int i = 0; i < 5; i++){
      makeColor();
    }
   
  }

  while( state == 000) {

    myservo.write(120);

    Magnet();
    // if( counter < 2 ){
      state = checkMagnet;
    // }
    //else {
      // state = finalmiddle;
    //}
  }

  while ( state == right){
    DropRight();
    delay(1000);
    state = middleR;
  }

  while( state == middleR){
    middlefromright();
    delay(1000);

    
    state = turn_back;
    
}

while( state == left){
  DropLeft();
  delay(1000);
  state  = middleL;
}

while( state == middleL)
{
  middlefromleft();
  delay(1000);
 
    state = turn_back;
  
}

while( state == finalmiddle ) {
  myservo.write(120);
  state = finalmiddle;
}


 

  while ( state == creep_backward ) {
    creepBack();
    delay(600);
    STOP();
    state = turn_back;
  }

  while ( state == turn_back ) {
    initial_turn();
    turn();
    state = go_back;
  }

  while ( state == go_back ) {
    forward();
    if ( (data[0] <= 200) && (data[2] <= 200) && (data[4] <= 200) && (data[6] <= 200) && (data[8] <= 200) && (data[10] <= 200) && (data[12] <= 200) && (data[14] <= 200) ) {
      delay(250);
      if( (data[0] <= 200) && (data[14] <= 200) ) {
        STOP();
        state = creep_forward;
      }
    }  
  }

  while ( state == creep_forward ) {
    creep();
    delay(600);
    STOP();
    state = turn_around;
  }

  while ( state == turn_around ) {
    initial_turn();
    turn();
    counter++;

    if (counter == 5) {
      state = start;
    }
  }

  
  
} 
