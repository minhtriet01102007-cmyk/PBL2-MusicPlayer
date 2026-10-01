#pragma once
#include "hashtablenode.h"
#include <vector>
#include <string>
using namespace std;

inline int hashKey(int k){ 
    return k < 0 ? -k : k; 
}
inline int hashKey(const std::string& s){
    unsigned int h = 5381;
    for (int i = 0; i < (int)s.size(); i++)
        h = h * 31 + (unsigned char)s[i];
    return (int)(h & 0x7FFFFFFF);
}
template <typename K, typename V>
class HashTable{
    private:
        vector<vector<HashEntry<K, V>>> buckets;
        int capacity;
        int count;
        int indexOf(const K& k) const;
        void rehash();
    public:
        HashTable(int cap = 101);
        bool insert(const K& k, const V& v);   // true nếu thêm mới, false nếu cập nhật
        V* find(const K& k);                   // nullptr nếu không có
        bool remove(const K& k);
        int size() const;
        void forEach(void (*visit)(const K&, V&));
};

template <typename K, typename V>
HashTable<K, V>::HashTable(int cap) : buckets(cap), capacity(cap), count(0){
}

template <typename K, typename V>
int HashTable<K, V>::indexOf(const K& k) const{
    return hashKey(k) % capacity;
}

template <typename K, typename V>
void HashTable<K, V>::rehash(){
    int newCap = capacity * 2 + 1;
    vector<vector<HashEntry<K, V>>> newBuckets(newCap);
    for (int i = 0; i < capacity; i++){
        for (int j = 0; j < (int)buckets[i].size(); j++){
            int idx = hashKey(buckets[i][j].key) % newCap;
            newBuckets[idx].push_back(buckets[i][j]);
        }
    }
    buckets = newBuckets;
    capacity = newCap;
}

template <typename K, typename V>
bool HashTable<K, V>::insert(const K& k, const V& v){
    int idx = indexOf(k);
    for (int j = 0; j < (int)buckets[idx].size(); j++){
        if (buckets[idx][j].key == k){
            buckets[idx][j].value = v;
            return false;
        }
    }
    if (count + 1 > capacity * 3 / 4){
        rehash();
        idx = indexOf(k);
    }
    HashEntry<K, V> e;
    e.key = k;
    e.value = v;
    buckets[idx].push_back(e);
    count++;
    return true;
}

template <typename K, typename V>
V* HashTable<K, V>::find(const K& k){
    int idx = indexOf(k);
    for (int j = 0; j < (int)buckets[idx].size(); j++){
        if (buckets[idx][j].key == k) return &buckets[idx][j].value;
    }
    return nullptr;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K& k){
    int idx = indexOf(k);
    for (int j = 0; j < (int)buckets[idx].size(); j++){
        if (buckets[idx][j].key == k) {
            buckets[idx][j] = buckets[idx][buckets[idx].size() - 1];
            buckets[idx].pop_back();
            count--;
            return true;
        }
    }
    return false;
}

template <typename K, typename V>
int HashTable<K, V>::size() const{ 
    return count; 
}

template <typename K, typename V>
void HashTable<K, V>::forEach(void (*visit)(const K&, V&)){
    for (int i = 0; i < capacity; i++)
        for (int j = 0; j < (int)buckets[i].size(); j++)
            visit(buckets[i][j].key, buckets[i][j].value);
}
