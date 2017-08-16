#include<Cmd.h>

void commandHandler(int command, byte* params, byte param_length);
Cmd cmd(Serial);

void setup() {
  Serial.begin( 115200 );
  cmd.setOnReceived( commandHandler );
}

void loop() {
  Serial.println(".");
  cmd.receive();
}

void commandHandler(int command, byte* params, byte param_length) {
  Serial.print( "Received command: " );
  Serial.println( command );
}
