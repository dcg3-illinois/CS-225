/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in MP3.1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator();
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in MP3.1
  ListNode * curr = head_;
  ListNode * n;
  for(int i = 0; i < length_; i++) {
    n = curr->next;
    delete curr;
    curr = n;
  }
  length_ = 0;
  head_ = NULL;
  tail_ = NULL;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in MP3.1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = head_;
  newNode -> prev = NULL;
  if(head_ != NULL) {
    head_->prev = newNode;
  }
  //the head should be newNode regardless
  head_ = newNode;
  //this checks to see if the list is empty
  if (tail_ == NULL) {
    tail_ = newNode;
  }
  length_++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in MP3.1
  ListNode * newNode = new ListNode(ndata);

  newNode->next = NULL;
  newNode->prev = tail_;
  if(tail_ != NULL) {
    tail_->next = newNode;
  }
  tail_ = newNode;
  //if the list is empty
  if(head_ == NULL){
    head_ = newNode;
  }
  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in MP3.1
  ListNode * curr = start;
  int tempLength = 0;

  for (int i = 0; i < splitPoint && curr != NULL; i++) {
    curr = curr->next;
  }

  if (curr != nullptr) {
      if(curr->prev != nullptr) {
        curr->prev->next = nullptr; //sets the previous node's nxt to null
      }
      
      curr->prev = nullptr; //sets current node's prev to null
      return curr;
  }

  return nullptr;
  // ListNode * temp = start;
  // for(int i = 0; i < splitPoint; i++) {
  //   temp = temp->next;
  // }
  // if(temp) {
  //   temp->prev->next = nullptr;
  //   temp->prev = nullptr;
  // }
  // return temp;
}

/**
  * Modifies List using the rules for a TripleRotate.
  *
  * This function will to a wrapped rotation to the left on every three 
  * elements in the list starting for the first three elements. If the 
  * end of the list has a set of 1 or 2 elements, no rotation all be done 
  * on the last 1 or 2 elements.
  * 
  * You may NOT allocate ANY new ListNodes!
  */
template <typename T>
void List<T>::tripleRotate() {
  //@todo Graded in MP3.1
  ListNode* first = head_;
  ListNode* next;
  int numDone = 0;
  while((length_ - numDone) >= 3) {
    next = first->next;
    //my issue here is that I was passing a reference of the next ptr
    //of first, so first's next pointer was changing instead of 2's ptr
    this->reverse(first, next);
    first = first->next;
    next = first->next;
    this->reverse(first, next);
    first = first->next->next;
    numDone = numDone + 3;
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in MP3.2

  //ptr to be used to switch things
  ListNode * temp; 
  //ptr to be used to iterate
  ListNode * curr = startPoint->next;
  //ptrs that point to the surrounding elements
  ListNode * endNext = endPoint->next;
  ListNode * startPrev = startPoint->prev;

  //switches prev and next pointers for all nodes
  while(curr != endPoint && curr!= nullptr) {
    temp = curr->next;
    curr->next = curr->prev;
    curr->prev = temp;

    curr = curr->prev;
  }
  //store start next and make start next = last something
  temp = startPoint->next;
  startPoint->next = endNext;
  startPoint->prev = temp;

  //swap pointers for last
  endPoint->next = endPoint->prev;
  endPoint->prev = startPrev;

  //surrounding nodes
  if(startPrev) { startPrev->next = endPoint; }
  if(endNext) { endNext->prev = startPoint; }
  
  //swap head and tail if necessary
  if(head_ == startPoint) { head_ = endPoint; }
  if(tail_ == endPoint) { tail_ = startPoint; }
  
  //swap the pointers
  temp = startPoint;
  startPoint = endPoint;
  endPoint = temp;
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in MP3.2
  ListNode * first = head_;//gets the head
  ListNode * last = first;
  int done = 0;
  while(last != nullptr) {
    while(last != nullptr && done < (n - 1)) {
      //check to see if last->next is empty
      if(last->next != nullptr) {
        last = last->next;
      }
      else {
        break;
      }
      done++;
    }
    reverse(first, last);
    done = 0;
    first = last->next;
    last = last->next;
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in MP3.2
  //create a running ptr for first ands second (and a temp)
  ListNode * runningF = first;
  ListNode * runningS = second;
  ListNode * tempNextS;
  //we need a pointer that will always point to the beginning of first
  ListNode * absoluteFirst = first;

  //loop through
  while(runningF != nullptr && runningS != nullptr) {
    if(runningS->data < runningF->data) {
      //we need to put runningS behind runningF
      //store the next val in second
      tempNextS = runningS->next;
      //perform the swap
      runningS->prev = runningF->prev;
      if(runningF->prev) { runningF->prev->next = runningS; }
      runningS->next = runningF;
      runningF->prev = runningS;
      //if these are the first two in the list, we need to change first pointer
      if(runningF == first && runningS == second) { absoluteFirst = runningS; }
      //iterate runningS
      runningS = tempNextS;
    }
    else {
      if(runningF->next == nullptr) { break; }
      runningF = runningF->next;
    }
  }
  //this is the case that we get to the last element in the first list
  if(runningF && (runningF->next == nullptr) && runningS) {
      runningF->next = runningS;
      runningS->prev = runningF;    
  }

  return absoluteFirst;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in MP3.2
  if(chainLength == 1) {
    return start;
  }
  ListNode * second = split(start, chainLength/2);
  start = merge(mergesort(start, chainLength/2), mergesort(second, chainLength - chainLength/2));
  return start;
}
