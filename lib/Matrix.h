#ifndef MATRIX_H
#define MATRIX_H

#define YELLOW_LED 1
#define BLUE_LED 2
#define UP 3
#define DOWN 4
#define LEFT 5
#define RIGHT 6

class Matrix{
    public:
        Matrix(int height, int width);
        void update();
        void push_col_data(char led_color ,char column_number, char data);
        void move_data(char led_color, char direction, char* new_data, char new_data_size);

    private:
        int _height, _width; 
        bool col_active = false;
        char row_count = 0;
        char yellow_col_data[8];
        char blue_col_data[8];

        int _light_on_time = 100; //microseconds

        void set_pins( char* pin_array, char size_of_array, char val); //Set the col or row pins all High or Low

        char row_pins[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        char colPins[8] = {9, 10, 11, 12, 13, 14, 15, 16};


};



#endif