#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <malloc.h>
#include <cstdint>

struct element final {
	char *data[3] = { nullptr, nullptr, nullptr };
	uint64_t aligned_data[2] = { 0, 0 };
	uint64_t length = 0;
	uint64_t index = 0xFFFFFFFFFFFFFFFFull;
	static uint64_t max_length;
	element() = default;
	element(const element &) = delete;
	auto operator=(const element &)->element & = delete;
	element(element &&) = delete;
	auto operator=(element &&)->element & = delete;
	~element() {
		for (auto i = 0; i < 3; ++i)
			delete[] data[i];
	}
};

uint64_t element::max_length = 0;

auto operator>>(std::istream &in, element &obj)->std::istream & {
	auto var = 0ull;
	auto ConvertToData = [&]() {
		auto GetLength = [&, var]() mutable {
			obj.length = 0;
			while (var) {
				++obj.length;
				var /= 10;
			}
		};
		auto ToString = [&, var]() mutable {
			obj.data[0] = new char[obj.length];
			for (auto i = 0; i < obj.length; ++i) {
				obj.data[0][obj.length - i - 1] = var % 10;
				var /= 10;
			}
		};
		obj.~element();
		GetLength();
		ToString();
	};
	in >> var;
	ConvertToData();
	return in;
}

auto operator<<(std::ostream &out, const element &obj)->std::ostream & {
	auto buffer = reinterpret_cast<char *>(alloca((obj.length + 1) * sizeof(char)));
	std::memcpy(buffer, obj.data[0], obj.length);
	for (auto i = 0; i < obj.length; ++i)
		buffer[i] += '0';
	buffer[obj.length] = 0;
	out << buffer;
	return out;
}

auto Align(element *arr, const uint64_t length) {
	auto GetMaxLength = [=]() {
		for (auto i = 0; i < length; ++i)
			element::max_length = std::max(element::max_length, arr[i].length);
	};
	auto Pad = [=]() {
		for (auto i = 0; i < length; ++i)
			if (arr[i].length < element::max_length) {
				delete[] arr[i].data[1];
				auto extra_length = element::max_length - arr[i].length;
				arr[i].data[1] = new char[extra_length];
				std::memset(arr[i].data[1], arr[i].data[0][arr[i].length - 1], extra_length);
			}
	};
	auto Duplicate = [=]() {};

	GetMaxLength();
	Pad();
}

auto main()->int {

	system("pause");
}
