#ifndef SNAKE_H
#define SNAKE_H

#include <Arduino.h>
#include <Constants.h>
#include <Controls.h>
#include <Matrix.h>

class Snake{
    public:
        Snake();
        void run(Matrix* matrix, Controls* controls);

    private:
        void reset();
        bool move_snake(Matrix* matrix);
        void generate_fruit(Matrix* matrix);
        byte snake_head_row;
        byte snake_head_col;
        byte snake_pieces_row[64];
        byte snake_pieces_col[64];
        byte snake_length = 1;

        byte snake_direction = LEFT;
        const unsigned int snake_speed = 300;
        unsigned long last_move_time = 0;
        bool lose = false;
        bool hit_fruit = false;
        byte fruit_row;
        byte fruit_col;


};

#endif