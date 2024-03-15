#include "RFID_Keys.h"

RFID_Keys::RFID_Keys(){
  accessUIDs = nullptr;
  nr_keys = 0;
}

RFID_Keys::~RFID_Keys(){
  if (accessUIDs) {
    for (int w = 0; w < nr_keys; w++) {  
      if (accessUIDs[w]) 
        delete[] accessUIDs[w];    
    }
  delete[] accessUIDs;
  }
}

void RFID_Keys::allocate_memory(int nr){
  accessUIDs = new byte* [nr]();
  nr_keys = nr;
}


void RFID_Keys::add_key(byte* vec, int index){
  accessUIDs[index] = new byte[4];
  for(int j = 0; j<4; j++)
    accessUIDs[index][j] = vec[j];
}
void RFID_Keys::print_keys(){
  Serial.print("\n");
  for(int i = 0; i<nr_keys; i++){
    for(int j = 0; j<4; j++){
      Serial.print(accessUIDs[i][j]);
      Serial.print(" ");
    }
    Serial.print("\n");
  }
}

bool RFID_Keys::check_key(byte* scanned_tag){
  if(accessUIDs == nullptr)
    return false;
  int ok = 1;
  for(int i = 0; i<nr_keys; i++){
    ok = 1;
    for(int j = 0; j<4; j++){
      if(accessUIDs[i][j] != scanned_tag[j])
        ok = 0; 
    }
    if(ok == 1)
      break;
  }
  if (ok == 1)
    return true;
  else
    return false;
}
