/*
A simple peek of how programming works in C++14 style
*/

#include <iostream>
#include <functional>
#include <cstdlib>

constexpr auto VERSION = (1ull << 63) + 0; //v1.0
constexpr auto PI = 3.1415926535897932384626433832795;

class _graphic {
protected:
	decltype(0.) val = 0.;
public:
	_graphic() = default;
	_graphic(const _graphic &) = delete;
	auto operator=(const _graphic &) -> _graphic & = delete;
	virtual ~_graphic() = default;
	virtual auto calc(double val, ...) -> void = 0;
	virtual auto display() -> decltype(val) const final {
		return val;
	}
};

class _circle :virtual public _graphic {
public:
	_circle() = default;
	_circle(const _circle &) = delete;
	auto operator=(const _circle &) -> _circle & = delete;
	auto calc(double val, ...) -> void override {
		_algo(val);
	}
private:
	std::function<void(double)> _algo = [this](double val) {
		this->val = val * val * PI;
	};
};

class _cylinder :virtual public _graphic, public _circle {
public:
	_cylinder() = default;
	_cylinder(const _cylinder &) = delete;
	auto operator=(const _cylinder &) -> _cylinder & = delete;
	auto calc(double val, ...) -> void override {
		_circle::calc(val);
		_algo(val, *(&val + 1));
	}
private:
	std::function<void(double, double)> _algo = [this](double val1, double val2) {
		this->val *= 2.;
		this->val += 2. * PI * val1 * val2;
	};
};

class weight final :virtual public _graphic, public _cylinder, public _circle {
public:
	weight() = default;
	weight(const weight &) = delete;
	auto operator=(const weight &) -> weight & = delete;
	auto calc(double val, ...) -> void override {
		_cylinder::calc(val, *(&val + 1));
		auto tmp = this->val * 2.;
		_cylinder::calc(*(&val + 2), *(&val + 3));
		tmp += this->val;
		_circle::calc(*(&val + 2));
		tmp -= this->val * 4.;
		this->val = tmp;
	}
};

auto main() -> void {
	weight *obj = new weight;
	obj->calc(10., 5., 5., 20.);
	std::cout << obj->display() << std::endl;
	delete obj;
	obj = nullptr;
	system("pause");
}
