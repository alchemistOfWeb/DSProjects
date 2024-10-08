#pragma once
// Create a custom vector class that would support all the functionality
// given in main.cpp.

// Again: Declare your functions inside your class, but define them outside of it.
//
//#include <vector>
//
//template <typename T>
//using Vector = std::vector<T>;


#include <stdexcept>
#include <initializer_list> 
#include <iostream>



template <class T>
class Vector {
public:
    Vector<T>(std::size_t size = 0);

    Vector<T>(std::size_t size, T filler);

    Vector(const std::initializer_list<T>& init_list);

    Vector(Vector<T>& other);

    Vector(Vector<T>&& other) noexcept;

    size_t size();

    T& operator[](const size_t& index);

    const T& operator[](const size_t& index) const;

    T& at(const size_t& pos);

    T& front();

    T& back();

    T* data();

    void push_back(T value);

    void pop_back();

    template <typename... Args>
    void emplace_back(Args&&... args);

    void reserve(size_t new_cap);

    size_t capacity();

    void shrink_to_fit();

    void clear();

    bool empty();

    void swap(Vector<T>& other);

    bool operator==(const Vector<T>& other) const;
    
    bool operator!=(const Vector<T>& other) const;

    Vector<T>& operator=(const Vector<T>& other);

    Vector<T>& operator=(const std::initializer_list<T>& init_list);

    ~Vector() noexcept;

private:
    T* m_arr = nullptr;
    std::size_t m_size; // number of elements
    std::size_t m_capacity; // real number of array elements in memory

    void reallocate(size_t new_capacity);
    
    size_t increaseCapacity();
};


// Vector methods implementations:
// public:
template <class T>
Vector<T>::Vector<T>(std::size_t size) : m_capacity(size) {
    if constexpr (std::is_default_constructible_v<T>) {
        m_arr = new T[size];
        m_size = size;
        for (size_t i = 0; i < m_size; i++) {
            m_arr[i] = T();
        }
    }
    else {
        m_arr = reinterpret_cast<T*>(operator new[](size * sizeof(T)));
        m_size = 0;
    }
}

template <class T>
Vector<T>::Vector<T>(std::size_t size, T filler) : m_size(size), m_capacity(size) {
    m_arr = new T[size];
    for (size_t i = 0; i < m_size; i++) {
        m_arr[i] = T(filler);
    }
}

template <class T>
Vector<T>::Vector<T>(const std::initializer_list<T>& init_list) {
    m_size = m_capacity = init_list.size();
    m_arr = new T[m_capacity];

    int i = 0;
    for (auto& el : init_list) {
        m_arr[i] = el;
        i++;
    }
}

template <class T>
Vector<T>::Vector<T>(Vector<T>& other) : m_arr(), m_size(other.m_size), m_capacity(other.m_capacity) {
    T* newarr = new T[other.m_capacity];

    for (size_t i = 0; i < m_size; i++) {
        newarr[i] = other.m_arr[i];
    }

    delete[] m_arr;
    m_arr = newarr;
}

template <class T>
Vector<T>::Vector<T>(Vector<T>&& other) noexcept : m_arr(other.m_arr), m_size(other.m_size), m_capacity(other.m_capacity) {
    other.m_arr = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template <class T>
size_t Vector<T>::size() {
    return m_size;
}

template <class T>
T& Vector<T>::operator[](const size_t& index) {
    return m_arr[index];
}

template <class T>
const T& Vector<T>::operator[](const size_t& index) const {
    return m_arr[index];
}

template <class T>
T& Vector<T>::at(const size_t& pos) {
    if (pos >= m_size)
        throw std::invalid_argument("value is out of bounce in at method");
    return m_arr[pos];
}

template <class T>
T& Vector<T>::front() {
    return m_arr[0];
}

template <class T>
T& Vector<T>::back() {
    return m_arr[m_size - 1];
}

template <class T>
T* Vector<T>::data() {
    return m_arr;
}

template <class T>
void Vector<T>::push_back(T value) {
    if (m_size == m_capacity) {
        reallocate(increaseCapacity());
    }
    m_arr[m_size++] = value;
}

template <class T>
void Vector<T>::pop_back() {
    --m_size;
}

template <class T>
template <typename... Args>
void Vector<T>::emplace_back(Args&&... args) {
    if (m_capacity == m_size) {
        reallocate(increaseCapacity());
    }
    new(&m_arr[m_size++]) T(std::forward<Args>(args)...);
}

template <class T>
void Vector<T>::reserve(size_t new_cap) {
    reallocate(m_capacity = new_cap);
}

template <class T>
size_t Vector<T>::capacity() {
    return m_capacity;
}

template <class T>
void Vector<T>::shrink_to_fit() {
    reallocate(m_capacity = m_size);
}

template <class T>
void Vector<T>::clear() {
    m_size = 0;
}

template <class T>
bool Vector<T>::empty() {
    return !m_size;
}

template <class T>
void Vector<T>::swap(Vector<T>& other) {
    T* tmparr = m_arr;
    size_t tmpsize = m_size;
    size_t tmpcap = m_capacity;

    m_arr = other.m_arr;
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    other.m_arr = tmparr;
    other.m_size = tmpsize;
    other.m_capacity = tmpcap;
}

template <class T>
bool Vector<T>::operator==(const Vector<T>& other) const {
    for (size_t i = 0; i < m_size; i++) {
        if (m_arr[i] != other.m_arr[i]) return false;
    }
    return true;
}

template <class T>
bool Vector<T>::operator!=(const Vector<T>& other) const {
    for (size_t i = 0; i < m_size; i++) {
        if (m_arr[i] == other.m_arr[i]) return true;
    }
    return false;
}

template <class T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    //T* newarr = new T[other.m_capacity];
    T* newarr = static_cast<T*>(operator new[](other.m_capacity * sizeof(T))); 
    // placement new
    // char* arr = new char[m_size * sizeof(T)] {};
    // T* t = reinterpret_cast<T*>(arr);
    // new (t + 5) MyClass{ value }; 
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    for (size_t i = 0; i < m_size; i++) {
        new (newarr + i) T(std::move(other.m_arr[i]));
    }

    delete[] m_arr;
    m_arr = newarr;
    return *this;
}

template <class T>
Vector<T>& Vector<T>::operator=(const std::initializer_list<T>& init_list) {
    m_size = m_capacity = init_list.size();

    delete[] m_arr;
    m_arr = new T[m_capacity];

    int i = 0;
    for (auto& el : init_list) {
        m_arr[i] = el;
        i++;
    }

    return *this;
}

template <class T>
Vector<T>::~Vector() noexcept {
    delete[] m_arr;
}

// Private:

template <class T>
void Vector<T>::reallocate(size_t new_capacity) {
    if (new_capacity <= m_size) return;
    if constexpr (std::is_default_constructible_v<T>) {
        T* newarr = new T[new_capacity];

        for (size_t i = 0; i < m_size; i++) {
            newarr[i] = std::move(m_arr[i]);
            m_arr[i] = T();
        }

        delete[] m_arr;
        m_arr = newarr;
    }
    else {
        T* newarr = static_cast<T*>(operator new[](new_capacity * sizeof(T)));

        for (size_t i = 0; i < m_size; i++) {
            new (newarr + i) T(std::move(m_arr[i]));
        }

        delete[] m_arr;
        m_arr = newarr;
    }
}

template <class T>
size_t Vector<T>::increaseCapacity() {
    return m_capacity == 0 ? m_capacity = 1 : m_capacity *= 2;
}
