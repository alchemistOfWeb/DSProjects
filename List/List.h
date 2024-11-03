#pragma once

#include <stdexcept>
#include <initializer_list> 
#include <iostream>
#include <forward_list>
#include <optional>


template <typename T>
class List
{
public:
    // CONSTRUCTORS
    List<T>(std::size_t size = 0);
    List<T>(std::size_t size, const T& filler);
    List(const std::initializer_list<T>& init_list);
    //template <class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, int> = 0>
    //List(_Iter first, _Iter last);
    List(const List<T>& other) noexcept;

    ~List();

    // PUBLIC METHODS
    bool empty() const;
    T& front();
    T& back();
    void push_front(const T& value);
    void push_back(const T& value);
    void pop_front();
    void pop_back();
    size_t size() const;

    // ITERATORS

private:
    struct Node;

    Node* head = nullptr;
    Node* tail = nullptr;
    Node* fake_head = nullptr;
    std::size_t m_size;

    // PRIVATE METHODS
    void clean();
};

////////////////////////////////////////////////////////////////////////////
// NODE

template <typename T>
struct List<T>::Node {
    T value;
    Node* next;
    Node* prev;
    Node(const T& val, Node* nxt = nullptr, Node* prv = nullptr)
        : value(val), next(nxt), prev(prv) {}
};


//
//template <class T>
//struct List<T>::Node {
//    std::optional<T> value;
//    Node* next;
//    Node* prev;
//
//    Node() noexcept : value(std::nullopt), next(nullptr), prev(nullptr) {
//        if constexpr (std::is_default_constructible_v<T>) {
//            value.emplace();
//        }
//    }
//
//    Node(const T& value, Node* next = nullptr) : value(value), next(next) {}
//
//};

// END node
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// CONSTRUCTORS

template <typename T>
List<T>::List(std::size_t size)
    : fake_head(new Node(T())), head(nullptr), tail(nullptr), m_size(0) {
    fake_head->next = head;
    for (std::size_t i = 0; i < size; ++i) {
        push_back(T());
    }
}


template <typename T>
List<T>::List(std::size_t size, const T& filler)
    : fake_head(new Node(T())), head(nullptr), tail(nullptr), m_size(0) {
    fake_head->next = head;
    for (std::size_t i = 0; i < size; ++i) {
        push_back(filler);
    }
}


template <typename T>
List<T>::List(const std::initializer_list<T>& init_list)
    : fake_head(new Node(T())), head(nullptr), tail(nullptr), m_size(0) {
    fake_head->next = head;
    for (const T& item : init_list) {
        push_back(item);
    }
}


template <typename T>
List<T>::List(const List<T>& other) noexcept
    : fake_head(new Node(T())), head(nullptr), tail(nullptr), m_size(0) {
    fake_head->next = head;
    for (Node* current = other.head; current; current = current->next) {
        push_back(current->value);
    }
}

template <typename T>
List<T>::~List() {
    clean();
    delete fake_head;
}

// END constructors
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS

template <typename T>
bool List<T>::empty() const {
    return m_size == 0;
}


template <typename T>
T& List<T>::front() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return head->value;
}


template <typename T>
T& List<T>::back() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    return tail->value;
}


template <typename T>
void List<T>::push_front(const T& value) {
    Node* new_node = new Node(value, head, nullptr);
    if (empty()) {
        head = tail = new_node;
    }
    else {
        head->prev = new_node;
        head = new_node;
    }
    fake_head->next = head;
    ++m_size;
}


template <typename T>
void List<T>::push_back(const T& value) {
    Node* new_node = new Node(value, nullptr, tail);
    if (empty()) {
        head = tail = new_node;
    }
    else {
        tail->next = new_node;
        tail = new_node;
    }
    ++m_size;
}


template <typename T>
void List<T>::pop_front() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    Node* temp = head;
    head = head->next;
    if (head) {
        head->prev = nullptr;
    }
    else {
        tail = nullptr;
    }
    fake_head->next = head;
    delete temp;
    --m_size;
}


template <typename T>
void List<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    Node* temp = tail;
    tail = tail->prev;
    if (tail) {
        tail->next = nullptr;
    }
    else {
        head = nullptr;
    }
    delete temp;
    --m_size;
}


template <typename T>
size_t List<T>::size() const {
    return m_size;
}

// END public methods
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS

template <typename T>
void List<T>::clean() {
    while (!empty()) {
        pop_front();
    }
}


// END private methods
////////////////////////////////////////////////////////////////////////////