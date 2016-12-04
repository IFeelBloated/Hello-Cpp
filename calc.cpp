#include <iostream>
#include <cstdint>
#include <cstdlib>

template<typename T>
class stack final {
private:
	T *base_ptr = nullptr;
	T *stack_ptr = nullptr;
public:
	auto &operator[](uint64_t index) {
		return *(stack_ptr - 1 - index);
	}
	auto operator+=(const T &obj) {
		*stack_ptr = obj;
		++stack_ptr;
	}
	auto operator--() {
		--stack_ptr;
	}
	auto IsEmpty() {
		return base_ptr == stack_ptr;
	}
	stack(uint64_t length = 0) {
		base_ptr = new T[length];
		stack_ptr = base_ptr;
	}
	stack(const stack &) = delete;
	stack(stack &&) = delete;
	auto &operator=(const stack &) = delete;
	auto &operator=(stack &&) = delete;
	~stack() {
		delete[] base_ptr;
	}
};

struct node final {
	int64_t value = 0;
	uint64_t priority = 0;
	bool IsOperator = false;
	auto &operator=(const char *string) {
		*this = node(string);
		return *this;
	}
	node(const char *string = nullptr) {
		auto IsNumber = [](auto value) {
			return value >= '0' && value <= '9';
		};
		auto IsSigned = [=]() {
			return string[0] == '+' || string[0] == '-';
		};
		auto OperatorToNode = [&]() {
			value = string[0];
			IsOperator = true;
			switch (string[0]) {
			case '+': case '-':
				priority = 1;
				break;
			case '*': case '/':
				priority = 2;
				break;
			case '(': case ')':
				priority = 3;
				break;
			default:
				break;
			}
		};
		auto IntegerToNode = [&]() {
			auto negative = false;
			auto StringToInteger = [&]() {
				auto GetLength = [=]() {
					auto i = 0ull;
					auto ptr = string;
					while (ptr[0] != ' ' && ptr[0] != 0 && ptr[0] != ')') {
						++i;
						++ptr;
					}
					return i;
				};
				value = 0;
				for (auto i = 0; i < GetLength(); ++i) {
					value *= 10;
					value += string[i] - '0';
				}
			};
			if (IsSigned()) {
				negative = string[0] == '-' ? true : false;
				++string;
			}
			StringToInteger();
			if (negative)
				value = -value;
		};
		if (string != nullptr)
			if (!IsNumber(string[0])) {
				if (IsSigned())
					if (IsNumber(string[1]))
						goto SIGNED;
				OperatorToNode();
			}
			else
				SIGNED: IntegerToNode();
	}
	node(const node &) = default;
	node(node &&) = default;
	auto operator=(const node &)->node & = default;
	auto operator=(node &&)->node & = default;
	~node() = default;
};

auto main()->int {
	system("pause");
}
