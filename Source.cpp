#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <memory>
using namespace std;
constexpr uint64_t _zero = 0;

class layer0 {
protected:
	static uint64_t _l0_count;
public:
	layer0() {
		++_l0_count;
		cout << "instance of layer0 constructed!" << endl;
	}
	friend void _show_l0_count();
	virtual ~layer0() {
		cout << "instance of layer0 destructed!" << endl;
		--_l0_count;
	}
};

uint64_t layer0::_l0_count = 0;

static inline void _show_l0_count() {
	cout << layer0::_l0_count;
}

class layer1_a :virtual protected layer0 {
public:
	layer1_a() {
		cout << "instance of layer1_a constructed!" << endl;
	}
	virtual void func() = 0;
	~layer1_a() {
		cout << "instance of layer1_a destructed!" << endl;
	}
};

class layer1_b :virtual protected layer0 {
public:
	layer1_b() {
		cout << "instance of layer1_b constructed!" << endl;
	}
	virtual void func() = 0;
	~layer1_b() {
		cout << "instance of layer1_b destructed!" << endl;
	}
};

template<typename T>
static void _pass_count_addr(uint64_t &addr) {
	addr = reinterpret_cast<uint64_t>(&layer2<T>::_l2_count);
}

template<typename T>
class layer2 : public layer1_a, public layer1_b {
private:
	static uint64_t _l2_count;
	T content;
public:
	layer2(T x) {
		cout << "instance of layer2 constructed!" << endl;
		++_l2_count;
		content = x;
	}
	void layer1_a::func() {
		cout << "layer2 func derived from layer1_a" << endl;
	}
	void layer1_b::func() {
		cout << "layer2 func derived from layer1_b" << endl;
	}
	friend void _pass_count_addr<T>(uint64_t &addr);
	const T operator +(const layer2<T> &obj) {
		return [=](const layer2<T> &obj1, const layer2<T> &obj2) -> T {return obj1.content + obj2.content;}(*this, obj);
	}
	~layer2() {
		--_l2_count;
		cout << "instance of layer2 destructed!" << endl;
	}
};

template<typename T>
uint64_t layer2<T>::_l2_count = _zero;

template<typename T>
void test(uint64_t &addr2, T val1, T val2 = "def") {
	uint64_t addr = 0xFFFFFFFFFFFFFFFF;
	shared_ptr<layer1_a> ptr1(new layer2<T>(val1));
	shared_ptr<layer1_b> ptr2(new layer2<T>(val2));
	ptr1->func();
	ptr2->func();
	_pass_count_addr<T>(addr);
	cout << "There are " << *reinterpret_cast<uint64_t *>(addr) << " objects referencing layer2." << endl;
	cout << "There are ";
	_show_l0_count();
	cout << " instances of layer 0" << endl;
	cout << "obj1 = " << val1 << endl;
	cout << "obj2 = " << val2 << endl;
	cout << "obj1 + obj2 = " << *dynamic_cast<layer2<T> *>(ptr1.get()) + *dynamic_cast<layer2<T> *>(ptr2.get()) << endl;
	addr2 = addr;
}

void main() {
	uint64_t addr = 0xFFFFFFFFFFFFFFFF;
	test<string>(addr, "abc");
	if (!*reinterpret_cast<uint64_t *>(addr))
		cout << "All objects have been destructed." << endl;
	system("pause");
}
