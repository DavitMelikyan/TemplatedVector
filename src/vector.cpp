#ifndef VECTOR_CPP
#define VECTOR_CPP
#include "../headers/vector.hpp"
#include <iostream>
#include <string>
#include <exception>
#include <memory>
#include <vector>

// constructors

template <typename T>
MyVector<T>::MyVector() noexcept : m_size{0}, m_capacity{0}, m_data{nullptr} { }

template <typename T>
MyVector<T>::MyVector(size_type size, const value_type& value) : MyVector{} {
	m_data = static_cast<value_type*>(operator new(sizeof(value_type) * size));
    try {
        std::uninitialized_fill_n(m_data, size, value);
        m_size = size;
        m_capacity = size;
    } catch (...) {
        operator delete(m_data);
        throw;
    }
}

template <typename T>
MyVector<T>::MyVector(const MyVector& other) : MyVector{} {
	m_data = static_cast<value_type*>(operator new(sizeof(value_type) * other.m_capacity));
    try {
        std::uninitialized_copy(other.m_data, other.m_data + other.m_size, m_data);
        m_size = other.m_size;
        m_capacity = other.m_capacity;
    } catch (...) {
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
MyVector<T>::MyVector(const std::initializer_list<T> &il) {
    m_data = static_cast<T *>(operator new(sizeof(T) * il.size()));
    try {
        std::uninitialized_copy(il.begin(), il.end(), m_data);
        m_capacity = m_size = il.size();
    }
    catch (...) {
        operator delete(m_data);
        throw;
    }
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& other) {
    if (this == &other) {
		return *this;
	}
	MyVector<value_type> tmp(other);
    swap(tmp);
    return *this;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    MyVector<T> tmp (std::move(other));
    swap(tmp);
    return *this;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const std::initializer_list<value_type>& ilist ) {
    MyVector<T> tmp(ilist);
    swap(tmp);
    return *this;
}

template <typename T>
MyVector<T>::~MyVector() {
    clear();
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
bool MyVector<T>::empty() const noexcept {
    return m_size == 0;
}

template <typename T>
typename MyVector<T>::size_type MyVector<T>::size() const noexcept {
    return m_size;
}

template <typename T>
void MyVector<T>::reserve( size_type new_cap ) {
    if (new_cap <= m_capacity) return;
    value_type* new_data = static_cast<pointer>(operator new(new_cap * sizeof(T)));
    try {
        std::uninitialized_copy(m_data, m_data + m_size, new_data);
    } catch (...) {
        operator delete(new_data);
        throw;
    }
    size_type tmp = m_size;
    clear();
    operator delete(m_data);
    m_data = new_data;
    m_capacity = new_cap;
    m_size = tmp;
}
template <typename T>
typename MyVector<T>::size_type MyVector<T>::capacity() const noexcept {
    return m_capacity;
}

// Modifiers

template <typename T>
void MyVector<T>::clear() {
    for (size_type i = 0; i < m_size; ++i) {
        m_data[i].~T();
    }
    m_size = 0;
}

template <typename T>
void MyVector<T>::insert(size_type pos, const T& value) {
    T* new_data = nullptr;
    size_type new_capac = m_capacity;
    if (m_size == m_capacity) {
        new_data = static_cast<T*>(operator new(sizeof(T) * m_capacity * 2));
        new_capac = m_size * 2;
    } else {
        new_data = static_cast<T*>(operator new(sizeof(T) * m_capacity));
    }
    for (size_type i = 0; i < pos; ++i) {
        new (new_data[i]) T(std::move(m_data[i]));
    }
    try {
        new (&new_data[pos]) T(value);
    } catch (...) {
        for (size_type i = 0; i < pos; ++i) {
            new (m_data[i]) T(std::move(m_data[i - 1]));
        }
        operator delete(new_data);
        throw;
    }
    for (size_type i = m_size; i > pos; --i) {
        new (&new_data[i]) T(std::move(new_data[i - 1]));
    }
    operator delete(m_data);
    m_data = new_data;
    ++m_size;
    m_capacity = new_capac;
}

template <typename T>
void MyVector<T>::insert(size_type pos, T&& value ) {
    if (m_capacity == m_size) reserve(m_capacity * 2 + 1);
    for (size_type i = m_size; i > pos; --i) {
        new (&m_data[i]) T(std::move(m_data[i - 1]));
    }
    new (&m_data[pos]) T(std::move(value));
    ++m_size;
}

template <typename T>
void MyVector<T>::insert(size_type pos, size_t count, const T& value) {
    T* new_data = nullptr;
    size_type new_capac = m_capacity;
    if (m_size + count >= m_capacity) {
        new_data = static_cast<T*>(operator new(sizeof(T) * (m_size + count) * 2));
        new_capac = (m_size + count) * 2;
    } else {
        new_data = static_cast<T*>(operator new(sizeof(T) * m_capacity));
    }
    for (size_type i = 0; i < pos; ++i) {
        new (&new_data[i]) T(std::move(m_data[i]));
    }
    try {
        std::uninitialized_fill_n(new_data + pos, count, value);
    } catch (...) {
        for (size_type i = 0; i < pos; ++i) {
            new (&m_data[i]) T(std::move(new_data[i]));
        }
        operator delete(new_data);
        throw;
    }

    for (size_type i = m_size + count - 1; i >= pos + count; --i) {
        new (&new_data[i]) T(std::move(m_data[i - count]));
    }

    size_type tmp = m_size;
    clear();
    operator delete(m_data);
    m_data = new_data;
    m_size = tmp + count;
    m_capacity = new_capac;
}

template <typename T>
void MyVector<T>::erase(size_type pos) {
    m_data[pos].~T();
    for (size_type i = pos; i < m_size - 1; ++i) {
        new (&m_data[i]) T(std::move(m_data[i + 1]));
    }
    --m_size;
}

template <typename T>
void MyVector<T>::erase(size_type first, size_type last) {
    for (size_type i = first; i < last; ++i) {
        m_data[i].~T();
    }
    for (size_type i = first; i < m_size - last + first; ++i) {
        new (&m_data[i]) T(std::move(m_data[i + last - first]));
    }
    m_size -= last - first;
}


template <typename T>
void MyVector<T>::push_back( const T& value ) {
    if (m_size == m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    new (&m_data[m_size]) T(value);
    ++m_size;
}

template <typename T>
void MyVector<T>::push_back( T&& value ) {
    if (m_size == m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    new (&m_data[m_size]) T(std::move(value));
    ++m_size;
}

template <typename T>
template<typename... Args>
void  MyVector<T>::emplace(size_type pos, Args&&... args ) {
    T* new_data = nullptr;
    size_type new_capac = m_capacity;
    if (m_size == m_capacity) {
        new_data = static_cast<T*>(operator new(sizeof(T) * m_capacity * 2));
        new_capac = m_size * 2;
    } else {
        new_data = static_cast<T*>(operator new(sizeof(T) * m_capacity));
    }
    for (size_type i = 0; i < pos; ++i) {
        new (&new_data[i]) T(std::move(m_data[i]));
    }
    try {
        new (&new_data[pos]) T(std::forward<Args>(args)...);
    } catch (...) {
        for (size_type i = 0; i < pos; ++i) {
            new (&m_data[i]) T(std::move(m_data[i - 1]));
        }
        operator delete(new_data);
        throw;
    }
    for (size_type i = m_size; i > pos; --i) {
        new (&new_data[i]) T(std::move(new_data[i - 1]));
    }
    operator delete(m_data);
    m_data = new_data;
    ++m_size;
    m_capacity = new_capac;
}

template <typename T>
template <typename... Args>
void MyVector<T>::emplace_back( Args&&... args ) {
    if (m_size == m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    new (&m_data[m_size]) T{std::forward<Args>(args)...};
    ++m_size;
}

template <typename T>
void MyVector<T>::pop_back() {
    m_data[m_size - 1].~T();
    --m_size;
}

template <typename T>
void MyVector<T>::resize(size_type count, const T& value) {
    if (count < m_size) {
        for (size_type i = count; i < m_size; ++i) {
            m_data[i].~T();
        }
    }
    else if (count > m_size) {
        if (count > m_capacity) {
            reserve(count);
        }
        std::uninitialized_fill_n(m_data + m_size, count - m_size, value);
    }
    m_size = count;
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