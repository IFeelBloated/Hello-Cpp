#include <iostream>
#include <algorithm>
#include <malloc.h>
#include <cstdint>
#include <cstdlib>

constexpr auto VERSION = (1ull << 63) + 0; //v1.0

static inline auto _2int(char val) {
	return val - '0';
}

auto _length(const char *val) {
	auto i = 0;
	while (val[i])
		++i;
	return i;
}

auto _length(uint64_t val) {
	auto i = 0;
	while (val) {
		++i;
		val /= 10;
	}
	return i;
}

class uintN_t final {
private:
	char *ptr = nullptr;
	int length = 0;
public:
	uintN_t() = default;
	uintN_t(const char *val) {
		length = _length(val);
		ptr = new char[length];
		for (auto i = 0; i < length; ++i)
			ptr[i] = _2int(val[i]);
	}
	uintN_t(uint64_t val) {
		length = _length(val);
		ptr = new char[length];
		for (auto i = 0; i < length; ++i) {
			ptr[length - i - 1] = val % 10;
			val /= 10;
		}
	}
	uintN_t(const uintN_t &obj) {
		length = obj.length;
		ptr = new char[length];
		memcpy(ptr, obj.ptr, length * sizeof(char));
	}
	uintN_t(uintN_t &&obj) {
		length = obj.length;
		ptr = obj.ptr;
		obj.ptr = nullptr;
	}
	~uintN_t() {
		delete[] ptr;
		ptr = nullptr;
	}
	auto operator=(uintN_t &&obj) {
		if (this != &obj) {
			length = obj.length;
			delete[] ptr;
			ptr = obj.ptr;
			obj.ptr = nullptr;
		}
		return *this;
	}
	auto operator=(const uintN_t &obj) {
		if (this != &obj) {
			length = obj.length;
			delete[] ptr;
			ptr = new char[length];
			memcpy(ptr, obj.ptr, length * sizeof(char));
		}
		return *this;
	}
	friend auto operator<<(std::ostream &, const uintN_t &)->std::ostream &;
	friend auto operator+(const uintN_t &, const uintN_t &);
};

auto operator<<(std::ostream &out, const uintN_t &obj)->std::ostream & {
	for (auto i = 0; i < obj.length; ++i)
		out << static_cast<int>(obj.ptr[i]);
	return out;
}

auto operator+(const uintN_t &a, const uintN_t &b) {
	uintN_t tmp;
	tmp.length = std::max(a.length, b.length) + 1;
	tmp.ptr = new char[tmp.length];
	tmp.ptr[0] = 0;
	if (a.length > b.length) {
		memcpy(tmp.ptr + 1, a.ptr, a.length);
		for (auto i = 0; i < b.length; ++i) {
			tmp.ptr[tmp.length - i - 1] += b.ptr[b.length - i - 1];
			if (tmp.ptr[tmp.length - i - 1] > 9) {
				tmp.ptr[tmp.length - i - 1] -= 10;
				++tmp.ptr[tmp.length - i - 2];
			}
		}
	}
	else {
		memcpy(tmp.ptr + 1, b.ptr, b.length);
		for (auto i = 0; i < a.length; ++i) {
			tmp.ptr[tmp.length - i - 1] += a.ptr[a.length - i - 1];
			if (tmp.ptr[tmp.length - i - 1] > 9) {
				tmp.ptr[tmp.length - i - 1] -= 10;
				++tmp.ptr[tmp.length - i - 2];
			}
		}
	}
	if (!tmp.ptr[0]) {
		--tmp.length;
		auto ptr = alloca(tmp.length * sizeof(char));
		memcpy(ptr, tmp.ptr + 1, tmp.length * sizeof(char));
		delete[] tmp.ptr;
		tmp.ptr = new char[tmp.length];
		memcpy(tmp.ptr, ptr, tmp.length * sizeof(char));
	}
	return tmp;
}

auto main()->void {
	uintN_t ultra_large = 0xFFFFFFFFFFFFFFFFull;
	uintN_t wtf;
	wtf = "173492749827498274924024047847892324234825723691230213092827474923492834793274924793274247294892374";
	std::cout << ultra_large + wtf + 1234567ull << std::endl;
	system("pause");
}
