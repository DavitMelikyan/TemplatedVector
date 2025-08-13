#include <iostream>
#include "vector.tpp"

template <typename T>
void printVec(const MyVector<T>& vec) {
	for (size_t i = 0; i < vec.size(); ++i) {
		std::cout << vec[i] << " ";
	}
	std::cout << std::endl;
}

int main() {
    MyVector<char> v1;
    v1.push_back('a');
    v1.push_back('b');
    v1.push_back('c');

    MyVector<char> v2(5);
    MyVector<char> v3(5, 'x');

    MyVector<char> v4 = v3;
    MyVector<char> v5 = std::move(v4);

    v1 = v3;
    v2 = std::move(v1);

    v3 = {'a', 'b', 'c', 'd', 'e'};

    v5.reserve(20);
    std::cout << v5.capacity() << std::endl;

    std::cout << v3.at(2) << std::endl;
    std::cout << v3[1] << std::endl;
    std::cout << v3.front() << std::endl;
    std::cout << v3.back() << std::endl;
    std::cout << *v3.data() << std::endl;

    std::cout << v3.empty() << std::endl;
    std::cout << v3.size() << std::endl;

    v3.clear();
    std::cout << v3.size() << std::endl;

    v5.push_back('y');
    v5.push_back('z');
    v5.emplace_back('m');
    v5.emplace_back('n');
    v5.pop_back();

    v5.resize(10);
    v5.resize(12, 'k');

    MyVector<char> v6 (3);
	v6 = {'a', 'b', 'c'};
    MyVector<char> v7 (3);
	v7 = {'x', 'y', 'z'};
    v6.swap(v7);

    std::cout << (v6 == v7) << std::endl;
    std::cout << (v6 != v7) << std::endl;
    std::cout << (v6 < v7) << std::endl;
    std::cout << (v6 > v7) << std::endl;
    std::cout << (v6 <= v7) << std::endl;
    std::cout << (v6 >= v7) << std::endl;

    printVec(v5);
    printVec(v6);
    printVec(v7);

    return 0;
}