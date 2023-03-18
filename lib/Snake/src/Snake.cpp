#include <Snake.h>
#include <Constants.h>
#include <Arduino.h>
#include <Matrix.h>
#include <Controls.h>

Snake::Snake(){}

void Snake::run(Matrix* matrix, Controls* controls){
    reset();
    
    //place the first snake piece
    matrix->reset(); //Clear the screen
    matrix->modify_cell(YELLOW_LED, snake_head_col, snake_head_row, 1);

    snake_pieces_col[0] = snake_head_col;
    snake_pieces_row[0] = snake_head_row;
    
    //Place the fruit
    generate_fruit(matrix);
  
    while( lose == false){

        if( millis() - last_move_time >= snake_speed){
            move_snake(matrix);
            last_move_time = millis();
        }

        byte button_val = controls->direction_state_change();
        if( button_val == UP || button_val == DOWN || button_val == LEFT || button_val == RIGHT){
            snake_direction = button_val; //Assign a new direction if pressed
        }

        matrix->update();
    }

    if(lose == true){
        matrix->reset();
        while(matrix->banner_text(YELLOW_LED,"YOU LOSE! ")){
            matrix->update();
        }
    }
    else{
        matrix->reset();
        while(matrix->banner_text(BLUE_LED,"YOU WIN! ")){
            matrix->update();
        }
    }
}

bool Snake::move_snake(Matrix* matrix){

    switch (snake_direction)
    {
        //Check if the snake contacts a border and lose if so, otherwise adjust current row or col
    case LEFT:
        if( snake_head_col == 0){
            lose = true;
            return false;
        }

        snake_head_col--;
        break;
    
    case RIGHT:
        if( snake_head_col == 7){
            lose = true;
            return false;
        }

        snake_head_col++;
        break;

    case UP:
        if( snake_head_row == 0){
            lose = true;
            return false;
        }

        snake_head_row--;
        break;

    case DOWN:
        if( snake_head_row == 7){
            lose = true;
            return false;
        }

        snake_head_row++;
        break;
    }

    //Check if snake hits itself
    if(matrix->val_at_cell(YELLOW_LED, snake_head_col, snake_head_row) == 1){
        lose = true;
        return false;
    }

    //Check if snake hits fruit
    if(matrix->val_at_cell(BLUE_LED, snake_head_col, snake_head_row) == 1){
        hit_fruit = true;
        matrix->modify_cell(BLUE_LED, fruit_col, fruit_row, 0);
    }

    //Move Snake head
    matrix->modify_cell(YELLOW_LED,snake_head_col,snake_head_row,1);
    byte tail_row = snake_pieces_row[snake_length-1];
    byte tail_col = snake_pieces_col[snake_length-1];

    Serial.print(tail_row);
    Serial.print(",");
    Serial.println(tail_col);

    for(byte i = 1; i < snake_length; i++){
        snake_pieces_col[i] = snake_pieces_col[i-1];
        snake_pieces_row[i] = snake_pieces_row[i-1];
    }
    snake_pieces_col[0] = snake_head_col;
    snake_pieces_row[0] = snake_head_row;

    //Remove the tail piece or leave in place if fruit was hit
    if(hit_fruit == false){
        matrix->modify_cell(YELLOW_LED, tail_col, tail_row, 0);
        Serial.println("Did not hit fruit");
    }
    else{
        snake_length++;
        snake_pieces_row[snake_length-1] = tail_row;
        snake_pieces_col[snake_length-1] = tail_col;
        generate_fruit(matrix);
        hit_fruit = false;
    }
    
    
}

void Snake::generate_fruit(Matrix* matrix){
    while(true){
        fruit_col = random(0,7);
        fruit_row = random(0,7);
        //Check that the fruit is generated at a location that the snake is not
        //If so, break, if not repeat until it is
        if(matrix->val_at_cell(YELLOW_LED, fruit_col, fruit_row) == 0){
            break;
        }
    }

    matrix->modify_cell(BLUE_LED, fruit_col, fruit_row, 1);
    
}

void Snake::reset(){
        snake_head_row = 3;
        snake_head_col = 4;

        for(byte i = 0; i < sizeof(snake_pieces_row); i++){
            snake_pieces_col[i] = 0;
            snake_pieces_row[i] = 0;
        }

        snake_length = 0;
        lose = false;
        last_move_time = 0;
        snake_direction = LEFT;
}
