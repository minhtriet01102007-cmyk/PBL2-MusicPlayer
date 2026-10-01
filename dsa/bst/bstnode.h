#pragma once
template <typename K, typename V>
struct BSTNode {
    K key;
    V value;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const K& k, const V& v);
};
template <typename K, typename V>
BSTNode<K, V>::BSTNode(const K& k, const V& v) : key(k), value(v), left(nullptr), right(nullptr) {}

