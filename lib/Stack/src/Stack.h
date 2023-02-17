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
        const byte block_width[8] = {1,1,2,2,3,3,4,4};
        const int block_speed[8] = {100,100,250,250,350,450,550,750};
        byte current_height = MATRIX_HEIGHT-1;
        byte left_block_position[8];
        byte current_direction = RIGHT;
        unsigned long last_move_time;
};

#endif