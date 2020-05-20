#include "MyMatrix.h"

MyMatrix::MyMatrix() :rows(0),cols(0), m(0, vector<int>(0))
{
}

MyMatrix::MyMatrix(unsigned int _rows, unsigned int _cols): rows(_rows),cols(_cols), m(_rows, vector<int>(_cols, 0))
{
}

MyMatrix::MyMatrix(MyMatrix & m1): rows(m1.rows), cols(m1.cols), m(m1.m)
{
}

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
	
	int val; //value that is being read
	rows = 0;
	while (getline(fin,line)) {
		++rows;
		ss << line;
		vector<int> temp;
		while (!ss.eof()) {
			ss >> val;
			temp.push_back(val);
		}
		ss.clear();
		if (rows == 1) cols = temp.size(); // first row column count defines how many columns are in a matrix
		if (cols == temp.size()) {
			m.push_back(temp);
		}
		else
		{
			//throw exception
		}
	}
	return true;
}

void MyMatrix::print_matrix()
{
	cout << "\nMatrix dim=" << rows << "x" << cols << endl;
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
			cout << m[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}

MyMatrix& MyMatrix::operator=(const MyMatrix & m2)
{
	rows = m2.rows;
	cols = m2.cols;
	m = m2.m;
	return *this;
}

MyMatrix operator*(const MyMatrix & m1, const MyMatrix & m2)
{
	if (m1.cols != m2.rows) {
		//throw exception
	}
	MyMatrix ret_val(m1.rows, m2.cols);
	for (size_t i = 0; i < m1.rows; ++i)
	{
		for (size_t j = 0; j < m2.cols; ++j) {
			for (size_t k = 0; k < m1.cols; ++k)
			{
				ret_val.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return ret_val;
}
