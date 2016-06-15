#include <iostream>
#include <cstdint>
#include <cstdlib>

constexpr auto VERSION = (1ull << 63) + 3; //v1.3

class staff {
protected:
	static uint64_t count;
	uint64_t *_data = nullptr;
	staff() = default;
public:
	staff(const staff &) = delete;
	staff(staff &&) = delete;
	~staff() {
		delete[] _data;
		_data = nullptr;
	}
	auto operator=(const staff &)->staff & = delete;
	auto operator=(staff &&)->staff & = delete;
	auto set_salary(uint64_t val) {
		_data[2] = val;
	}
	auto get_salary() const {
		return _data[2];
	}
	auto set_SN(uint64_t val) {
		_data[0] = val;
	}
	auto get_SN() const {
		return _data[0];
	}
	auto set_rank(uint64_t val) {
		_data[1] = val;
	}
	auto get_rank() const {
		return _data[1];
	}
	virtual uint64_t calc(uint64_t, ...) const = 0;
};

class parttime final :public staff {
public:
	parttime(uint64_t val, ...) {
		_data = new uint64_t[5]{ count, val, 0, *(&val + 1), *(&val + 2) };
		_data[2] = calc(_data[3], _data[4]);
		++count;
	}
	parttime(const parttime &) = delete;
	parttime(parttime &&) = delete;
	~parttime() = default;
	auto operator=(const parttime &)->parttime & = delete;
	auto operator=(parttime &&obj)->parttime & = delete;
	uint64_t calc(uint64_t val, ...) const override {
		return val * *(&val + 1);
	}
};

class fulltime final :public staff {
public:
	fulltime(uint64_t val, ...) {
		_data = new uint64_t[4]{ count, val, 0, *(&val + 1) };
		_data[2] = calc(_data[3]);
		++count;
	}
	fulltime(const fulltime &) = delete;
	fulltime(fulltime &&) = delete;
	~fulltime() = default;
	auto operator=(const fulltime &)->fulltime & = delete;
	auto operator=(fulltime &&obj)->fulltime & = delete;
	uint64_t calc(uint64_t val, ...) const override {
		return 2000 + 50 * val;
	}
};

class sales final :public staff {
public:
	sales(uint64_t val, ...) {
		_data = new uint64_t[4]{ count, val, 0, *(&val + 1) };
		_data[2] = calc(_data[3]);
		++count;
	}
	sales(const sales &) = delete;
	sales(sales &&) = delete;
	~sales() = default;
	auto operator=(const sales &)->sales & = delete;
	auto operator=(sales &&obj)->sales & = delete;
	uint64_t calc(uint64_t val, ...) const override {
		return 1000 + static_cast<uint64_t>(0.01 * val);
	}
};

uint64_t staff::count = 0;

static inline auto operator<<(std::ostream &out, const staff &obj)->std::ostream & {
	out << "SN: " << obj.get_SN() << std::endl;
	out << "rank: " << obj.get_rank() << std::endl;
	out << "salary(USD): " << obj.get_salary();
	return out;
}

auto main()->void {
	parttime array1[] = { { 3, 2000, 100 },{ 1, 100, 130 },{ 5, 80000, 20 } };
	fulltime array2[] = { { 6, 10 },{ 4, 5 },{ 0, 2 } };
	sales array3[] = { { 2, 100000 },{ 3, 5000000 },{ 4, 800000000 } };
	auto _rank_statistics = [&](uint64_t rank) {
		auto i = 0ull;
		for (auto &x : array1)
			if (x.get_rank() > rank)
				++i;
		for (auto &x : array2)
			if (x.get_rank() > rank)
				++i;
		for (auto &x : array3)
			if (x.get_rank() > rank)
				++i;
		if (!i)
			std::cout << "no member has a rank superior to rank" << rank << std::endl;
		else if (i == 1ull)
			std::cout << "1 member has a rank superior to rank" << rank << std::endl;
		else
			std::cout << i << " members have ranks superior to rank" << rank << std::endl;
	};
	for (auto &x : array1)
		std::cout << x << std::endl << std::endl;
	for (auto &x : array2)
		std::cout << x << std::endl << std::endl;
	for (auto &x : array3)
		std::cout << x << std::endl << std::endl;
	_rank_statistics(6);
	_rank_statistics(5);
	_rank_statistics(3);
	staff *poly[3] = { &array1[0], &array2[0], &array3[0] };
	std::cout << std::endl << "Polymorphism: parttime, fulltime, sales:" << std::endl;
	for (auto x : poly)
		std::cout << x->calc(100000, 200) << std::endl;
	system("pause");
}
