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
    
    Vector(std::size_t size=0): m_size(size), m_capacity(size) {
        m_arr = new T[size];
        for (size_t i = 0; i < m_size; i++) {
            m_arr[i] = T();
        }
    }

    Vector(std::size_t size, T filler): m_size(size), m_capacity(size) {
        m_arr = new T[size];
        for (size_t i = 0; i < m_size; i++) {
            m_arr[i] = filler;
        }
    }

    Vector(const std::initializer_list<T>& init_list) {
        m_size = m_capacity = init_list.size();
        m_arr = new T[m_capacity];
        
        int i = 0;
        for (auto& el : init_list) {
            m_arr[i] = el;
            i++;
        }
    }

    Vector(Vector<T>& other) : m_arr(), m_size(other.m_size), m_capacity(other.m_capacity) {
        T* newarr = new T[other.m_capacity];

        for (size_t i = 0; i < m_size; i++) {
            newarr[i] = other.m_arr[i];
        }

        delete[] m_arr;
        m_arr = newarr;
    }

    //Vector(Vector<T>&& other) : m_arr(), m_size(other.m_size), m_capacity(other.m_capacity) {
    //    
    //}

    size_t size() {
        return m_size;
    }

    T& operator[](const size_t& index) {
        return m_arr[index];
    }

    const T& operator[](const size_t& index) const {
        return m_arr[index];
    }

    T& at(const size_t& pos) {
        if (pos >= m_size)
            throw std::invalid_argument("value is out of bounce in at method");
        return m_arr[pos];
    }

    T& front() {
        return m_arr[0];
    }

    T& back() {
        return m_arr[m_size - 1];
    }

    T* data() {
        return m_arr;
    }


    void push_back(T value) {
        if (m_size == m_capacity) {
            reallocate(m_capacity += 3);
        }
        m_arr[m_size++] = value;
    }

    T pop_back() {
        return m_arr[m_size--];
    }

    //T& emplace_back(T& args...) {
    //    m_arr[m_size++] = T(args...);
    //    reutrn m_arr[m_size - 1];
    //}

    void reserve(size_t new_cap) {
        if (new_cap <= m_capacity) return;
        reallocate(m_capacity = new_cap);
    }

    size_t capacity() {
        return m_capacity;
    }

    void shrink_to_fit() {
        reallocate(m_capacity = m_size);
    }

    void clear() {
        m_size = 0;
    }

    bool empty() {
        return !m_size;
    }

    void swap(Vector& other) {
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

    bool operator==(const Vector& other) const {
        for (size_t i = 0; i < m_size; i++) {
            if (m_arr[i] != other.m_arr[i]) return false;
        }
        return true;
    }
    
    bool operator!=(const Vector& other) const {
        for (size_t i = 0; i < m_size; i++) {
            if (m_arr[i] == other.m_arr[i]) return true;
        }
        return false;
    }

    Vector& operator=(const Vector& other) {
        T* newarr = new T[other.m_capacity];
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        
        for (size_t i = 0; i < m_size; i++) {
            newarr[i] = other.m_arr[i];
        }

        delete[] m_arr;
        m_arr = newarr;
        return *this;
    }

    Vector& operator=(const std::initializer_list<T>& init_list) {
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

    ~Vector() {
        delete[] m_arr;
    }

private:
    T* m_arr = nullptr;
    std::size_t m_size; // number of elements
    std::size_t m_capacity; // real number of array elements in memory

    void reallocate(size_t new_capacity) {
        if (new_capacity <= m_size) return;
        T* newarr = new T[new_capacity];
        for (size_t i = 0; i < m_size; i++) {
            newarr[i] = std::move(m_arr[i]);
            m_arr[i] = T();
        }

        delete[] m_arr;
        m_arr = newarr;
    }

};
