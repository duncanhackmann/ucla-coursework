//
//  Dictionary.cpp
//  Project 4
//
//  Created by Duncan Hackmann on 6/2/23.
//


#include "Dictionary.h"
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>

void removeNonLetters(std::string& s);
std::string sortString(const std::string& s);

class DictionaryImpl
{
public:
    DictionaryImpl(int maxBuckets);
    ~DictionaryImpl();
    void insert(std::string word);
    void lookup(std::string letters, void callback(std::string)) const;

private:
    std::vector<std::list<std::string>> m_hashTable;
    int m_maxBuckets;

    size_t hash(const std::string& key) const;
};

DictionaryImpl::DictionaryImpl(int maxBuckets)
    : m_maxBuckets(maxBuckets)
{
    m_hashTable.resize(m_maxBuckets);
}

DictionaryImpl::~DictionaryImpl()
{
}

void DictionaryImpl::insert(std::string word)
{
    removeNonLetters(word);
    if (!word.empty())
    {
        std::string sortedWord = sortString(word);
        size_t index = hash(sortedWord);
        m_hashTable[index].push_back(word);
    }
}

void DictionaryImpl::lookup(std::string letters, void callback(std::string)) const
{
    if (callback == nullptr)
        return;

    removeNonLetters(letters);
    if (letters.empty())
        return;

    std::string sortedLetters = sortString(letters);
    size_t index = hash(sortedLetters);

    const std::list<std::string>& bucket = m_hashTable[index];
    for (const auto& word : bucket)
    {
        if (sortString(word) == sortedLetters)
            callback(word);
    }
}

size_t DictionaryImpl::hash(const std::string& key) const
{
    size_t hashValue = 0;
    for (char c : key)
    {
        hashValue = hashValue * 31 + c;
    }
    return hashValue % m_maxBuckets;
}

void removeNonLetters(std::string& s)
{
    s.erase(std::remove_if(s.begin(), s.end(), [](char c) { return !isalpha(c); }), s.end());
    std::transform(s.begin(), s.end(), s.begin(), [](char c) { return tolower(c); });
}

std::string sortString(const std::string& s)
{
    std::string sorted = s;
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}

Dictionary::Dictionary(int maxBuckets)
{
    m_impl = new DictionaryImpl(maxBuckets);
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(std::string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(std::string letters, void callback(std::string)) const
{
    m_impl->lookup(letters, callback);
}
