#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
  /** @todo [Part 1] */
  //instantiate stuff to NULL
  traversal = NULL;
  image = nullptr;
}

ImageTraversal::Iterator::Iterator(ImageTraversal* trav, const PNG* png, Point & s, double tolerance) {
  //initialize everything
  traversal = trav;
  t = tolerance;
  curr = s;
  start = s;
  image = png;
  
  //resize the 2D vector
  visited.resize(png->width());
  for(size_t i = 0; i < visited.size(); i++) {
    visited.at(i).resize(png->height());
  }
  
  //push the neighbor nodes of start
  pushSurrouding(start);
  //mark curr as visited
  if(inBounds(start)) {
    visited.at(start.x).at(start.y) = true;
  }
}

bool ImageTraversal::Iterator::inBounds(Point p) {
  //checks bounds of x and y
  if(p.x < 0 || p.x >= image->width()) { return false; }
  if(p.y < 0 || p.y >= image->height()) { return false; }
  return true;
}

bool ImageTraversal::Iterator::isValid(Point p) {
  //check to see if visited
  if(visited.at(p.x).at(p.y)) {
    return false;
  }
  //check to see if within tolerance
  if(calculateDelta(image->getPixel(start.x, start.y), image->getPixel(p.x, p.y)) >= t) {
    return false;
  }
  return true;
}

void ImageTraversal::Iterator::pushSurrouding(Point p) {
  //create new neighbor points
  Point R(p.x+1, p.y);
  Point D(p.x, p.y+1);
  Point L(p.x-1, p.y);
  Point U(p.x, p.y-1);

  //check if within bounds and not visited
  if(inBounds(R) && !(visited.at(R.x).at(R.y))) {
    traversal->add(R);
  }
  if(inBounds(D) && !(visited.at(D.x).at(D.y))) {
    traversal->add(D);
  }
  if(inBounds(L) && !(visited.at(L.x).at(L.y))) {
    traversal->add(L);
  }
  if(inBounds(U) && !(visited.at(U.x).at(U.y))) {
    traversal->add(U);
  }
}


/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */  
  
  //we want to make sure we're not popping something empty
  if(!traversal->empty()) {
    //get whatever is on top of the structure
    Point p = traversal->pop();
    //if it's not valid and traversal isn't empty, get the next one
    while(!isValid(p) && !traversal->empty()) {
      visited.at(p.x).at(p.y) = true;
      p = traversal->pop();
    }

    //now we have to check R, D, L, U
    if(!traversal->empty()) {
      curr = p;
      visited.at(curr.x).at(curr.y) = true;
      pushSurrouding(curr);
    }
    else { 
      //set curr to .end() value
      Point temp(image->width()+1, image->height()+1);
      curr = temp;
    }
  }
  return *this;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return curr;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  return !(curr == other.curr);
}

