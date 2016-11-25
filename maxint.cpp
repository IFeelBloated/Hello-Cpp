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
	auto operator=(element &&obj)->element & {
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
	auto operator=(const element &)->element & = delete;
	~element() {
		delete[] data;
	}
};

uint64_t element::max_length = 0;

auto operator<(const element &a, const element &b) {
	if (a.aligned_data != b.aligned_data)
		return a.aligned_data < b.aligned_data;
	else if (a.length == b.length)
		return false;
	else {
		auto max_length = std::max(a.length, b.length);
		auto max_string = (a.length > b.length) ? a.data : b.data;
		auto ptr = max_string + 1;
		while (ptr[0] == max_string[0] && ptr < max_string + max_length)
			++ptr;
		if (ptr[0] > max_string[0])
			return a.length > b.length;
		else
			return b.length > a.length;
	}
}

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

auto operator<<(std::ostream &out, const element &obj)->std::ostream & {
	auto buffer = reinterpret_cast<char *>(alloca((obj.length + 1) * sizeof(char)));
	auto StringToBuffer = [&]() {
		std::memcpy(buffer, obj.data, obj.length * sizeof(char));
		for (auto i = 0; i < obj.length; ++i)
			buffer[i] += '0';
		buffer[obj.length] = 0;
	};
	StringToBuffer();
	out << buffer;
	return out;
}

auto Align(element *arr, const uint64_t length) {
	auto GetMaxLength = [=]() {
		for (auto i = 0; i < length; ++i)
			element::max_length = std::max(element::max_length, arr[i].length);
	};
	auto GetAlignedData = [=]() {
		auto buffer = reinterpret_cast<char *>(alloca(element::max_length * sizeof(char)));
		auto DuplicateDigits = [=](const element &obj) {
			std::memcpy(buffer, obj.data, obj.length * sizeof(char));
			auto ptr = buffer + obj.length;
			auto extra_length = element::max_length - obj.length;
			while (extra_length / obj.length) {
				std::memcpy(ptr, buffer, obj.length);
				extra_length -= obj.length;
				ptr += obj.length;
			}
			if (extra_length)
				std::memcpy(ptr, buffer, extra_length);
		};
		auto StringToInteger = [=](element &obj) {
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
}

auto main()->int {
	system("pause");
}
