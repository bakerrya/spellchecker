#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "hashtable.h"

using namespace std;

template <typename T>
void HashTable<T>::setPrimes(std::vector<long>& vprimes)
{
    int i = 0;
    int j = 0;

    vprimes[0] = 0;
    vprimes[1] = 0;
    int n = vprimes.capacity();

    for (i = 2; i < n; ++i)
        vprimes[i] = 1;

    for( i = 2; i*i < n; ++i)
    {
        if (vprimes[i] == 1)
            for(j = i + i ; j < n; j += i)
                vprimes[j] = 0;
    }
}

template <typename T>
unsigned long HashTable<T>::prime_below (long n)
{
    if (n > max_prime)
    {
        std::cerr << "** input too large for prime_below()\n";
        return 0;
    }
    if (n == max_prime)
    {
        return max_prime;
    }
    if (n <= 1)
    {
        std::cerr << "** input too small \n";
        return 0;
    }

  // now: 2 <= n < max_prime
    std::vector <long> v (n+1);
    setPrimes(v);
    while (n > 2)
    {
        if (v[n] == 1)
            return n;
        --n;
    }
    return 2;
}
                         
template <typename T>
HashTable<T>::HashTable(size_t size){
    currentSize = prime_below(size);
    lists.resize(currentSize);
}

template <typename T>
void HashTable<T>::makeEmpty(){
    for (auto & list : lists){
        list.clear();
    }
    currentSize = 0;
} 

template <typename T>
void HashTable<T>::clear(){
    makeEmpty();
}

template <typename T>
HashTable<T>::~HashTable(){
    makeEmpty();
}

template <typename T>
size_t HashTable<T>::myhash( const T &x ){
    static hash<T> hf;
    return hf(x) % lists.size();
}

template <typename T>
bool HashTable<T>::contains( const T &x ){
    auto & selectedList = lists[myhash(x)];
    return find(begin(selectedList), end(selectedList), x) != end(selectedList);
}

template <typename T>
void HashTable<T>::rehash(){
    vector<list<T>> oldList = lists;
    
    lists.resize(prime_below(oldList.size() * 2));
    for (auto & entry : lists)
    {
        entry.clear();
    }
    
    currentSize = 0;

    for (auto & entry : oldList){
        for (auto & x : entry )
        {
            insert(std::move(x));
        }
    }
}

template <typename T>
bool HashTable<T>::insert( const T &x ){
    auto & selectedList = lists[myhash(x)];
    if (find(begin(selectedList), end(selectedList), x) != end(selectedList)){
        return false;
    }
    selectedList.push_back(x);
    if (++currentSize > lists.size()){
        rehash();
    }
    
    return true;
}
template <typename T>
bool HashTable<T>::insert( T &&x ){
    auto & selectedList = lists[myhash(x)];
    if (find(begin(selectedList), end(selectedList), x) != end(selectedList)){
        return false;
    }
    selectedList.push_back(std::move(x));
    if (++currentSize > lists.size()){
        rehash();
    }
    
    return true;
}
template <typename T>
bool HashTable<T>::remove( const T &x ){
    auto & selectedList = lists[myhash(x)];
    auto itr = find(begin(selectedList), end(selectedList), x);

    if (itr == end(selectedList)){
        return false;
    }

    selectedList.erase(itr);
    --currentSize;
    return true;

}

template <typename T>
bool HashTable<T>::load(const char * filename){
    fstream fs(filename);

    if (!fs.is_open()){
        cout << "error";
        return false;
    }

    T element;

    while (fs >> element){
        insert(element);
    }
   
    fs.close();
    return true;
}

template <typename T>
void HashTable<T>::dump(){
    for (auto & list : lists){
        for (auto & x : list){
            cout << x << "\t";
        }
        cout << endl;
    }
}

#endif
