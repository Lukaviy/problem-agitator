#include <fstream>
#include <string>

using namespace std;

void js_encode(ifstream& in, ofstream& out) {
	out << '\"';
	while (!in.eof()) {
		string row;
		getline(in, row);
		out << row << "\\n\\" << endl;
	}
	out << '\"';
}

int main(int argc, char** argv) {
	if (argc != 4 ) {
		return 1;
	}

	ifstream fin_input(argv[1]);
	ifstream fin_log(argv[2]);
	ofstream fout_logjs(argv[3]);

	if (!fin_input || !fin_log || !fout_logjs) {
		return 1;
	}

	fout_logjs << "data = {" << endl << "input: " << endl;
	js_encode(fin_input, fout_logjs);

	fout_logjs << ',' << endl << "output: " << endl;
	js_encode(fin_log, fout_logjs);

	fout_logjs << '}' << endl;
}
