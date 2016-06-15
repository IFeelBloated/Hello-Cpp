#include <iostream>
#include <cstdint>
#include <cstdlib>

constexpr auto VERSION = (1ull << 63) + 0; //v1.0

auto _length(const char *val) {
	auto i = 0ull;
	while (val[i])
		++i;
	return i;
}

class fakestring final {
private:
	char *ptr = nullptr;
	uint64_t length = 0;
public:
	fakestring() = default;
	fakestring(const char *val) {
		length = _length(val);
		ptr = new char[length];
		memcpy(ptr, val, length * sizeof(char));
	}
	fakestring(const fakestring &obj) {
		length = obj.length;
		ptr = new char[length];
		memcpy(ptr, obj.ptr, length * sizeof(char));
	}
	fakestring(fakestring &&obj) {
		length = obj.length;
		ptr = obj.ptr;
		obj.ptr = nullptr;
	}
	~fakestring() {
		delete[] ptr;
		ptr = nullptr;
	}
	auto operator=(fakestring &&obj)->fakestring & {
		if (this != &obj) {
			length = obj.length;
			delete[] ptr;
			ptr = obj.ptr;
			obj.ptr = nullptr;
		}
		return *this;
	}
	auto operator=(const fakestring &obj)->fakestring & {
		if (this != &obj) {
			length = obj.length;
			delete[] ptr;
			ptr = new char[length];
			memcpy(ptr, obj.ptr, length * sizeof(char));
		}
		return *this;
	}
	friend auto operator<<(std::ostream &, const fakestring &)->std::ostream &;
	friend auto operator+(const fakestring &, const fakestring &);
	friend auto operator==(const fakestring &, const fakestring &);
	auto operator+=(const fakestring &str)->fakestring & {
		auto oldlength = length;
		length += str.length;
		auto tmp = new char[length];
		memcpy(tmp, ptr, oldlength * sizeof(char));
		memcpy(tmp + oldlength, str.ptr, str.length * sizeof(char));
		delete[] ptr;
		ptr = tmp;
		tmp = nullptr;
		return *this;
	}
	auto operator[](uint64_t index)->char & {
		return ptr[index];
	}
	const char &operator[](uint64_t index) const {
		return ptr[index];
	}
};

auto operator<<(std::ostream &out, const fakestring &obj)->std::ostream & {
	for (auto i = 0ull; i < obj.length; ++i)
		out << obj.ptr[i];
	return out;
}

auto operator+(const fakestring &a, const fakestring &b) {
	fakestring tmp;
	tmp.length = a.length + b.length;
	tmp.ptr = new char[tmp.length];
	memcpy(tmp.ptr, a.ptr, a.length * sizeof(char));
	memcpy(tmp.ptr + a.length, b.ptr, b.length * sizeof(char));
	return tmp;
}

auto operator==(const fakestring &a, const fakestring &b) {
	if (&a != &b)
		if (a.length != b.length)
			return false;
		else {
			for (auto i = 0ull; i < a.length; ++i)
				if (a.ptr[i] != b.ptr[i])
					return false;
			return true;
		}
		return true;
}

auto operator!=(const fakestring &a, const fakestring &b) {
	return !(a == b);
}

auto main()->void {
	const fakestring s0 = " whatever";
	fakestring s1 = "123" + s0 + "!456 ";
	fakestring s2;
	s1 += "fakestring";
	s1[12] = s0[0];
	s2 = s1;
	std::cout << s2 << std::endl << (s0 == " whatever" ? "true" : "false") << std::endl;
	system("pause");
}
