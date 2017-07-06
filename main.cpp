#include "engine.h"
#include <fstream>
#include <iostream>
#include <chrono>

int main() {

	std::ifstream fin("map.txt");

	int x_size, y_size, area_size;

	fin >> x_size >> y_size >> area_size;

	GameEngine_t::Map_t map(x_size, y_size);

	for (int y = 0; y < y_size; y++) {
		for (int x = 0; x < x_size; x++) {
			fin >> map.at(x, y);
		}
	}

	GameEngine_t engine(map, area_size);

	auto t = std::chrono::system_clock::now();
	for (int i = 0; i < 100; i++) {
		engine.make_move(0, GameEngine_t::LEFT);
		engine.make_move(1, GameEngine_t::STOP);
		engine.turn();
		for (int i = 0; i < engine.players_count(); i++) {
			GameEngine_t::Player_t& player = engine.player_by_place(i);
			//std::cout << char('A' + player.id) << ' ' << player.score << std::endl;
		}
		//std::cout << engine.map();
	}

	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - t).count() << std::endl;


	system("pause");
}
