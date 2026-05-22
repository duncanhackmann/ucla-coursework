//
//  Sequence.cpp
//  CS32 Project2
//
//  Created by Duncan Hackmann on 4/24/23.
//

#include <stdio.h>
#include "Sequence.h"

Sequence::Sequence() {
    head = nullptr;     //create empty linked list
    tail = nullptr;
    m_size = 0;
}

Sequence::~Sequence() {
    Node *p;
    p = head;
    while (p != nullptr) {          //delete all nodes
        Node *n = p->m_next;
        delete p;
        p = n;
    }
}

Sequence::Sequence(const Sequence& other) {
    if (other.head == nullptr) {
        head = nullptr;
        tail = nullptr;
        m_size = 0;
    } else {
        head = nullptr;     //make empty list
        tail = nullptr;
        m_size = 0;
        ItemType p;
        for (int i = 0; i < other.m_size; i++) {
            other.get(i, p);
            addToRear(p);
        }
        m_size = other.m_size;
    }
}

Sequence& Sequence::operator=(const Sequence &rhs) {
    if (this != &rhs) {
        Sequence temp(rhs);
        swap(temp);
    }
    return *this;
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
    
    if (pos < 0) {
        return -1;
    } else if (pos == 0) {
        addToFront(value);
    } else if (pos == size()) {
        addToRear(value);
    } else {
        addItem(pos, value);
    }
    return pos;
}


int Sequence::insert(const ItemType &value) {
    if (head == nullptr)
            addToFront(value);
        
        else if (head->data >= value)        //check if it is the first value
        {
            addToFront(value);
            return 0;
        }
        
        else{
            Node *p;
            int counter = 1;
                                                //find  position one above
            for (p = head; p != nullptr && p->m_next != nullptr && value > p->m_next->data; p = p->m_next) {
                counter++;
            }

            addItem(counter, value);
            return counter;
        }
        return 0;
    }


bool Sequence::erase(int pos) {
    if (pos < 0 || pos >= size()) {
        return false;
    }
    if (head == nullptr) {
        return false;           //need conditions to be met to erase
    }
    
    return deleteItem(pos);
}

int Sequence::remove(const ItemType &value) {
    if (head == nullptr) {
        return 0;
    }
    
    Node *p = head;
    int count = 0;
    int pos = 0;
    while (p != nullptr) {
        if (p->data == value) {
            p = p->m_next;
            erase(pos);
            count++;        //delete if match is found and increase count
            pos--;
        } else {
            p = p->m_next;
        }
        pos++;
    }
    return count;       //num of items removed
}

// If 0 <= pos < size(), copy into value the item in position pos
// of the sequence and return true.  Otherwise, leave value unchanged
// and return false.
bool Sequence::get(int pos, ItemType &value) const {
    if (pos < 0 || pos >= size()) {
        return false;
    }
    if (head == nullptr) {
        return false;
    }
    Node *p;
    int count = 0;
    for (p = head; p != nullptr; p = p->m_next) {       //find position and point p to it
        if (count == pos) {
            value = p->data;                            //copy data
            return true;
        }
        count++;
    }
    return false;
}

// If 0 <= pos < size(), replace the item at position pos in the
// sequence with value and return true.  Otherwise, leave the sequence
// unchanged and return false.
bool Sequence::set(int pos, const ItemType& value) {
    if (pos < 0 || pos >= size()) {
        return false;
    }
    if (head == nullptr) {
        return false;
    }
    Node *p;
    int count = 0;
    for (p = head; p != nullptr; p = p->m_next) {
        if (count == pos) {
            break;
        }
        count++;
    }
    p->data = value;
    return true;
}

// Let p be the smallest integer such that value == the item at
// position p in the sequence; if no such item exists, let p be -1.
// Return p.
int Sequence::find(const ItemType &value) const {
    Node *p;
    int count = 0;
    for (p = head; p != nullptr; p = p->m_next) {
        if (p->data == value) {
            return count;
        }
        count++;
    }
    return -1;
}

// Exchange the contents of this sequence with the other one.
void Sequence::swap(Sequence &other) {
    
    Node *h = head;         //temp pointers
    Node *t = tail;
    
    head = other.head;          //swap heads and tails
    tail = other.tail;
    
    other.head = h;
    other.tail = t;
    
    int tempSize = m_size;      //swap sizes
    m_size = other.m_size;
    other.m_size = tempSize;
}

void Sequence::addItem(int pos, const ItemType &value) {
    if (head == nullptr) {
        addToFront(value);
    } else if (pos == 0) {
        addToFront(value);
    } else if (pos == size()) {
        addToRear(value);
    } else {
        int count = 0;
        Node *p = head;
        while (p != nullptr) {
            if (count == pos-1) {
                break;
            }
            count++;
            p = p->m_next;
        }
        
        if (p->m_next == nullptr) {
            addToRear(value);
        } else {
            Node *newNode = new Node;
            newNode->data = value;
            newNode->m_next = p->m_next;
            if (newNode->m_next->m_prev != nullptr) {
                newNode->m_next->m_prev = newNode;
            }
            newNode->m_prev = p;
            p->m_next = newNode;
            m_size++;
        }
    }
}

void Sequence::addToFront(const ItemType &value) {
    Node *p;
    p = new Node;
    p->data = value;
    
    if (head == nullptr) {
        p->m_next = head;
        p->m_prev = nullptr;
        head = p;
        tail = p;
    } else {
        p->m_next = head;
        p->m_prev = nullptr;
        p->m_next->m_prev = p;
        head = p;
    }
    m_size++;
}

void Sequence::addToRear(const ItemType &value) {
    if (head == nullptr || tail == nullptr) {
        addToFront(value);
    } else {
        Node *n;
        n = new Node;
        n->data = value;
        n->m_next = nullptr;
        n->m_prev = tail;
        tail->m_next = n;
        tail = n;
        m_size++;
    }
}

bool Sequence::deleteItem(int pos) {
    if (pos == 0) {
        if (m_size > 1) {
            Node *kill = head;
            head = kill->m_next;
            kill->m_next->m_prev = nullptr;
            delete kill;
            m_size--;
            return true;
        } else {
            Node *kill = head;
            head = kill->m_next;
            kill->m_prev = nullptr;
            delete kill;
            m_size--;
            return true;
        }
    } else {
        int count = 0;
        Node *p = head;
        while (p != nullptr) {
            if (count == pos-1) {
                break;
            }
            count++;
            p = p->m_next;
        }
        if (p->m_next->m_next != nullptr) {
            Node *kill = p->m_next;
            p->m_next = kill->m_next;
            kill->m_next->m_prev = kill->m_prev;
            delete kill;
            m_size--;
            return true;
        } else if (p->m_next->m_next == nullptr) {
            Node *kill = p->m_next;
            p->m_next = nullptr;
            tail = p;
            delete kill;
            m_size--;
            return true;
        }
    }
    return false;
}

int subsequence(const Sequence& seq1, const Sequence& seq2) {
    if (seq1.empty()|| seq2.empty()) {
        return -1;
    }
        
    if (seq1.size() < seq2.size()) {
        return -1;
    }
    if (seq1.size() == seq2.size())
        {
            ItemType p;
            ItemType r;
            for (int k = 0; k < seq1.size(); k++) {     //if both seq are the same size they have to be identical
                seq1.get(k,p);
                seq2.get(k,r);
                if (!(p == r))
                    return -1;
                
            }
            return 0;
        }
    
    for (int k = 0; k <= seq1.size(); k++)
        {
            ItemType a;
            ItemType b;
            seq1.get(k, a);
            seq2.get(0, b);
            if (a == b) {
                int first = k;
                int i = 1;
                
                for (int j = k+1; j < (seq2.size()+first); j++) {
                    seq1.get(j, a);
                    seq2.get(i, b);
                    if (a != b) {
                        first = -1;
                        break;
                    }
                    i++;
                }
                return first;
            }
        }
        return -1;
}

void concatReverse(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
    for (int i = 0; i < result.size(); i++) {       //empty result
        result.erase(i);
    }
    int pos = 0;
    for (int i = seq1.size(); i > 0; i--) {         //add data from seq1 to result in reverse order
        ItemType x;
        seq1.get(i, x);
        result.insert(pos, x);
        pos++;
    }
    
    for (int i = seq2.size(); i > 0; i--) {         //add data from seq2 to result in reverse order
        ItemType x;
        seq2.get(i, x);
        result.insert(pos, x);
        pos++;
    }
}
