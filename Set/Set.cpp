#include "Set.h"


Set::Set() : m_size(0) {}

Set::Set(const std::initializer_list<int>& init_list) {
    for (const int item : init_list) {
        insert(item);
    }
}

Set::~Set() noexcept {
    delete m_treeRoot;
}

bool Set::empty() {
    return !m_size;
}

size_t Set::size() {
    return m_size;
}

bool Set::contains(int value) {
    TreeNode** current = &m_treeRoot;
    
    while ((*current) != nullptr) {
        if (value == (*current)->m_value) return true;
        if (value > (*current)->m_value) {
            current = &(*current)->m_right;
            continue;
        }
        if (value < (*current)->m_value) {
            current = &(*current)->m_left;
            continue;
        }
    }
    return false;
}

void Set::insert(int value) {
    TreeNode** current = &m_treeRoot;
    while ((*current) != nullptr) {
        if (value == (*current)->m_value) return;
        if (value > (*current)->m_value) {
            current = &(*current)->m_right;
            continue;
        }
        if (value < (*current)->m_value) {
            current = &(*current)->m_left;
            continue;
        }
    }
    *current = new TreeNode(value); 
    m_size++;
    return;
}

void Set::erase(int value) {
    TreeNode** current = &m_treeRoot;

    // look for the node
    while (true) {
        if (value == (*current)->m_value) break;
        if (value > (*current)->m_value) {
            current = &(*current)->m_right;
            continue;
        }
        if (value < (*current)->m_value) {
            current = &(*current)->m_left;
            continue;
        }
        if (!(*current)) return;
    }
    bool hasRight = (*current)->m_right != nullptr;
    bool hasLeft = (*current)->m_left != nullptr;

    if (!hasLeft && hasRight) { // has only right
        TreeNode* tmp = (*current)->m_right;
        (*current)->m_right = nullptr;
        delete (*current);
        (*current) = tmp;
    }
    else if (hasLeft && !hasRight) { // has only left
        TreeNode* tmp = (*current)->m_left;
        (*current)->m_left = nullptr;
        delete (*current);
        (*current) = tmp;
    }
    else if (hasLeft && hasRight) { // has both
        // go to right then left left left...
        TreeNode** curr = &(*current)->m_right;
        while ((*curr)->m_left != nullptr) {
            curr = &(*curr)->m_left;
        }
        (*current)->m_value = (*curr)->m_value;
        TreeNode* tmp = (*curr)->m_right;
        (*curr)->m_right = nullptr;
        delete (*curr);
        (*curr) = tmp;
    }
    else { // has no children
        delete (*current);
    }
    m_size--;
}

// Tree 
////////////////////////////////////////////////////////////////////////////

Set::TreeNode::TreeNode(int value) : m_value(value) {}
