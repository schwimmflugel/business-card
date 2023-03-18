#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Constants.h>
#include <Matrix.h>
#include <Controls.h>
#include <Stack.h>
#include <Space.h>
#include <Snake.h>



class Menu{
    public:
        Menu();
        void run( Matrix* matrix, Controls* controls);
        void banner_text(Matrix* matrix, byte* text, byte length_of_array);

    private:
        byte current_page = 0;
        int pan_speed = 200;
        unsigned long last_update_time = 0;
        byte banner_count = 0;
        int* prev_text;
        byte active_screen = 0;
        //Controls controls;
        Stack stack;
        Space space;
        Snake snake;



};


#endif

