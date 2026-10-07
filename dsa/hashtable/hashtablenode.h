#pragma once

template <typename K, typename V>
struct HashNode{
    K key;
    V value;
    HashNode* next;
    HashNode(const K& k, const V& v);
};

template <typename K, typename V>
HashNode<K, V>::HashNode(const K& k, const V& v) : key(k), value(v), next(nullptr){}
