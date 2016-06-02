#include <iostream>
#include <cstdlib>

class A {
public:
	auto f1() {}
	A() {
		std::cout << "constructor from A" << std::endl;
		std::cout << "i1=" << i1 << " j1=" << j1 << std::endl;
	}
	~A() {
		std::cout << "Destructor from A" << std::endl;
	}
private:
	int i1 = 10;
protected:
	int j1 = i1;
};

class B :public A {
public:
	auto f2() {}
	B() {
		std::cout << "constructor from B" << std::endl;
		std::cout << "i2=" << i2 << " j2=" << j2 << std::endl;
	}
	~B() {
		std::cout << "Destructor from B" << std::endl;
	}
protected:
	int j2 = 21;
private:
	int i2 = 20;
};

class C final :public B {
public:
	auto f3() {}
	C() {
		std::cout << "constructor from C" << std::endl;
		std::cout << "i3=" << i3 << " j3=" << j3 << std::endl;
	}
	~C() {
		std::cout << "Destructor from C" << std::endl;
	}
protected:
	int j3 = 21;
private:
	int i3 = 20;
};

auto main()->void {
	[]() {
		A a;
		B b;
		C c;
	}();
	system("pause");
}

/* A::f1(), A::j1 -> accessible
   A::i1 -> direct access denied, accessible via raw memory manipulation
*/

/* A::f1() -> accessible
   A::i1, A::j1 -> direct peripheral access denied, accessible via raw memory manipulation
*/

/* A::f1(), A::j1, B::f2(), B::j2 -> accessible
   A::i1, B::i2 -> direct access denied, accessible via raw memory manipulation
*/

/* A::f1(), B::f2() -> accessible
   A::i1, A::j1, B::i2, B::j2 -> direct peripheral access denied, accessible via raw memory manipulation
*/

/* Constructor: inner objects/inherited classes -> current object
   Destructor: reverse order of the constructor
*/

/* protected and private derivation:
     A::f1() goes inaccessible to object b and c, accessible via reinterpret_cast-ing the pointer to A *
   private derivation:
     A::f1(), A::j1 goes inaccessible to C::f3(), accessible via raw memory manipulation
*/
