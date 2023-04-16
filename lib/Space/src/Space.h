#ifndef SPACE_H
#define SPACE_H

#include <Arduino.h>
#include <Constants.h>
#include <Controls.h>
#include <Matrix.h>

class Space{
    public:
        Space();
        void run(Matrix* matrix, Controls* controls);

    private:
        void reset();
        const byte player_start_position = 0b00001000;
        const unsigned int default_start_speed = 300;
        const byte slow_down_speed = 500;
        byte player_postion = 4;
        int cycle_count = 0;
        const byte default_energy = 8;
        byte energy = default_energy;
        unsigned long last_move_time;
        bool lose = false;
        unsigned int current_speed = default_start_speed;
        const byte fastest_speed = 50;
        byte freeze_cycles = 0;
};


#endif