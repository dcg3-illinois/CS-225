
#include "cs225/PNG.h"
#include "FloodFilledImage.h"
#include "Animation.h"

#include "imageTraversal/DFS.h"
#include "imageTraversal/BFS.h"

#include "colorPicker/RainbowColorPicker.h"
#include "colorPicker/GradientColorPicker.h"
#include "colorPicker/GridColorPicker.h"
#include "colorPicker/SolidColorPicker.h"
#include "colorPicker/MyColorPicker.h"

using namespace cs225;

int main() {

  // @todo [Part 3]
  // - The code below assumes you have an Animation called `animation`
  // - The code provided below produces the `myFloodFill.png` file you must
  //   submit Part 3 of this assignment -- uncomment it when you're ready.
  Animation animation;

  PNG hadtodoit;
  hadtodoit.readFromFile("../hadtodoit.png");

  FloodFilledImage flood(hadtodoit);
  
  DFS deez(hadtodoit, Point(252,252), 0.7);
  BFS bees(hadtodoit, Point(252, 140), 0.35);
  
  MyColorPicker colorp;
  //RainbowColorPicker rainbow(0.05);
  
  flood.addFloodFill(deez, colorp);
  flood.addFloodFill(bees, colorp);
  animation = flood.animate(1000);
  
  PNG lastFrame = animation.getFrame( animation.frameCount() - 1 );
  lastFrame.writeToFile("myFloodFill.png");
  animation.write("myFloodFill.gif");



  return 0;
}
