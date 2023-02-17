#include <Matrix.h>
#include <Arduino.h>
#include <Constants.h>

Matrix::Matrix(byte height, byte width){
    _height = height;
    _width = width;
    set_pins(row_pins, MATRIX_WIDTH, INPUT);
    set_pins(col_pins, MATRIX_HEIGHT, INPUT);

}

void Matrix::set_pins( const byte* pin_array, byte size_of_array, byte pin_mode, byte val = LOW){
    for( byte i = 0; i < size_of_array; i++){
        if( pin_mode == INPUT){
            pinMode(pin_array[i], INPUT);
        }
        else if (pin_mode == OUTPUT)
        {
            pinMode(pin_array[i], OUTPUT);
            digitalWrite(pin_array[i], val);
        }
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

void Matrix::push_row_data(byte led_color, byte row_number, byte row_data){
    //Push a byte of data to a selected row
    byte temp_val;
    for( byte i = 0; i < MATRIX_WIDTH; i++){
        switch(led_color){
            case BLUE_LED:
                temp_val = 0b00000001 & (row_data >> (MATRIX_WIDTH - i - 1)); //get the bit at the appropriate collumn of the supplied data byte
                blue_col_data[i] |= (temp_val << row_number); //Assign the bit to the correct position in the collumn 
                break;
            case YELLOW_LED:
                temp_val = 0b00000001 & (row_data >> (MATRIX_WIDTH - i - 1)); //get the bit at the appropriate collumn of the supplied data byte
                yellow_col_data[i] |= (temp_val << row_number); //Assign the bit to the correct position in the collumn 
                break;
        }
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
            byte _cnt; //Use a separate counter for positioning at the front of the matrix
            for( byte i = new_data_size; i < MATRIX_WIDTH; i++){
                //Copy to byte from the array starting at index of new data size
                //If inputting 4 collumns, move index 4 (col5) to index 0 (col1)
                byte tmp_col = array_ptr[i]; //Get the value of the array using the pointer plus index
                array_ptr[_cnt] = tmp_col; //Reassign the shifted collumn the temp value
                Serial.println(array_ptr[_cnt]);
                _cnt++; //increment the secondary counter
            } 

            _cnt = 0;
            //Push in new data to the columns to overwrite
            //If inputting 4 collumns, start at index 4 (col5) and write until index 7 (col8)
            for( byte i = MATRIX_WIDTH - new_data_size; i < _width; i++){
                array_ptr[i] = new_data[_cnt];
                _cnt++;
            }  
            break;

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
            break;

    }
}


void Matrix::update(){
    //Create an array of bitwise comparator values
    //byte comparators[8] = {1, 2, 4, 8, 16, 32, 64, 128};
    byte temp_val;
    //Serial.println("Updating Matrix");
    

    //Scan through each collumn from left to right
    for( byte i = 0; i < _width; i++ ){
        if( col_active == true){
            Serial.println("Another collumn already active");
            break;
        }

        col_active = true;


        //Light the Yellow LEDs
        set_pins(row_pins, MATRIX_WIDTH, OUTPUT, LOW); //Change the rows to outputs and set LOW

        if( yellow_col_data[i] > 0){ //Check if there's even data in the collumn
            pinMode(col_pins[i], OUTPUT); //Change the collumn to an output and set LOW to sink current
            digitalWrite(col_pins[i], LOW);

            temp_val = yellow_col_data[i];
            for( byte j = 0; j < _height; j++){  //Cycle through the rows starting at the bottom which is Row 0
                if( ((temp_val >> j) & 0b00000001) == true){
                    digitalWrite(row_pins[j], HIGH);    //Briefly turn the light on 
                    delayMicroseconds(_light_on_time);
                    digitalWrite(row_pins[j], LOW);
                }
            }
            pinMode(col_pins[i],INPUT);
        }
        else{
            Serial.println("No Data");
        }



        //Light the Blue LEDs
        /* set_pins(row_pins, MATRIX_HEIGHT, HIGH);
        
        digitalWrite(col_pins[i], HIGH);

        if( blue_col_data[i] > 0){ //Check if there's even data in the collumn
            temp_val = blue_col_data[i];
            for( byte j = 0; j< _height; j++){  //Cycle through the rows starting at the bottom which is Row 0
                if(((blue_col_data[i] >> j) & 0b00000001) == true){
                    digitalWrite(row_pins[j], LOW);    //Briefly turn the light on 
                    delayMicroseconds(_light_on_time);
                    digitalWrite(row_pins[j], HIGH);
                }
            }
        } */

        //set_pins(row_pins, MATRIX_WIDTH, INPUT);
        //set_pins(col_pins, MATRIX_HEIGHT, INPUT);

        col_active = false;
        
    }
}

void Matrix::slide_row(byte led_color, byte direction, byte row){
    //Slide data within a row a one space either left or right
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
            for( byte i = 1; i < 8; i++){ //Don't need to start at 0 since collumn 0 will be lost in a left shift
                if(( array_ptr[i] & masks_on[row]) > 0){ //Bit is a 1
                    array_ptr[i-1] |= masks_on[row];
                }
                else{
                    array_ptr[i-1] &= masks_off[row];
                }
            }
            array_ptr[MATRIX_WIDTH-1] &= masks_off[row];
            break;

        case RIGHT:
            for( byte i = 6; i > 0; i--){ //Don't need to start at 0 since collumn 0 will be lost in a left shift
                if( (array_ptr[i] & masks_on[row]) > 0){ //Bit is a 1
                    array_ptr[i+1] |= masks_on[row];
                }
                else{
                    array_ptr[i+1] &= masks_off[row];
                }
            }
            array_ptr[0] &= masks_off[row];
            break;

    }
}


