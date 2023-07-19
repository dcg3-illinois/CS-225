/**
 * @file binarytree.hpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
#include "InorderTraversal.h"
#include <iostream>

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + std::max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
    // Call recursive helper function on the root
    printLeftToRight(root);

    // Finish the line
    std::cout << std::endl;
}

/**
 * Private helper function for the public printLeftToRight function.
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node* subRoot) const
{
    // Base case - null node
    if (subRoot == NULL)
        return;

    // Print left subtree
    printLeftToRight(subRoot->left);

    // Print this node
    std::cout << subRoot->elem << ' ';

    // Print right subtree
    printLeftToRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 *  (not creating a flipped copy).
 */
    template <typename T>
void BinaryTree<T>::mirror()
{
    //your code here
    mirror(root);
}

/**
 * Private helper function for the public mirror function.
 * @param subRoot
 * @return nothing
 */
template <typename T>
void BinaryTree<T>::mirror(Node* subRoot) const
{
    if(subRoot == nullptr) {
        return;
    }
    Node* temp = subRoot->left;
    subRoot->left = subRoot->right;
    subRoot->right = temp;
    mirror(subRoot->left);
    mirror(subRoot->right);
}


/**
 * isOrdered() function iterative version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedIterative() const
{
    // your code here
    InorderTraversal<T> t(getRoot());
    typename TreeTraversal<T>::Iterator it = t.begin();
    if(!(*it)) { return true; }

    //get the first elem
    T curr = (*it)->elem; 
    //check to see if any element is less than the previous
    for(typename InorderTraversal<T>::Iterator cheese = ++t.begin(); cheese != t.end(); ++cheese) {
        if((*cheese)->elem < curr) { return false; }
        curr = (*cheese)->elem;
    }
    
    return true;
}

/**
 * isOrdered() function recursive version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedRecursive() const
{
    // create an iterator starting at the first val
    std::vector<T> allEls;
    isOrderHelper(allEls, getRoot());

    for(size_t i = 0; i < allEls.size() - 1; i++) {
        if(allEls.at(i) > allEls.at(i+1)) { return false; }
    }
    return true;
}

template <typename T>
void BinaryTree<T>::isOrderHelper(std::vector<T> & els, Node* curr) const {
    if(!curr) { return; }

    isOrderHelper(els, curr->left);
    els.push_back(curr->elem);
    isOrderHelper(els, curr->right);

    return;
}

