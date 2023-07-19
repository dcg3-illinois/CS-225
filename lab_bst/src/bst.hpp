/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    BST<K,V>::Node* & location = find(root, key);

    //case that key is not in tree
    if(location == nullptr) { return V(); }

    //case that key is in the tree
    return location->value;
}

template <class K, class V>
struct BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
    //base case
    //if subtree is null, it is still a reference to a child pointer
    //of the previous node, so we can return it
    if(!subtree || subtree->key == key) { return subtree; }

    //recursive statements
    if(subtree->key > key) { 
        return find(subtree->left, key); 
    }

    else { 
        return find(subtree->right, key); 
    }
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    //find where the node is
    BST<K,V>::Node* & location = find(root, key);

    BST<K,V>::Node* toInsert = new BST<K,V>::Node(key, value);

    //if the location is not null, then the key is already in
    //the BST. Otherwise, we add it as a child to the reference
    //we got from find
    if(location == nullptr) {
        location = toInsert;
    }
}

// template <class K, class V>
// void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
// {
//     // your code here
// }

template <class K, class V>
struct BST<K,V>::Node* & BST<K,V>::getPredecessor(Node* toad) {
    Node* rightHand = toad->left;
    
    //this is the case where the leftmost node is the largest
    if(!rightHand || !rightHand->right) {
        Node * & toReturn = toad->left;
        return toReturn;
    }

    while(rightHand) {
        //we want to go right until the step the last rightmost node
        if(rightHand->right && rightHand->right->right) { rightHand = rightHand->right; }

        else { break; }
    }

    //return a reference to the pointer pointing at the IOP
    Node * & toReturn = rightHand->right;
    return toReturn;
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    K tempK;
    V tempV;
    //swap values
    tempV = first->value;
    first->value = second->value;
    second->value = tempV;

    //swap keys
    tempK = first->key;
    first->key = second->key;
    second->key = tempK;
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    Node * & location = find(root, key);
    //if it's not in the BST, we're done!
    if(!location) { return; }

    //otherwise, find the IOP
    Node * & iop = getPredecessor(location);
    //if there's no IOP, we just skip over location
    if(!iop) {
        Node * next = location->right;
        delete location;
        location = next;
        return;
    }
    //else, we swap them, delete the node that's now out of place,
    //and replace it with its left child
    swap(iop, location);
    //now location pointer has iop's data
    Node * next = iop->left;
    delete iop;
    iop = next;
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    // your code here
}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    BST<K,V> tree;
    for(size_t i = 0; i < inList.size(); i++) {
        tree.insert(inList.at(i).first, inList.at(i).second);
    }
    return tree;
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    //there will be n different possible heights
    std::vector<int> heights;
    
    for(size_t i = 0; i < inList.size(); i++) {
        heights.push_back(0);
    }
    
    std::sort(inList.begin(), inList.end());
    do {
        BST<K,V> temp = listBuild(inList);
        heights.at(temp.height()) += 1;
    } while ( std::next_permutation(inList.begin(), inList.end()) );

    return heights;
}