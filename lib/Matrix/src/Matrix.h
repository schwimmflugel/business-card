#ifndef MATRIX_H
#define MATRIX_H

#include <Arduino.h>
#include <Constants.h>


class Matrix{
    public:
        Matrix(byte height, byte width);
        void update();
        void push_col_data(byte led_color ,byte column_number, byte data);
        void push_row_data(byte led_color, byte row_number, byte data);
        void move_data(byte led_color, byte direction, byte* new_data, byte new_data_size);
        void slide_row(byte led_color, byte direction, byte row);
        void reset(byte led_color = 0);
        bool banner_text(byte led_color, byte* text, byte length_of_array, bool loop = false);

    private:
        int _height, _width; 
        bool col_active = false;
        byte row_count = 0;
        byte yellow_col_data[8];
        byte blue_col_data[8];

        int banner_pan_speed = 200;
        unsigned long last_update_time = 0;
        byte banner_count = 0;
        int* prev_text;
        

        int _light_on_time = 100; //microseconds

        void set_pins( const byte* pin_array, byte size_of_array, byte pin_mode, byte val = LOW); //Set the col or row pins all High or Low

        const byte row_pins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
        const byte col_pins[8] = {A3, A2, A1, A0, 13, 12, 11, 10};
        const byte masks_off[8] = {0b11111110, 0b11111101, 0b11111011, 0b11110111, 0b11101111, 0b11011111, 0b10111111, 0b01111111};
        const byte masks_on[8] = {1,2,4,8,16,32,64,128};


};



#endif