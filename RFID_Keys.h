#ifndef RFID_Keys_h
#define RFID_Keys_h

#include "Arduino.h"

class RFID_Keys{
private:
  byte** accessUIDs;
  int nr_keys;
public:
  RFID_Keys();
  ~RFID_Keys();
  int get_nr_keys();
  void allocate_memory(int nr);
  void add_key(byte* vec, int index);
  void print_keys();
  bool check_key(byte*);
};


#endif
