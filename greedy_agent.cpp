#include <iostream>
#include <fstream>
#include <string>

using namespace std;

inline float sqr(int a) {
	return a * a;
}

float dist(int x1, int y1, int x2, int y2) {
	return sqrt(sqr(x2 - x1) + sqr(y2 - y1));
}

template <typename T>
T constrain(T min, T max, T val) {
	return val > max ? max : val < min ? min : val;
}


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

	char* data() const {
		return _data;
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

int main() {
	int x_size, y_size, area_size, players_count;
	char letter;

	cin >> x_size >> y_size >> area_size >> players_count >> letter;

	Map_t map(x_size, y_size);

	int x_pos = 0, y_pos = 0;

	//ofstream fout("greedy_log.txt");

	int step = 0;

	for (;;) {
		cin.getline(map.data(), x_size * y_size);

		for (int y = 0; y < y_size; y++) {
			for (int x = 0; x < x_size; x++) {
				cin >> map.at(x, y);
				if (map.at(x, y) == letter) {
					x_pos = x;
					y_pos = y;
				}
			}
		}

		int nearest_x = 0, nearest_y = 0, nearest_dist = INT_MAX;

		for (int y = 0; y < y_size; y++) {
			for (int x = 0; x < x_size; x++) {
				char cell = map.at(x, y);
				if (cell >= '0' && cell <= '9') {
					float d = dist(x_pos, y_pos, x, y);
					if (d < nearest_dist) {
						nearest_x = x;
						nearest_y = y;
						nearest_dist = d;
					}
				}
			}
		}

		if (nearest_x < x_pos) {
			cout << 'L' << endl;
		} else if (nearest_x > x_pos) {
			cout << 'R' << endl;
		} else if (nearest_y < y_pos) {
			cout << 'U' << endl;
		} else if (nearest_y > y_pos) {
			cout << 'D' << endl;
		} else {
			cout << 'S' << endl;
		}
	}
}