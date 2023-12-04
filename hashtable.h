//
// Created by Ryan Baker on 11/20/23.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <fstream>

namespace cop4530 {

template <typename T>
class HashTable {
public:
    HashTable(size_t size = 101);
    ~HashTable();

    bool contains(const T &x);
    bool insert(const T & x);
    bool insert (T &&x);
    bool remove(const T &x);
    void clear();
    bool load(const char *filename);
    void dump();
    bool write_to_file(const char *filename);

private:
    void makeEmpty();
    void rehash();

    static const unsigned int default_capacity = 11;
    static const unsigned int max_prime = 1301081;

    size_t myhash(const T &x);
    unsigned long prime_below(long);
    void setPrimes(std::vector<long> &);

    std::vector<std::list<T>> lists;
    int currentSize;

};

#include "hashtable.hpp"

}


#endif
