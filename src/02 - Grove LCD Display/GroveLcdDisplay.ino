/*
  Copyright (c) 2017 Nicolò Carandini. All right reserved. 

  This file is licensed to you under the MIT license.
  See the LICENSE file in the project root for more information.

  This library use the following libraries:
  - Arduino Grove - LCD RGB Backlight Library (on GitHub)
    https://github.com/Seeed-Studio/Grove_LCD_RGB_Backlight>
  - TPCWare ColorConverter
    https://github.com/ncarandini/TPCWare_ColorConverter
*/

// Receive text with an end-marker (tipically a \n (newline)
// from the Console and echo it to the same Console and to the LCD.
// Note that the LCD contains a circular buffer of 40 chars, divided in
// two lines of 20 chars.

// To make it more interesting, a rainbow method changes randomly the
// Hue and Saturation of the LCD backlight color.

#include <Wire.h>
#include <rgb_lcd.h>
#include <ColorConverter.h>

rgb_lcd lcd;
ColorConverter colorConverter;

const float fixedHSValue = 100;
const float minHue = 0;
const float maxHue = 360;
const float minSaturation = 30;
const float maxSaturation = 100;
const float deltaHue = 3;
const float deltaSaturation = 3;

float actualHue, actualSaturation;

const byte numChars = 64;
char msg[numChars];   // an array to store the received data
boolean newData = false;

void setup() {
  // set inizial color of LCD backlight
  randomSeed(analogRead(0));
  actualHue = float(random(minHue, maxHue));
  actualSaturation = int(random(minSaturation, maxSaturation));
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Set the LCD backlight color
  setLcdBacklightFromHSV(actualHue, actualSaturation, fixedHSValue);

  // Print a message to the LCD.
  lcd.clear();
  lcd.print("Backlight rainbow is a nice feature!");
  delay(1000);

  // set serial device
  Serial.begin(9600);
  Serial.println("Ready to receive your message.");
}

void loop() {
  recvWithEndMarker();
  showMsg();
  rainbow();
  lcd.scrollDisplayLeft();
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
  if (!newData) {
    // wait a bit:
    delay(200);
  }
}

void showMsg() {
  if (newData == true) {
    // Print back to the serial console
    Serial.print("This just in ... ");
    Serial.println(msg);
    
    // Print a message to the LCD
    lcd.clear();
    lcd.print(msg);

    // reset flag
    newData = false;
  }
}

void rainbow() {

  static float lastHue = actualHue;
  static float targetHue
    targetHue = float(random(minHue, maxHue));
    Serial.print("New targetHue: ");
    Serial.println(targetHue);
  }
  if (actualHue < targetHue)
  {
    actualHue += deltaHue;
  }
  else
  {
    actualHue -= deltaHue;
  }

  // change saturation
  if ((lastSaturation <= targetSaturation) && (actualSaturation >= targetSaturation) ||(lastSaturation >= targetSaturation) && (actualSaturation <= targetSaturation))
  {
    targetSaturation = float(random(minSaturation, maxSaturation));
    Serial.print("New targetSaturation: ");
    Serial.println(targetSaturation);
  }
  if (actualSaturation < targetSaturation)
  {
    actualSaturation += deltaSaturation;
  }
  else
  {
    actualSaturation -= deltaSaturation;
  }
  
  // debug
  Serial.print("H: ");
  Serial.print(actualHue);
  Serial.print(", S: ");
  Serial.println(actualSaturation);
  
  // Set the LCD backlight color
  setLcdBacklightFromHSV(actualHue, actualSaturation, fixedHSValue);
}

void setLcdBacklightFromHSV(float h, float s, float v)
{
  byte r = 0;
  byte g = 0;
  byte b = 0;

  colorConverter.fromHSVToRGB(h, s, v, &r, &g, &b);
  
  // debug
  Serial.print("R: ");
  Serial.print(r);
  Serial.print(", G: ");
  Serial.print(g);
  Serial.print(", B: ");
  Serial.println(b);
  
  // set color of LCD backlight
  lcd.setRGB(r, g, b);
}

