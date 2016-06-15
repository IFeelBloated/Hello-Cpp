#include <iostream>
#include <fstream>
#include <cstdlib>

constexpr auto VERSION = (1ull << 63) + 0; //v1.0

auto main()->void {
	std::fstream file;
	char a[][3] = { { 1, 2, 3 },{ 4, 5, 6 },{ 7, 8, 9 } };
	char b[3][3];
	memset(b, 0, sizeof(b));
	file.open("data.dat", std::ios::out);
	file.write(reinterpret_cast<char *>(a), sizeof(a));
	file.close();
	file.open("data.dat", std::ios::in);
	file.read(reinterpret_cast<char *>(b), sizeof(b));
	file.close();
	for (auto x : b) {
		for (auto i = 0; i < 3; ++i)
			std::cout << static_cast<int>(x[i]) << " ";
		std::cout << std::endl;
	}
	system("pause");
	system("del data.dat");
}
