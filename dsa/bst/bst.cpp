#include "bst.h"
#include <iostream>

BST::BST()
{
    root = nullptr;
}

BST::~BST()
{
    destroy(root);
}

BSTNode* BST::insert(BSTNode* node, int value)
{
    if (node == nullptr)
    {
        BSTNode* newNode = new BSTNode;

        newNode->data = value;
        newNode->left = nullptr;
        newNode->right = nullptr;

        return newNode;
    }

    if (value < node->data)
    {
        node->left = insert(node->left, value);
    }
    else if (value > node->data)
    {
        node->right = insert(node->right, value);
    }

    return node;
}

void BST::insert(int value)
{
    root = insert(root, value);
}

bool BST::search(BSTNode* node, int value) const
{
    if (node == nullptr)
    {
        return false;
    }

    if (node->data == value)
    {
        return true;
    }

    if (value < node->data)
    {
        return search(node->left, value);
    }

    return search(node->right, value);
}

bool BST::search(int value) const
{
    return search(root, value);
}

void BST::inorder(BSTNode* node) const
{
    if (node == nullptr)
    {
        return;
    }

    inorder(node->left);

    std::cout << node->data << " ";

    inorder(node->right);
}

void BST::inorder() const
{
    inorder(root);
    std::cout << std::endl;
}

void BST::preorder(BSTNode* node) const
{
    if (node == nullptr)
    {
        return;
    }

    std::cout << node->data << " ";

    preorder(node->left);
    preorder(node->right);
}

void BST::preorder() const
{
    preorder(root);
    std::cout << std::endl;
}

void BST::postorder(BSTNode* node) const
{
    if (node == nullptr)
    {
        return;
    }

    postorder(node->left);
    postorder(node->right);

    std::cout << node->data << " ";
}

void BST::postorder() const
{
    postorder(root);
    std::cout << std::endl;
}

void BST::destroy(BSTNode* node)
{
    if (node == nullptr)
    {
        return;
    }

    destroy(node->left);
    destroy(node->right);

    delete node;
}