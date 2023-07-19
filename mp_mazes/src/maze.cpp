/* Your code here! */
#include "maze.h"
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
using namespace cs225;
using namespace std;

SquareMaze::SquareMaze() {

}
        
bool SquareMaze::canTravel(int x, int y, int dir) const {
    //check that within bounds
    if(x >= width_ || y >= height_ || x < 0 || y < 0) {
        return false;
    }
    //+1 in x (go right)
    if(dir == 0) {
        if(x+1 < width_ && !maze.at(y).at(x).rWall) {
            return true;
        }
    }
    //+1 in y (go down)
    else if (dir == 1) {
        if(y+1 < height_ && !maze.at(y).at(x).bWall) {
            return true;
        }
    }
    //-1 in x (go left)
    else if (dir == 2) {
        if(x-1 >= 0 && !maze.at(y).at(x-1).rWall) {
            return true;
        }
    }
    //-1 in y (go up)
    else if (dir == 3) {
        if(y-1 >= 0 && !maze.at(y-1).at(x).bWall) {
            return true;
        }
    }
    return false;
}

PNG* SquareMaze::drawMaze() const {
    PNG* temp = new PNG(width_ * 10 + 1, height_ * 10 + 1);
    HSLAPixel black(0,0,0);

    //blacken first row and col
    for(unsigned int c = 10; c < temp->width(); c++) {
        temp->getPixel(c,0) = black;
    }
    for(unsigned int r = 0; r < temp->height(); r++) {
        temp->getPixel(0, r) = black;
    }
    
    //loop through every square
    for(int row = 0; row < height_; row++) {
        for(int col = 0; col < width_; col++) {            
            if(maze.at(row).at(col).rWall == true) {
                for(int i = 0; i < 11; i++) {
                    temp->getPixel((col+1)*10,row*10+i) = black;
                }
            }
            if(maze.at(row).at(col).bWall == true) {
                for(int i = 0; i < 11; i++) {
                    temp->getPixel(col*10+i, (row+1)*10) = black;
                }
            }
        }
    }

    return temp;
}

PNG* SquareMaze::drawMazeWithSolution() {
    PNG* temp = drawMaze();
    // vector<int> sol = solveMaze();

    // HSLAPixel red(0,1,0.5,1);

    // int currX = 5;
    // int currY = 5;

    // std::map<int, int> rev{{0,1}, {2,-1}, {1,1}, {3,-1}};

    // for(size_t i = 0; i < sol.size(); i++) {
    //     if(sol.at(i) == 0 || sol.at(i) == 2) {
    //         int dir = rev[sol.at(i)];
    //         for(int k = 0; k < 10; k++) {
    //             temp->getPixel(currX, currY) = red;
    //             currX += dir;
    //         }
    //     }
    //     if(sol.at(i) == 1 || sol.at(i) == 3) {
    //         int dir = rev[sol.at(i)];
    //         for(int k = 0; k < 10; k++) {
    //             temp->getPixel(currX, currY) = red;
    //             currY += dir;
    //         }
    //     }
    // }

    // currX -= 5;
    // currY += 5;

    // for(int i = 1; i < 10; i++) {
    //     temp->getPixel(currX, currY).l = 1;
    //     currX++;
    // }


    return temp;
}

void SquareMaze::makeMaze(int width, int height) {
    //so row comes first
    maze.clear();
    
    width_ = width;
    height_ = height;
    
    maze.resize(height);
    dsets.addelements(width*height);
    //sets the coordinates for each square
    for(int r = 0; r < height; r++) {
        maze.at(r).resize(width);
        for(int c = 0; c < width; c++) {
            //create temp square and set values
            Square temp;
            
            temp.xcoord = c;
            temp.ycoord = r;
            temp.rWall = true;
            temp.bWall = true;

            maze.at(r).at(c) = temp;
            //get a pointer to this square and store in random
            Square * tempPtr = &maze.at(r).at(c);
            random.push_back(tempPtr);
        }
    }
    //randomly remove walls
    randomlyRemove();
}

void SquareMaze::randomlyRemove() {
    //shuffle the random list
    std::random_shuffle(random.begin(), random.end());
    
    //we want to remove as many walls as possible from each to 
    //ensure they will all be in the same set
    for(size_t i = 0; i < random.size(); i++) {
        int c = random.at(i)->xcoord;
        int r = random.at(i)->ycoord;
        int root = dsets.find(r*width_ + c);
        
        //check to see if it's not all the way on the right
        if(c < (width_ - 1) && root != dsets.find(r*width_ + c + 1)) {
            setWall(c, r, 0, false);
            dsets.setunion(r*width_ + c, r*width_ + c + 1);
        }
        root = dsets.find(r*width_ + c);
        if(r < (height_ - 1) && root != dsets.find((r+1)*width_ + c)) {
            setWall(c, r, 1, false);
            dsets.setunion(r*width_+c, (r+1)*width_+c);
        }
    }
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    //bottom wall
    if(dir == 1) {
        maze.at(y).at(x).bWall = exists;
    }
    //right wall
    else if(dir == 0) {
        maze.at(y).at(x).rWall = exists;
    }
}

vector<int> SquareMaze::solveMaze() {
    vector<int> solution;
    //vector to check if elements in bottom row have been checked
    int bottomLeft = width_;
    
    //make a vector to see if things have been visited
    visited.clear();
    visited.resize(height_);
    for(int r = 0; r < height_; r++) {
        visited.at(r).resize(width_);
    }

    maze.at(0).at(0).prev = -1;
    visited.at(0).at(0) = 1;
    q.push(maze.at(0).at(0));
    int x = 0;
    int y = 0;
    while(!q.empty()) {
        Square temp = q.front();
        addSurrounding(temp.xcoord, temp.ycoord);
        //check if this is on the bottom row
        if(temp.ycoord == height_ - 1) {
            //check if that was the last to be made 1
            if(--bottomLeft == 0) {
                break;
            }
        }

        q.pop();
    }
    //last is still stored in q.front()
    std::map<int, int> rev{{0,2}, {2,0}, {1,3}, {3,1}};
    
    Square temp = q.front();
    int next = temp.prev;

    while(next != -1) {
        next = temp.prev;
        int p = rev[next];
        int nextX = temp.xcoord;
        int nextY = temp.ycoord;

        if(next == -1) {
            break;
        }
        solution.push_back(p);
        if(next == 2) {
            nextX = temp.xcoord - 1;
        }
        else if(next == 3) {
            nextY = temp.ycoord - 1;
        }
        else if(next == 0) {
            nextX = temp.xcoord + 1;
        }
        else if(next == 1) {
            nextY = temp.ycoord + 1;
        }
        temp = maze.at(nextY).at(nextX);
    }
    reverse(solution.begin(), solution.end());
    return solution;
}

void SquareMaze::addSurrounding(int x, int y) {
    //right
    if(x < width_ - 1 && canTravel(x, y, 0) && visited.at(y).at(x+1) != 1) {
        
        visited.at(y).at(x+1) = 1;
        //2 means go to the left
        maze.at(y).at(x+1).prev = 2;
        q.push(maze.at(y).at(x+1));
    }
    //down
    if(y < height_ - 1 && canTravel(x, y, 1) && visited.at(y+1).at(x) != 1) {
        
        visited.at(y+1).at(x) = 1;
        //3 means up
        maze.at(y+1).at(x).prev = 3;
        q.push(maze.at(y+1).at(x));
    }
    //left
    if(x > 0 && canTravel(x, y, 2) && visited.at(y).at(x-1) != 1) {
        
        visited.at(y).at(x-1) = 1;
        //0 means right
        maze.at(y).at(x-1).prev = 0;
        q.push(maze.at(y).at(x-1));
    }
    //up
    if(y > 0 && canTravel(x, y, 3) && visited.at(y-1).at(x) != 1) {
        
        visited.at(y-1).at(x) = 1;
        //1 means down
        maze.at(y-1).at(x).prev = 1;
        q.push(maze.at(y-1).at(x));
    }
}