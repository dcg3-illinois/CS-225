/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    /*
        x
         \
          x
           \
            x
    */
    //do some swappage
    Node* r = t->right;
    t->right = r->left;
    r->left = t;
    t = r;
    //update heights just in case
    updateHeight(t);
    updateHeight(t->left);
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    /*
            x
           /
          x
         /
        x
    */
    Node* l = t->left;
    t->left = l->right;
    l->right = t;
    t = l;
    updateHeight(t);
    updateHeight(t->right);
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);
}

/**
Helper function for rebalance and other functions
*/

template <class K, class V>
int AVLTree<K, V>::balance(Node * node) {
    //returns the different bt right and left children heights
    return heightOrNeg1(node->right) - heightOrNeg1(node->left);
}

template <class K, class V>
void AVLTree<K, V>::updateHeight(Node * node) {
    //returns the max height of children + 1
    node->height = std::max(heightOrNeg1(node->left), heightOrNeg1(node->right)) + 1;
}


template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    // your code here
    if(!subtree) { return;}
    //if heavy right
    if(balance(subtree) == 2) {
        //this checks for elbow
        if(balance(subtree->right) == -1)
            rotateRightLeft(subtree);
        //otherwise it's a stick
        else
            rotateLeft(subtree);
    }
    //if heavy left
    else if(balance(subtree) == -2) {
        if(balance(subtree->left) == 1)
            rotateLeftRight(subtree);
        else
            rotateRight(subtree);
    }
    updateHeight(subtree);
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    //if we're at null, we insert
    if(!subtree) {
        Node* newN = new Node(key,value);
        subtree = newN;
    }
    else {
        //otherwise we navigate based on where we are
        if(subtree->key < key) {
            insert(subtree->right, key, value);
        }
        else if(subtree->key > key) {
            insert(subtree->left, key, value);
        }
        //update height and balance on the way up
        updateHeight(subtree);
        rebalance(subtree);
    }
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        remove(subtree->left, key);
    } 
    else if (key > subtree->key) {
        remove(subtree->right, key);
    } 
    else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            delete subtree;
            subtree = NULL;
            return;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            Node*& iop = getPredecessor(subtree);
            Node* toDelete = iop;
            //now iop contains subtree's data
            swap(subtree, iop);
            //set the right pointer of previous of iop to iop->left
            iop = iop->left;
            //get rid of the thing we wanted to remove
            delete toDelete;
        } else {
            /* one-child remove */
            Node* toDelete = subtree;
            if(subtree->left) { subtree = subtree->left; }
            else { subtree = subtree->right; }
            delete toDelete;
        }
        // your code here
    }
    rebalance(subtree);
}

template <class K, class V>
typename AVLTree<K, V>::Node* & AVLTree<K, V>::getPredecessor(Node*& node) {
    Node* iop = node->left;
    while(iop->right && iop->right->right) {
        iop = iop->right;
    }
    return iop->right;
}