#include <Space.h>
#include <Constants.h>
#include <Arduino.h>
#include <Matrix.h>
#include <Controls.h>

Space::Space(){
}

void Space::run(Matrix* matrix, Controls* controls){
    reset();
    byte energy_disp[1];
    int step_count = 0;
    matrix->reset(); //Clear the screen
    matrix->push_row_data(BLUE_LED, 7, player_start_position);
    energy_disp[0] = (255 >> (8 - energy));
    matrix->push_col_data(BLUE_LED, 0, energy_disp);
  
    while( lose == false){
        if( matrix->val_at_cell(YELLOW_LED, player_postion, 7) == true ){ //Check if there is an asteroid in the player posititon
            lose = true;
            break;
        }

        //unsigned int speed;

        //Use the slow down
/*         if( controls->read_select() == A_BUTTON){
            speed = slow_down_speed;
        }
        else{
            speed = current_speed;
        } */
        byte select_val = controls->select_state_change();
        
        if( select_val == A_BUTTON && energy > 0 && freeze_cycles == 0){
            freeze_cycles = 5;
            energy--;
            energy_disp[0] = 255 << (8 - energy);
            matrix->push_col_data(BLUE_LED, 0, energy_disp);
        }

        //Use the cannon
        if( select_val == B_BUTTON && energy > 0){
            for(int i = 6; i >= 0; i--){
                if(matrix->val_at_cell(YELLOW_LED, player_postion, i) == 1){
                    matrix->modify_cell(YELLOW_LED, player_postion, i, 0);
                    energy--;
                    energy_disp[0] = 255 << (8 - energy);
                    matrix->push_col_data(BLUE_LED, 0, energy_disp);
                    //step_count - 3;
                    break;
                } 
            }
        }


        //Generate a new asteroid and shift others down
        if( millis() - last_move_time >= current_speed){
            if(freeze_cycles > 0){
                freeze_cycles--;
                last_move_time = millis();
            }

            else{
                byte random_num = random(0,7);
                byte temp_val = 1 << random_num; //Create a new asteroid
                matrix->move_data(YELLOW_LED, DOWN, &temp_val, 1);
                step_count++;
                cycle_count++;

                //Regain energy after 10 steps
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

    //Subtract 8 since the first 8 cycles while the first asteroid moves shouldn't count towards score
    matrix->display_score(BLUE_LED, "Score:", cycle_count-8);
}

void Space::reset(){
    player_postion = 4;
    last_move_time = 0;
    lose = false;
    current_speed = default_start_speed;
    energy = default_energy;
    cycle_count = 0;
}