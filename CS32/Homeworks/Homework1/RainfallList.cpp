//
//  RainfallList.cpp
//  Homework1
//
//  Created by Duncan Hackmann on 4/17/23.
//

#include <stdio.h>
#include "RainfallList.h"

RainfallList::RainfallList() {
    m_size = m_data.size();
}

bool RainfallList::add(unsigned long rainfall) {
    if (rainfall < 0 || rainfall > 400 || size() == DEFAULT_MAX_ITEMS) {
        return false;
    }
    m_data.insert(m_data.size(), rainfall);
    return true;
}

bool RainfallList::remove(unsigned long rainfall) {
    int p = m_data.find(rainfall);
    if (p == -1) {
        return false;
    }
    m_data.erase(p);
    return true;
}

int RainfallList::size() const {
    return m_data.size();
}

unsigned long RainfallList::minimum() const {
    if (m_data.empty()) {
        return NO_RAINFALLS;
    }
    
    unsigned long max;
    m_data.get(0, max);
        
    unsigned long value;
    for (int i = 1; i < m_data.size(); i++)
    {
        m_data.get(i, value);
        if (value < max)
            max = value;
        }

    return max;
}

unsigned long RainfallList::maximum() const {
    
    if (m_data.empty()) {
        return NO_RAINFALLS;
    }
    
    unsigned long max;
    m_data.get(0, max);
        
    unsigned long value;
    for (int i = 1; i < m_data.size(); i++)
    {
        m_data.get(i, value);
        if (value < max)
            max = value;
        }

    return max;
}

