#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

int main() {
	int x_size, y_size, area_size, players_count;
	char letter;

	cin >> x_size >> y_size >> area_size >> players_count >> letter;

	char moves[] = { 'L', 'R', 'U', 'D', 'S' };

	srand(time(0));

	for (;;) {
		//int step;
		//cin >> step;
		for (int y = 0; y < y_size; y++) {
			for (int x = 0; x < x_size; x++) {
				char t;
				cin >> t;
			}
		}

		cout << moves[rand() % sizeof moves] << endl;
	}
}
