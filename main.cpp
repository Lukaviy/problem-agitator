#include "engine.h"
#include <fstream>
#include <iostream>
#include <chrono>

int main() {

	std::ifstream fin("map.txt");

	int x_size, y_size, area_size;

	fin >> x_size >> y_size >> area_size;

	Map_t map(x_size, y_size);

	for (int y = 0; y < y_size; y++) {
		for (int x = 0; x < x_size; x++) {
			fin >> map.at(x, y);
		}
	}

	GameEngine_t engine(map, area_size);

	auto t = std::chrono::system_clock::now();
	for (int i = 0; i < 200000; i++)
		engine.make_move(0, GameEngine_t::LEFT);

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - t).count() << std::endl;

	std::cout << engine.map();

	system("pause");
}
