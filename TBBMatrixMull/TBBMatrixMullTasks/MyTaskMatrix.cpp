//#include "MyParallelMatrix.h"
//
//
//MyParallelMatrix::MyParallelMatrix() :rows(0), cols(0), m(0, vector<int>(0))
//{
//}
//
//MyParallelMatrix::MyParallelMatrix(unsigned int _rows, unsigned int _cols) : rows(_rows), cols(_cols), m(_rows, vector<int>(_cols, 0))
//{
//}
//
//MyParallelMatrix::MyParallelMatrix(MyParallelMatrix & m1) : rows(m1.rows), cols(m1.cols), m(m1.m)
//{
//}
//
//bool MyParallelMatrix::load_data(string filename)
//{
//	ifstream fin;
//	fin.open(filename);
//	if (!fin.is_open())
//	{
//		std::cout << "File cannot be opened\n";
//		return false;
//	}
//	stringstream ss;
//	string line;
//
//	int val; // value that is being read
//	rows = 0;
//	char ch;
//	while (getline(fin, line)) {
//		ss << line;
//		vector<int> temp;
//		while (!ss.eof()) {
//			ss >> val;
//			if (ss.fail()) { // if something other than int is encountered or the row is empty
//				if (!ss.eof()) // if the row is not empty, then it is an invalid input
//					throw MyParallelMatrix::InvalidData(filename);
//				else { // in case the row is empty just skip it
//					break;
//				}
//			}
//			temp.push_back(val);
//		}
//		ss.clear();
//
//		if (temp.size() == 0) { // allows for skipping of empty rows either in the begining, the middle or at the end
//			continue;
//		}
//		++rows;
//		if (rows == 1) cols = temp.size(); // first nonempty row column count defines how many columns are in a matrix
//		if (cols == temp.size()) {
//			m.push_back(temp);
//		}
//		else
//		{
//			throw MyParallelMatrix::InvalidData(filename);
//		}
//	}
//	return true;
//}
//
//void MyParallelMatrix::print_matrix()
//{
//	cout << "\nMatrix dim=" << rows << "x" << cols << endl;
//
//	for (size_t i = 0; i < rows; i++)
//	{
//
//		if (i == 0) { // write the column numbers
//			cout << string(5, ' ');
//			for (size_t j = 0; j < cols; j++)
//			{
//				cout.width(10);
//				if (i == 0) cout << j + 1 << ".";
//			}
//			cout << endl << string(12 * cols, '_') << endl;;
//		}
//
//		cout.width(4);
//		cout << std::right << i + 1 << ".|"; // write row numbers
//
//		for (size_t j = 0; j < cols; j++) {
//			cout.width(10);
//			cout << std::right << m[i][j] << " ";
//		}
//		cout << endl;
//	}
//	cout << endl;
//}
//
//unsigned int MyParallelMatrix::getRowSize()
//{
//	return rows;
//}
//
//unsigned int MyParallelMatrix::getColSize()
//{
//	return cols;
//}
//
//MyParallelMatrix& MyParallelMatrix::operator=(const MyParallelMatrix & m2)
//{
//	rows = m2.rows;
//	cols = m2.cols;
//	m = m2.m;
//	return *this;
//}
//
//std::vector<std::vector<int>>& MyParallelMatrix::getData()
//{
//	return m;
//}
//
//
//MyParallelMatrix operator*(const MyParallelMatrix & m1, const MyParallelMatrix & m2)
//{
//	if (m1.cols != m2.rows) {
//		throw MyParallelMatrix::IncompatibleDimensions();
//	}
//	MyParallelMatrix ret_val(m1.rows, m2.cols);
//	for (size_t i = 0; i < m1.rows; ++i)
//	{
//		for (size_t j = 0; j < m2.cols; ++j) {
//			for (size_t k = 0; k < m1.cols; ++k)
//			{
//				ret_val.m[i][j] += m1.m[i][k] * m2.m[k][j];
//			}
//		}
//	}
//	return ret_val;
//}
//
