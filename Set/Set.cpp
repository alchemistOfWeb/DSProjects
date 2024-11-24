#include "Set.h"


Set::Set() : m_size(0) {}

Set::Set(const Set& other) {
    m_size = other.m_size;
    m_treeRoot = other.m_treeRoot;
}

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
    TreeNode* parent = nullptr;
    bool ismin = true;
    bool ismax = true;
    while ((*current) != nullptr) {
        if (value == (*current)->m_value) return;
        parent = *current;
        if (value > (*current)->m_value) {
            
            current = &(*current)->m_right;
            ismin = false;
            continue;
        }
        if (value < (*current)->m_value) {
            current = &(*current)->m_left;
            ismax = false;
            continue;
        }
    }
    *current = new TreeNode(value, parent);
    if (ismin) m_min = *current;
    if (ismax) m_max = *current;
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


// Iteration methods
////////////////////////////////////////////////////////////////////////////
Set::iterator Set::begin() {
    return iterator(m_min);
}

Set::iterator Set::end() {
    return iterator(m_max);
}


// Tree 
////////////////////////////////////////////////////////////////////////////

Set::TreeNode::TreeNode(int value, Set::TreeNode* parent) : m_value(value), m_parent(parent) {}

// Iterator
////////////////////////////////////////////////////////////////////////////
Set::iterator::iterator(Set::TreeNode* node) : m_current(node) {}

const int& Set::iterator::operator*() {
    return m_current->m_value;
}

Set::iterator Set::iterator::operator++(int) {
    // post inc operator
    Set::iterator tmp = *this;
    this->goNext();
    return tmp;
}

Set::iterator& Set::iterator::operator++() {
    // pre inc operator
    this->goNext();
    return *this;
}

bool Set::iterator::operator==(const iterator& other) const {
    return m_current == other.m_current;
}

bool Set::iterator::operator!=(const iterator& other) const {
    return m_current != other.m_current;
}

// Iterator helper:

void Set::iterator::goNext() {
    // In the start of each iteration we are already on the leftest node
    // looked position so we can go either right or up
    if (m_current->m_right != nullptr) {
        m_current = m_current->m_right;

        if (m_current->m_left != nullptr) {
            // go only left... untill we reach the leftest node
            m_current = m_current->m_left;
            while (m_current->m_left != nullptr)
                m_current = m_current->m_left;
        } // else: there are no lefter elements but all righter elements have greater values

        return;
    }

    if (m_current->m_parent != nullptr) {
        TreeNode* prev = nullptr;
        do {
            prev = m_current;
            m_current = m_current->m_parent;
        } while (m_current->m_right == prev);
    }

    return;
}
