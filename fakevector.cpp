#include <iostream>
#include <cstdint>
#include <cstdlib>

constexpr auto VERSION = (1ull << 63) + 0; //v1.0

template<typename T>
struct node {
	T *data = nullptr;
	node *ptr = nullptr;
};

template<typename T>
class fakevector final {
public:
	uint64_t length = 0;
	node<T> *ptr = nullptr;
public:
	fakevector() = default;
	fakevector(const fakevector &) = delete;
	fakevector(fakevector &&obj) {
		length = obj.length;
		ptr = obj.ptr;
		obj.ptr = nullptr;
	}
	~fakevector() {
		for (auto i = 0ull; i < length; ++i) {
			delete ptr->data;
			ptr->data = nullptr;
			auto *tmp = ptr->ptr;
			delete ptr;
			ptr = tmp;
		}
	}
	auto operator=(const fakevector &)->fakevector & = delete;
	auto operator=(fakevector &&obj)->fakevector & {
		if (this != &obj) {
			length = obj.length;
			ptr = obj.ptr;
			obj.ptr = nullptr;
		}
		return *this;
	}
	auto push_back(const T &obj) {
		++length;
		auto *next = new node<T>;
		next->data = new T(obj);
		next->ptr = ptr;
		ptr = next;
	}
	auto operator[](uint64_t index)->T & {
		auto *dest = ptr;
		for (auto i = 0ull; i < length - index - 1; ++i)
			dest = dest->ptr;
		return *(dest->data);
	}
	auto size() {
		return length;
	}
};

auto main()->void {
	fakevector<decltype(0.)> vec;
	vec.push_back(1.23);
	vec.push_back(2.34);
	vec.push_back(3.45);
	for (auto i = 0ull; i < vec.size(); ++i)
		std::cout << "vec[" << i << "] = " << vec[i] << std::endl;
	vec[0] = 3.14;
	std::cout << std::endl << "vec[0] = " << vec[0] << std::endl;
	system("pause");
}
