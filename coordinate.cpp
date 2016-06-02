#include <iostream>
#include <cstdlib>

constexpr auto VERSION = (1ull << 63) + 0; //v1.0
constexpr auto PI = 3.1415926535897932384626433832795;

class coordinate {
public:
	coordinate() = default;
	coordinate(double x, double y) {
		data = new double[2]{ x, y };
	}
	coordinate(const coordinate &obj) {
		if (obj.data != nullptr) {
			data = new double[2];
			memcpy(data, obj.data, sizeof(double) * 2);
		}
	}
	coordinate(coordinate &&obj) {
		data = obj.data;
		obj.data = nullptr;
	}
	virtual ~coordinate() {
		delete[] data;
		data = nullptr;
	}
	auto operator=(const coordinate &obj)->coordinate & {
		if (this != &obj && obj.data != nullptr) {
			data = new double[2];
			memcpy(data, obj.data, sizeof(double) * 2);
		}
		return *this;
	}
	auto operator=(coordinate &&obj)->coordinate & {
		if (this != &obj) {
			data = obj.data;
			obj.data = nullptr;
		}
		return *this;
	}
	virtual auto calc()->double {
		return 0.;
	}
	friend auto operator<<(std::ostream &, const coordinate &)->std::ostream &;
protected:
	double *data = nullptr;
};

class circle :virtual public coordinate {
public:
	circle() = default;
	circle(double x, double y, double r) {
		data = new double[3]{ x, y, r };
	}
	circle(const circle &obj) {
		if (obj.data != nullptr) {
			data = new double[3];
			memcpy(data, obj.data, sizeof(double) * 3);
		}
	}
	circle(circle &&obj) {
		data = obj.data;
		obj.data = nullptr;
	}
	~circle() override = default;
	auto operator=(const circle &obj)->circle {
		if (this != &obj && obj.data != nullptr) {
			data = new double[3];
			memcpy(data, obj.data, sizeof(double) * 3);
		}
		return *this;
	}
	auto operator=(circle &&obj)->circle {
		if (this != &obj) {
			data = obj.data;
			obj.data = nullptr;
		}
		return *this;
	}
	auto calc()->double override {
		if (data != nullptr)
			return PI * data[2] * data[2];
		return 0.;
	}
};

class rectangle :virtual public coordinate {
public:
	rectangle() = default;
	rectangle(double x, double y, double a, double b) {
		data = new double[4]{ x, y, a, b };
	}
	rectangle(const rectangle &obj) {
		if (obj.data != nullptr) {
			data = new double[4];
			memcpy(data, obj.data, sizeof(double) * 4);
		}
	}
	rectangle(rectangle &&obj) {
		data = obj.data;
		obj.data = nullptr;
	}
	~rectangle() override = default;
	auto operator=(const rectangle &obj)->rectangle {
		if (this != &obj && obj.data != nullptr) {
			data = new double[4];
			memcpy(data, obj.data, sizeof(double) * 4);
		}
		return *this;
	}
	auto operator=(rectangle &&obj)->rectangle {
		if (this != &obj) {
			data = obj.data;
			obj.data = nullptr;
		}
		return *this;
	}
	auto calc()->double override {
		if (data != nullptr)
			return data[2] * data[3];
		return 0.;
	}
};

auto operator<<(std::ostream &out, const coordinate &obj)->std::ostream & {
	if (obj.data == nullptr)
		out << "wtf?" << std::endl;
	else if (typeid(obj) == typeid(coordinate))
		out << "coordinates: (" << obj.data[0] << ", " << obj.data[1] << ")" << std::endl;
	else if (typeid(obj) == typeid(circle))
		out << "shape: circle" << std::endl << "area: " << const_cast<coordinate &>(obj).calc() << std::endl;
	else if (typeid(obj) == typeid(rectangle))
		out << "shape: rectangle" << std::endl << "area: " << const_cast<coordinate &>(obj).calc() << std::endl;
	else
		out << "wtf?" << std::endl;
	return out;
}

auto main()->void {
	coordinate p = { 2.3, 4.2 };
	circle s1 = { 0., 0., 1.234 };
	rectangle s2 = { 0., 0., 6.4, 4.7 };
	std::cout << p << std::endl << s1 << std::endl << s2 << std::endl;
	system("pause");
}
