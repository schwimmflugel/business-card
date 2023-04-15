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
    byte energy_disp[1];
    int step_count = 0;
    matrix->reset(); //Clear the screen
    matrix->push_row_data(BLUE_LED, 7, player_start_position);
    energy_disp[0] = (255 >> (8 - energy));
    matrix->push_col_data(BLUE_LED, 0, energy_disp);
  
    while( true || lose == false){
        if( matrix->val_at_cell(YELLOW_LED, player_postion, 7) == true ){ //Check if there is an asteroid in the player posititon
            lose = true;
            break;
        }

        unsigned int speed;

        //Use the slow down
        if( controls->read_select() == A_BUTTON){
            speed = slow_down_speed;
        }
        else{
            speed = current_speed;
        }


        if( millis() - last_move_time >= speed){
            byte random_num = random(0,7);
            byte temp_val = 1 << random_num; //Create a new asteroid
            matrix->move_data(YELLOW_LED, DOWN, &temp_val, 1);
            step_count++;

            if(step_count == 10 ){
                step_count = 0;
                if( energy < default_energy){
                    energy++;
                    energy_disp[0] = 255 << (8 - energy);
                    matrix->push_col_data(BLUE_LED, 0, energy_disp);
                }
            }

            last_move_time = millis();
            if( current_speed > fastest_speed){
                current_speed--;
            }
        }

        //Use the cannon
        if( controls->select_state_change() == B_BUTTON && energy > 0){
            for(int i = 6; i >= 0; i--){
                if(matrix->val_at_cell(YELLOW_LED, player_postion, i) == 1){
                    matrix->modify_cell(YELLOW_LED, player_postion, i, 0);
                    energy--;
                    energy_disp[0] = 255 << (8 - energy);
                    matrix->push_col_data(BLUE_LED, 0, energy_disp);
                    step_count - 3;
                    break;
                } 
            }
        }

        byte button_val = controls->direction_state_change();
        if( button_val == LEFT && player_postion > 1){
            matrix->modify_cell(BLUE_LED,player_postion, 7, 0);
            player_postion--;
            matrix->modify_cell(BLUE_LED,player_postion, 7, 1);
        }

        else if (button_val == RIGHT && player_postion < 7){
            matrix->modify_cell(BLUE_LED,player_postion, 7, 0);
            player_postion++;
            matrix->modify_cell(BLUE_LED,player_postion, 7, 1);
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
    energy = default_energy;
}