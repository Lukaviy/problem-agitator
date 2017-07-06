#include <iostream>
#include <fstream>

#include "engine.h"

using namespace std;

int main() {
	ifstream fin("input.txt");

	int x_size, y_size, area_size, max_steps;
	fin >> x_size >> y_size >> area_size >> max_steps;

	GameEngine_t::Map_t map(x_size, y_size);

	for (int y = 0; y < y_size; y++) {
		for (int x = 0; x < x_size; x++) {
			fin >> map.at(x, y);
		}
	}

	GameEngine_t game(map, area_size);

	//Sending game rules to agents
	for (int player_id = 0; player_id < game.players_count(); player_id++) {
		cout << player_id << "W#" << x_size << ' ' << y_size << ' ' << area_size << endl;
		cout.flush();
	}

	//Game loop
	do {
		for (int i = 0; i < game.players_count(); i++) {
			int player_id;
			char term, player_move;
			cin >> player_id;
			cin >> term;

			if (term == 'T') {
				game.kill_player(player_id);
				cerr << "Player " << player_id << ", accidently terminated" << endl;
			}

			if (term != '#') {
				cerr << "Unhandled error" << endl;
				return -2;
			}

			cin >> player_move;

			game.make_move(player_id,
				player_move == 'L' ? GameEngine_t::LEFT :
				player_move == 'R' ? GameEngine_t::RIGHT :
				player_move == 'U' ? GameEngine_t::UP :
				player_move == 'D' ? GameEngine_t::DOWN :
				GameEngine_t::STOP
			);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	} while (game.step());

	for (int i = 0; i < game.players_count(); i++) {
		cout << i << "S#" << endl;
	}
	cout.flush();

	return 0;
}
