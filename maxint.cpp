#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <malloc.h>
#include <cstdint>

struct element final {
	char *data = nullptr;
	uint64_t aligned_data = 0;
	uint64_t length = 0;
	static uint64_t max_length;
	element() = default;
	element(element &&obj) {
		data = obj.data;
		aligned_data = obj.aligned_data;
		length = obj.length;
		obj.data = nullptr;
	}
	auto &operator=(element &&obj) {
		if (this != &obj) {
			delete[] data;
			data = obj.data;
			aligned_data = obj.aligned_data;
			length = obj.length;
			obj.data = nullptr;
		}
		return *this;
	}
	element(const element &) = delete;
	auto &operator=(const element &) = delete;
	~element() {
		delete[] data;
	}
};

decltype(element::max_length) element::max_length = 0;

auto operator<(const element &a, const element &b) {
	if (a.aligned_data != b.aligned_data)
		return a.aligned_data < b.aligned_data;
	else if (a.length == b.length)
		return false;
	else {
		auto min_length = std::min(a.length, b.length);
		auto min_string = (a.length > b.length) ? b.data : a.data;
		auto ptr = min_string;
		while (ptr[0] == min_string[0] && ptr < min_string + min_length - 1)
			++ptr;
		if (ptr[0] > min_string[0])
			return a.length > b.length;
		else
			return b.length > a.length;
	}
}

auto &operator>>(std::istream &in, element &obj) {
	auto var = 0ull;
	auto ConvertToData = [&]() {
		auto GetLength = [&, var]() mutable {
			obj.length = 0;
			while (var) {
				++obj.length;
				var /= 10;
			}
		};
		auto ConvertToString = [&, var]() mutable {
			delete[] obj.data;
			obj.data = new char[obj.length];
			for (auto i = 0; i < obj.length; ++i) {
				obj.data[obj.length - i - 1] = var % 10;
				var /= 10;
			}
		};
		GetLength();
		ConvertToString();
	};
	in >> var;
	ConvertToData();
	return in;
}

auto &operator<<(std::ostream &out, const element &obj) {
	auto buffer = reinterpret_cast<char *>(alloca((obj.length + 1) * sizeof(char)));
	auto StringToBuffer = [=, &obj]() {
		std::memcpy(buffer, obj.data, obj.length * sizeof(char));
		for (auto i = 0; i < obj.length; ++i)
			buffer[i] += '0';
		buffer[obj.length] = 0;
	};
	StringToBuffer();
	out << buffer;
	return out;
}

auto Align = [](auto arr, auto length) {
	auto GetMaxLength = [=]() {
		for (auto i = 0; i < length; ++i)
			element::max_length = std::max(element::max_length, arr[i].length);
	};
	auto GetAlignedData = [=]() {
		auto buffer = reinterpret_cast<char *>(alloca(element::max_length * sizeof(char)));
		auto DuplicateDigits = [=](auto &obj) {
			std::memcpy(buffer, obj.data, obj.length * sizeof(char));
			auto ptr = buffer + obj.length;
			auto extra_length = element::max_length - obj.length;
			while (extra_length / obj.length) {
				std::memcpy(ptr, buffer, obj.length * sizeof(char));
				extra_length -= obj.length;
				ptr += obj.length;
			}
			if (extra_length)
				std::memcpy(ptr, buffer, extra_length * sizeof(char));
		};
		auto StringToInteger = [=](auto &obj) {
			obj.aligned_data = 0;
			for (auto i = 0; i < element::max_length; ++i) {
				obj.aligned_data *= 10;
				obj.aligned_data += buffer[i];
			}
		};
		for (auto i = 0; i < length; ++i) {
			DuplicateDigits(arr[i]);
			StringToInteger(arr[i]);
		}
	};
	GetMaxLength();
	GetAlignedData();
};

auto main()->int {
	auto count = 0;
	std::cin >> count;
	auto arr = new element[count];
	for (auto i = 0; i < count; ++i)
		std::cin >> arr[i];
	Align(arr, count);
	std::sort(arr, arr + count);
	for (auto i = count - 1; i >= 0; --i)
		std::cout << arr[i];
	std::cout << std::endl;
	delete[] arr;
	system("pause");
}
