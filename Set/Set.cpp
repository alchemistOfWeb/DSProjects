#include "Set.h"


Set::Set() : m_size(0) {}

bool Set::empty() {
    return this->m_treeRoot == nullptr;
}

size_t Set::size() {
    return this->m_size;
}

bool Set::contains(int value) {
    return false;
}

void Set::insert(int value) {
    
}

void Set::erase(int value) {

}

// Tree 
////////////////////////////////////////////////////////////////////////////
//Set::Tree::Tree() :value(0) {}
