#include <Stack.h>
#include <Constants.h>
#include <Arduino.h>
#include <Matrix.h>
#include <Controls.h>

Stack::Stack(){
}

void Stack::run(Matrix* matrix, Controls* controls){
    reset();
    matrix->reset(YELLOW_LED); //Clear the screen
    byte first_row_binary = 255 << (MATRIX_WIDTH - block_width[current_height]);
    matrix->push_row_data(YELLOW_LED, current_height, first_row_binary); //Set the first row of blocks

    while( true ){
        if( current_height == 0 ){
            break;
        }
        if( millis() - last_move_time >= block_speed[current_height]){
            switch(current_direction){
                case RIGHT:
                    if(left_block_position[current_height] == MATRIX_WIDTH - block_width[current_height] - 1){//Check if the row of blocks is against the wall, -1 since it will move once more before switching direction
                        current_direction = LEFT;
                    }
                    matrix->slide_row(YELLOW_LED, RIGHT, current_height);
                    left_block_position[current_height]++;
                    last_move_time = millis();
                    break;

                case LEFT:
                    if(left_block_position[current_height] == 1 ){//Check if the row of blocks is against the wall, -1 since it will move once more before switching direction
                        current_direction = RIGHT;
                    }
                    matrix->slide_row(YELLOW_LED, LEFT, current_height);
                    left_block_position[current_height]--;
                    last_move_time = millis();
                    break;
            }
        }

        byte button_val = controls->select_state_change();

        if( button_val == A_BUTTON ){
            current_direction = RIGHT;
            current_height--;
            byte row_data_binary = 255 << (MATRIX_WIDTH - block_width[current_height]);
            matrix->push_row_data(YELLOW_LED, current_height, row_data_binary); //Push the next row of blocks to the grid, does not check if blocks were overhanging.

        }

        matrix->update();
    }

}

void Stack::reset(){
    current_height = MATRIX_HEIGHT-1;
    for( byte i = 0; i < MATRIX_WIDTH; i++){
        left_block_position[i] = 0;
    }
    current_direction = RIGHT;
    last_move_time = 0;
}