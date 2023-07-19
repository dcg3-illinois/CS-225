/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num) {
    for(int i = 0; i < num; i++) {
        nodes.push_back(-1);
    }
    numSets += num;
}

int DisjointSets::find(int elem) {
    if(size_t(elem) >= nodes.size()) {
        return -1;
    }
    if(nodes.at(elem) < 0) {
        return elem;
    }
    return find(nodes.at(elem));
}

void DisjointSets::setunion(int a, int b) {
    //get the roots of the nodes you receive
    int rootA = find(a);
    int rootB = find(b);
    //the case where they are in the same set already
    if(rootA == rootB) { return; }
    if(size(rootB) > size(rootA)) {
        nodes.at(rootB) += nodes.at(rootA);
        nodes.at(rootA) = rootB;
    }
    else {
        nodes.at(rootA) += nodes.at(rootB);
        nodes.at(rootB) = rootA;
    }
    numSets--;
}

int DisjointSets::size(int elem) {
    //find the root of the elem
    int root = find(elem);

    //return the negative of the value
    return nodes.at(root)*(-1);
}

int DisjointSets::getNumSets() {
    return numSets;
}