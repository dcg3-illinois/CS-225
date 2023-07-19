/* Your code here! */
#include "cs225/PNG.h"
#include "dsets.h"
#include <vector>
#include <queue>

using namespace std;

class SquareMaze {
    public:
        struct Square {
            int xcoord;
            int ycoord;
            bool rWall = true;
            bool bWall = true;
            int prev = 4;
        };
        
        SquareMaze();
        
        bool canTravel(int x, int y, int dir) const;

        cs225::PNG* drawMaze() const;

        cs225::PNG* drawMazeWithSolution();

        void randomlyRemove();

        void makeMaze(int width, int height);

        void setWall(int x, int y, int dir, bool exists);

        std::vector<int> solveMaze();

        void addSurrounding(int x, int y);
    private:
        int width_;
        int height_;
        DisjointSets dsets;

        queue<Square> q;
        vector<vector<int>> visited;
        
        //the 2D vector which contains all of the nodes
        vector<vector<Square>> maze;
        //1D vector which will be shuffled to make maze
        vector<Square*> random;
};

#pragma once