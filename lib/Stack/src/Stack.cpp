#include <Stack.h>
#include <Constants.h>
#include <Arduino.h>
#include <Matrix.h>
#include <Controls.h>

Stack::Stack(){
}

void Stack::run(Matrix* matrix, Controls* controls){
    reset();
    byte temp_val;
    matrix->reset(); //Clear the screen
    byte first_row_binary = 255 << (MATRIX_WIDTH - block_width[current_height]);
    matrix->push_row_data(YELLOW_LED, current_height, first_row_binary); //Set the first row of blocks

    while( true || lose == false){
        if( current_height == 255 ){ //255 occurs after current_height = 0 is subtracted -- onece more
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
            Serial.println(current_height);
            if(current_height < 7){ //Don't check for overhanging blocks on the first row
                byte current_right_position = left_block_position[current_height] + block_width[current_height] - 1;
                byte prev_right_position = left_block_position[current_height + 1] + block_width[current_height + 1] - 1;
                
                //In this case, the block is completely in
                if(left_block_position[current_height] >= left_block_position[current_height + 1] && 
                current_right_position <= prev_right_position){
                    Serial.println("Completely In");
                    }

                //In this case there is only overhang on the left
                else if( left_block_position[current_height] < left_block_position[current_height + 1] &&
                    current_right_position <= prev_right_position){
                        Serial.println("Overhang to the Left");
                        temp_val = current_right_position - left_block_position[current_height + 1] + 1; //Number of blocks on top of eachother
                        adjust_block_width(current_height, temp_val);
                        left_block_position[current_height] = left_block_position[current_height + 1]; //Reassign the left position based on the lower row
                        byte replace_block = ( 255 << (MATRIX_WIDTH - temp_val));
                        replace_block >>= left_block_position[current_height];
                        matrix->push_row_data(YELLOW_LED, current_height, replace_block);
                        if( temp_val == 0){
                            lose = true;
                            break;
                        }
                    }
                //In this case, there is only overhand on the right
                else if(current_right_position > prev_right_position &&
                    left_block_position[current_height] >= left_block_position[current_height + 1]){
                        Serial.println("Overhang to the Right");
                        temp_val = prev_right_position - left_block_position[current_height] + 1;
                        Serial.print("Blocks on top: ");
                        Serial.println(temp_val);
                        adjust_block_width(current_height, temp_val);
                        left_block_position[current_height] = prev_right_position - (temp_val - 1); //Reassign the left position based on the lower row right block
                        Serial.print("Left postion: ");
                        Serial.println(left_block_position[current_height]);
                        byte replace_block = ( 255 << (MATRIX_WIDTH - temp_val));
                        replace_block >>= left_block_position[current_height];
                        matrix->push_row_data(YELLOW_LED, current_height, replace_block);
                        if( temp_val == 0){
                            lose = true;
                            break;
                        }
                    }
                
                //In this case, the block must be completely outside the lower block so lose and exit
                else{
                    Serial.println("Completely out");
                    lose = true;
                    break;
                    }
            }
            if(lose == false){
                current_direction = RIGHT;
                current_height--;
                Serial.println(current_height);
                byte row_data_binary = 255 << (MATRIX_WIDTH - block_width[current_height]);
                matrix->push_row_data(YELLOW_LED, current_height, row_data_binary); //Push the next row of blocks to the grid, does not check if blocks were overhanging.
            }
        }

        matrix->update();
    }

    //Hold the last screen for a little to see the end move before the result text is displayed
    unsigned long game_end_time = millis();
    while(millis() - game_end_time <= 500){
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

void Stack::reset(){
    current_height = MATRIX_HEIGHT-1;
    for( byte i = 0; i < MATRIX_WIDTH; i++){
        left_block_position[i] = 0;
        block_width[i] = default_block_width[i]; //Reassign the block width array based on the default widths
    }
    current_direction = RIGHT;
    last_move_time = 0;
    lose = false;
}

void Stack::adjust_block_width(byte start_row, byte new_width){
    for(byte i = start_row; i >= 0 && i != 255; i--){
        if( block_width[i] > new_width){ //Make the block width smaller if the default size is smaller
            block_width[i] = new_width;
        }
    }
}