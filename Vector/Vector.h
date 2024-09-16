// Create a custom vector class that would support all the functionality
// given in main.cpp.

// Again: Declare your functions inside your class, but define them outside of it.
//
//#include <vector>
//
//template <typename T>
//using Vector = std::vector<T>;


#include <initializer_list> 


template <typename T>
class Vector {
private:

    T* m_arr;
    std::size_t m_size; // number of elements
    std::size_t m_capacity; // real number of array elements in memory

public:
    //Vector() : m_arr(new T[3]), m_size(0), m_capacity(3) {}
    Vector(std::size_t size=0): m_arr(new T[size]), m_size(0), m_capacity(size) {}

    Vector(std::size_t size, T filler): m_arr(new T[size]), m_size(size), m_capacity(size) {
        for (size_t i = 0; i < m_size; i++) {
            m_arr[i] = filler;
        }
    }

    Vector(const std::initializer_list<T>& init_list) {

    }

    size_t size() {
        return m_capacity;
    }

    ~Vector() {

    }
};
