#ifndef MATRIX_H
#define MATRIX_H

#include <Arduino.h>
#include "Constants.h"


class Matrix{
    public:
        Matrix(byte height, byte width);
        void update();
        void push_col_data(byte led_color ,byte column_number, byte data);
        void move_data(byte led_color, byte direction, byte* new_data, byte new_data_size);
        void reset(byte led_color);

    private:
        int _height, _width; 
        bool col_active = false;
        byte row_count = 0;
        byte yellow_col_data[8];
        byte blue_col_data[8];
        

        int _light_on_time = 100; //microseconds

        void set_pins( byte* pin_array, byte size_of_array, byte val); //Set the col or row pins all High or Low

        byte row_pins[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        byte colPins[8] = {9, 10, 11, 12, 13, 14, 15, 16};


};



#endif