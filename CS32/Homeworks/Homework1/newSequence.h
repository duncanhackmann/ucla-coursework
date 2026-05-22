//
//  newSequence.h
//  Homework1
//
//  Created by Duncan Hackmann on 4/17/23.
//

#ifndef newSequence_h
#define newSequence_h

#include <string>

using ItemType = unsigned long;

const int DEFAULT_MAX_ITEMS = 160;

class Sequence {
public:
    Sequence();
    
    Sequence(int numberItems);
    ~Sequence();    // Destructor
    Sequence(const Sequence &old);  // Copy Constructor
    Sequence& operator= (const Sequence &src);  // Assignment Operator
    
    bool empty() const;
    int size() const;
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    const int find(const ItemType& value) const;
    
    void swap(Sequence& other);
    
private:
    ItemType* m_data;
    int m_size;
    int m_capacity;
};


#endif /* newSequence_h */
