import tkinter as tk
import random

# Define the size of the squares
square_size = 10

class MazeApp:
    def __init__(self, master, num_squares):
        self.master = master
        self.num_squares = num_squares
        self.grid = [[1 for x in range(self.num_squares)] for y in range(self.num_squares)]
        self.canvas = tk.Canvas(self.master, width=self.num_squares*square_size, height=self.num_squares*square_size)
        self.canvas.pack()
        #self.draw_maze()

        self.stack = [(1,0)] #[(self.num_squares//2,0)]
        current = self.stack[-1]
        x, y = current
        self.grid[y][x] = 0 #Populate the first center square
        count = 0
        largest_count = 0
        largest_x = 0
        largest_y = 0
  

        while self.stack:
            #print(stack)
            current = self.stack[-1] #Pull the most recent addition in the stack (last index)
            x, y = current
            neighbors = [(x+1, y), (x-1, y), (x, y+1), (x, y-1)]
            #print(neighbors)
            random.shuffle(neighbors)
            for next_x, next_y in neighbors:
                if next_x < 1 or next_x > self.num_squares - 2 or next_y < 1 or next_y > self.num_squares - 2:
                    pass #Don't do anything if conflicting with an outer wall
                elif next_x > self.num_squares - 1 or next_y > self.num_squares -1:
                    pass
                elif self.valid_space(next_x, next_y) and self.grid[next_y][next_x] == 1:
                    self.grid[next_y][next_x] = 0
                    self.stack.append((next_x, next_y))
                    count += 1
                    if count > largest_count:
                        largest_count = count
                        print(largest_count)
                        largest_x = next_x
                        largest_y = next_y
                    #print(self.stack)
                    break
            else:
                #print("Popping value...")
                x,y = self.stack.pop() #If the for loop exits without any successful neighbors, remove the last index from the grid
                count -= 1

        self.grid[largest_y][largest_x] = 2

        
        # Draw the maze on the canvas
        for y in range(self.num_squares):
            for x in range(self.num_squares):
                if self.grid[y][x] == 1:
                    self.canvas.create_rectangle(x*square_size, y*square_size, (x+1)*square_size, (y+1)*square_size, fill="black")
                elif self.grid[y][x] == 2:
                    self.canvas.create_rectangle(x*square_size, y*square_size, (x+1)*square_size, (y+1)*square_size, fill="blue")

    def valid_space(self, x, y):
        neighbors = [(x+1, y), (x-1, y), (x, y+1), (x, y-1)]
        previous = self.stack[-1]
        prev_x, prev_y = previous

        if x == 0 or x == self.num_squares -1 or y == 0 or y == self.num_squares -1: #Not a valid square if it is wall space
            return False  

        for neighbor_x, neighbor_y in neighbors: #Check if any neighbors are already taken, if so return False since the space can't be used
            if neighbor_x == prev_x and neighbor_y == prev_y: #Ignore the neighbor call that is the previously taken spot
                continue
            if neighbor_x == 0 or neighbor_x == self.num_squares -1 or neighbor_y == 0 or neighbor_y == self.num_squares - 1: #Ignore any neightbors that are walls
                continue

            if self.grid[neighbor_y][neighbor_x] == 0: #If a neighbor to a cell is already filled, then it is not valid
                #print("X: %d and Y: %d invalid"%(x, y))
                return False
        else:
            return True #If all neighbors have been evaluated and it did not return False, then return True as a valid space
    
root = tk.Tk()
app = MazeApp(root, 16)

root.mainloop()



