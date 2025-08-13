#ifndef VECTOR_TPP
#define VECTOR_TPP
#include "vector.hpp"
#include <iostream>
#include <string>
#include <exception>

// constructors

template <typename T>
MyVector<T>::MyVector() : m_size{0}, m_capacity{0}, m_data{nullptr} { }

template <typename T>
MyVector<T>::MyVector(size_type size, const value_type& value) : m_size{size}, m_capacity{size * 2}, m_data{new value_type[m_capacity]} {
	for (size_type i = 0; i < m_size; ++i) {
        m_data[i] = value;
	}
}

template <typename T>
MyVector<T>::MyVector(const MyVector& other) : m_size{other.m_size}, m_capacity{other.m_capacity}, m_data{new value_type[m_capacity]} {
	for (size_type i = 0; i < m_size; ++i) {
		m_data[i] = other.m_data[i];
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
	delete []m_data;
	m_size = other.m_size;
	m_capacity = other.m_capacity;
	m_data = new value_type[m_capacity];
	for (size_type i = 0; i < m_size; ++i) {
		m_data[i] = other.m_data[i];
	}
	return *this;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& other) {
    if (this == &other) {
        return *this;
    }
    delete []m_data;
	m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_data = other.m_data;
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
    return *this;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(std::initializer_list<value_type> ilist ) {
    delete[] m_data;
    m_size = ilist.size();
    m_capacity = m_size * 2;
    m_data = new value_type[m_capacity];
    size_type i = 0;
    for (const auto& elem : ilist) {
        m_data[i++] = elem;
    }
    return *this;
}

template <typename T>
MyVector<T>::~MyVector() {
    delete []m_data;
	m_size = 0;
    m_capacity = 0;
    m_data = nullptr;
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
T* MyVector<T>::data() {
    return m_data;
}

template <typename T>
const T* MyVector<T>::data() const {
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
    value_type* new_data = new value_type[new_cap];
    for (size_type i = 0; i < m_size; ++i) {
        new_data[i] = m_data[i];
    }
    delete[] m_data;
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
void MyVector<T>::push_back( const T& value ) {
    if (m_size == m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    m_data[m_size] = value;
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
    --m_size;
}

template <typename T>
void MyVector<T>::resize( size_type count ) {
    if (m_size == count) return;
    else if (count >= m_capacity) reserve(count * 2 + 1);
    m_size = count;
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