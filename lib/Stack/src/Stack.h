#ifndef STACK_H
#define STACK_H

#include <Arduino.h>
#include <Constants.h>
#include <Controls.h>
#include <Matrix.h>



class Stack{
    public:
        Stack();
        void run(Matrix* matrix, Controls* controls);

    private:
        void reset();
        void adjust_block_width(byte start_row, byte new_width);
        const byte default_block_width[8] = {1,1,2,2,3,3,4,4};
        byte block_width[8];
        const int block_speed[8] = {75,100,200,250,300,400,550,750};
        byte adjusted_block_width = block_width[7];
        byte current_height = MATRIX_HEIGHT-1;
        byte left_block_position[8];
        byte current_direction = RIGHT;
        unsigned long last_move_time;
        bool lose = false;
};

#endif