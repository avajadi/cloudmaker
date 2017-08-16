#ifndef CMD_H
#define CMD_H
#include "Arduino.h"

using namespace std;

class Cmd {
  HardwareSerial stream;
  const int START = 0xFF;
  const int STOP = 0xFE;
  void( *onReceived) (int command, byte* params, byte param_length);
  int fromByteArray( byte* bytes );
public:
  Cmd( HardwareSerial serial );
  void receive();
  void setOnReceived( void(*func)(int command, byte* params, byte param_length));
};

#endif
