#include <Maze.h>
#include <Constants.h>
#include <Arduino.h>
#include <Matrix.h>
#include <Controls.h>

Maze::Maze(){}

void Maze::run(Matrix* matrix, Controls* controls){
    reset();

    matrix->reset();

    generate_maze();
    matrix->modify_cell(BLUE_LED, (player_col % 8), (player_row % 8), 1);
    update_maze(matrix);

    while( true ){
        byte button_val = controls->direction_state_change();
        if( button_val == UP || button_val == DOWN || button_val == LEFT || button_val == RIGHT){
            move_player(matrix, button_val);
        }

        matrix->update();

        //end game if player position readhes the ed position
        if( player_col == largest_col && player_row == largest_row ){
            break;
        }
    }

    matrix->reset();


}

void Maze::generate_maze(){
    //Set all bits in the maze array to high
    for(int i = 0; i < sizeof(collumn); i++){
        collumn[i] = 255;
    }
    
    byte start_pos = random(1, (8*num_grids) - 1 );//Pick a random place to begin maze
    
    collumn[start_pos] = 0b11111110;
    stack_row[0] = 0;
    stack_col[0] = start_pos;
    player_row = 0;
    player_col = start_pos;
    stack_length++;

    while( stack_length > 0 ){
        //pull the most recent block from the stack
        byte current_row = stack_row[stack_length-1];
        byte current_col = stack_col[stack_length-1];

        byte neighbors[] = {UP, DOWN, LEFT, RIGHT};
        // Shuffle the neighbor array with isher-Yates shuffle 
        for (int i = 3; i > 0; i--) {
            byte j = random(i+1);
            byte temp = neighbors[i];
            neighbors[i] = neighbors[j];
            neighbors[j] = temp;
        }

        byte next_row, next_col;
        //cycle through the neighbors to find the first valid space
        for( byte i = 0; i < 4; i++){
            bool pass_for_loop = false;
            switch( neighbors[i] ){
                case UP:
                    if( current_row <= 1){
                        pass_for_loop = true;
                        break; //skip the UP if the row is the 2nd one from the top which can't go up
                    }
                    next_row = current_row - 1;
                    next_col = current_col;
                    break;

                case DOWN:
                    if( current_row >= ((8 * num_grids) - 2 )){
                        pass_for_loop = true;
                        break; //skip the Down if the row is the 2nd to last one which can't go down in to the wall
                    }
                    next_row = current_row + 1;
                    next_col = current_col;
                    break;

                case LEFT:
                    if( current_col <= 1){
                        pass_for_loop = true;
                        break; //skip the LEFT if the col is the 2nd from left one which can't go up
                    }
                    next_row = current_row;
                    next_col = current_col - 1;
                    break;

                case RIGHT:
                    if( current_col >= ((8 * num_grids) - 2 )){
                        pass_for_loop = true;
                        break; //skip the RIGHT if the col is the 2nd to last COL
                    }
                    next_row = current_row;
                    next_col = current_col + 1;
                    break;
            }

            if( pass_for_loop == true){
                continue;
            }

            if( valid_space(next_row, next_col) == true ){
                modify_grid_space(next_row, next_col, 0);
                stack_length++;
                stack_col[stack_length-1] = next_col;
                stack_row[stack_length-1] = next_row;

                if(stack_length > largest_length){
                    largest_length = stack_length;
                    largest_row = next_row;
                    largest_col = next_col;
                }
                break;
            }
           
            //Remove the last index in the stack if last loop iteration and no space valid
            if( i == 3){
                //stack_col[stack_length-1] = 0;
                //stack_row[stack_length-1] = 0;
                stack_length--;
                break;
            }
        }
    }

    end_position_index = (largest_col / 8) + (4 * (largest_row / 8));
    
}



bool Maze::valid_space(byte _row, byte _col){
    /*Function to check if the next space to be taken is a valid space by checking the neighbors of that space*/
    byte neighbors[] = {UP, DOWN, LEFT, RIGHT};
    // Shuffle the neighbor array with Fisher-Yates shuffle 

    byte prev_row = stack_row[stack_length - 1];
    byte prev_col = stack_col[stack_length - 1];

    //return false if the grid space is already taken
    if( val_at_grid_space(_row, _col) == 0){
        return false;
    }

    //Return false for any wall spaces
    if( _row == 0 || _row == ((8*num_grids)-1) || _col == 0 || _col == ((8*num_grids)-1)){
        return false;
    }

    byte neighbor_row, neighbor_col;
    //cycle through the neighbors
    for( byte i = 0; i < 4; i++){
        bool pass_for_loop = false;
        switch( neighbors[i] ){
            case UP:
                if( _row == 1){
                    pass_for_loop = true;
                    break; //skip the UP if the row is the 2nd one from the top which can't go up
                }
                neighbor_row = _row - 1;
                neighbor_col = _col;
                break;
 
            case DOWN:
                if( _row == ((8 * num_grids) - 2 )){
                    pass_for_loop = true;
                    break; //skip the Down if the row is the 2nd to last one which can't go down in to the wall
                }
                neighbor_row = _row + 1;
                neighbor_col = _col;
                break;

            case LEFT:
                if( _col == 1){
                    pass_for_loop = true;
                    break; //skip the LEFT if the col is the 2nd from left one which can't go up
                }
                neighbor_row = _row;
                neighbor_col = _col - 1;
                break;

            case RIGHT:
                if( _col == ((8 * num_grids) - 2 )){
                    pass_for_loop = true;
                    break; //skip the RIGHT if the col is the 2nd to last COL
                }                
                neighbor_row = _row;
                neighbor_col = _col + 1;
                break;
        }

        if( pass_for_loop == true){
            continue;
        }

        //Ignore the neighbor call that is the previously taken spot
        if( neighbor_row == prev_row && neighbor_col == prev_col){
            continue;
        }

        /*Ignore walls but this should be done in the switch statement
        if neighbor_x == 0 or neighbor_x == self.num_squares -1 or neighbor_y == 0 or neighbor_y == self.num_squares - 1: #Ignore any neightbors that are walls
            continue
        */

        //If a neighbor to a cell is already filled, then it is not valid
        if( val_at_grid_space(neighbor_row, neighbor_col) == 0){
            return false;
        }
    }

    return true; //If all neighbors have been evaluated and it did not return False, then return True as a valid space
}


/*

0 1 2 3 4 5 6 7 8 9 10 11 ...
32 33 34 36 37 38 39 40 ....
64 65 66 67 68 69 70 ...


[1
1
1
1
1
1
1
1
1]

Row 7 is in the 1st byte index

*/
bool Maze::val_at_grid_space(byte _row, byte _col){
    int index = ((_row / 8 ) * 32) + _col;
    return ( (collumn[index] >> (_row % 8)) & 0b00000001 ); //Return the bit at the specified row in the byte
}

//Set value in grid to on or off
void Maze::modify_grid_space( byte _row, byte _col, byte val){
    int index = ((_row / 8 ) * 32) + _col;
    if( val == 1){
        collumn[index] |= masks_on[ _row % 8];
    }
    else{
        collumn[index] &= masks_off[ _row % 8];
    }
}

void Maze::update_maze(Matrix* matrix){
    //grab the maze grid section with the player in it
    //which 4x4 grid space is the player in
    /*
    0 1 2 3
    4 5 6 7
    8 9 ...
    */
    byte index = (player_col / 8) + (4 * (player_row / 8));
    if(index == prev_player_index){
        return; //Skip pushing new data if the player is still in the same quadrant
    }

    for( byte i = 0; i < 8; i++){
        matrix->push_col_data(YELLOW_LED,i,collumn[(index*8)+i]);
    }

    //display the end space if in the same index as player
    if( end_position_index == index){
        matrix->modify_cell(BLUE_LED, (largest_col % 8), (largest_row % 8), 1);

    }



    //matrix->modify_cell(BLUE_LED, (player_col % 8), (player_row % 8), 1);
}

void Maze::move_player(Matrix* matrix,byte direction){
    switch( direction){
        case UP:
            if(player_row == 0){
                return;
            }
            if(val_at_grid_space(player_row-1, player_col) == 1){
                return;
            }
            break;

        case DOWN:
            if(player_row == ((8 * num_grids) - 1)){
                return;
            }
            if(val_at_grid_space(player_row + 1, player_col) == 1){
                return;
            }
            break;

        case LEFT:
            if(player_col == 0){
                return;
            }
            if(val_at_grid_space(player_row, player_col - 1) == 1){
                return;
            }
            break;

        case RIGHT:
            if(player_row == ((8 * num_grids) - 1)){
                return;
            }
            if(val_at_grid_space(player_row, player_col + 1) == 1){
                return;
            }
            break;
    }

    //matrix->modify_cell(BLUE_LED, (player_col % 8), (player_row % 8), 0);

    switch (direction){
    case UP:
        player_row--;
        break;
    case DOWN:
        player_row++;
        break;
    case LEFT:
        player_col--;
        break;
    case RIGHT:
        player_col++;
        break;

    }
    matrix->reset(BLUE_LED); //Remove old player position and end spot

    matrix->modify_cell(BLUE_LED, (player_col % 8), (player_row % 8), 1);
    update_maze(matrix); //Update the maze for the new player position

}

void Maze::reset(){
        for(int i = 0; i < 128; i++){
            stack_row[i] = 0;
            stack_col[i] = 0;
        }
        largest_length = 0;
        stack_length = 0;
        
        bool end = false;
}

