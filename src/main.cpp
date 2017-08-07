#include <Arduino.h>
#include <Wire.h>
#include <LIDARLite.h>

// Switch these if the stepper turns in the wrong direction
#define CLOCKWISE LOW
#define COUNTER_CLOCKWISE HIGH
#define M1 8
#define M2 9
#define M3 10
#define PULSES_PER_360 200

LIDARLite lidar;

// defines pins numbers
const int stepPin = 3;
const int dirPin = 4;

void sweep( int degrees );
void measure_and_report( int position );
void step( int direction );
void turn( int direction, int steps );

void setup() {
  // Set the two stepper control pins as Outputs
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
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
    sweep( 365 );
  }
  delay(1000);
}

void sweep( int degrees ) {
  int steps = (int) degrees / 360 * PULSES_PER_360;
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
  Serial.print( "position:" );
  Serial.print( position );
  Serial.print( ",distance:" );
  Serial.println( distance );
}
