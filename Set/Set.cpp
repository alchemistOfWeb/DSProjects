#include "Set.h"


Set::Set() : m_size(0) {
    createFakeEnd();
}

Set::Set(const Set& other) {
    m_size = other.m_size;
    m_max = other.m_max;
    m_min = other.m_min;
    m_treeRoot = deepCopy(other.m_treeRoot);
}

Set::Set(const std::initializer_list<int>& init_list) {
    createFakeEnd();
    for (const int item : init_list) {
        insert(item);
    } // m_max replacement issue
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

    // balanceSubtree(*current);
    // TODO; after each insert go up to the root of the tree checking balance factor;
    // if balance of left 
    TreeNode* currRoot = parent;
    while (currRoot) {
        int leftHeight = height(currRoot->m_left);
        int rightHeight = height(currRoot->m_right);

        if (leftHeight - rightHeight >= 2) {
            // do right turn
            TreeNode* heir = currRoot->m_left;
            if (height(heir->m_right) > height(heir->m_left)) {
                leftTurn(heir);
                heir->m_parent->m_height++;
            }
            rightTurn(currRoot);
            break;
        }
        else if (rightHeight - leftHeight >= 2) {
            // do left turn
            TreeNode* heir = currRoot->m_right;
            if (height(heir->m_left) > height(heir->m_right)) {
                rightTurn(heir);
                heir->m_parent->m_height++;
            }
            leftTurn(currRoot);

            break;
        }
        else {
            currRoot->m_height = std::max(leftHeight, rightHeight) + 1;
        }
        currRoot = currRoot->m_parent;
    }
    return;
}

int Set::height(Set::TreeNode* node) {
    return node != nullptr ? node->m_height : 0;
}

void Set::leftTurn(Set::TreeNode* currRoot) {
    TreeNode* heir = currRoot->m_right;
    if (!heir->m_left) currRoot->m_height--;
    currRoot->m_right = heir->m_left;
    TreeNode* tmpParent = currRoot->m_parent;
    
    // if we dont deal with m_treeRoot then tmpParent cannot be nullptr
    if (currRoot == m_treeRoot) {
        m_treeRoot = heir;
    }
    else {
        (currRoot == tmpParent->m_right ? tmpParent->m_right : tmpParent->m_left) = heir;
    }

    currRoot->m_parent = heir;
    heir->m_parent = tmpParent;
    heir->m_left = currRoot;

    currRoot = heir;
}

void Set::rightTurn(Set::TreeNode* currRoot) {
    TreeNode* heir = currRoot->m_left;
    if (!heir->m_right) currRoot->m_height--;
    currRoot->m_left = heir->m_right;
    TreeNode* tmpParent = currRoot->m_parent;
    
    // if we dont deal with m_treeRoot then tmpParent cannot be nullptr
    if (currRoot == m_treeRoot) {
        m_treeRoot = heir;
    }
    else {
        (currRoot == tmpParent->m_right ? tmpParent->m_right : tmpParent->m_left) = heir;
    }

    currRoot->m_parent = heir;
    heir->m_parent = tmpParent;
    heir->m_right = currRoot;
    currRoot = heir;
}

void Set::erase(int value) {
    //TreeNode** current = &m_treeRoot;

    iterator pos = find(value);

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
    if ((*current)==nullptr) return;
    commonErase(current);

    //// look for the node
    //while (true) {
    //    if (value == (*current)->m_value) break;
    //    if (value > (*current)->m_value) {
    //        current = &(*current)->m_right;
    //        continue;
    //    }
    //    if (value < (*current)->m_value) {
    //        current = &(*current)->m_left;
    //        continue;
    //    }    
    //    if ((*current) == nullptr || (*current) == m_fakeEnd) return;
    //}

    //bool hasRight = ((*current)->m_right != nullptr) && ((*current)->m_right != m_fakeEnd);
    //bool hasLeft = (*current)->m_left != nullptr;

    //if (!hasLeft && hasRight) { // has only right
    //    TreeNode* tmp = (*current)->m_right;
    //    tmp->m_parent = (*current)->m_parent;

    //    if ((*current) == m_min) {
    //        TreeNode** curr = &(*current)->m_right;
    //        while ((*curr)->m_left != nullptr) {
    //            curr = &(*curr)->m_left;
    //        }
    //        m_min = (*curr);
    //    }

    //    (*current)->m_right = nullptr;
    //    (*current)->m_parent = nullptr;

    //    if (*current != m_treeRoot) {
    //        delete (*current);
    //        (*current) = tmp;
    //    }
    //    else {
    //        delete m_treeRoot;
    //        m_treeRoot = tmp;
    //    }
    //}
    //else if (hasLeft && !hasRight) { // has only left
    //    TreeNode* tmp = (*current)->m_left;
    //    tmp->m_parent = (*current)->m_parent;
    //    (*current)->m_left = nullptr;
    //    (*current)->m_parent = nullptr;

    //    if ((*current)->m_right == m_fakeEnd) {
    //        (*current)->m_right = nullptr;
    //        TreeNode* tmpcurr = tmp;
    //        while (tmpcurr->m_right != nullptr) {
    //            tmpcurr = tmpcurr->m_right;
    //        }
    //        m_max = tmpcurr;
    //        tmpcurr->m_right = m_fakeEnd;
    //    }

    //    delete (*current);
    //    (*current) = tmp;
    //}
    //else if (hasLeft && hasRight) { // has both
    //    // go to right then left left left...
    //    TreeNode** curr = &(*current)->m_right;
    //    while ((*curr)->m_left != nullptr) {
    //        curr = &(*curr)->m_left;
    //    }
    //    (*current)->m_value = (*curr)->m_value;
    //    TreeNode* tmp = (*curr)->m_right;
    //    if (tmp != nullptr) {
    //        tmp->m_parent = (*curr)->m_parent;
    //    }
    //    (*curr)->m_right = nullptr;
    //    (*curr)->m_parent = nullptr;
    //    delete (*curr);
    //    (*curr) = tmp;
    //}
    //else { // has no children
    //    if ((*current) == m_max) {
    //        (*current)->m_right = nullptr;
    //        m_max = (*current)->m_parent;
    //    }
    //    delete (*current);
    //    m_max->m_right = m_fakeEnd;
    //}
    //m_size--;
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

    commonErase(current);
}

void Set::commonErase(TreeNode** current) {
    bool hasRight = ((*current)->m_right != nullptr) && ((*current)->m_right != m_fakeEnd);
    bool hasLeft = (*current)->m_left != nullptr;

    if (!hasLeft && hasRight) { // has only right
        TreeNode* tmp = (*current)->m_right;
        tmp->m_parent = (*current)->m_parent;
        if ((*current) == m_min) {
            TreeNode** curr = &(*current)->m_right;
            while ((*curr)->m_left != nullptr) {
                curr = &(*curr)->m_left;
            }
            m_min = (*curr);
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
void Set::createFakeEnd() {
    m_fakeEnd = new TreeNode(-1, nullptr, 0);
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

Set::TreeNode::TreeNode(int value, Set::TreeNode* parent, int height) 
    : m_value(value), m_parent(parent), m_height(height) {}

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
