/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;


template <int Dim>
int KDTree<Dim>::partition(std::vector<Point<Dim>>& points, int left, int right, int dimension) {
  int index = left;
  //we want to run through all of the elements left of the last el
  for(int run = left; run <= right - 1; run++) {
    //we want to keep everything smaller than the right val to the left
    //and everything greater to the right
    if(smallerDimVal(points.at(run), points.at(right), dimension)) {
      std::swap(points.at(index), points.at(run));
      index++;
    }
  }
  //we now want to put the last num in it's rightful place
  std::swap(points.at(index), points.at(right));
  return index;
}

template <int Dim>
Point<Dim> KDTree<Dim>::select(std::vector<Point<Dim>> & points, int left, int right, int medIndex, int dimension) {
  //if left is the same as right, we are done
  if(left == right) {
    return points.at(left);
  }
  //get the index that our rightmost point belongs in
  int index = partition(points, left, right, dimension);

  //median is where it's supposed to be
  if(index == medIndex) {
    return points.at(index);
  }
  //we chose something greater than median
  else if(index > medIndex) {
    return select(points, left, index-1, medIndex, dimension);
  }
  //we chose something less than median
  else {
    return select(points, index + 1, right, medIndex, dimension);
  }
}






template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
  if(first[curDim] < second[curDim]) { return true; }
  if(first[curDim] > second[curDim]) { return false; }
  else {
    return first < second;
  }
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
  double distance_curr = distance(currentBest, target);
  double distance_potential = distance(potential, target);

  //potential is farther
  if(distance_curr < distance_potential) {
    return false;
  }
  //potential is closer
  else if(distance_curr > distance_potential) {
    return true;
  }
  return potential < currentBest;
  
}

template <int Dim>
double KDTree<Dim>::distance(Point<Dim> p1, Point<Dim> p2) const {
  double d = 0;

  for(size_t i = 0; i < Dim; i++) {
    d += (p1[i] - p2[i])*(p1[i] - p2[i]);
  }
  
  return (d);
}


template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    size = newPoints.size();
    std::vector<Point<Dim>> points = newPoints;
    root = nextRoot(points, 0, int(points.size())-1, (int(points.size()) - 1)/2, 0);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::nextRoot(std::vector<Point<Dim>> & points, int left, int right, int medIndex, int dimension) {
  
  if(left == right) {
    return new KDTreeNode(points.at(left));
  }
  else if(left > right) {
    return nullptr;
  }
  
  //select returns the median point
  KDTreeNode* returnVal = new KDTreeNode(select(points, left, right, medIndex, dimension));

  //set the left child
  returnVal->left = nextRoot(points, left, medIndex-1, (medIndex-1+left)/2, (dimension+1)%Dim);
  returnVal->right = nextRoot(points, medIndex + 1, right, (right+medIndex+1)/2, (dimension+1)%Dim);

  return returnVal;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  root = copy(other.root);
  size = other.size;
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copy(KDTreeNode* copyRoot) {
  //if the root is null, just return null
  if(!copyRoot) {
    return nullptr;
  }
  //copy everything over
  KDTreeNode* toReturn = new KDTreeNode(copyRoot->point);
  toReturn->left = copy(copyRoot->left);
  toReturn->right = copy(copyRoot->right);

  return toReturn;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  root = copy(rhs.root);
  size = rhs.size;
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
   delee(root);
}

template <int Dim>
void KDTree<Dim>::delee(KDTreeNode* subroot) {
  if(!subroot) {
    return;
  }
  delee(subroot->left);
  delee(subroot->right);
  delete subroot;
}



template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    if(!root) {
      return Point<Dim>();
    }
    return findN(root, query, 0);
}


template<int Dim>
Point<Dim> KDTree<Dim>::findN(KDTreeNode* subroot, const Point<Dim>& query, int dimension) const{
  bool wentLeft = false;
  //this is a leaf, so we set it to best
  if(!subroot->left && !subroot->right) {
    return subroot->point;
  }
  //so we know it's not a leaf
  Point<Dim> cBest = subroot->point;
  //we set potBest to cBest for the case that the dimension is smaller or greater,
  //but there is no node in that direction
  Point<Dim> potBest = cBest;

  bool goLeft = smallerDimVal(query, subroot->point, dimension);

  //the point we are looking for has smaller val in this dimension
  if(goLeft && subroot->left != nullptr) {
    //so go left
    potBest = findN(subroot->left, query, (dimension+1)%Dim);
    wentLeft = true;
  }
  //query is greater, so go right
  else if(!goLeft && subroot->right != nullptr) {
    potBest = findN(subroot->right, query, (dimension+1)%Dim);
  }
  //we give preference to the best of the children (see 7,2 5,2 ex)
  if(shouldReplace(query, cBest, potBest)) {
    //if we are, then we replace
    cBest = potBest;
  }
  //bestDis is now the distance between best and query
  double radiusSquared = distance(cBest, query);
  //if splitting point is less than radius, there may be better
  //options in other portions
  double thisDistanceS = (subroot->point[dimension] - query[dimension])*(subroot->point[dimension] - query[dimension]);

  if(thisDistanceS <= radiusSquared) {
    //if we already went left, go right
    if(wentLeft && subroot->right != nullptr) {
      potBest = findN(subroot->right, query, (dimension+1)%Dim);
    }
    //and vice versa
    else if(!wentLeft && subroot->left != nullptr) {
      potBest = findN(subroot->left, query, (dimension+1)%Dim);
    }
  }
  
  if(shouldReplace(query, cBest, potBest)) {
    //cBest now contains absolute best of subtree
    cBest = potBest;
  }

  return cBest;
}