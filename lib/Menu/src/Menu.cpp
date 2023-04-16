#include <Menu.h>
#include <Constants.h>
#include <Arduino.h>
#include <Matrix.h>
#include <Controls.h>
#include <Stack.h>
#include <Space.h>
#include <Snake.h>
#include <Maze.h>

Menu::Menu(){
    //Controls controls = Controls(A4, A5);
    Stack stack();
    Space space();
    Snake snake();
    Maze maze();
}


void Menu::run(Matrix* matrix, Controls* controls){
    byte val;

    //Read for a left or right button to scroll menu, change screen accordingly
    val = controls->direction_state_change();
    if( val == RIGHT){
        active_screen++;
        if( active_screen > 4 ){
            active_screen = 4;
        }
    }
    else if (val == LEFT)
    {
        active_screen--;
        if(active_screen == 255){
            active_screen = 0;
        }
    }

    switch(active_screen){
        case 0:
            matrix->banner_text(WHITE_LED, "HELLO! ", true);
            break;

        case 1:
            matrix->banner_text(BLUE_LED, "STACKER  ", true);
            break;

        case 2:
            matrix->banner_text(BLUE_LED, "MAZED  ", true);
            break; 

        case 3:
            matrix->banner_text(BLUE_LED, "SPACE RUN  ", true);
            break;  

        case 4:
            matrix->banner_text(BLUE_LED, "SNAKEY  ", true);
            break;  
    }

    val = controls->select_state_change();
    if(val == A_BUTTON){
        switch(active_screen){
            case 1:              
                stack.run(matrix, controls);
                break;
            case 2:
                maze.run(matrix, controls);
                break;
            case 3:
                space.run(matrix, controls);
                break;
            case 4:
                snake.run(matrix, controls);
                break;
        }

    }
}