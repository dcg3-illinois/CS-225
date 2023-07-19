/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "nim_learner.h"
#include <ctime>


/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true, true) {
    /* Your code goes here! */
    //loop from 0 to startingTokens (including startingTokens)
    for(unsigned i = 0; i <= startingTokens; i++) {
      //create vertices to represent the vertex corresponding to i
      Vertex temp1 = "p1-" + std::to_string(i);
      Vertex temp2 = "p2-" + std::to_string(i);
      //insert them
      g_.insertVertex(temp1);
      g_.insertVertex(temp2);

      //we can start inserting edges if it's not the 0th vertex
      if(i > 0) {
        //if there is more than one starting token then the vertices start
        //having more than 1 outgoing edge
        if(i >= 2) {
          Vertex prev21 = "p1-" + std::to_string(i-2);
          Vertex prev22 = "p2-" + std::to_string(i-2);
          g_.insertEdge(temp1, prev22);
          g_.setEdgeWeight(temp1, prev22, 0);
          g_.insertEdge(temp2, prev21);
          g_.setEdgeWeight(temp2, prev21, 0);
        }
        //and then we throw in the other outgoing edge
        Vertex prev11 = "p1-" + std::to_string(i-1);
        Vertex prev12 = "p2-" + std::to_string(i-1);
        g_.insertEdge(temp1, prev12);
        g_.setEdgeWeight(temp1, prev12, 0);
        g_.insertEdge(temp2, prev11);
        g_.setEdgeWeight(temp2, prev11, 0);
      }
    }
    startingVertex_ = "p1-" + std::to_string(startingTokens);
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const {
  vector<Edge> path;
 /* Your code goes here! */
  //initialize variables
  Vertex curr = startingVertex_;
  Vertex next;
  Vertex end1 = "p1-0";
  Vertex end2 = "p2-0";
  //add edges to path until we finish the game
  while(curr != end1 && curr != end2) {
    //choice will tell us which path to go down
    int choice = rand() % 2;
    vector<Vertex> adj = g_.getAdjacent(curr);
    //set next to choice if there are two options otherwise
    //just set it to the only option
    next = (adj.size() > 1) ? adj.at(choice) : adj.at(0);
    
    //get the edge that corresponds to our choice and add it
    Edge temp = g_.getEdge(curr, next);
    path.push_back(temp);

    //set curr equal to our new current location
    curr = next;
  }
  return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path) {
 /* Your code goes here! */
  Vertex l = path.at(path.size()-1).dest;
  //if player 1 wins, we want to add 1 to each of their moves,
  //so addedW will be 1
  int addedW = (l == "p2-0") ? 1 : -1;

  for(size_t i = 0; i < path.size(); i++) {
    
    Edge e = path.at(i);
    int currW = g_.getEdgeWeight(e.source, e.dest);
    
    //we want addedW applied to the first and then every other,
    //and the negative added to the rest
    int multiplier;
    if(e.source[1] == '1') {
      multiplier = 1;
    }
    else {
      multiplier = -1;
    }
    //set the edge weight
    g_.setEdgeWeight(e.source, e.dest, currW + addedW*multiplier);
  }
}

/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold) {
  for (const Vertex & v : g_.getVertices()) {
    for (const Vertex & w : g_.getAdjacent(v)) {
      int weight = g_.getEdgeWeight(v, w);

      // Label all edges with positve weights as "WINPATH"
      if (weight > threshold)           { g_.setEdgeLabel(v, w, "WIN"); }
      else if (weight < -1 * threshold) { g_.setEdgeLabel(v, w, "LOSE"); }
    }
  }
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const {
  return g_;
}
