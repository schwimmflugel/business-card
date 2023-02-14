#ifndef CONTROLS_H
#define CONTROLS_H

#include <Arduino.h>
#include <Constants.h>


class Controls{
    public:
        Controls();
        Controls(byte direction_pin, byte select_pin);
        byte direction_state_change();
        byte select_state_change();


    private:
        byte read_direction();
        byte read_select();

        byte current_direction = 0;
        byte current_select = 0;
        byte _direction_pin;
        byte _select_pin;
        byte _prev_direction = 0;
        byte _prev_select = 0;


};



#endif