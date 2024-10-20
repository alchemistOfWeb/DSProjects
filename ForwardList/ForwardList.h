#pragma once
// Create a custom singly linked list class that would support all the functionality
// given in main.cpp.


#include <stdexcept>
#include <initializer_list> 
#include <iostream>


template <class T>
class ForwardList
{
public:
    ForwardList<T>(std::size_t size=0);
    ForwardList<T>(std::size_t size, const T& filler);
    
    bool empty();
    T& front();
    const T& front() const;
    void push_front(const T& value);
    void pop_front();
    bool operator==(const ForwardList<T>& other) const;
    bool operator!=(const ForwardList<T>& other) const;
    ForwardList(const ForwardList<T>& other);
    ForwardList<T>& operator=(const ForwardList<T>& other);
    
    template<class... Args>
    void emplace_front(Args&&... args);
    void clear();

    ~ForwardList() noexcept;


    class iterator;
    iterator begin();
    iterator end();

    class const_iterator;
    const_iterator begin() const;
    const_iterator end() const;

private:
    struct Node {
        T value;
        Node* next;

        Node(): value(T()), next(nullptr) {}
        Node(const T& value, Node* next = nullptr) : value(value), next(next) {}
    };

    Node* head = nullptr;
    std::size_t m_size;

    void clean();
};


// ITERATOR IMPLEMENTATION
template <class T>
class ForwardList<T>::iterator {
    friend class ForwardList<T>::const_iterator;
public:
    iterator(Node* node) : current(node) {}

    T& operator*() {
        return current->value;
    }

    T* operator->() {
        return &current->value;
    }

    iterator& operator++() {
        current = current->next;
        return *this;
    }

    iterator operator++(int) {
        iterator tmp = *this;
        current = current->next;
        return tmp;
    }

    bool operator==(const iterator& other) const {
        return current == other.current;
    }

    bool operator!=(const iterator& other) const {
        return current != other.current;
    }

private:
    Node* current;
};

template <class T>
ForwardList<T>::iterator ForwardList<T>::begin() {
    return iterator(head);
}

template <class T>
ForwardList<T>::iterator ForwardList<T>::end() {
    return iterator(nullptr);
}

// END ITERATOR IMPLEMENTATION



// CONST ITERATOR IMPLEMENTATION
template <class T>
class ForwardList<T>::const_iterator {
public:
    const_iterator(Node* node) : current(node) {}
    const_iterator(const iterator& it) : current(it.current) {}

    const T& operator*() const {
        return current->value;
    }

    const T* operator->() const {
        return &current->value;
    }

    const_iterator& operator++() {
        current = current->next;
        return *this;
    }

    const_iterator operator++(int) {
        const_iterator tmp = *this;
        current = current->next;
        return tmp;
    }

    bool operator==(const const_iterator& other) const {
        return current == other.current;
    }

    bool operator!=(const const_iterator& other) const {
        return current != other.current;
    }

private:
    Node* current;
};

template <class T>
ForwardList<T>::const_iterator ForwardList<T>::begin() const {
    return const_iterator(head);
}

template <class T>
ForwardList<T>::const_iterator ForwardList<T>::end() const {
    return const_iterator(nullptr);
}

// END CONST ITERATOR IMPLEMENTATION



template<class T>
ForwardList<T>::ForwardList(std::size_t size) : m_size(size) {
    Node** current = &head;
    for (std::size_t i = 0; i < size; ++i) {
        *current = new Node{};
        current = &((*current)->next);
    }

}

template<class T>
ForwardList<T>::ForwardList(std::size_t size, const T& filler) : m_size(size) {
    Node** current = &head;
    for (std::size_t i = 0; i < size; ++i) {
        *current = new Node{filler, nullptr};
        current = &((*current)->next);
    }
}

template<class T>
bool ForwardList<T>::empty() {
    return m_size == 0;
}

template<class T>
T& ForwardList<T>::front() {
    return head->value;
}

template<class T>
const T& ForwardList<T>::front() const {
    return head->value; 
}


template<class T>
void ForwardList<T>::push_front(const T& value) {
    Node* current = new Node{value, head};
    head = current;
    m_size++;
}


template<class T>
void ForwardList<T>::pop_front() {
    if (head == nullptr) {
        throw std::out_of_range("List is empty");
    }
    Node* current = head->next;
    delete head;
    head = current;
    m_size--;
}


template<class T>
void ForwardList<T>::clear() {
    clean();
}


template<class T>
void ForwardList<T>::clean() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    m_size = 0;
}


template<class T>
ForwardList<T>::ForwardList(const ForwardList<T>& other) : m_size(other.m_size), head(nullptr) {
    if (!other.head) {
        return;
    }

    head = new Node{ other.head->value, nullptr };
    Node* current = head;
    Node* otherCurrent = other.head->next;

    while (otherCurrent) {
        current->next = new Node{ otherCurrent->value, nullptr };
        current = current->next;
        otherCurrent = otherCurrent->next;
    }
}


template<class T>
bool ForwardList<T>::operator==(const ForwardList<T>& other) const {
    if (m_size != other.m_size) {
        return false;
    }

    Node* current = head;
    Node* otherCurrent = other.head;

    while (current && otherCurrent) {
        if (current->value != otherCurrent->value) {
            return false;
        }
        current = current->next;
        otherCurrent = otherCurrent->next;
    }

    return current == nullptr && otherCurrent == nullptr;
}

template<class T>
bool ForwardList<T>::operator!=(const ForwardList<T>& other) const {
    return !(*this == other);
}

template<class T>
ForwardList<T>& ForwardList<T>::operator=(const ForwardList<T>& other) {
    if (this == &other) {
        return *this;
    }

    clean();

    m_size = other.m_size;

    if (!other.head) {
        head = nullptr;
        return *this;
    }

    head = new Node{ other.head->value, nullptr };
    Node* current = head;
    Node* otherCurrent = other.head->next;

    while (otherCurrent) {
        current->next = new Node{ otherCurrent->value, nullptr };
        current = current->next;
        otherCurrent = otherCurrent->next;
    }

    return *this;
}

template <class T>
template <class... Args>
void ForwardList<T>::emplace_front(Args&&... args) {
    Node* current = new Node{};
    current->value = T(std::forward<Args>(args)...);
    current->next = head;
    head = current;
    m_size++;
}

template<class T>
ForwardList<T>::~ForwardList() noexcept {
    clean();
    m_size = 0;
}


