#pragma once
#include <vector>
#include <ostream>
#include <iostream>

struct Point_t {
	int x, y;
	Point_t(int x, int y) : x(x), y(y) {}
	Point_t() : x(0), y(0) {}

	Point_t operator+(const Point_t& b) const {
		return Point_t(x + b.x, y + b.y);
	}

	Point_t operator-(const Point_t& b) const {
		return Point_t(x - b.x, y - b.y);
	}
};

struct Player_t {
	int score;
	Point_t pos;

	Player_t() : score(0) { }
	Player_t(int x, int y) : score(0), pos(x, y) { }
	Player_t(Point_t p) : score(0), pos(p) { }
};

template <typename T>
T constrain(T min, T max, T val) {
	return val > max ? max : val < min ? min : val;
}

template <typename T>
int cmp_range(T min, T max, T val) {
	return val > max ? 1 : val < min ? -1 : 0;
}

class Map_t {
private:
	char* _data;
	int _x_size, _y_size;
public:
	Map_t(int x_size, int y_size, char* ptr = nullptr) {
		_x_size = x_size;
		_y_size = y_size;
		_data = new char[_x_size * _y_size];

		if (ptr) {
			memcpy(_data, ptr, _x_size * _y_size);
		}
	}

	Map_t(Map_t& e) {
		_x_size = e._x_size;
		_y_size = e._y_size;

		_data = new char[_x_size * _y_size];

		memcpy(_data, e._data, _x_size * _y_size);
	}

	~Map_t() {
		delete[] _data;
	}

	char& at(int x, int y) const {
		return *(_data + _x_size * y + x);
	}

	char& at(Point_t p) const {
		return at(p.x, p.y);
	}

	char& safe_at(int x, int y) const {
		return at(constrain(0, _x_size - 1, x), constrain(0, _y_size - 1, y));
	}

	void clear(char val = char()) const {
		memset(_data, val, _x_size * _y_size);
	}

	int x_size() const {
		return _x_size;
	}

	int y_size() const {
		return _y_size;
	}

	bool is_valid_point(int x, int y) const {
		return x >= 0 && x < _x_size && y >= 0 && y < _y_size;
	}

	bool is_valid_point(Point_t p) const {
		return is_valid_point(p.x, p.y);
	}

	friend std::ostream& operator<<(std::ostream& os, const Map_t& map) {
		for (int y = 0; y < map._y_size; y++) {
			for (int x = 0; x < map._x_size; x++) {
				os << map.at(x, y);
			}
			os << std::endl;
		}
		return os;
	}
};

class GameEngine_t {
private:
	Map_t _map;
	Map_t _scream_areas_mask;
	int _scream_area_size;

	std::vector<Player_t> _players;
public:
	enum Move_t {
		LEFT, RIGHT, UP, DOWN, STOP
	};

	class GameEngineException {};
	class BadMap : public GameEngineException {};
	class BadScreamAreaSize : public GameEngineException {};

	GameEngine_t(Map_t map, int scream_area_size) 
		: _map (map), _scream_areas_mask(map.x_size(), map.y_size())
	{
		if (scream_area_size < 1) {
			throw BadScreamAreaSize();
		}
		_scream_area_size = scream_area_size;

		static const int max_player_count = 'Z' - 'A' + 1;

		struct PlayerInfo_t {
			Point_t pos;
			int player_id;
			bool finded;
		} players[max_player_count];

		memset(players, 0, sizeof players);

		for (int y = 0; y < map.y_size(); y++) {
			for (int x = 0; x < map.x_size(); x++) {
				char cell = map.at(x, y);
				if (cell != '.' && (cell < '0' || cell > '9') && (cell < 'A' || cell > 'Z')) {
					throw BadMap();
				}

				if (cell >= 'A' && cell <= 'Z') {
					int player_id = cell - 'A';
					PlayerInfo_t& player = players[player_id];
					if (player.finded) {
						throw BadMap();
					}
					player = PlayerInfo_t{ Point_t(x, y), player_id, true };
				}
			}
		}

		bool ended = false;
		int player_count = 0;
		for (int i = 0; i < max_player_count; i++) {
			if (!players[i].finded) {
				ended = true;
			} else {
				if (ended) {
					throw BadMap();
				}
				player_count++;
			}
		}

		if (player_count < 1 || player_count > max_player_count) {
			throw BadMap();
		}

		_players.resize(player_count);

		for (int i = 0; i < player_count; i++) {
			_players[players[i].player_id] = Player_t(players[i].pos);
		}
	}

	int players_count() const {
		return _players.size();
	}
		
	void make_move(int player_id, Move_t move) {
		Player_t& player = _players[player_id];
		_scream_areas_mask.clear();

		for (int y = -_scream_area_size; y <= _scream_area_size; y++) {
			if (cmp_range(0, _scream_areas_mask.y_size() - 1, player.pos.y + y)) {
				continue;
			}
			for (int x = -(_scream_area_size - abs(y)); x <= _scream_area_size - abs(y); x++) {
				if (cmp_range(0, _scream_areas_mask.x_size() - 1, player.pos.x + x)) {
					continue;
				}
				char& t = _scream_areas_mask.at(player.pos + Point_t(x, y));
				t = t == 0 ? player_id + 1 : -1;
			}
		}
		
		for (int y = 0; y < _map.y_size(); y++) {
			for (int x = 0; x < _map.x_size(); x++) {
				int mask = _scream_areas_mask.at(x, y);
				char& cell = _map.at(x, y);
				if (cell >= '0' && cell <= '9') {
					if (mask > 0) {
						if (cell == '9') {
							cell = '.';
							_players[mask - 1].score++;
						} else {
							cell++;
						}
					} else {
						cell = '0';
					}
				}
			}
		}
		
		Point_t d =
			move == LEFT ? Point_t(-1, 0) :
			move == UP ? Point_t(0, -1) :
			move == RIGHT ? Point_t(1, 0) :
			move == DOWN ? Point_t(0, 1) :
			Point_t(0, 0);

		Point_t next_pos = player.pos + d;

		if (_map.is_valid_point(next_pos) && _map.at(next_pos) == '.') {
			_map.at(player.pos) = '.';
			player.pos = next_pos;
			_map.at(next_pos) = 'A' + player_id;
		}

		for (int y = 0; y < _map.y_size(); y++) {
			for (int x = 0; x < _map.x_size(); x++) {
				char& cell = _map.at(x, y);
				int mask = _scream_areas_mask.at(x, y);
				if (cell < '0' || cell > '9' || _scream_areas_mask.at(x, y) <= 0) {
					continue;
				}
				Player_t& p = _players[mask - 1];
				if (x < p.pos.x && _map.at(x + 1, y) == '.') {
					_map.at(x + 1, y) = cell;
					cell = '.';
				} else if (x > p.pos.x && _map.at(x - 1, y) == '.') {
					_map.at(x - 1, y) = cell;
					cell = '.';
				} else if (y < p.pos.y && _map.at(x, y + 1) == '.') {
					_map.at(x, y + 1) = cell;
					cell = '.';
				} else if (y > p.pos.y && _map.at(x, y - 1) == '.') {
					_map.at(x, y - 1) = cell;
					cell = '.';
				}
			}
		}
	}

	const Map_t& map() const {
		return _map;
	}
};