#pragma once
#include "iostream"
/* 
  Your task here is to define a set of integers. Since this is not a template class 
  you must declare your functions in the header file and define them in .cpp file.
*/

class Set {
public:
    Set();
    Set(const Set& other);
    Set(const std::initializer_list<int>& init_list);
    ~Set() noexcept;

    bool empty();
    size_t size();
    bool contains(int value);
    void insert(int value);
    void erase(int value);
    


    Set& operator=(const Set& other);
    bool operator==(const Set& other) const;
    bool operator!=(const Set& other) const;

    class iterator;

    iterator find(int value) const;
    iterator begin() const;
    iterator end() const;
    iterator upper_bound(int value) const;
    iterator lower_bound(int value) const;
    void erase(iterator pos);

private:
    size_t m_size;
    struct TreeNode;
    TreeNode* m_treeRoot = nullptr;
    TreeNode* m_fakeEnd = nullptr;

    TreeNode* m_min = nullptr;
    TreeNode* m_max = nullptr;

    TreeNode* deepCopy(TreeNode* node);
    bool deepCheckEqual(const Set& other) const;
    void replaceNodeInParent(TreeNode* node, TreeNode* newNode);
    TreeNode* findMin(TreeNode* node) const;
    TreeNode* findMax(TreeNode* node) const;
    void commonErase(TreeNode** current);
};

struct Set::TreeNode {
    // Implemented as an AVL Tree
    int m_value;
    TreeNode* m_left = nullptr;
    TreeNode* m_right = nullptr;
    TreeNode* m_parent = nullptr; // TODO: use it in all addings of a node
    TreeNode(int value, TreeNode* parent=nullptr);
    //TreeNode();
};

class Set::iterator {
    friend class Set;
public:
    //using iterator_category = std::forward_iterator_tag;
    using value_type = const int;
    //using pointer = const int*;
    //using reference = const int&;
    using difference_type = std::ptrdiff_t;

    iterator(TreeNode* node = nullptr);
    const int& operator*();
    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);
    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;
private:
    TreeNode* m_current;

    void goNext();
};