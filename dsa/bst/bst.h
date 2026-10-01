#pragma once
#include "bstnode.h"
#include <vector>
#include <string>

template <typename K, typename V>
class BST{
    private:
        BSTNode<K, V>* root;
        BSTNode<K, V>* insertNode(BSTNode<K, V>* node, const K& k, const V& v);
        BSTNode<K, V>* removeNode(BSTNode<K, V>* node, const K& k, bool& success);
        BSTNode<K, V>* findMinNode(BSTNode<K, V>* node);
        void inOrderNode(BSTNode<K, V>* node, void (*visit)(const K&, V&)) const;
        void prefixSearchNode(BSTNode<K, V>* node, const K& prefix, std::vector<V>& results) const;
        void destroy(BSTNode<K, V>* node);
    public:
        BST();
        ~BST();
        BST(const BST&) = delete;
        BST& operator=(const BST&) = delete;
        void insert(const K& k, const V& v);
        bool remove(const K& k);
        V* find(const K& k);
        void inOrder(void (*visit)(const K&, V&)) const;
        std::vector<V> searchByPrefix(const K& prefix) const;
        bool empty() const;
        void clear();
};
template <typename K, typename V>
BST<K, V>::BST() : root(nullptr) {}

template <typename K, typename V>
BST<K, V>::~BST() {
    destroy(root);
}

template <typename K, typename V>
void BST<K, V>::destroy(BSTNode<K, V>* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}
template <typename K, typename V>
BSTNode<K, V>* BST<K, V>::insertNode(BSTNode<K, V>* node, const K& k, const V& v) {
    if (node == nullptr) {
        return new BSTNode<K, V>(k, v);
    }
    if (k < node->key) {
        node->left = insertNode(node->left, k, v);
    } else if (node->key < k) {
        node->right = insertNode(node->right, k, v);
    } else {
        node->value = v; // Cập nhật nếu trùng key
    }
    return node;
}

template <typename K, typename V>
BSTNode<K, V>* BST<K, V>::findMinNode(BSTNode<K, V>* node) {
    BSTNode<K, V>* cur = node;
    while (cur && cur->left != nullptr) {
        cur = cur->left;
    }
    return cur;
}

template <typename K, typename V>
BSTNode<K, V>* BST<K, V>::removeNode(BSTNode<K, V>* node, const K& k, bool& success) {
    if (node == nullptr) {
        success = false;
        return nullptr;
    }

    if (k < node->key) {
        node->left = removeNode(node->left, k, success);
    } else if (node->key < k) {
        node->right = removeNode(node->right, k, success);
    } else {
        success = true;
        // Trường hợp 0 hoặc 1 node con
        if (node->left == nullptr) {
            BSTNode<K, V>* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            BSTNode<K, V>* temp = node->left;
            delete node;
            return temp;
        }

        // Trường hợp có 2 node con: lấy giá trị nhỏ nhất bên cây con phải
        BSTNode<K, V>* temp = findMinNode(node->right);
        node->key = temp->key;
        node->value = temp->value;
        node->right = removeNode(node->right, temp->key, success);
    }
    return node;
}

template <typename K, typename V>
void BST<K, V>::inOrderNode(BSTNode<K, V>* node, void (*visit)(const K&, V&)) const {
    if (node == nullptr) return;
    inOrderNode(node->left, visit);
    visit(node->key, node->value);
    inOrderNode(node->right, visit);
}

template <typename K, typename V>
void BST<K, V>::prefixSearchNode(BSTNode<K, V>* node, const K& prefix, std::vector<V>& results) const {
    if (node == nullptr) return;
    prefixSearchNode(node->left, prefix, results);
    if (node->key.rfind(prefix, 0) == 0) {
        results.push_back(node->value);
    }
    prefixSearchNode(node->right, prefix, results);
}

template <typename K, typename V>
void BST<K, V>::insert(const K& k, const V& v) {
    root = insertNode(root, k, v);
}

template <typename K, typename V>
bool BST<K, V>::remove(const K& k) {
    bool success = false;
    root = removeNode(root, k, success);
    return success;
}

template <typename K, typename V>
V* BST<K, V>::find(const K& k) {
    BSTNode<K, V>* cur = root;
    while (cur != nullptr) {
        if (k < cur->key) cur = cur->left;
        else if (cur->key < k) cur = cur->right;
        else return &(cur->value);
    }
    return nullptr;
}

template <typename K, typename V>
void BST<K, V>::inOrder(void (*visit)(const K&, V&)) const {
    inOrderNode(root, visit);
}

template <typename K, typename V>
std::vector<V> BST<K, V>::searchByPrefix(const K& prefix) const {
    std::vector<V> results;
    prefixSearchNode(root, prefix, results);
    return results;
}

template <typename K, typename V>
bool BST<K, V>::empty() const {
    return root == nullptr;
}

template <typename K, typename V>
void BST<K, V>::clear() {
    destroy(root);
    root = nullptr;
}
