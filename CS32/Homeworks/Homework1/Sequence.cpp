//
//  Sequence.cpp
//  Homework1
//
//  Created by Duncan Hackmann on 4/16/23.
//

#include <stdio.h>
#include "Sequence.h"


Sequence::Sequence() {
    m_size = 0;
}

bool Sequence::empty() const {
    if (m_size == 0) {
        return true;
    } else {
        return false;
    }
}

int Sequence::size() const {
    return m_size;
}

int Sequence::insert(int pos, const ItemType &value) {
    
    if (pos < 0 || pos > size() || size() == DEFAULT_MAX_ITEMS) {
        return -1;
    }
    

    int index = size();

    while (index != pos) {
        m_data[index] = m_data[index - 1];
        index--;
    }

    m_data[pos] = value;
    m_size++;
    return pos;
}


int Sequence::insert(const ItemType &value) {
    
    if (size() == DEFAULT_MAX_ITEMS) {
        return -1;
    }
            
        
        int p = size();
        
        for (int i = 0; i < size(); i++)
            if (value <= m_data[i]) {
                p = i;
                break;
            }
        
        int index = size();
        
        while (index != p) {
            m_data[index] = m_data[index - 1];
            index--;
        }
        
        m_data[p] = value;
        m_size++;
        return p;
}


bool Sequence::erase(int pos) {
    if (pos < 0 || pos >= size()) {
        return false;
    }
    
    m_size--;
    
    for (int i = pos; i < size()-1; i++) {
        m_data[i] = m_data[i+1];
    }
    return true;
}

int Sequence::remove(const ItemType &value) {
    
    int count = 0;
    for (int i = 0; i <= size(); i++) {
        if (m_data[i] == value) {
            erase(i);
            count++;
        }
    }
    return count;
}

// If 0 <= pos < size(), copy into value the item in position pos
// of the sequence and return true.  Otherwise, leave value unchanged
// and return false.
bool Sequence::get(int pos, ItemType &value) const {
    if (pos < 0 || pos >= size()) {
        return false;
    }
    value = m_data[pos];
    return true;
}

// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.
bool Sequence::set(int pos, const ItemType& value) {
    if (pos < 0 || pos >= size()) {
        return false;
    }
    m_data[pos] = value;
    return true;
}
  
// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.
const int Sequence::find(const ItemType &value) const {
    for (int i = 0; i < size(); i++) {
            if (m_data[i] == value)
                return i;
    }
    return -1;
}

// Exchange the contents of this sequence with the other one.
void Sequence::swap(Sequence &other) {
    
    int max = size();
    if (max < other.m_size) {
        max = other.m_size;
    }
    
    ItemType placeholder;
    for (int i = 0; i < max; i++) {
        placeholder = m_data[i];
        m_data[i] = other.m_data[i];
        other.m_data[i] = placeholder;
    }
    
    int temp = m_size;
        m_size = other.m_size;
        other.m_size = temp;
}
