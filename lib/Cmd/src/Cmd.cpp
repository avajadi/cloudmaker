#include "Cmd.h"


Cmd::Cmd( HardwareSerial serial ) : stream(serial){
}

void Cmd::setOnReceived( void(*func)(int command, byte* params, byte param_length)) {
  onReceived = func;
}

int Cmd::fromByteArray( byte* bytes ) {
    int intValue = 0;
    for( byte i = 0 ; i < 4 ; i++ ) {
      intValue |= bytes[i]<<(3-i);
    }
  return intValue;
}

void Cmd::receive() {
	int c = 0;
  byte command[4], params[32], param_length = 0;

  // Wait for command start
	while( c != START ) {
		while( stream.available() == 0 ) {};
		c = stream.read();
	}
  // Receive command in first 4 bytes
  while( stream.available() < 4 ) {};
  for( byte i = 0 ; i < 4 ; i++ ) {
		command[i] = stream.read();
  }
  while( param_length < 32 ) {
		while( stream.available() == 0 ) {};
		c = stream.read();
    if( c == STOP ) {
      break;
    }
    params[++param_length] = c;
	}
  onReceived( command, params, param_length);

}
