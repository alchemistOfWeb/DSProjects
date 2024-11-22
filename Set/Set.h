#pragma once
#include "iostream"
/* 
  Your task here is to define a set of integers. Since this is not a template class 
  you must declare your functions in the header file and define them in .cpp file.
*/

class Set {
public:
    Set();
    bool empty();
    size_t size();

    bool contains(int value);
    void insert(int value);
    void erase(int value);
private:
    size_t m_size;
    struct TreeNode;
    TreeNode* m_treeRoot = nullptr;
};

struct TreeNode {
    // Implemented as an AVL Tree
    int value;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    //TreeNode();
};