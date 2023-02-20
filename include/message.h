#include <Arduino.h>

// This will be the data structure for the received message 
typedef struct struct_message {
  String rfid;
  float amount;
} struct_message;
