#include <Space.h>
#include <Constants.h>
#include <Arduino.h>
#include <Matrix.h>
#include <Controls.h>

Space::Space(){
}

void Space::run(Matrix* matrix, Controls* controls){
    reset();
    byte temp_val;
    matrix->reset(); //Clear the screen
    matrix->push_row_data(BLUE_LED, 7, player_start_position);
  
    while( true || lose == false){
        if( matrix->val_at_cell(YELLOW_LED, player_postion, 7) == true ){ //Check if there is an asteroid in the player posititon
            lose = true;
            break;
        }
        if( millis() - last_move_time >= current_speed){
            byte random_num = random(0,7);
            byte temp_val = 1 << random_num; //Create a new asteroid
            matrix->move_data(YELLOW_LED, DOWN, &temp_val, 1);
            last_move_time = millis();
            if( current_speed > fastest_speed){
                current_speed--;
            }
        }

        byte button_val = controls->direction_state_change();
        if( button_val == LEFT && player_postion > 0){
            matrix->slide_row(BLUE_LED, LEFT, 7);
            player_postion--;
        }
        else if (button_val == RIGHT && player_postion < 7){
             matrix->slide_row(BLUE_LED, RIGHT, 7);
             player_postion++;
        }

        matrix->update();
    }

    if(lose == true){
        matrix->reset(YELLOW_LED);
        while(matrix->banner_text(YELLOW_LED,"YOU LOSE! ")){
            matrix->update();
        }
    }
    else{
        matrix->reset(YELLOW_LED);
        while(matrix->banner_text(BLUE_LED,"YOU WIN! ")){
            matrix->update();
        }
    }
}

void Space::reset(){
    player_postion = 4;
    last_move_time = 0;
    lose = false;
    current_speed = default_start_speed;
}