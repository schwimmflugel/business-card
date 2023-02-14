#include <Matrix.h>
#include <Arduino.h>
#include "Constants.h"

Matrix::Matrix(byte height, byte width){
    _height = height;
    _width = width;
}

void Matrix::set_pins( byte* pin_array, byte size_of_array, byte val){
    for( byte i = 0; i < size_of_array; i++){
        digitalWrite(pin_array[i], val);
    }
}

void Matrix::push_col_data(byte led_color ,byte column_number, byte data){
    if( led_color == BLUE_LED){
        blue_col_data[column_number] = data;
    }
    else if (led_color == YELLOW_LED){
        yellow_col_data[column_number] = data;
    }
}

void Matrix::reset( byte led_color ){
    if( led_color == BLUE_LED){
        for( byte i = 0; i < 8; i++){
            blue_col_data[i] = 0;
        }
    }
    else if (led_color == YELLOW_LED){
        for( byte i = 0; i < 8; i++){
            yellow_col_data[i] = 0;
        }
    }
}


void Matrix::move_data(byte led_color, byte direction, byte* new_data, byte new_data_size){
    /*Function to move data around the matrix grid
    LED Color assigns whether its the array for BLUE_LED or YELLOW_LED
    Directions specifies whether to move the data LEFT, RIGHT, UP, or DOWN
    new_data can be an array of new collumn data so you can move one collumn at a time or multiple
    new_data_size if the number of collumns being passed to the function -> just us sizeof(new_data)    */
    byte* array_ptr;

    switch(led_color){
        case BLUE_LED:
            array_ptr = blue_col_data;
            break;
        case YELLOW_LED:
            array_ptr = yellow_col_data;
            break;
    }


    switch(direction){

        case LEFT:
            for( byte i = new_data_size; i < _width; i++){
                //Copy to byte from the array starting at index of new data size
                //If inputting 4 collumns, move index 4 (col5) to index 0 (col1)
                byte _cnt = 0; //Use a separate counter for positioning at the front of the matrix
                byte tmp_col = *(array_ptr + i); //Get the value of the array using the pointer plus index
                *(array_ptr + _cnt) = tmp_col; //Reassign the shifted collumn the temp value
                _cnt++; //increment the secondary counter
            } 

            //Push in new data to the columns to overwrite
            //If inputting 4 collumns, start at index 4 (col5) and write until index 7 (col8)
            for( byte i = _width - new_data_size; i < _width; i++){
                byte _cnt = 0;
                *(array_ptr + i) = new_data[_cnt];
                _cnt++;
            }  

        case RIGHT:
            for( byte i = new_data_size; i < _width; i++){
                //Copy to byte from the array starting at index of new data size
                //If inputting 4 collumns, move index 4 (col5) to index 8 (col1)
                byte _cnt = _width; //Use a separate counter for positioning at the end of the matrix
                byte tmp_col = *(array_ptr + i); //Get the value of the array using the pointer plus index
                *(array_ptr + _cnt) = tmp_col; //Reassign the shifted collumn the temp value
                _cnt--; //increment the secondary counter
            } 

            //Push in new data to the columns to overwrite
            //If inputting 1 collumn, start at index 7 and write only that
            //If inputting 4 collumns, start at index 4 (col5) and write until index 0 (col1)
            for( byte i = _width - new_data_size; i >= 0; i--){
                byte _cnt = new_data_size - 1;
                *(array_ptr + i) = new_data[_cnt];
                _cnt--;
            }  

    }
}


void Matrix::update(){
    //Create an array of bitwise comparator values
    byte comparators[8] = {1, 2, 4, 8, 16, 32, 64, 128};


    //Scan through each collumn from left to right
    for( byte i = 0; i < _width; i++ ){
        if( col_active == true){
            Serial.println("Another collumn already active");
            break;
        }

        col_active = true;


        //Light the Yellow LEDs
        digitalWrite(colPins[i], LOW);

        if( yellow_col_data[i] > 0){ //Check if there's even data in the collumn
            for( byte j = 0; j< _height; j++){  //Cycle through the rows starting at the bottom which is Row 0
                if( yellow_col_data[i] & comparators[j] == true){
                    digitalWrite(row_pins[j], HIGH);    //Briefly turn the light on 
                    delayMicroseconds(_light_on_time);
                    digitalWrite(row_pins[j], LOW);
                }
            }
        }



        //Light the Blue LEDs
        set_pins(row_pins, _height, HIGH);
        
        digitalWrite(colPins[i], HIGH);

        if( blue_col_data[i] > 0){ //Check if there's even data in the collumn
            for( byte j = 0; j< _height; j++){  //Cycle through the rows starting at the bottom which is Row 0
                if( blue_col_data[i] & comparators[j] == true){
                    digitalWrite(row_pins[j], LOW);    //Briefly turn the light on 
                    delayMicroseconds(_light_on_time);
                    digitalWrite(row_pins[j], HIGH);
                }
            }
        }


        digitalWrite(colPins[i], LOW);
        set_pins(row_pins, _height, LOW); //Set everything low again for the next collumn

        col_active = false;
        
    }
}

