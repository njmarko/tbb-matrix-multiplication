#include "MyParallelMatrix.h"

bool validate_pp_results(const MyMatrix& result, int rows_m1, int cols_m1, int rows_m2, int cols_m2)
{

	MyMatrix valid_result;
	int rows_res;
	int cols_res;
	string validResFilename = "../ValidResults/" +
		to_string(rows_m1) + "x" + to_string(cols_m1) + "mull" +
		to_string(rows_m2) + "x" + to_string(cols_m2) + ".txt";

	load_data_ppfor(validResFilename, valid_result, rows_res, cols_res);
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

void save_pp_result(const string & outFilename, const MyMatrix & res, int rows, int cols)
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


bool load_data_ppfor(const std::string& filename, MyMatrix& m, int& rows, int& cols)
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
					throw PPInvalidData(filename);
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
			throw PPInvalidData(filename);
		}
	}
	return true;
}

void print_matrix_ppfor(const MyMatrix& m, const int rows, const int cols)
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
// 2D matrix
// 1 1     4 4 4     9 9 9
// 2 2     5 5 5    18 18 18
// 3 3              27 27 27

// 1D matrix
// 1 1 2 2 3 3					 3x2
// 4 4 4 5 5 5					 2x3
// 9 9 9 18 18 18 27 27 27		 3x3

void multiply_parallel(const MyMatrix & m1, const  MyMatrix & m2, MyMatrix& m3,
	const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2)
{
	if (cols_m1 != rows_m2) {
		throw PPIncompatibleDimensions();
	}

	tbb::parallel_for(blocked_range<int>(0, rows_m1), PPMatrixMull(m1,m2,m3,rows_m1,cols_m1,rows_m2,cols_m2));
}

void multiply_parallel_nested(const MyMatrix & m1, const  MyMatrix & m2, MyMatrix& m3,
	const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2)
{
	if (cols_m1 != rows_m2) {
		throw PPIncompatibleDimensions();
	}

	tbb::parallel_for(blocked_range<int>(0, rows_m1), PPMatrixMull(m1, m2, m3, rows_m1, cols_m1, rows_m2, cols_m2));
}

void multiply_parallel_3D(const MyMatrix & m1, const MyMatrix & m2, MyMatrix & m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2)
{
	if (cols_m1 != rows_m2) {
		throw PPIncompatibleDimensions();
	}
	
	tbb::parallel_for(blocked_range3d<int>(0, rows_m1, 0, cols_m2, 0, cols_m1), PPMatrixMull3D(m1, m2, m3, rows_m1, cols_m1, rows_m2, cols_m2));
}

void pptranspose(const MyMatrix& src, MyMatrix& dst, const int rows, const int cols) {
	for (int i = 0; i < cols; ++i)
	{
		for (int j = 0; j < rows; ++j) {
			dst[j + i*rows] = src[i + j*cols];
		}
	}
}

void multiply_parallel_transposed(const MyMatrix & m1, const MyMatrix & m2, MyMatrix & m3,
	const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2)
{
	if (cols_m1 != rows_m2) {
		throw PPIncompatibleDimensions();
	}
	MyMatrix m2_transposed(rows_m2*cols_m2);
	pptranspose(m2, m2_transposed, rows_m2, cols_m2);
	//print_matrix(m2_transposed, cols_m2, rows_m2);

	tbb::parallel_for(blocked_range<int>(0, rows_m1), PPMatrixMullTransposed(m1, m2_transposed, m3, rows_m1, cols_m1, rows_m2, cols_m2));
}

void multiply_parallel_transposed_3d(const MyMatrix & m1, const MyMatrix & m2, MyMatrix & m3,
	const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2)
{
	if (cols_m1 != rows_m2) {
		throw PPIncompatibleDimensions();
	}
	MyMatrix m2_transposed(rows_m2*cols_m2);
	pptranspose(m2, m2_transposed, rows_m2, cols_m2);
	//print_matrix(m2_transposed, cols_m2, rows_m2);
	tbb::parallel_for(blocked_range3d<int>(0, rows_m1, 0, cols_m2, 0, cols_m1), PPMatrixMullTransposed3D(m1, m2_transposed, m3, rows_m1, cols_m1, rows_m2, cols_m2));
}


void mull_parallel_transp_inner_prod(const MyMatrix & m1, const MyMatrix & m2, MyMatrix & m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2)
{
	if (cols_m1 != rows_m2) {
		throw PPIncompatibleDimensions();
	}
	MyMatrix m2_transposed(rows_m2*cols_m2);
	pptranspose(m2, m2_transposed, rows_m2, cols_m2);
	//print_matrix(m2_transposed, cols_m2, rows_m2);
	tbb::parallel_for(blocked_range<int>(0, rows_m1), PPMatrixMullTransposedInnerProduct(m1, m2_transposed, m3, rows_m1, cols_m1, rows_m2, cols_m2));
}

// 1D matrix
// 1 1 2 2 3 3					 3x2
// 4 5 4 5 4 5					 3x2 transposed

void mull_parallel_transp_inner_prod_2d(const MyMatrix & m1, const MyMatrix & m2, MyMatrix & m3,
	const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2)
{
	if (cols_m1 != rows_m2) {
		throw PPIncompatibleDimensions();
	}

	MyMatrix m2_transposed(rows_m2*cols_m2);
	pptranspose(m2, m2_transposed, rows_m2, cols_m2);
	//print_matrix(m2_transposed, cols_m2, rows_m2);

	static affinity_partitioner ap;
	const int processor_count = std::thread::hardware_concurrency();

	if (rows_m1*cols_m2 <= 1000000)
	{
		tbb::parallel_for(blocked_range2d<int>(0, rows_m1, (int)rows_m1 / processor_count*3.4 + 1, 0, cols_m2, (int)cols_m2 / processor_count*2.2 + 1),
			PPMatrixMullTransposedInnerProduct2D(m1, m2_transposed, m3, rows_m1, cols_m1, rows_m2, cols_m2), ap);
	}
	else
	{
		tbb::parallel_for(blocked_range2d<int>(0, rows_m1, 500, 0, cols_m2, 100),
			PPMatrixMullTransposedInnerProduct2D(m1, m2_transposed, m3, rows_m1, cols_m1, rows_m2, cols_m2),auto_partitioner());
	}
}
