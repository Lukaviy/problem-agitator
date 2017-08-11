#include <iostream>
#include <fstream>

#include "engine.h"

using namespace std;

class msg_to_player {
	int _player_id;
public:
	msg_to_player(int player_id) : _player_id(player_id) {}

	friend ostream& operator<<(ostream& os, const msg_to_player& msg) {
		return os << msg._player_id + 1 << '#';
	}
};

class awake_player {
	int _player_id;
public:
	awake_player(int player_id) : _player_id(player_id) {}

	friend ostream& operator<<(ostream& os, const awake_player& msg) {
		return os << msg._player_id + 1 << "W#" << endl;
	}
};

class kill_player {
	int _player_id;
public:
	kill_player(int player_id) : _player_id(player_id) {}

	friend ostream& operator<<(ostream& os, const kill_player& msg) {
		return os << msg._player_id + 1 << "S#" << endl;
	}
};

int main() {
	ifstream fin("input.txt");
	ofstream flog("log.txt");
	ofstream flog2("log2.txt");

	int x_size, y_size, area_size, max_steps;
	fin >> x_size >> y_size >> area_size >> max_steps;

	GameEngine::Map_t map(x_size, y_size);

	for (int y = 0; y < y_size; y++) {
		for (int x = 0; x < x_size; x++) {
			fin >> map.at(x, y);
		}
	}

	try {
		GameEngine::GameEngine_t game(map, area_size, max_steps);

		flog << game.players_count() << endl;

		//Sending game rules to agents
		for (int player_id = 0; player_id < game.players_count(); player_id++) {
			cout <<
				awake_player(player_id) <<
				msg_to_player(player_id) << x_size << ' ' << y_size << ' ' << area_size << ' ' << game.players_count() << ' ' << game.player_by_id(player_id).letter() << endl;
		}

		//Game loop
		do {
			//Sending game state to all players
			for (int player_id = 0; player_id < game.players_count(); player_id++) {
				if (!game.player_by_id(player_id).alive) {
					continue;
				}
				cout << awake_player(player_id) << msg_to_player(player_id) << endl;

				cout << msg_to_player(player_id);

				cout << game.map().data();

				cout << endl;
			}

			flog2 << game.current_step() << endl;

			//Receiving moves
			int readed_moves = 0;
			while (readed_moves < game.players_alive()) {
				int player_id;
				char term, player_move;
				cin >> player_id;
				cin >> term;

				player_id--;

				if (term == 'T') {
					if (game.player_by_id(player_id).alive) {
						game.kill_player(player_id);
						flog << 'T' << player_id << endl;
					}
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					continue;
				}

				if (term != '#') {
					flog << "U" << endl;
					return -2;
				}

				cin >> player_move;

				game.make_move(player_id,
					player_move == 'L' ? GameEngine::LEFT :
					player_move == 'R' ? GameEngine::RIGHT :
					player_move == 'U' ? GameEngine::UP :
					player_move == 'D' ? GameEngine::DOWN :
					GameEngine::STOP
				);
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				flog << player_id << ' ' << player_move << endl;

				readed_moves++;
			}
			flog << 'S' << endl;

			//flog2 << game.current_step() << endl;
		} while (game.step());

		for (int i = 0; i < game.players_count(); i++) {
			cout << kill_player(i);
		}

		flog << 'E' << endl;

		for (int i = 0; i < game.players_count(); i++) {
			flog << game.player_by_id(i).score << endl;
		}

		cout.flush();
	} catch(GameEngine::BadMap) {
		cerr << "Bad map" << endl;
		return 2;
	} catch (GameEngine::BadScreamAreaSize) {
		cerr << "Bad scream area size" << endl;
		return 2;
	}

	return 0;
}
