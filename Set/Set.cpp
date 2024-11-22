#include "Set.h"


Set::Set() : m_size(0) {}

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
    *current = new TreeNode(value); // *current = or **current =???
    m_size++;
    return;
}

void Set::erase(int value) {

}

// Tree 
////////////////////////////////////////////////////////////////////////////

Set::TreeNode::TreeNode(int value) : m_value(value) {}
