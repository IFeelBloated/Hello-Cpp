#include <iostream>
#include <cstdlib>

constexpr auto VERSION = (1ull << 63) + 0; //v1.0
constexpr auto _ZEROPOINT_THRESHOLD = 1e-16;

class complex final {
private:
	double *_data = new double[2];
public:
	complex() = default;
	complex(double val) {
		_data[0] = val;
		_data[1] = 0.;
	}
	complex(double val1, double val2) {
		_data[0] = val1;
		_data[1] = val2;
	}
	complex(const complex &) = delete;
	complex(complex &&obj) {
		*this = static_cast<complex &&>(obj);
	}
	~complex() {
		delete[] _data;
		_data = nullptr;
	}
	auto operator=(complex &&obj)->complex & {
		if (this != &obj) {
			delete[] _data;
			_data = obj._data;
			obj._data = nullptr;
		}
		return *this;
	}
	auto operator=(const complex &)->complex & = delete;
	friend auto operator+(const complex &a, const complex &b)->complex;
	friend auto operator+(const complex &a, double b)->complex;
	friend auto operator+(double a, const complex &b)->complex;
	friend auto operator<<(std::ostream &out, const complex &obj)->std::ostream &;
};

static inline auto operator+(const complex &a, const complex &b)->complex {
	complex tmp(a._data[0] + b._data[0], a._data[1] + b._data[1]);
	return tmp;
}

static inline auto operator+(double a, const complex &b)->complex {
	complex tmp(a + b._data[0], b._data[1]);
	return tmp;
}

static inline auto operator+(const complex &a, double b)->complex {
	complex tmp(a._data[0] + b, a._data[1]);
	return tmp;
}

auto operator<<(std::ostream &out, const complex &obj)->std::ostream & {
	auto _zero = [](double val) {
		if (abs(val) < _ZEROPOINT_THRESHOLD)
			return true;
		else
			return false;
	};
	if (_zero(obj._data[0]) && _zero(obj._data[1]))
		out << 0;
	else if (_zero(obj._data[0]))
		out << obj._data[1] << "i";
	else if (_zero(obj._data[1]))
		out << obj._data[0];
	else
		if (obj._data[1] > 0.)
			out << obj._data[0] << "+" << obj._data[1] << "i";
		else
			out << obj._data[0] << obj._data[1] << "i";
	return out;
}

auto main()->void {
	complex val1;
	val1 = { 1.23, 0.57 };
	complex val2 = { 2.34, 1.68 };
	complex val3 = -3.57;
	complex val4 = val1 + val2 + val3;
	complex val5 = -1.23 + val1;
	std::cout << "val1 = " << val1 << std::endl;
	std::cout << "val2 = " << val2 << std::endl;
	std::cout << "val3 = " << val3 << std::endl;
	std::cout << "val1 + val2 + val3 = " << val4 << std::endl;
	std::cout << "-1.23 + val1 = " << val5 << std::endl;
	system("pause");
}
