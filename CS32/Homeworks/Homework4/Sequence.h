//
//  Sequence.h
//  Homework 4
//
//  Created by Duncan Hackmann on 5/30/23.
//


#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <iostream>
#include <cassert>

template <typename ItemType>
class Sequence
{
public:
    Sequence();
    ~Sequence();

    bool isEmpty() const;
    int getLength() const;
    bool insert(int newPosition, const ItemType& newEntry);
    bool remove(int position);
    void clear();
    ItemType getEntry(int position) const;
    void setEntry(int position, const ItemType& newEntry);

    // Function templates
    template <typename Predicate>
    Sequence<ItemType> subsequence(Predicate pred) const;
    template <typename Predicate>
    Sequence<ItemType> concatReverse(Predicate pred) const;

private:
    struct Node
    {
        ItemType item;
        Node* prev;
        Node* next;
    };

    Node* head;
    int itemCount;

    Node* getNodeAt(int position) const;
};

template <typename ItemType>
Sequence<ItemType>::Sequence() : head(nullptr), itemCount(0) {}

template <typename ItemType>
Sequence<ItemType>::~Sequence()
{
    clear();
}

template <typename ItemType>
bool Sequence<ItemType>::isEmpty() const
{
    return itemCount == 0;
}

template <typename ItemType>
int Sequence<ItemType>::getLength() const
{
    return itemCount;
}

template <typename ItemType>
bool Sequence<ItemType>::insert(int newPosition, const ItemType& newEntry)
{
    if (newPosition < 1 || newPosition > itemCount + 1)
        return false;

    Node* newNode = new Node{newEntry, nullptr, nullptr};

    if (newPosition == 1)
    {
        newNode->next = head;
        if (head != nullptr)
            head->prev = newNode;
        head = newNode;
    }
    else
    {
        Node* prevNode = getNodeAt(newPosition - 1);
        Node* nextNode = prevNode->next;

        newNode->next = nextNode;
        newNode->prev = prevNode;

        prevNode->next = newNode;
        if (nextNode != nullptr)
            nextNode->prev = newNode;
    }

    itemCount++;
    return true;
}

template <typename ItemType>
bool Sequence<ItemType>::remove(int position)
{
    if (position < 1 || position > itemCount)
        return false;

    Node* nodeToDelete = getNodeAt(position);

    if (nodeToDelete == head)
        head = nodeToDelete->next;
    else
        nodeToDelete->prev->next = nodeToDelete->next;

    if (nodeToDelete->next != nullptr)
        nodeToDelete->next->prev = nodeToDelete->prev;

    delete nodeToDelete;
    itemCount--;

    return true;
}

template <typename ItemType>
void Sequence<ItemType>::clear()
{
    while (!isEmpty())
        remove(1);
}

template <typename ItemType>
ItemType Sequence<ItemType>::getEntry(int position) const
{
    assert(position >= 1 && position <= itemCount);

    Node* nodePtr = getNodeAt(position);
    return nodePtr->item;
}

template <typename ItemType>
void Sequence<ItemType>::setEntry(int position, const ItemType& newEntry)
{
    assert(position >= 1 && position <= itemCount);

    Node* nodePtr = getNodeAt(position);
    nodePtr->item = newEntry;
}

template <typename ItemType>
template <typename Predicate>
Sequence<ItemType> Sequence<ItemType>::subsequence(Predicate pred) const
{
    Sequence<ItemType> result;

    Node* currentNode = head;
    while (currentNode != nullptr)
    {
        if (pred(currentNode->item))
            result.insert(result.getLength() + 1, currentNode->item);

        currentNode = currentNode->next;
    }

    return result;
}

template <typename ItemType>
template <typename Predicate>
Sequence<ItemType> Sequence<ItemType>::concatReverse(Predicate pred) const
{
    Sequence<ItemType> result;
    Node* currentNode = head;

    while (currentNode != nullptr)
    {
        if (pred(currentNode->item))
            result.insert(1, currentNode->item);

        currentNode = currentNode->next;
    }

    return result;
}

template <typename ItemType>
typename Sequence<ItemType>::Node* Sequence<ItemType>::getNodeAt(int position) const
{
    assert(position >= 1 && position <= itemCount);

    Node* nodePtr = head;
    for (int i = 1; i < position; ++i)
        nodePtr = nodePtr->next;

    return nodePtr;
}

#endif
