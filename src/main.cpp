#include <Arduino.h>

#include <Matrix.h>
#include <Constants.h>
#include <Menu.h>
#include <Controls.h>

Matrix matrix(8,8);
Controls controls(A4, A5);
Menu menu;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
  menu.run(&matrix, &controls);
  
  
  matrix.update();
  
}