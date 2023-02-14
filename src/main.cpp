#include <Arduino.h>

#include <Matrix.h>
#include <Constants.h>
#include <Menu.h>

Matrix matrix(8,8);
Menu menu;


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  menu.run(&matrix);
  
  
  matrix.update();
  
}