/* Your code here! */

#include <vector>

using namespace std;

class DisjointSets{
    public:
        void addelements(int num);
        int find(int elem);
        void setunion(int a, int b);
        int size(int elem);
        int getNumSets();
    private:
        std::vector<int> nodes;
        int numSets;
};

#pragma once