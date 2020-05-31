#include "MyMatrix.h"



bool load_data(const std::string& filename, MyMatrix& m, int& rows, int& cols)
{
	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open())
	{
		std::cout << "File cannot be opened\n";
		return false;
	}
	std::stringstream ss;
	std::string line;
	
	int val; // value that is being read
	rows = 0;
	while (getline(fin,line)) {
		ss << line;
		std::vector<int> temp;
		while (!ss.eof()) {
			ss >> val;
			if (ss.fail()) { // if something other than int is encountered or the row is empty
				if(!ss.eof()) // if the row is not empty, then it is an invalid input
					throw InvalidData(filename);
				else { // in case the row is empty just skip it
					break;
				}
			}
			temp.push_back(val);
		}
		ss.clear();

		if (temp.size() == 0) { // allows for skipping of empty rows either in the begining, the middle or at the end
			continue;
		}
		++rows;
		if (rows == 1) cols = (int)temp.size(); // first nonempty row column count defines how many columns are in a matrix
		if (cols == temp.size()) {
			m.insert(m.end(),temp.cbegin(),temp.cend());
		}
		else
		{
			throw InvalidData(filename);
		}
	}
	return true;
}

void print_matrix(const MyMatrix& m, const int rows, const int cols)
{
	cout << "\nMatrix dim=" << rows << "x" << cols << endl;

	for(size_t i = 0; i < rows; i++)
	{

		if (i == 0) { // write the column numbers
			cout << string(5,' ');
			for (size_t j = 0; j < cols; j++)
			{
				cout.width(10);
				if (i == 0) cout << j + 1 << ".";
			}
			cout << endl << string(6 + 11 * cols, '_') << endl;;
		}

		cout.width(4);
		cout << std::right << i + 1 << ".|"; // write row numbers

		for (size_t j = 0; j < cols; j++) {
			cout.width(10);
			cout << std::right << m[i*cols + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
// 2D matrix
// 1 1     4 4 4     9 9 9
// 2 2     5 5 5    18 18 18
// 3 3              27 27 27

// 1D matrix
// 1 1 2 2 3 3					 3x2
// 4 4 4 5 5 5					 2x3
// 9 9 9 18 18 18 27 27 27		 3x3

void multiply_serial(const MyMatrix & m1,const  MyMatrix & m2,  MyMatrix& m3,const int rows_m1,const int cols_m1,const int rows_m2,const int cols_m2)
{
	if (cols_m1 != rows_m2) {
		throw IncompatibleDimensions();
	}

	for (size_t i = 0; i < rows_m1; ++i)
	{
		for (size_t j = 0; j < cols_m2; ++j) {
			for (size_t k = 0; k < cols_m1; ++k)
			{
				m3[i*rows_m1 + j] += m1[i*cols_m1 + k] * m2[k*cols_m2 + j];
			}
		}
	}
}

void multiply_serial_transposed(const MyMatrix & m1, const MyMatrix & m2, MyMatrix & m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2)
{
	if (cols_m1 != rows_m2) {
		throw IncompatibleDimensions();
	}

	for (size_t i = 0; i < rows_m1; ++i)
	{
		for (size_t j = 0; j < cols_m2; ++j) {
			for (size_t k = 0; k < cols_m1; ++k)
			{
				m3[i*rows_m1 + j] += m1[i*cols_m1 + k] * m2[k*cols_m2 + j];
			}
		}
	}
}
