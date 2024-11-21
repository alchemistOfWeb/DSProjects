#pragma once

#include <stdexcept>
#include <initializer_list> 
#include <iostream>
#include <forward_list>
#include <optional>


template <class T>
class ForwardList
{
public:
    // CONSTRUCTORS
    ForwardList<T>(std::size_t size=0);
    ForwardList<T>(std::size_t size, const T& filler);
    ForwardList(const std::initializer_list<T>& init_list);
    
    template <class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, int> = 0>
    ForwardList(_Iter first, _Iter last);

    ForwardList(const ForwardList<T>& other);
    

    // METHODS
    bool empty();
    T& front();
    const T& front() const;
    void push_front(const T& value);
    void pop_front();
    
    template<class... Args>
    void emplace_front(Args&&... args);

    void clear();
    

    // OVERLOADS
    bool operator==(const ForwardList<T>& other) const;
    bool operator!=(const ForwardList<T>& other) const;
    ForwardList<T>& operator=(const ForwardList<T>& other);


    // DESTRUCTOR
    ~ForwardList() noexcept;


    // ITERATORS
    class iterator;
    class const_iterator;

    // ITERATION METHODS
    iterator begin();
    iterator end();
    iterator before_begin();

    const_iterator cbegin() const;
    const_iterator cend() const;

    const_iterator begin() const;
    const_iterator end() const;

private:
    struct Node;

    Node* head = nullptr;
    Node* fake_head = nullptr;
    std::size_t m_size;

    void clean();
}; // END FORWARDLIST DECLARATION


////////////////////////////////////////////////////////////////////////////
// NODE
template <class T>
struct ForwardList<T>::Node {
    std::optional<T> value;
    Node* next;

    Node() noexcept : value(std::nullopt), next(nullptr) {
        if constexpr (std::is_default_constructible_v<T>) {
            value.emplace();
        }
    }

    Node(const T& value, Node* next = nullptr) : value(value), next(next) {}

};

// END NODE
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// FORWARD LIST CONSTRUCTORS

template<class T>
ForwardList<T>::ForwardList(std::size_t size) : m_size(size) {
    fake_head = new Node();
    Node** current = &(fake_head->next);
    for (std::size_t i = 0; i < size; ++i) {
        *current = new Node{};
        current = &((*current)->next);
    }

}


template<class T>
ForwardList<T>::ForwardList(std::size_t size, const T& filler) : m_size(size) {
    fake_head = new Node();
    Node** current = &(fake_head->next);

    for (std::size_t i = 0; i < size; ++i) {
        *current = new Node{filler, nullptr};
        current = &((*current)->next);
    }
}


template<class T>
ForwardList<T>::ForwardList(const std::initializer_list<T>& init_list) {
    fake_head = new Node();
    Node** current = &(fake_head->next);

    for (const T& value : init_list) {
        *current = new Node{ value, nullptr };
        current = &((*current)->next);
    }
}

template <class T>
template <class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, int>>
ForwardList<T>::ForwardList(_Iter first, _Iter last) : head(nullptr), m_size(0) {
    fake_head = new Node();
    Node** current = &(fake_head->next);

    while (first != last) {
        *current = new Node{ *first, nullptr };
        current = &((*current)->next);
        ++first;
        ++m_size;
    }
}


template<class T>
ForwardList<T>::ForwardList(const ForwardList<T>& other) : m_size(other.m_size) {

    if (!other.fake_head) {
        return;
    }

    fake_head = new Node();
    head = new Node{ other.head->value.value(), nullptr};
    fake_head->next = head;
    Node* current = head;
    Node* otherCurrent = other.head->next;

    while (otherCurrent) {
        current->next = new Node{ otherCurrent->value.value(), nullptr};
        current = current->next;
        otherCurrent = otherCurrent->next;
    }
}

// END FORWARD LIST CONSTRUCTORS
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// FORWARD LIST METHODS

template<class T>
bool ForwardList<T>::empty() {
    return m_size == 0;
}

template<class T>
T& ForwardList<T>::front() {
    return head->value.value();
}

template<class T>
const T& ForwardList<T>::front() const {
    return head->value.value();
}


template<class T>
void ForwardList<T>::push_front(const T& value) {
    Node* current = new Node{value, head};
    head = current;
    m_size++;
}
//
//template <class T>
//void ForwardList<T>::push_back(const T& value) {
//    Node* newNode = new Node{value, nullptr};
//
//    if (empty()) {
//        head = newNode;
//    }
//    else {
//        Node* current = head;
//        while (current->next) {
//            current = current->next;
//        }
//        current->next = newNode;
//    }
//    m_size++;
//}


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


template <class T>
template <class... Args>
void ForwardList<T>::emplace_front(Args&&... args) {
    Node* current = new Node{};
    current->value.value() = T(std::forward<Args>(args)...);
    current->next = head;
    head = current;
    m_size++;
}


// END FORWARD LIST METHODS
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// OPERATOR OVERLOADS

template<class T>
bool ForwardList<T>::operator==(const ForwardList<T>& other) const {
    if (m_size != other.m_size) {
        return false;
    }

    Node* current = head;
    Node* otherCurrent = other.head;

    while (current && otherCurrent) {
        if (current->value.value() != otherCurrent->value.value()) {
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

    head = new Node{ other.head->value.value(), nullptr };
    fake_head->next = head;
    Node* current = head;
    Node* otherCurrent = other.head->next;

    while (otherCurrent) {
        current->next = new Node{ otherCurrent->value.value(), nullptr };
        current = current->next;
        otherCurrent = otherCurrent->next;
    }

    return *this;
}


// END OPERATOR OVERLOADS
////////////////////////////////////////////////////////////////////////////


template<class T>
ForwardList<T>::~ForwardList() noexcept {
    clean();
    m_size = 0;
}





////////////////////////////////////////////////////////////////////////////
// ITERATOR

template <class T>
class ForwardList<T>::iterator {
    friend class ForwardList<T>::const_iterator;
public:
    iterator(Node* node) : current(node) {}

    T& operator*() {
        return current->value.value();
    }

    T* operator->() {
        return &(current->value.value());
    }

    iterator& operator++() {
        current = current->next;
        std::cout << "current: " << current << std::endl;
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

// END ITERATOR
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// CONST ITERATOR
template <class T>
class ForwardList<T>::const_iterator {
public:
    const_iterator(Node* node) : current(node) {}
    const_iterator(const iterator& it) : current(it.current) {}

    const T& operator*() const {
        return current->value.value();
    }

    const T* operator->() const {
        return &(current->value.value());
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

// END CONST ITERATOR
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// ITERATION METHODS

template <class T>
ForwardList<T>::iterator ForwardList<T>::begin() {
    return iterator(head);
}

template <class T>
ForwardList<T>::iterator ForwardList<T>::end() {
    return iterator(nullptr);
}

template <class T>
ForwardList<T>::const_iterator ForwardList<T>::cbegin() const {
    return const_iterator(head);
}

template <class T>
ForwardList<T>::const_iterator ForwardList<T>::cend() const {
    return const_iterator(nullptr);
}

template <class T>
ForwardList<T>::const_iterator ForwardList<T>::begin() const {
    return const_iterator(head);
}

template <class T>
ForwardList<T>::const_iterator ForwardList<T>::end() const {
    return const_iterator(nullptr);
}

template <class T>
ForwardList<T>::iterator ForwardList<T>::before_begin() {
    return iterator(fake_head->next);
}

// END ITERATION METHODS
////////////////////////////////////////////////////////////////////////////
