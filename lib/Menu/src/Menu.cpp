#include <Menu.h>
#include <Constants.h>
#include <Arduino.h>
#include <Matrix.h>
#include <Controls.h>
#include <Stack.h>

Menu::Menu(){
    //Controls controls = Controls(A4, A5);
    Stack stack();
}


void Menu::run(Matrix* matrix, Controls* controls){
    byte val;

    //Read for a left or right button to scroll menu, change screen accordingly
    val = controls->direction_state_change();
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
            //banner_text(matrix, HELLO_text, sizeof(HELLO_text));
            matrix->banner_text(BLUE_LED, HELLO_text, sizeof(HELLO_text), true);
            break;

        case 1:
            matrix->banner_text(YELLOW_LED, STACK_text, sizeof(STACK_text), true);
            break;

        case 2:
            matrix->banner_text(YELLOW_LED, MAZE_text, sizeof(MAZE_text), true);
            break;            
    }

    val = controls->select_state_change();
    if(val == A_BUTTON){
        switch(active_screen){
            case 1:              
                stack.run(matrix, controls);
                break;
            case 2:
                //maze.run(matrix);
                break;
        }

    }
}