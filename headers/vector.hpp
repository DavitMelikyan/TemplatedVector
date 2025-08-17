#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <iostream>


template <typename T>
class MyVector {
	public:
		using size_type = std::size_t;
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
		MyVector();
		explicit MyVector(size_type size, const value_type& value = T());
		MyVector(const MyVector& other);
		MyVector(MyVector&& other) noexcept;
		MyVector& operator=(const MyVector& other);
		MyVector& operator=(MyVector&& other) noexcept;
		MyVector& operator=(const std::initializer_list<value_type>& ilist );
		~MyVector();
		
		reference at( size_type pos );
		const_reference at( size_type pos ) const;
		reference operator[]( size_type pos );
		const_reference operator[]( size_type pos ) const;
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		pointer data();
		const_pointer data() const;

		bool empty() const;
		size_type size() const;
		void reserve( size_type new_cap );
		size_type capacity() const;

		void clear();
		void insert(size_type pos, const T& value);
		void insert(size_type pos, T&& value );
		void insert(size_type pos, size_t count, const T& value);
		template<typename... Args>
		void emplace(size_type pos, Args&&... args );
		void push_back( const T& value );
		void push_back( T&& value );
		template<typename... Args >
		reference emplace_back( Args&&... args );
		void pop_back();
		void resize( size_type count );
		void resize( size_type count, const T& value );
		void swap( MyVector& other ) noexcept;
	private: 
		size_type m_size;
		size_type m_capacity;
		value_type* m_data;
};

template <class T> 
bool operator==(const MyVector<T>& x, const MyVector<T>& y);

template <class T> 
bool operator< (const MyVector<T>& x, const MyVector<T>& y);

template <class T> 
bool operator!=(const MyVector<T>& x, const MyVector<T>& y);

template <class T> 
bool operator> (const MyVector<T>& x, const MyVector<T>& y);

template <class T> 
bool operator>=(const MyVector<T>& x, const MyVector<T>& y);

template <class T> 
bool operator<=(const MyVector<T>& x, const MyVector<T>& y);


#include "../src/vector.cpp"
#endif