#include "MyMatrix.h"

bool MyMatrix::load_data(string filename)
{
	ifstream fin;
	fin.open(filename);
	if (!fin.is_open())
	{
		std::cout << "File cannot be opened\n";
		return false;
	}
	stringstream ss;
	string line;

	int t;
	while (getline(fin,line)) {
		ss << line;
		vector<int> temp;
		while (!ss.eof()) {
			ss >> t;
			temp.emplace_back(t);
		}
		ss.clear();
		m.emplace_back(temp);
	}
	return true;
}

void MyMatrix::print_matrix()
{
	for (size_t i = 0; i < m.size(); i++)
	{
		for (size_t j = 0; j < m[i].size(); j++)
			cout << m[i][j] << " ";
		cout << endl;
	}
}
