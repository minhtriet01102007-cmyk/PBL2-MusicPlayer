#pragma once
#include "bstnode.h"
#include <iostream>

class BST{
    private:
        BSTNode* root;
        int size;
        // Các hàm đệ quy phụ trợ nội bộ
        BSTNode* insertHelper(BSTNode* node, int value);
        BSTNode* removeHelper(BSTNode* node, int value);
        bool searchHelper(BSTNode* node, int value) const;
        void inorderHelper(BSTNode* node) const;
        void clearHelper(BSTNode* node);
        BSTNode* findMin(BSTNode* node) const;
    public:
        BST();
        ~BST();
        void insert(int value);
        void remove(int value);
        bool search(int value) const;
        void inorder() const; // Duyệt cây in ra thứ tự tăng dần
        bool IsEmpty() const;
        int getSize() const;
        void clear();
};
inline BST::BST(){
    this->root = nullptr;
    this->size = 0;
}
inline BST::~BST(){
    this->clear();
}
inline bool BST::IsEmpty() const{
    return this->root == nullptr;
}
inline int BST::getSize() const{
    return this->size;
}
inline BSTNode* BST::findMin(BSTNode* node) const{ // Tìm node nhỏ nhất ở cây con bên phải 
    while (node->left != nullptr){
        node = node->left;
    }
    return node;
}
inline BSTNode* BST::insertHelper(BSTNode* node, int value){
    if (node == nullptr){
        BSTNode* newNode = new BSTNode;
        newNode->data = value;
        newNode->left = nullptr;
        newNode->right = nullptr;
        this->size++;
        return newNode;
    }
    if (value < node->data){
        node->left = insertHelper(node->left, value);
    } else if (value > node->data){
        node->right = insertHelper(node->right, value);
    }
    return node;
}
inline void BST::insert(int value){
    this->root = insertHelper(this->root, value);
}
inline bool BST::searchHelper(BSTNode* node, int value) const{
    if (node == nullptr){
        return false;
    }
    if (node->data == value){
        return true;
    }
    if (value < node->data){
        return searchHelper(node->left, value);
    }
    return searchHelper(node->right, value);
}
inline bool BST::search(int value) const{
    return searchHelper(this->root, value);
}
inline BSTNode* BST::removeHelper(BSTNode* node, int value){
    if (node == nullptr){
        return nullptr;
    }
    if (value < node->data){
        node->left = removeHelper(node->left, value);
    } else if (value > node->data){
        node->right = removeHelper(node->right, value);
    } else {
        if (node->left == nullptr){
            BSTNode* temp = node->right;
            delete node;
            this->size--;
            return temp;
        }
        else if (node->right == nullptr){
            BSTNode* temp = node->left;
            delete node;
            this->size--;
            return temp;
        }
        BSTNode* temp = findMin(node->right);
        node->data = temp->data;
        node->right = removeHelper(node->right, temp->data);
    }
    return node;
}
inline void BST::remove(int value){
    this->root = removeHelper(this->root, value);
}
inline void BST::inorderHelper(BSTNode* node) const{
    if (node != nullptr){
        inorderHelper(node->left);
        std::cout << node->data << " ";
        inorderHelper(node->right);
    }
}
inline void BST::inorder() const{
    if (IsEmpty()){
        std::cout << "Cay BST dang rong" << std::endl;
        return;
    }
    std::cout << "Inorder Traversal: ";
    inorderHelper(this->root);
    std::cout << std::endl;
}
inline void BST::clearHelper(BSTNode* node){
    if (node != nullptr){
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }
}
inline void BST::clear(){
    clearHelper(this->root);
    this->root = nullptr;
    this->size = 0;
}
