#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include "cs225/PNG.h"
#include "dsets.h"
#include "maze.h"
#include "mazereader.h"

using namespace cs225;
using namespace std;
using std::cout;
using std::endl;
using std::string;


TEST_CASE("PUT YOUR TESTS HERE!!!", "[tag1][tag2]") {
    // Feel free to use this to put your own test cases.
    // Look at other test files for inspiration!
    string expected = "I put my own test cases here.";
    string actual = "I DON'T HAVE ANY TEST CASES :'(";
    REQUIRE(expected == actual);
}

TEST_CASE("draw22", "[own]") {
    // Feel free to use this to put your own test cases.
    // Look at other test files for inspiration!
    SquareMaze maze;
    maze.makeMaze(2,2);
    PNG * actualOutput = maze.drawMaze();
    actualOutput->writeToFile("drawSmall"+ string(".png"));
    vector<int> temp = maze.solveMaze();
    REQUIRE(1 == 1);
}