#ifndef VECTOR_TPP
#define VECTOR_TPP
#include "vector.hpp"
#include <iostream>
#include <string>
#include <exception>
#include <memory>


// constructors

template <typename T>
MyVector<T>::MyVector() : m_size{0}, m_capacity{0}, m_data{nullptr} { }

template <typename T>
MyVector<T>::MyVector(size_type size, const value_type& value) : m_size{0}, m_capacity{size * 2}, m_data{nullptr} {
	m_data = static_cast<value_type*>(operator new(sizeof(value_type) * m_capacity));
    value_type* constructed = m_data;
    try {
        constructed = std::uninitialized_fill_n(m_data, size, value);
        m_size = size;
    } catch (...) {
        size_type count = static_cast<size_type>(constructed - m_data);
        for (size_type i = 0; i < count; ++i) {
            m_data[i].~value_type();
        }
        operator delete(m_data);
        throw;
    }
}

template <typename T>
MyVector<T>::MyVector(const MyVector& other) : m_size{0}, m_capacity{other.m_capacity}, m_data{nullptr} {
	m_data = static_cast<value_type*>(operator new(sizeof(value_type) * m_capacity));
    value_type* constructed = m_data;
    try {
        constructed = std::uninitialized_copy(other.m_data, other.m_data + other.m_size, m_data);
        m_size = other.m_size;
    } catch (...) {
        size_type count = static_cast<size_type>(constructed - m_data);
        for (size_type i = 0; i < count; ++i) {
            m_data[i].~value_type();
        }
        operator delete(m_data);
        throw;
    }
}

template <typename T>
MyVector<T>::MyVector(MyVector&& other) noexcept : m_size{other.m_size}, m_capacity{other.m_capacity}, m_data{other.m_data} {
    other.m_size = 0;
    other.m_capacity = 0;
    other.m_data = nullptr;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other) {
    if (this == &other) {
		return *this;
	}
	MyVector<value_type> tmp(other);
    swap(tmp);
    for (size_type i = 0; i < tmp.m_size; ++i) {
        tmp.m_data[i].~T();
    }
    return *this;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    for (size_type i = 0; i < m_size; ++i) {
        m_data[i].~T();
    }
	m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_data = other.m_data;
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
    return *this;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const std::initializer_list<value_type>& ilist ) {
    MyVector tmp;
    tmp.m_capacity = ilist.size() * 2;
    tmp.m_data = static_cast<value_type*>(operator new(sizeof(value_type) * tmp.m_capacity));
    tmp.m_size = 0;
    value_type* constructed = tmp.m_data;
    try {
        constructed = std::uninitialized_copy(ilist.begin(), ilist.end(), tmp.m_data);
        tmp.m_size = ilist.size();
    } catch (...) {
        size_type count = static_cast<size_type>(constructed - tmp.m_data);
        for (size_type i = 0; i < count; ++i) {
            tmp.m_data[i].~value_type();
        }
        operator delete(tmp.m_data);
        throw;
    }
    swap(tmp);
    return *this;
}

template <typename T>
MyVector<T>::~MyVector() {
    for (size_type i = 0; i < m_size; ++i) {
        m_data[i].~T();
    }
    operator delete(m_data);
}

// element access

template <typename T>
typename MyVector<T>::reference MyVector<T>::at( size_type pos ) {
    if (pos >= m_size) throw std::out_of_range{"Index out of range"};
    return m_data[pos];
}

template <typename T>
typename MyVector<T>::const_reference MyVector<T>::at( size_type pos ) const {
    if (pos >= m_size) throw std::out_of_range{"Index out of range"};
    return m_data[pos];
}

template <typename T>
typename MyVector<T>::reference MyVector<T>::operator[]( size_type pos ) {
    return m_data[pos];
}

template <typename T>
typename MyVector<T>::const_reference MyVector<T>::operator[]( size_type pos ) const {
    return m_data[pos];
}

template <typename T>
typename MyVector<T>::reference MyVector<T>::front() {
    return m_data[0];
}

template <typename T>
typename MyVector<T>::const_reference MyVector<T>::front() const {
    return m_data[0];
}

template <typename T>
typename MyVector<T>::reference MyVector<T>::back() {
    return m_data[m_size - 1];
}

template <typename T>
typename MyVector<T>::const_reference MyVector<T>::back() const {
    return m_data[m_size - 1];
}

template <typename T>
typename MyVector<T>::pointer MyVector<T>::data() {
    return m_data;
}

template <typename T>
typename MyVector<T>::const_pointer MyVector<T>::data() const {
    return m_data;
}

// capacity

template <typename T>
bool MyVector<T>::empty() const {
    return m_size == 0;
}

template <typename T>
typename MyVector<T>::size_type MyVector<T>::size() const {
    return m_size;
}

template <typename T>
void MyVector<T>::reserve( size_type new_cap ) {
    if (new_cap <= m_capacity) return;
    value_type* new_data = static_cast<pointer>(operator new(new_cap * sizeof(value_type)));
    value_type* constructed = new_data;
    try {
        constructed = std::uninitialized_copy(m_data, m_data + m_size, new_data);
    } catch (...) {
        size_type count = static_cast<size_type>(constructed - new_data);
        for (size_type i = 0; i < count; ++i) {
            new_data[i].~value_type();
        }
        operator delete(new_data);
        throw;
    }
    for (size_type i = 0; i < m_size; ++i) {
        m_data[i].~value_type();
    }
    operator delete(m_data);
    m_data = new_data;
    m_capacity = new_cap;
}
template <typename T>
typename MyVector<T>::size_type MyVector<T>::capacity() const {
    return m_capacity;
}

// Modifiers

template <typename T>
void MyVector<T>::clear() {
    m_size = 0;
}

template <typename T>
void MyVector<T>::insert(size_type pos, const T& value) {
    if (m_size == 0 || pos == m_size) {
        push_back(value);
        return;
    }
    if (m_capacity == m_size) reserve(m_capacity * 2 + 1);
    value_type* new_data = static_cast<value_type*>(operator new(sizeof(value_type) * m_capacity));
    value_type* constructed = new_data;
    try {
        constructed = std::uninitialized_copy(m_data, m_data + pos, new_data);
        new (static_cast<void*>(constructed)) value_type(value);
        ++constructed;
        constructed = std::uninitialized_copy(m_data + pos, m_data + m_size, constructed);
    } catch (...) {
        size_type count = static_cast<size_type>(constructed - new_data);
        for (size_type i = 0; i < count; ++i) {
            new_data[i].~value_type();
        }
        operator delete(new_data);
        throw;
    }

    for (size_type i = 0; i < m_size; ++i) {
        m_data[i].~value_type();
    }
    operator delete(m_data);

    m_data = new_data;
    ++m_size;
}

template <typename T>
void MyVector<T>::insert(size_type pos, T&& value ) {
    if (m_size == 0 || pos == m_size) {
        push_back(std::move(value));
        return;
    }
    if (m_capacity == m_size) reserve(m_capacity * 2 + 1);
    value_type* new_data = static_cast<value_type*>(operator new(sizeof(value_type) * m_capacity));
    value_type* constructed = new_data;
    try {
        constructed = std::uninitialized_move(m_data, m_data + pos, new_data);
        new (static_cast<void*>(constructed)) value_type(std::move(value));
        ++constructed;
        constructed = std::uninitialized_move(m_data + pos, m_data + m_size, constructed);
    } catch (...) {
        size_type count = static_cast<size_type>(constructed - new_data);
        for (size_type i = 0; i < count; ++i) {
            new_data[i].~value_type();
        }
        operator delete(new_data);
        throw;
    }

    for (size_type i = 0; i < m_size; ++i) {
        m_data[i].~value_type();
    }
    operator delete(m_data);

    m_data = new_data;
    ++m_size;
}

template <typename T>
void MyVector<T>::insert(size_type pos, size_t count, const T& value) {
    if (m_size == 0 || pos == m_size) {
        while (count--) push_back(value);
        return;
    }
    if (m_capacity <= m_size + count) reserve(m_size + count);
    T* new_data = static_cast<T*>(operator new(sizeof(T) * m_capacity));
    T* constructed = new_data;
    try {
        constructed = std::uninitialized_copy(m_data, m_data + pos, new_data);
        for (size_type i = 0; i < count; ++i) {
            new (constructed) T(value);
            ++constructed;
        }
        constructed = std::uninitialized_copy(m_data + pos, m_data + m_size, constructed);

    } catch (...) {
        size_type count = static_cast<size_type>(constructed - new_data);
        for (size_type i = 0; i < count; ++i) {
            new_data[i].~value_type();
        }
        operator delete(new_data);
        throw;
    }

    for (size_type i = 0; i < m_size; ++i) m_data[i].~T();
    operator delete(m_data);

    m_data = new_data;
    m_size += count;
}

template <typename T>
void MyVector<T>::push_back( const T& value ) {
    if (m_size == m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    m_data[m_size] = value;
    ++m_size;
}

template <typename T>
template<typename... Args>
void  MyVector<T>::emplace(size_type pos, Args&&... args ) {
    if (m_size == 0 || pos == m_size) {
        emplace_back(std::forward<Args>(args)...);
        return;
    }
    if (m_capacity == m_size) reserve(m_capacity * 2 + 1);
    value_type* new_data = static_cast<value_type*>(operator new(sizeof(value_type) * m_capacity));
    value_type* constructed = new_data;
    try {
        constructed = std::uninitialized_move(m_data, m_data + pos, new_data);
        new (static_cast<void*>(constructed)) value_type{std::forward<Args>(args)...};
        ++constructed;
        constructed = std::uninitialized_move(m_data + pos, m_data + m_size, constructed);
    } catch (...) {
        size_type count = static_cast<size_type>(constructed - new_data);
        for (size_type i = 0; i < count; ++i) {
            new_data[i].~value_type();
        }
        operator delete(new_data);
        throw;
    }

    for (size_type i = 0; i < m_size; ++i) {
        m_data[i].~value_type();
    }
    operator delete(m_data);

    m_data = new_data;
    ++m_size;
}

template <typename T>
void MyVector<T>::push_back( T&& value ) {
    if (m_size == m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    m_data[m_size] = std::move(value);
    ++m_size;
}

template <typename T>
template <typename... Args>
typename MyVector<T>::reference MyVector<T>::emplace_back( Args&&... args ) {
    if (m_size == m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    m_data[m_size] = value_type{std::forward<Args>(args)...};
    ++m_size;
    return m_data[m_size - 1];
}

template <typename T>
void MyVector<T>::pop_back() {
    if (empty()) throw std::out_of_range{"Vector is empty"};
    --m_size;
}

template <typename T>
void MyVector<T>::resize( size_type count ) {
    if (m_size == count) return;
    else if (count < m_size) {
        for (size_type i = count; i < m_size; ++i) {
            m_data[i].~T();
        }
        m_size = count;
    }
    else if (count > m_size) {
        if (count > m_capacity) {
            reserve(count * 2 + 1);
        }
        size_type i = m_size;
        try {
            for (; i < count; ++i) {
                new (m_data + i) T();
            }
        } catch (...) {
            for (size_type j = m_size; j < i; ++j) {
                m_data[j].~T();
            }
            throw;
        }

        m_size = count;
    }
}

template <typename T>
void MyVector<T>::resize(size_type count, const T& value ) {
    if (m_size == count) return;
    else if (m_size > count) m_size = count;
    else {
        if (count >= m_capacity) reserve(count * 2 + 1);
        for (size_type i = m_size; i < count; ++i) {
            m_data[i] = value;
        }
        m_size = count;
    }
}

template <typename T>
void MyVector<T>::swap( MyVector& other ) noexcept {
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
    std::swap(m_data, other.m_data);
}

// Non-member functions

template <class T> 
bool operator==(const MyVector<T>& x, const MyVector<T>& y) {
    if (x.size() != y.size()) return false;
    for (size_t i = 0; i < x.size(); ++i) {
        if (x[i] != y[i]) return false;
    }
    return true;
}

template <class T> 
bool operator!=(const MyVector<T>& x, const MyVector<T>& y) {
    return !(x == y);
}

template <class T> 
bool operator< (const MyVector<T>& x, const MyVector<T>& y) {
    size_t minSize = x.size() < y.size() ? x.size() : y.size();
    for (size_t i = 0; i < minSize; ++i) {
        if (x[i] < y[i]) return true;
        if (x[i] > y[i]) return false;
    }
    return x.size() < y.size();
}


template <class T> 
bool operator> (const MyVector<T>& x, const MyVector<T>& y) {
    return y < x;
}

template <class T> 
bool operator>=(const MyVector<T>& x, const MyVector<T>& y) {
    return !(x < y);
}

template <class T> 
bool operator<=(const MyVector<T>& x, const MyVector<T>& y) {
    return !(y < x);
}


#endif