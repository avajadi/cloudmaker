#include <Arduino.h>
#include <Wire.h>
#include <LIDARLite.h>

// Switch these if the stepper turns in the wrong direction
#define CLOCKWISE LOW
#define COUNTER_CLOCKWISE HIGH
#define MS1 8
#define MS2 9
#define MS3 10
#define STEP_FULL 0
#define STEP_HALF 1
#define STEP_QUARTER 2
#define STEP_EIGHTH 3
#define STEP_SIXTEENTH 7
#define PULSES_PER_360_DEFAULT 200

LIDARLite lidar;

// defines pins numbers
const int stepPin = 3;
const int dirPin = 4;
int stepping = STEP_FULL;
int steps_per_360 = PULSES_PER_360_DEFAULT;
float degrees_per_step = 1.8;
void sweep( int degrees );
void measure_and_report( int position );
void step( int direction );
void turn( int direction, int steps );
void set_stepping( int step_size );
void setup() {
  // Set the two stepper control pins as Outputs
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  set_stepping(STEP_QUARTER);
  Serial.begin( 115200 );

  // Set up the Lidar
  lidar.begin(0, true);
  delay(5000);
}

void loop() {
  Serial.println( "Awaiting command");
  char command[5];
  command [0] = '\0';
  int chars = Serial.readBytes(command, 5 );
  Serial.print( "got chars:" );
  Serial.println( chars );
  if( command[0] == 's' ) {
    Serial.println( "Sweep command" );
    sweep( 360 );
  }
  delay(1000);
}

void sweep( int degrees ) {
  int steps = (int) degrees / 360 * steps_per_360;
  for(int x = 0; x < steps; x++) {
    step(CLOCKWISE);
    delay(10);
    measure_and_report( x );
  }
  Serial.println( "#END");
  turn( COUNTER_CLOCKWISE, steps );
}
void step( int direction ) {
  digitalWrite(dirPin,direction); // Enables the motor to move in a particular direction
  digitalWrite(stepPin,HIGH);
  delayMicroseconds(500);
  digitalWrite(stepPin,LOW);
}

void turn( int direction, int steps ) {
  for( int x = 0 ; x < steps; x++ ) {
    step( direction );
    delay(2);
  }
}

void measure_and_report( int position ) {
  lidar.configure(0);
  int distance = -1;
  distance = lidar.distance();
  Serial.print( "angle:" );
  Serial.print( (float)position * degrees_per_step );
  Serial.print( ",distance:" );
  Serial.println( distance );
}

void set_stepping( int step_size ) {
  boolean ms1_value = 1 == ( step_size & 1 );
  boolean ms2_value = 2 == ( step_size & 2 );
  boolean ms3_value = 4 == ( step_size & 4 );
  digitalWrite(MS1, ms1_value);
  digitalWrite(MS2, ms2_value);
  digitalWrite(MS3, ms3_value);
  switch( step_size ) {
    case STEP_FULL:
      steps_per_360 = PULSES_PER_360_DEFAULT;
      break;
    case STEP_HALF:
      steps_per_360 = PULSES_PER_360_DEFAULT * 2;
      break;
    case STEP_QUARTER:
      steps_per_360 = PULSES_PER_360_DEFAULT * 4;
      break;
    case STEP_EIGHTH:
      steps_per_360 = PULSES_PER_360_DEFAULT * 8;
      break;
    case STEP_SIXTEENTH:
      steps_per_360 = PULSES_PER_360_DEFAULT * 16;
      break;
    default:
      Serial.print("ERROR: Unknown step size: ");
      Serial.println( step_size );
  }
  degrees_per_step = 360.0 / steps_per_360;
}
