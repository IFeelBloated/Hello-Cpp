#include <iostream>
#include <functional>
#include <cstdint>
#include <cstdlib>

constexpr auto VERSION = (1ull << 63) + 0; //v1.0

class staff final {
private:
	uint64_t *_data = new uint64_t[3]{ 0,0,0 };
	std::function<void(uint64_t)> _salary_calc = [this](uint64_t val) {
		constexpr auto c = 5000ull;
		constexpr auto coeff = 10000ull;
		if (!val)
			_data[2] = coeff * _data[1] + c;
		else
			_data[2] = val;
	};
public:
	staff() {
		_salary_calc(0);
	}
	staff(uint64_t val) {
		_data[0] = val;
		_salary_calc(0);
	}
	staff(uint64_t val1, uint64_t val2) {
		_data[0] = val1;
		_data[1] = val2;
		_salary_calc(0);
	}
	staff(uint64_t val1, uint64_t val2, uint64_t val3) {
		_data[0] = val1;
		_data[1] = val2;
		_salary_calc(val3);
	}
	staff(const staff &) = delete;
	staff(staff &&obj) {
		*this = static_cast<staff &&>(obj);
	}
	~staff() {
		delete[] _data;
		_data = nullptr;
	}
	auto operator=(const staff &)->staff & = delete;
	auto operator=(staff &&obj)->staff & {
		if (this != &obj) {
			delete[] _data;
			_data = obj._data;
			obj._data = nullptr;
		}
		return *this;
	}
	friend auto operator<<(std::ostream &out, const staff &obj)->std::ostream &;
	std::function<void(uint64_t)> &set_salary = _salary_calc;
	auto get_salary()->const uint64_t & {
		return _data[2];
	}
	auto set_SN(uint64_t val)->void {
		_data[0] = val;
	}
	auto get_SN()->const uint64_t & {
		return _data[0];
	}
	auto set_rank(uint64_t val)->void {
		_data[1] = val;
	}
	auto get_rank()->const uint64_t & {
		return _data[1];
	}
};

static inline auto operator<<(std::ostream &out, const staff &obj)->std::ostream & {
	out << "SN: " << obj._data[0] << std::endl;
	out << "rank: " << obj._data[1] << std::endl;
	out << "salary(USD): " << obj._data[2];
	return out;
}

auto main()->void {
	staff test1 = 1;
	staff test2 = { 2, 1 };
	staff test3;
	test3 = { 3, 3, 45000 };
	std::cout << test1 << std::endl << std::endl << test2 << std::endl << std::endl << test3 << std::endl << std::endl;
	test3.set_salary(0);
	std::cout << test3 << std::endl;
	system("pause");
}
