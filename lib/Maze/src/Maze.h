#ifndef MAZE_H
#define MAZE_H

#include <Arduino.h>
#include <Constants.h>
#include <Controls.h>
#include <Matrix.h>

class Maze{
    public:
        Maze();
        void run(Matrix* matrix, Controls* controls);

    private:
        void reset();
        void generate_maze();
        void update_maze(Matrix* matrix);
        void move_player(Matrix* matrix, byte direction);
        void difficulty_select(Matrix* matrix, Controls* controls);
        bool valid_space(byte _row, byte _col);
        bool val_at_grid_space( byte _row, byte _col);
        void modify_grid_space( byte _row, byte _col, byte val);
        //4x4 grid of 8 bit grid = 32bits x 32 bits
        const byte max_difficulty = 4;
        byte num_grids = 1;
        byte collumn[128]; //8x4x4
        byte stack_row[128];
        byte stack_col[128];
        int largest_length;
        int stack_length;
        int largest_row, largest_col;
        int end_position_index = 255;
        int player_row, player_col;
        int prev_player_index = 255;
        
        bool end = false;

};

#endif