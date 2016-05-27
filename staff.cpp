#include <iostream>
#include <functional>
#include <array>
#include <cstdint>
#include <cstdlib>

constexpr auto VERSION = (1ull << 63) + 1; //v1.1

class staff final {
private:
	static uint64_t count;
	uint64_t *_data = new uint64_t[3]{ count, 0, 0 };
	std::function<void(uint64_t)> _salary_calc = [this](uint64_t val) {
		constexpr auto c = 5000ull;
		constexpr auto coeff = 10000ull;
		if (!val)
			_data[2] = coeff * _data[1] + c;
		else
			_data[2] = val;
	};
public:
	staff(uint64_t val1 = 0, uint64_t val2 = 0) {
		_data[1] = val1;
		_salary_calc(val2);
		++count;
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
	auto get_salary() {
		return _data[2];
	}
	auto set_SN(uint64_t val) {
		_data[0] = val;
	}
	auto get_SN() {
		return _data[0];
	}
	auto set_rank(uint64_t val) {
		_data[1] = val;
		_salary_calc(0);
	}
	auto get_rank() {
		return _data[1];
	}
};

uint64_t staff::count = 0;

static inline auto operator<<(std::ostream &out, const staff &obj)->std::ostream & {
	out << "SN: " << obj._data[0] << std::endl;
	out << "rank: " << obj._data[1] << std::endl;
	out << "salary(USD): " << obj._data[2];
	return out;
}

auto main()->void {
	std::array<staff, 20> _test_array;
	auto _rank_statistics = [&](uint64_t rank) {
		auto i = 0ull;
		for (auto &x : _test_array)
			if (x.get_rank() > rank)
				++i;
		if (!i)
			std::cout << "no member has a rank superior to rank" << rank << std::endl;
		else if (i == 1ull)
			std::cout << "1 member has a rank superior to rank" << rank << std::endl;
		else
			std::cout << i << " members have ranks superior to rank" << rank << std::endl;
	};
	_test_array[0].set_rank(6);
	_test_array[4].set_rank(3);
	_test_array[7].set_rank(4);
	_test_array[11].set_rank(1);
	_test_array[13].set_rank(5);
	for (auto &x : _test_array)
		std::cout << x << std::endl << std::endl;
	_rank_statistics(6);
	_rank_statistics(5);
	_rank_statistics(3);
	system("pause");
}

