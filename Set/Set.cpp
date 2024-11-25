#include "Set.h"


Set::Set() : m_size(0) {
    m_fakeEnd = new TreeNode(-1);
}

Set::Set(const Set& other) {
    m_size = other.m_size;
    m_max = other.m_max;
    m_min = other.m_min;
    m_treeRoot = deepCopy(other.m_treeRoot);
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
    bool fakeEndReached = false;

    while ((*current) != nullptr) {
        if (value == (*current)->m_value) return;
        parent = *current;
        if (value > (*current)->m_value) {
            current = &(*current)->m_right;
            ismin = false;
            if ((*current) == m_fakeEnd) {
                fakeEndReached = true;
                break;
            }
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
    
    if (ismax || fakeEndReached) {
        m_max = *current;
        (*current)->m_right = m_fakeEnd;
    }
    
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
        
        if (!(*current) || (*current) == m_fakeEnd) return;
    }
    
    bool hasRight = ((*current)->m_right != nullptr) && ((*current)->m_right != m_fakeEnd);
    bool hasLeft = (*current)->m_left != nullptr;

    if (!hasLeft && hasRight) { // has only right
        TreeNode* tmp = (*current)->m_right;
        if ((*current) == m_min) {
            m_min = (*current)->m_right;
        }
        (*current)->m_right = nullptr;
        delete (*current);
        (*current) = tmp;
    }
    else if (hasLeft && !hasRight) { // has only left
        TreeNode* tmp = (*current)->m_left;
        (*current)->m_left = nullptr;

        if ((*current)->m_right == m_fakeEnd) {
            (*current)->m_right = nullptr;
            TreeNode* tmpcurr = tmp;
            while (tmpcurr->m_right != nullptr)
            {
                tmpcurr = tmpcurr->m_right;
            }
            m_max = tmpcurr;
            tmpcurr->m_right = m_fakeEnd;
        }

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
        if ((*current) == m_max) {
            (*current)->m_right = nullptr;
            m_max = (*current)->m_parent;
        }
        delete (*current);
        m_max->m_right = m_fakeEnd;
    }
    m_size--;
}


Set& Set::operator=(const Set& other) {
    m_size = other.m_size;
    m_max = other.m_max;
    m_min = other.m_min;
    m_treeRoot = deepCopy(other.m_treeRoot);
    return *this;
}

bool Set::operator==(const Set& other) const {
    return deepCheckEqual(other);
}

bool Set::operator!=(const Set& other) const {
    return !deepCheckEqual(other);
}

// Iteration methods
////////////////////////////////////////////////////////////////////////////
Set::iterator Set::begin() const {
    return iterator(m_min);
}

Set::iterator Set::end() const {
    return iterator(m_fakeEnd);
    //return iterator(nullptr);
}

Set::iterator Set::find(int value) const {
    TreeNode* current = m_treeRoot;
    while (current != nullptr) {
        if (current->m_value == value) {
            return iterator(current);
        }
        current = value > current->m_value ? current->m_right : current->m_left;
    }
    return iterator(m_fakeEnd);
}

Set::iterator Set::upper_bound(int value) const {
    TreeNode* current = m_treeRoot;
    TreeNode* result = m_fakeEnd;

    while (current != nullptr) {
        if (current->m_value > value) {
            result = current;
            current = current->m_left;
        }
        else {
            current = current->m_right; 
        }
    }

    return iterator(result);
}

Set::iterator Set::lower_bound(int value) const {
    TreeNode* current = m_treeRoot;
    TreeNode* result = m_fakeEnd;

    while (current != nullptr) {
        if (current->m_value >= value) {
            result = current;
            current = current->m_left;
        }
        else {
            current = current->m_right;
        }
    }

    return iterator(result);
}

void Set::erase(iterator pos) {
    if (pos == end()) return;

    TreeNode** current = &(pos.m_current);
    
    if (!(*current) || (*current) == m_fakeEnd) return;
    if ((*current)->m_parent) {
        if ((*current)->m_parent->m_right == (*current)) {
            current = &((*current)->m_parent->m_right);
        }
        else {
            current = &((*current)->m_parent->m_left);
        }
    }

    bool hasRight = ((*current)->m_right != nullptr) && ((*current)->m_right != m_fakeEnd);
    bool hasLeft = (*current)->m_left != nullptr;

    if (!hasLeft && hasRight) { // has only right
        TreeNode* tmp = (*current)->m_right;
        tmp->m_parent = (*current)->m_parent;
        if ((*current) == m_min) {
            // TODO: make loop for searching min element from m_right 
            TreeNode** curr = &(*current)->m_right;
            while ((*curr)->m_left != nullptr) {
                curr = &(*curr)->m_left;
            }
            m_min = (*curr);
            //m_min = (*current)->m_right;
        }
        (*current)->m_right = nullptr;
        (*current)->m_parent = nullptr;
        if (*current != m_treeRoot) {
            delete (*current);
            (*current) = tmp;
        }
        else {
            delete m_treeRoot;
            m_treeRoot = tmp;
        }
    }
    else if (hasLeft && !hasRight) { // has only left
        TreeNode* tmp = (*current)->m_left;
        tmp->m_parent = (*current)->m_parent;
        (*current)->m_left = nullptr;
        (*current)->m_parent = nullptr;

        if ((*current)->m_right == m_fakeEnd) {
            (*current)->m_right = nullptr;
            TreeNode* tmpcurr = tmp;
            while (tmpcurr->m_right != nullptr) {
                tmpcurr = tmpcurr->m_right;
            }
            m_max = tmpcurr;
            tmpcurr->m_right = m_fakeEnd;
            //m_fakeEnd->m_parent = tmpcurr; // does it really need ????
        }

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
        if (tmp != nullptr) {
            tmp->m_parent = (*curr)->m_parent;
        }
        (*curr)->m_right = nullptr;
        (*curr)->m_parent = nullptr;
        delete (*curr);
        (*curr) = tmp;
    }
    else { // has no children
        if (m_treeRoot == (*current)) {
            m_treeRoot = m_min = m_fakeEnd;
        }
        if ((*current) == m_min) {
            m_min = (*current)->m_parent;
        }
        if ((*current) == m_max) {
            (*current)->m_right = nullptr;
            m_max = (*current)->m_parent;
            if (m_max != nullptr) {
                m_max->m_right = m_fakeEnd;
            }
        }
        if ((*current)->m_parent) {
            if ((*current)->m_parent->m_right == (*current)) {
                (*current)->m_parent->m_right = nullptr;
            }
            else {
                (*current)->m_parent->m_left = nullptr;
            }
        }
        delete (*current);
    }
    m_size--;
}

// Private methods
////////////////////////////////////////////////////////////////////////////
void Set::replaceNodeInParent(TreeNode* node, TreeNode* newNode) {
    if (node->m_parent) {
        if (node == node->m_parent->m_left) {
            node->m_parent->m_left = newNode;
        }
        else if (node == node->m_parent->m_right) {
            node->m_parent->m_right = newNode;
        }
    }
    else {
        m_treeRoot = newNode; 
    }

    if (newNode) {
        newNode->m_parent = node->m_parent;
    }
}

Set::TreeNode* Set::findMin(TreeNode* node) const {
    while (node && node->m_left != nullptr) {
        node = node->m_left;
    }
    return node;
}

Set::TreeNode* Set::findMax(TreeNode* node) const {
    while (node && node->m_right != m_fakeEnd) {
        node = node->m_right;
    }
    return node;
}



Set::TreeNode* Set::deepCopy(Set::TreeNode* node) {
    TreeNode* newnode =  new TreeNode(node->m_value, node->m_parent);
    newnode->m_left = node->m_left != nullptr ? deepCopy(node->m_left) : nullptr;
    newnode->m_right = node->m_right != nullptr ? deepCopy(node->m_right) : nullptr;
    return newnode;
}

bool Set::deepCheckEqual(const Set& other) const {
    iterator it = begin();
    iterator itOther = other.begin();

    iterator thisEnd = iterator(m_max);
    iterator otherEnd = iterator(other.m_max);
    while ((it != thisEnd) && (itOther != otherEnd)) {
        if (*it != *itOther) return false;
        ++it;
        ++itOther;
    }

    if (it == thisEnd && itOther == otherEnd) return true;
    return false;
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

void Set::iterator::goNext() {
    // In the start of each iteration we are already on the leftest node
    // looked position so we can go either right or up
    if (m_current == nullptr) {
        return;
    }
    
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

    while (m_current->m_parent != nullptr && m_current == m_current->m_parent->m_right) {
        m_current = m_current->m_parent;
    }

    m_current = m_current->m_parent;
}
