#pragma once

struct BSTNode
{
    int data;
    BSTNode* left;
    BSTNode* right;
};

struct BST
{
private:
    BSTNode* root;

    BSTNode* insert(BSTNode* node, int value);
    bool search(BSTNode* node, int value) const;

    void inorder(BSTNode* node) const;
    void preorder(BSTNode* node) const;
    void postorder(BSTNode* node) const;

    void destroy(BSTNode* node);

public:
    BST();
    ~BST();

    void insert(int value);
    bool search(int value) const;

    void inorder() const;
    void preorder() const;
    void postorder() const;
};