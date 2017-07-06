#include <iostream>

using namespace std;

int main() {
	int x_size, y_size, area_size;

	cin >> x_size >> y_size >> area_size;

	char moves[] = { 'L', 'R', 'U', 'D', 'S' };

	for (;;) {
		cout << moves[rand() % sizeof moves] << endl;
		cout.flush();
	}
}
