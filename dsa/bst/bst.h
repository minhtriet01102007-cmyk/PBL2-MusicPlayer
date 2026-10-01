#pragma once
#include "bstnode.h"

template <typename K, typename V>
class BST{
    private:
        BSTNode<K, V>* root;
        BSTNode<K, V>* insertNode(BSTNode<K, V>* node, const K& k, const V& v);
        void inOrderNode(BSTNode<K, V>* node, void (*visit)(const K&, V&));
        void destroy(BSTNode<K, V>* node);
    public:
        BST();
        ~BST();
        void insert(const K& k, const V& v);
        V* find(const K& k);
        void inOrder(void (*visit)(const K&, V&));
};

template <typename K, typename V>
BST<K, V>::BST() : root(nullptr){
}

template <typename K, typename V>
BST<K, V>::~BST(){ 
    destroy(root); 
}

template <typename K, typename V>
void BST<K, V>::destroy(BSTNode<K, V>* node){
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

template <typename K, typename V>
BSTNode<K, V>* BST<K, V>::insertNode(BSTNode<K, V>* node, const K& k, const V& v){
    if (node == nullptr) return new BSTNode<K, V>(k, v);
    if (k < node->key) node->left = insertNode(node->left, k, v);
    else if (node->key < k) node->right = insertNode(node->right, k, v);
    else node->value = v;   // trùng khóa thì cập nhật giá trị
    return node;
}

template <typename K, typename V>
void BST<K, V>::insert(const K& k, const V& v){ 
    root = insertNode(root, k, v); 
}

template <typename K, typename V>
V* BST<K, V>::find(const K& k){
    BSTNode<K, V>* cur = root;
    while (cur != nullptr){
        if (k < cur->key) cur = cur->left;
        else if (cur->key < k) cur = cur->right;
        else return &cur->value;
    }
    return nullptr;
}

template <typename K, typename V>
void BST<K, V>::inOrderNode(BSTNode<K, V>* node, void (*visit)(const K&, V&)){
    if (node == nullptr) return;
    inOrderNode(node->left, visit);
    visit(node->key, node->value);
    inOrderNode(node->right, visit);
}

template <typename K, typename V>
void BST<K, V>::inOrder(void (*visit)(const K&, V&)){
    inOrderNode(root, visit); 
}
