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
    for( byte i = 0; i < MATRIX_WIDTH; i++){ //Cycle through all Collumns
        switch(led_color){
            case BLUE_LED:
                temp_val = 0b00000001 & (row_data >> (MATRIX_WIDTH - i - 1)); //get the bit at the appropriate collumn of the supplied data byte
                blue_col_data[i] |= (temp_val << row_number); //Assign the bit to the correct position in the collumn 
                break;
            case YELLOW_LED:
                temp_val = 0b00000001 & (row_data >> (MATRIX_WIDTH - i - 1)); //get the bit at the appropriate collumn of the supplied data byte
                if( temp_val == 1){
                    yellow_col_data[i] |= masks_on[row_number];
                }
                else{
                    yellow_col_data[i] &= masks_off[row_number];
                }
                yellow_col_data[i] |= (temp_val << row_number); //Assign the bit to the correct position in the collumn 
                break;
        }
    }
}

void Matrix::reset( byte led_color = 0){
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
    else{
        for( byte i = 0; i < 8; i++){
            yellow_col_data[i] = 0;
            blue_col_data[i] = 0;
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
                //Serial.println(array_ptr[_cnt]);
                _cnt++; //increment the secondary counter
            } 

            _cnt = 0;
            //Push in new data to the columns to overwrite
            //If inputting 4 collumns, start at index 4 (col5) and write until index 7 (col8)
            for( byte i = MATRIX_WIDTH - new_data_size; i < MATRIX_WIDTH; i++){
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

            _cnt = 0;
            //Push in new data to the columns to overwrite
            //If inputting 1 collumn, start at index 7 and write only that
            //If inputting 4 collumns, start at index 4 (col5) and write until index 0 (col1)
            for( byte i = _width - new_data_size; i >= 0; i--){
                _cnt = new_data_size - 1;
                array_ptr[i] = new_data[_cnt];
                _cnt--;
            }  
            break;

        case DOWN:
            //Compile a byte of data for each collumn to enter
            for( byte i = 0; i < MATRIX_WIDTH; i++){
                byte get_bit = (new_data[0] >> ((MATRIX_WIDTH - i) - 1)) & 0b000000001;
                    if( get_bit == 1){
                        array_ptr[i] <<= new_data_size; //Shift the collumn data down and out by new data size
                        array_ptr[i] |= get_bit; //And the new bit to the collumn
                    }
                    else if(get_bit == 0){
                        array_ptr[i] <<= new_data_size; //Shift the collumn data down and out by new data size, the new input is already 0
                    }
            }

    }
}


void Matrix::update(){
    //Create an array of bitwise comparator values
    //byte comparators[8] = {1, 2, 4, 8, 16, 32, 64, 128};
    byte temp_val;
    //Serial.println("Updating Matrix");
    

    //Scan through each collumn from left to right and do the yellow LEDs
    set_pins(row_pins, MATRIX_HEIGHT, OUTPUT, LOW); //Change the rows to outputs and set LOW
    for( byte i = 0; i < _width; i++ ){
        if( col_active == true){
            Serial.println("Another collumn already active");
            break;
        }

        col_active = true;


        //Light the Yellow LEDs
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

        col_active = false;
    }

    set_pins(row_pins, MATRIX_HEIGHT, INPUT);

    //Scan through each collumn from left to right and do the Blue LEDs
    for( byte i = 0; i < _width; i++ ){
        if( col_active == true){
            Serial.println("Another collumn already active");
            break;
        }

        col_active = true;


        //Light the BlueLEDs
        //set_pins(col_pins, MATRIX_WIDTH, OUTPUT, LOW); //Change the rows to outputs and set LOW

        if( blue_col_data[i] > 0){ //Check if there's even data in the collumn
            pinMode(col_pins[i], OUTPUT); //Change the collumn to an output and set HIGH to source current
            digitalWrite(col_pins[i], HIGH);

            temp_val = blue_col_data[i];
            for( byte j = 0; j < _height; j++){  //Cycle through the rows starting at the bottom which is Row 0
                if( ((temp_val >> j) & 0b00000001) == true){
                    pinMode(row_pins[j], OUTPUT);
                    digitalWrite(row_pins[j], LOW);    //Briefly turn the light on 
                    delayMicroseconds(_light_on_time);
                    pinMode(row_pins[j], INPUT);
                }
            }
            pinMode(col_pins[i],INPUT);
        }

        col_active = false;
        
    }
}

void Matrix::slide_row(byte led_color, byte direction, byte row){
    //Slide data within a row a one space either left or right
    byte* array_ptr;
    byte temp_val;

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
            for( byte i = 1; i < MATRIX_WIDTH; i++){ //Don't need to start at 0 since collumn 0 will be lost in a left shift
                temp_val = 0b00000001 & (array_ptr[i] >> row);
                if( temp_val == 1){ //Bit is a 1
                    array_ptr[i-1] |= masks_on[row];
                }
                else{
                    array_ptr[i-1] &= masks_off[row];
                }
            }
            array_ptr[MATRIX_WIDTH-1] &= masks_off[row];
            break;

        case RIGHT:
            for( byte i = MATRIX_WIDTH - 2; i >= 0 && i != 255; i--){ //Don't need to start at 0 since collumn 0 will be lost in a left shift
                //Had to add an exit case for != 255 because on the last loop it does 0 - 1 which = 255, so this runs it doewn to zero and then stops the wraparound.
                /* Serial.print("Column ");
                Serial.print(i);
                Serial.print(": ");
                Serial.println(array_ptr[i], BIN); */
                temp_val = 0b00000001 & (array_ptr[i] >> row);
                if( temp_val == 1 ){ //Bit is a 1
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


bool Matrix::banner_text(byte led_color, byte* text_string, bool loop = false){

    static byte* prev_text_string = nullptr;

    if( text_string != prev_text_string){ //Reset the banner text if the text passed to the function has changed pointer address
        reset();
        last_update_time = 0;
        char_count = 0;
        char_byte_count = 0;
        prev_text_string = text_string;
    }

    if( millis() - last_update_time >= banner_pan_speed){
        
        if(char_byte_count == get_char_size(text_string[char_count])){
            char_count++; //move to the next character in the string if reached the end of the current char bytes
            char_byte_count = 0;
        }

         if(text_string[char_count] == '\0'){ //null character indicating the end of the text string
            if( loop == false){
                return false; //Return false when done looping
            }
            char_count = 0;
            char_byte_count = 0; //Reset banner_count if at the end of text to loop banner again
        }


        move_data(led_color, LEFT, &get_char_bytes(text_string[char_count])[char_byte_count], 1);
        last_update_time = millis();
        char_byte_count++; //Track which byte we are in in the current character in the text string
    }
    return true;
}

bool Matrix::val_at_cell(byte led_color, byte col, byte row){
    //Function to query the value of the Yellow or Blue array at a certain point and return the value
    switch(led_color){
    case BLUE_LED:
        if( (blue_col_data[col] & masks_on[row]) > 0 ){
            return true;
        }
        break;
    case YELLOW_LED:
        if( (yellow_col_data[col] & masks_on[row]) > 0 ){
            return true;
        }
        break;
    }
    return false;
}

void Matrix::modify_cell(byte led_color, byte col, byte row, bool val){
    /*Function to modify the value of a specific cell in the light matrix*/    
    switch(led_color){
        case BLUE_LED:
            if( val == true){ //set position to on
                blue_col_data[col] = blue_col_data[col] | masks_on[row];
            }
            else{ //set the position to off
                blue_col_data[col] = blue_col_data[col] & masks_off[row];
            }
            break;

        case YELLOW_LED:
            if( val == true){ //set position to on
                yellow_col_data[col] = yellow_col_data[col] | masks_on[row];
            }
            else{ //set the position to off
                yellow_col_data[col] = yellow_col_data[col] & masks_off[row];
            }
            break;

    }
}

byte* Matrix::get_char_bytes(char val){
    switch (val){
        case ' ':
            return blank;
        case 'A':
            return A;
        case 'B':
            return B;
        case 'C':
            return C;
        case 'D':
            return D;
        case 'E':
            return E;
        case 'F':
            return F;
        case 'G':
            return G;
        case 'H':
            return H;
        case 'I':
            return I;
        case 'J':
            return J;
        case 'K':
            return K;
        case 'L':
            return L;
        case 'M':
            return M;
        case 'N':
            return N;
        case 'O':
            return O;
        case 'P':
            return P;
        case 'Q':
            return Q;
        case 'R':
            return R;
        case 'S':
            return S;
        case 'T':
            return T;
        case 'U':
            return U;
        case 'V':
            return V;
        case 'W':
            return W;
        case 'X':
            return X;
        case 'Y':
            return Y;
        case 'Z':
            return Z;
        case '1':
            return ONE;
        case '2':
            return TWO;
        case '3':
            return THREE;
        case '4':
            return FOUR;
        case '5':
            return FIVE;
        case '6':
            return SIX;
        case '7':
            return SEVEN;
        case '8':
            return EIGHT;
        case '9':
            return NINE;
        case '0':
            return ZERO;
        case '!':
            return EXCLAMATION;
        case '?':
            return QUESTION;
        case '.':
            return DOT;
        
    }
}

byte Matrix::get_char_size(char val){
    switch(val){
        case ' ':
            return sizeof(blank);
        case 'A':
            return sizeof(A);
        case 'B':
            return sizeof(B);
        case 'C':
            return sizeof(C);
        case 'D':
            return sizeof(D);
        case 'E':
            return sizeof(E);
        case 'F':
            return sizeof(F);
        case 'G':
            return sizeof(G);
        case 'H':
            return sizeof(H);
        case 'I':
            return sizeof(I);
        case 'J':
            return sizeof(J);
        case 'K':
            return sizeof(K);
        case 'L':
            return sizeof(L);
        case 'M':
            return sizeof(M);
        case 'N':
            return sizeof(N);
        case 'O':
            return sizeof(O);
        case 'P':
            return sizeof(P);
        case 'Q':
            return sizeof(Q);
        case 'R':
            return sizeof(R);
        case 'S':
            return sizeof(S);
        case 'T':
            return sizeof(T);
        case 'U':
            return sizeof(U);
        case 'V':
            return sizeof(V);
        case 'W':
            return sizeof(W);
        case 'X':
            return sizeof(X);
        case 'Y':
            return sizeof(Y);
        case 'Z':
            return sizeof(Z);
        case '1':
            return sizeof(ONE);
        case '2':
            return sizeof(TWO);
        case '3':
            return sizeof(THREE);
        case '4':
            return sizeof(FOUR);
        case '5':
            return sizeof(FIVE);
        case '6':
            return sizeof(SIX);
        case '7':
            return sizeof(SEVEN);
        case '8':
            return sizeof(EIGHT);
        case '9':
            return sizeof(NINE);
        case '0':
            return sizeof(ZERO);
        case '!':
            return sizeof(EXCLAMATION);
        case '?':
            return sizeof(QUESTION);
        case '.':
            return sizeof(DOT);
    }
}

