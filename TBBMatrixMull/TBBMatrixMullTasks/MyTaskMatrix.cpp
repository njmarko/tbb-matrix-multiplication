#include "MyTaskMatrix.h"

bool load_data(const std::string& filename, MyMatrix& m, int& rows, int& cols)
{
	std::ifstream fin;
	fin.open(filename);
	if (!fin.is_open())
	{
		throw runtime_error("File cannot be opened\n");
		return false;
	}
	std::stringstream ss;
	std::string line;

	int val; // value that is being read
	rows = 0;
	while (getline(fin, line)) {
		ss << line;
		std::vector<int> temp;
		while (!ss.eof()) {
			ss >> val;
			if (ss.fail()) { // if something other than int is encountered or the row is empty
				if (!ss.eof()) // if the row is not empty, then it is an invalid input
					throw TaskInvalidData(filename);
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
			m.insert(m.end(), temp.cbegin(), temp.cend());

			//for concurent vector
			//for (size_t i = 0; i < temp.size(); i++)
			//{
			//	m.push_back(temp[i]);
			//}
		}
		else
		{
			throw TaskInvalidData(filename);
		}
	}
	return true;
}


void print_matrix(const MyMatrix& m, const int rows, const int cols)
{
	cout << "\nMatrix dim=" << rows << "x" << cols << endl;

	for (size_t i = 0; i < rows; i++)
	{

		if (i == 0) { // write the column numbers
			cout << string(5, ' ');
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

void task_transpose(const MyMatrix& src, MyMatrix& dst, const int rows, const int cols) {
	for (int i = 0; i < cols; ++i)
	{
		for (int j = 0; j < rows; ++j) {
			dst[j + i*rows] = src[i + j*cols];
		}
	}
}

bool valid_task_results(const MyMatrix & result, int rows_m1, int cols_m1, int rows_m2, int cols_m2)
{

	MyMatrix valid_result;
	int rows_res;
	int cols_res;
	string validResFilename = "../ValidResults/" +
		to_string(rows_m1) + "x" + to_string(cols_m1) + "mull" +
		to_string(rows_m2) + "x" + to_string(cols_m2) + ".txt";

	load_data(validResFilename, valid_result, rows_res, cols_res);
	if (rows_res != rows_m1 || cols_res != cols_m2)
	{

	}
	for (int i = 0; i < rows_m1; i++)
	{
		for (int j = 0; j < cols_m2; j++)
		{
			if (valid_result[i*cols_m2 + j] != result[i*cols_m2 + j]) {
				return false;
			}
		}
	}
	return true;
}

bool save_task_results(const string & outFilename, const MyMatrix & res, int rows, int cols)
{
	ofstream out(outFilename);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			out << res[i*cols + j] << " ";
		}
		out << endl;
	}
	out.flush();
	out.close();
}
