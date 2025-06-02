#include <Ultrasonic.h>
#include <SoftwareSerial.h>
// Arduino all in one robot car features:-
// . Bluetooth controlled
// . Voice controlled
// . IR remote controlled
// . Speed controlled
// . Going forward obstacle detection
// . Obstacle avoiding mode

#include <Servo.h>


Servo servo;

const int echo_pin = 4;
const int trig_pin = 5;

const int enable = 6;

const int lmf = 7; // Left motor forward
const int lmb = 8; // Left motor backward
const int rmf = 9; // Right motor forward
const int rmb = 10; // Right motor backward

const int left_ir_sensor = 11; // Left IR sensor
const int right_ir_sensor = 12; // Right IR sensor


int left_ir_value, right_ir_value; // Variable for storing value from left ir sensor and right ir sensor
int obs, hand; // Variable for changing modes
int Speed = 100; // Variable for controling the spped of motor
int distance_left, distance_front, distance_right; // Variable for getting front, left and right distance using ultrasonic sensor
int set = 25; // Variable for obstacle avoidance to set the minimum distance
int check_obs; // Variable for going forward obstacle dectection
SoftwareSerial BT(0, 1);

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  servo.attach(3);
  servo.write(80);

  pinMode(echo_pin, INPUT);
  pinMode(trig_pin, OUTPUT);

  pinMode(enable, OUTPUT);

  pinMode(lmf, OUTPUT);
  pinMode(lmb, OUTPUT);
  pinMode(rmf, OUTPUT);
  pinMode(rmb, OUTPUT);

  pinMode(left_ir_sensor, INPUT);
  pinMode(right_ir_sensor, INPUT);

  delay(500);
}

void loop() {
  if (BT.available() > 0) {
    int val = BT.read();
    BT.println(val);

    // Robot movement commands
    if (val == 1) { // Go forward
      check_obs = 1;
      forward();
    }
    else if (val == 2) { // Go backward
      backward();
    }
    else if (val == 3) { // Rotate left
      rotate_left();
    }
    else if (val == 4) { // Rotate right
      rotate_right();
    }
    else if (val == 5) { // Stop
      stp();
    }
    else if (val == 22) { // Obstacle avoiding mode
      obs = obs + 1;
      if (obs == 2) {
        obs = 0;
        stp();
      }
    }
    else if (val == 23) { // Hand follower mode
      hand = hand + 1;
      if (hand == 2) {
        hand = 0;
        stp();
      }
    }
    else if (val == 24) { // Speed low
      Speed = 130;
     
      delay(250);
     
    }
    else if (val == 25) { // Speed medium
      Speed = 190;
     
      delay(250);
     
    }
    else if (val == 26) { // Speed high
      Speed = 255;
      
      delay(250);
      
    }

    // Voice controlled commands
    else if (val == 27) { // Voice command go forward
   
      forward();
      delay(500);
      stp();
      
    }
    else if (val == 28) { // Voice command go backward
    
      backward();
      delay(500);
      stp();
     
    }
    else if (val == 29) { // Voice command turn left
      servo.write(180);
      rotate_left();
      delay(400);
      stp();
      servo.write(80);
    }
    else if (val == 30) { // Voice command turn right
      servo.write(0);
      rotate_right();
      delay(400);
      stp();
      servo.write(80);
    }
  
  } // VOID LOOP STARTED HERE

  

  left_ir_value = digitalRead(left_ir_sensor); // Store data from 'left ir sensor' to 'left ir data' variable
  right_ir_value = digitalRead(right_ir_sensor); // Store data from 'right ir sensor' to 'right ir data' variable

 //************ FUNCTION FOR OBSTACLE AVOIDING MODE ******************
 
  if (obs == 1) {
    distance_front = get_distance();
    if ((distance_front <= set) || (left_ir_value == 1) || (right_ir_value == 1))  {
      
      hc_sr4();
    }
    else {
      forward();
      
    }
  }
}


// ********************************************************************************** FUNCTIONS FOR OBSTACLE AVOIDING MODE ********************************************************************

int get_distance() {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  long duration = pulseIn(echo_pin, HIGH);
  int distance_cm = duration * 0.034 / 2;  // Calculate distance in centimeters

  return distance_cm;
}

void hc_sr4() {
  stp();
  servo.write(0);
  delay(500);
  distance_right = get_distance();
  delay(100);
  servo.write(180);
  delay(500);
  distance_left = get_distance();
  delay(100);
  servo.write(90);
  delay(300);
  compareDistance();
}

void compareDistance() {
  if (distance_left > distance_right) {
    rotate_left();
    delay(350);
  }
  else if (distance_right > distance_left) {
    rotate_right();
    delay(350);
  }
  else {
    backward();
    delay(500);
    rotate_left();
    delay(500);
  }
}

// ********************************************************************************** FUNCTIONS FOR ROBOT MOVEMENT ****************************************************************************
void forward() {
  digitalWrite(lmf, HIGH);
  digitalWrite(lmb, LOW);
  digitalWrite(rmf, HIGH);
  digitalWrite(rmb, LOW);
  analogWrite(enable, Speed);
}
void backward() {
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, HIGH);
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, HIGH);
  analogWrite(enable, Speed);
}
void stp() {
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, LOW);
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, LOW);
  check_obs = 0;
}
void left() {
  digitalWrite(lmf, HIGH); 
  digitalWrite(lmb, LOW);
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, LOW);
  analogWrite(enable, 100);
}
void right() {
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, LOW);
  digitalWrite(rmf, HIGH);
  digitalWrite(rmb, LOW);
  analogWrite(enable, 100);
}
void rotate_left() {
  digitalWrite(lmf, HIGH);
  digitalWrite(lmb, LOW);
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, HIGH);
  analogWrite(enable, 100);
}
void rotate_right() {
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, HIGH);
  digitalWrite(rmf, HIGH);
  digitalWrite(rmb, LOW);
  analogWrite(enable, 100);
}
