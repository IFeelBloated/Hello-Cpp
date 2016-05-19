#include <iostream>
#include <cstdlib>

constexpr auto VERSION = (1ull << 63) + 0; //v1.0
constexpr auto PI = 3.1415926535897932384626433832795;
constexpr auto DENSITY = 100.;

class _graphic_3d {
protected:
	double *val = new double[2];
public:
	_graphic_3d() = default;
	_graphic_3d(const _graphic_3d &) = delete;
	_graphic_3d(_graphic_3d &&obj) {
		*this = static_cast<_graphic_3d &&>(obj);
	}
	virtual ~_graphic_3d() {
		delete[] val;
		val = nullptr;
	}
	auto operator=(const _graphic_3d &)->_graphic_3d & = delete;
	auto operator=(_graphic_3d &&obj)->_graphic_3d & {
		if (this != &obj) {
			delete[] val;
			val = obj.val;
			obj.val = nullptr;
		}
		return *this;
	}
	virtual auto calc(double val, ...)->void {
		this->val[0] = val;
		this->val[1] = *(&val + 1);
	}
	const double *ptrstealer() const {
		return val;
	}
	friend auto operator+(const _graphic_3d &a, const _graphic_3d &b)->_graphic_3d;
	friend auto operator-(const _graphic_3d &a, const _graphic_3d &b)->_graphic_3d;
};

static inline auto operator+(const _graphic_3d &a, const _graphic_3d &b)->_graphic_3d {
	_graphic_3d tmp;
	tmp.calc(a.val[0] + b.val[0], a.val[1] + b.val[1]);
	return tmp;
}

static inline auto operator-(const _graphic_3d &a, const _graphic_3d &b)->_graphic_3d {
	_graphic_3d tmp;
	tmp.calc(a.val[0] - b.val[0], a.val[1] - b.val[1]);
	return tmp;
}

class ball final :public _graphic_3d {
public:
	auto calc(double val, ...)->void override {
		this->val[0] = 4 * PI * val * val;
		this->val[1] = 4. / 3. * PI * val * val * val;
	}
};

class cuboid final :public _graphic_3d {
public:
	auto calc(double val, ...)->void override {
		this->val[0] = 2. * (val * *(&val + 1) + val * *(&val + 2) + *(&val + 1) * *(&val + 2));
		this->val[1] = val * *(&val + 1) * *(&val + 2);
	}
};

auto main()->void {
	ball ballstuff;
	cuboid cuboidstuff;
	_graphic_3d overlapped;
	ballstuff.calc(3.);
	cuboidstuff.calc(4., 3., 3.);
	overlapped.calc(4., 0.);
	_graphic_3d combo = ballstuff + cuboidstuff + ballstuff - overlapped;
	std::cout << "surface area: " << combo.ptrstealer()[0] << " cm^2" << std::endl;
	std::cout << "mass: " << DENSITY * combo.ptrstealer()[1] << " g" << std::endl;
	system("pause");
}
