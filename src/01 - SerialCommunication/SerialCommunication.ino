/*
  Copyright (c) 2017 Nicolò Carandini. All right reserved. 

  This file is licensed to you under the MIT license.
  See the LICENSE file in the project root for more information.

  This library use the code from http://forum.arduino.cc/index.php?topic=396450
*/

// Receive text with an end-marker (tipically a \n (newline)
// from the Console and echo it to the same Console.

#include <Wire.h>

const byte numChars = 64;
char msg[numChars];   // an array to store the received data
boolean newData = false;

void setup() {
   // set serial device
  Serial.begin(9600);
  Serial.println("Ready to receive your message...");
}

void loop() {
  recvWithEndMarker();
  showMsg();
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      msg[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      msg[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showMsg() {
  if (newData == true) {
    // Print back to the serial console
    Serial.print("This is what I've received: ");
    Serial.println(msg);
    
    // reset flag
    newData = false;
  }
}
