//
//  Sequence.h
//  CS32 Project2
//
//  Created by Duncan Hackmann on 4/24/23.
//

#ifndef Sequence_h
#define Sequence_h
#include <string>

using ItemType = std::string;

const int DEFAULT_MAX_ITEMS = 160;

class Sequence {
public:
    Sequence();    // Create an empty sequence (i.e., one whose size() is 0).
    ~Sequence();        //destructor
    Sequence(const Sequence& other);    //copy constructor
    Sequence& operator=(const Sequence& rhs);   //assignment operator
    bool empty() const;  // Return true if the sequence is empty, otherwise false.
    int size() const;    // Return the number of items in the sequence.
    int insert(int pos, const ItemType& value);
      // Insert value into the sequence so that it becomes the item at
      // position pos.  The original item at position pos and those that
      // follow it end up at positions one greater than they were at before.
      // Return pos if 0 <= pos <= size() and the value could be
      // inserted.  (It might not be, if the sequence has a fixed capacity,
      // e.g., because it's implemented using a fixed-size array.)  Otherwise,
      // leave the sequence unchanged and return -1.  Notice that
      // if pos is equal to size(), the value is inserted at the end.

    int insert(const ItemType& value);
      // Let p be the smallest integer such that value <= the item at
      // position p in the sequence; if no such item exists (i.e.,
      // value > all items in the sequence), let p be size().  Insert
      // value into the sequence so that it becomes the item in position
      // p.  The original item in position p and those that follow it end
      // up at positions one greater than before.  Return p if the value
      // was actually inserted.  Return -1 if the value was not inserted
      // (perhaps because the sequence has a fixed capacity and is full).
     
    bool erase(int pos);
      // If 0 <= pos < size(), remove the item at position pos from
      // the sequence (so that all items that followed that item end up at
      // positions one lower than they were at before), and return true.
      // Otherwise, leave the sequence unchanged and return false.
     
    int remove(const ItemType& value);
      // Erase all items from the sequence that == value.  Return the
      // number of items removed (which will be 0 if no item == value).

    bool get(int pos, ItemType& value) const;
      // If 0 <= pos < size(), copy into value the item in position pos
      // of the sequence and return true.  Otherwise, leave value unchanged
      // and return false.

    bool set(int pos, const ItemType& value);
      // If 0 <= pos < size(), replace the item at position pos in the
      // sequence with value and return true.  Otherwise, leave the sequence
      // unchanged and return false.

    int find(const ItemType& value) const;
      // Let p be the smallest integer such that value == the item at
      // position p in the sequence; if no such item exists, let p be -1.
      // Return p.

    void swap(Sequence& other);
      // Exchange the contents of this sequence with the other one.
    
private:
    int m_size;
    struct Node {
            ItemType data;
            Node* m_next;
            Node* m_prev;
    };
    
    Node *head;
    Node *tail;
    
    void addItem(int pos, const ItemType& value);
    void addToFront(const ItemType& value);
    void addToRear(const ItemType& value);
    bool deleteItem(int pos);
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void concatReverse(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif /* Sequence_h */
