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
	auto operator+=(T &&obj) {
		*stack_ptr = static_cast<T &&>(obj);
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
	stack(stack &&obj) {
		base_ptr = obj.base_ptr;
		stack_ptr = obj.stack_ptr;
		obj.base_ptr = nullptr;
		obj.stack_ptr = nullptr;
	}
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
	auto operator==(int64_t value) {
		return this->value == value;
	}
	auto operator!=(int64_t value) {
		return this->value != value;
	}
	node(const char *string = nullptr) {
		auto IsNumber = [](auto value) {
			return value >= '0' && value <= '9';
		};
		auto IsSigned = [=]() {
			return string[0] == '+' || string[0] == '-';
		};
		auto OperatorToNode = [&] {
			value = string[0];
			IsOperator = true;
			switch (value) {
			case '+': case '-':
				priority = 1;
				break;
			case '*': case '/':
				priority = 2;
				break;
			default:
				break;
			}
		};
		auto IntegerToNode = [&]() {
			auto negative = false;
			auto StringToInteger = [&] {
				auto GetLength = [=]() {
					auto i = 0ull;
					auto ptr = string;
					while (ptr[0] != ' ' && ptr[0] && ptr[0] != ')') {
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

auto operator""NodesStack(uint64_t length) {
	return stack<node>(length);
}

auto operator""IntegersStack(uint64_t length) {
	return stack<int64_t>(length);
}

auto AnalyzeExpr = [](auto string, auto arr) {
	auto i = 0ull;
	while (string[0]) {
		if (string[0] != ' ') {
			arr[i] = string;
			++i;
		}
		if (string[0] == '(' || string[0] == ')' || string[0] == ' ')
			++string;
		else
			while (string[0] != ' ' && string[0] && string[0] != ')')
				++string;
	}
};

auto InfixToRPN = [](auto RPNArray, auto InfixArray, auto length) {
	auto RPNIndex = 0ull;
	auto OperatorStack = 10000NodesStack;
	auto PopToRPNArray = [&]() {
		RPNArray[RPNIndex] = OperatorStack[0];
		++RPNIndex;
		--OperatorStack;
	};
	auto OperandToRPNArray = [&](auto &obj) {
		RPNArray[RPNIndex] = obj;
		++RPNIndex;
	};
	auto OperatorToRPNArray = [&](auto &obj) {
		if (obj == '(')
			OperatorStack += obj;
		else if (obj == ')') {
			while (OperatorStack[0] != '(')
				PopToRPNArray();
			--OperatorStack;
		}
		else
			if (obj.priority > OperatorStack[0].priority)
				OperatorStack += obj;
			else {
				while (!OperatorStack.IsEmpty() && obj.priority <= OperatorStack[0].priority)
					PopToRPNArray();
				OperatorStack += obj;
			}
	};
	auto StackCleanup = [&]() {
		while (!OperatorStack.IsEmpty())
			PopToRPNArray();
	};
	for (auto i = 0ull; i < length; ++i)
		if (InfixArray[i].IsOperator)
			OperatorToRPNArray(InfixArray[i]);
		else
			OperandToRPNArray(InfixArray[i]);
	StackCleanup();
};

auto &operator<<(std::ostream &out, const node &obj) {
	if (obj.IsOperator)
		out << static_cast<char>(obj.value);
	else
		out << obj.value;
	return out;
}

auto CalculateRPN = [](auto RPNArray, auto length) {
	auto RPNStack = 10000IntegersStack;
	auto Calculate = [&](auto &obj) {
		auto Add = [&]() {
			RPNStack[1] += RPNStack[0];
			--RPNStack;
		};
		auto Subtract = [&]() {
			RPNStack[1] -= RPNStack[0];
			--RPNStack;
		};
		auto Multiply = [&]() {
			RPNStack[1] *= RPNStack[0];
			--RPNStack;
		};
		auto Divide = [&]() {
			RPNStack[1] /= RPNStack[0];
			--RPNStack;
		};
		switch (obj.value) {
		case '+':
			Add();
			break;
		case '-':
			Subtract();
			break;
		case '*':
			Multiply();
			break;
		case '/':
			Divide();
			break;
		default:
			break;
		}
	};
	for (auto i = 0ull; i < length; ++i)
		if (!RPNArray[i].IsOperator)
			RPNStack += RPNArray[i].value;
		else
			Calculate(RPNArray[i]);
	return RPNStack[0];
};

auto main()->int {
	constexpr auto MaximumLength = 10485760ull;
	auto Buffer = new char[MaximumLength];
	auto InputExpression = [=] {
		std::cout << "Expression: ";
		std::cin.getline(Buffer, MaximumLength);
	};
	auto GetNodeCount = [=](auto rpn) {
		auto i = 0ull;
		auto ptr = Buffer;
		while (ptr[0]) {
			if (rpn) {
				if (ptr[0] == ' ')
					++i;
			}
			else
				if (ptr[0] == ' ' || ptr[0] == '(' || ptr[0] == ')')
					++i;
			++ptr;
		}
		return i + 1;
	};
	InputExpression();
	auto NodeLength = GetNodeCount(false);
	auto RPNLength = GetNodeCount(true);
	auto NodeArray = new node[NodeLength];
	auto RPNArray = new node[RPNLength];
	auto PrintRPN = [=]() {
		std::cout << "RPN: ";
		for (auto i = 0ull; i < RPNLength; ++i)
			std::cout << RPNArray[i] << ' ';
		std::cout << std::endl;
	};
	auto PrintResult = [=]() {
		std::cout << "Result: " << CalculateRPN(RPNArray, RPNLength) << std::endl;
	};
	AnalyzeExpr(Buffer, NodeArray);
	InfixToRPN(RPNArray, NodeArray, NodeLength);
	PrintRPN();
	PrintResult();
	delete[] RPNArray;
	delete[] NodeArray;
	delete[] Buffer;
	system("pause");
}
