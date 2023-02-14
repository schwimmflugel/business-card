#include <Menu.h>
#include <Constants.h>
#include <Arduino.h>
#include <Matrix.h>
#include <Controls.h>

Menu::Menu(){
    Controls controls = Controls(A4, A5);
}

void Menu::banner_text(Matrix* matrix, const byte* text, byte length_of_array){
    byte* text_cast = const_cast<byte*>(text);

    static byte* prev_text = nullptr;

    if( text != prev_text){ //Reset the banner text if the text passed to the function has changed pointer address
        matrix->reset(YELLOW_LED);
        last_update_time = 0;
        banner_count = 0;
        prev_text = text_cast;
    }
    if( millis() - last_update_time >= pan_speed){
        if( banner_count == length_of_array - 1){
            banner_count = 0; //Reset banner_count if at the end of text to loop banner again
        }

        matrix->move_data(YELLOW_LED, LEFT, text_cast + banner_count, 1);
        last_update_time = millis();
        banner_count++; //Track where the banner is to start looping again if needed
    }
}

void Menu::run(Matrix* matrix){
    
    byte val = controls.direction_state_change();
    if( val == RIGHT){
        active_screen++;
    }
    else if (val == LEFT)
    {
        active_screen--;
        if(active_screen < 0){
            active_screen = 0;
        }
    }

    switch(active_screen){
        case 0:
            banner_text(matrix, HELLO_text, sizeof(HELLO_text));
            break;

        case 1:
            banner_text(matrix, STACK_text, sizeof(STACK_text));
            
    }
}