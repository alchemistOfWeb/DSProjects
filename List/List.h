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
    template <class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, int> = 0>
    ForwardList(_Iter first, _Iter last);

    ForwardList(const ForwardList<T>& other);

    // METHODS
private:
    struct Node;

    Node* head = nullptr;
    Node* fake_head = nullptr;
    std::size_t m_size;

    void clean();
};

////////////////////////////////////////////////////////////////////////////
// NODE
template <class T>
struct List<T>::Node {
    std::optional<T> value;
    Node* next;
    Node* prev;

    Node() noexcept : value(std::nullopt), next(nullptr), prev(nullptr) {
        if constexpr (std::is_default_constructible_v<T>) {
            value.emplace();
        }
    }

    Node(const T& value, Node* next = nullptr) : value(value), next(next) {}

};

// NODE
////////////////////////////////////////////////////////////////////////////