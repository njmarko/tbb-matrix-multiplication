#pragma once
/**
Author: Marko Njegomir sw-38-2018
Project: Serial Matrix Multiplication using Intel's TBB library
Date: 5/17/2020
*/

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tbb\parallel_for.h>
#include <tbb\tbb.h>
#include <tbb\task.h>
#include <algorithm>
#include <numeric>
#include <thread>

using namespace std;
using namespace tbb;

/**
* Represents 1D matrix
*/
typedef std::vector<int> MyMatrix;
//typedef concurrent_vector<int> MyMatrix;



class PPMatrixMull;
class PPMatrixMullColls;
class PPMatrixMullRows;


/**
* Functor that does basic multiplication of the matrices with standard blocked range.
*/
class PPMatrixMull {
	const MyMatrix& m1, m2;
	MyMatrix& m3;
	const int rows_m1, cols_m1, rows_m2, cols_m2;
public:
	void operator()(const tbb::blocked_range<int>&r)const {
		for (int i = r.begin(); i != r.end(); ++i)
		{
			for (int j = 0; j < cols_m2; ++j) {
				for (int k = 0; k < cols_m1; ++k) {
					m3[i*rows_m1 + j] += m1[i*cols_m1 + k] * m2[k*cols_m2 + j];
				}
			}
		}
	}
	PPMatrixMull(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3,
		const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
		:m1(_m1),m2(_m2),m3(_m3),rows_m1(_rows_m1),cols_m1(_cols_m1),rows_m2(_rows_m2),cols_m2(_cols_m2)
	{}
};

/**
* Functor that goes trough matrix columns and writes the results of the matrix multiplication
*/
class PPMatrixMullColls {
	const MyMatrix& m1, m2;
	MyMatrix& m3;
	const int rows_m1, cols_m1, rows_m2, cols_m2, index_i;
public:
	void operator()(const tbb::blocked_range<int>&r)const {
		for (int j = r.begin(); j != r.end(); ++j)
		{
			for (size_t k = 0; k < cols_m1; ++k) {
				m3[index_i*rows_m1 + j] += m1[index_i*cols_m1 + k] * m2[k*cols_m2 + j];
			}
		}
	}
	PPMatrixMullColls(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3,
		const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2, const int _index_i)
		:m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2), index_i(_index_i)
	{}
};

/**
* Functor that goes trough rows and has a nested parallel for function that goes trough columns
*/
class PPMatrixMullRows {
	const MyMatrix& m1, m2;
	MyMatrix& m3;
	const int rows_m1, cols_m1, rows_m2, cols_m2;
public:
	void operator()(const tbb::blocked_range<int>&r)const {
		for (int i = r.begin(); i != r.end(); ++i)
		{
			int index_i = i;
			tbb::parallel_for(blocked_range<int>(0, cols_m2), PPMatrixMullColls(m1, m2, m3, rows_m1, cols_m1, rows_m2, cols_m2, index_i));
		}
	}
	PPMatrixMullRows(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3,
		const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
		:m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2)
	{}
};

/**
* Functor that multiplies matrix A matrix B by using a 3D blocked range.
*/
class PPMatrixMull3D {
	const MyMatrix& m1, m2;
	MyMatrix& m3;
	const int rows_m1, cols_m1, rows_m2, cols_m2;
public:
	void operator()(const tbb::blocked_range3d<int>&r)const {
		for (int i = r.pages().begin(); i != r.pages().end(); ++i)
		{
			for (int j = r.rows().begin(); j != r.rows().end(); ++j) {
				for (int k = r.cols().begin(); k != r.cols().end(); ++k) {
					m3[i*rows_m1 + j] += m1[i*cols_m1 + k] * m2[k*cols_m2 + j];
				}
			}
		}
	}
	PPMatrixMull3D(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3,
		const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
		:m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2)
	{}
};

/**
* Functor that multiplies matrix A with transposed matrix B by using a standard blocked range.
*/
class PPMatrixMullTransposed {
	const MyMatrix& m1, m2_transposed;
	MyMatrix& m3;
	const int rows_m1, cols_m1, rows_m2, cols_m2;
public:
	void operator()(const tbb::blocked_range<int>&r)const {
		for (int i = r.begin(); i != r.end(); ++i)
		{
			for (size_t j = 0; j < cols_m2; ++j) {
				for (size_t k = 0; k < cols_m1; ++k)
				{
					m3[i*rows_m1 + j] += m1[i*cols_m1 + k] * m2_transposed[j*cols_m1 + k];
				}
			}
		}
	}
	PPMatrixMullTransposed(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3,
		const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
		:m1(_m1), m2_transposed(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2)
	{}
};

/**
* Functor that multiplies matrix A with transposed matrix B by using a 3D blocked range.
*/
class PPMatrixMullTransposed3D {
	const MyMatrix& m1, m2_transposed;
	MyMatrix& m3;
	const int rows_m1, cols_m1, rows_m2, cols_m2;
public:
	void operator()(const tbb::blocked_range3d<int>&r)const {
		for (int i = r.pages().begin(); i != r.pages().end(); ++i)
		{
			for (int j = r.rows().begin(); j != r.rows().end(); ++j) {
				for (int k = r.cols().begin(); k != r.cols().end(); ++k) {
					m3[i*rows_m1 + j] += m1[i*cols_m1 + k] * m2_transposed[j*cols_m1 + k];
				}
			}
		}
	}
	PPMatrixMullTransposed3D(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3,
		const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
		:m1(_m1), m2_transposed(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2)
	{}
};

/**
* Functor that multiplies matrix A with transposed matrix B by using a standard blocked range
* and inner_product function for calculating each element in the resulting matrix C
*/
class PPMatrixMullTransposedInnerProduct {
	const MyMatrix& m1, m2_transposed;
	MyMatrix& m3;
	const int rows_m1, cols_m1, rows_m2, cols_m2;
public:
	void operator()(const tbb::blocked_range<int>&r)const {
		for (int i = r.begin(); i != r.end(); ++i)
		{
			for (size_t j = 0; j < cols_m2; ++j) {
				MyMatrix::const_iterator cit_m1_beg = m1.cbegin() + i*cols_m1;
				MyMatrix::const_iterator cit_m1_end = m1.cbegin() + (i + 1)*cols_m1;
				MyMatrix::const_iterator cit_m2_beg = m2_transposed.cbegin() + j*rows_m2;
				m3[i*rows_m1 + j] = std::inner_product(cit_m1_beg, cit_m1_end, cit_m2_beg, 0);
			}
		}
	}
	PPMatrixMullTransposedInnerProduct(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3,
		const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
		:m1(_m1), m2_transposed(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2)
	{}
};

/**
* Functor that multiplies matrix A with transposed matrix B by using a 2D blocked range
* and inner_product function for calculating each element in the resulting matrix C
*/
class PPMatrixMullTransposedInnerProduct2D {
	const MyMatrix& m1, m2_transposed;
	MyMatrix& m3;
	const int rows_m1, cols_m1, rows_m2, cols_m2;
public:
	void operator()(const tbb::blocked_range2d<int>&r)const {
		for (int i = r.rows().begin(); i != r.rows().end(); ++i)
		{
			for (int j = r.cols().begin(); j != r.cols().end(); ++j) {
				MyMatrix::const_iterator cit_m1_beg = m1.cbegin() + i*cols_m1;
				MyMatrix::const_iterator cit_m1_end = m1.cbegin() + (i + 1)*cols_m1;
				MyMatrix::const_iterator cit_m2_beg = m2_transposed.cbegin() + j*rows_m2;
				m3[i*rows_m1 + j] = std::inner_product(cit_m1_beg, cit_m1_end, cit_m2_beg, 0);
			}
		}
	}
	PPMatrixMullTransposedInnerProduct2D(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3,
		const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
		:m1(_m1), m2_transposed(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2)
	{}
};

/**
* Loads the data from file into the appropriate matrix, and sets the rows and cols values
* @param filename const reference to a name of the file that is being loaded
* @param m reference to a matrix where the data will be stored
* @param rows reference to a variable that represents matrix rows
* @param cols reference to a variable that represents matrix columns
* @returns true if the operation was successful
*/
bool load_data_ppfor(const std::string& filename, MyMatrix& m, int& rows, int& cols);

/**
* Prints the matrix
* @param m cosnt reference to a matrix
* @param rows dst number of rows
* @param cols number of columns
*/
void print_matrix_ppfor(const MyMatrix& m, const int rows, const int cols);


/**
* Multiplies the matrices by using a basic parallel for loop with standard blocked range
* @param m1 const reference of first matrix
* @param m2 nst reference of second matrix
* @param m3 reference of the result matrix
* @param rows_m1 dst number of rows in first matrix
* @param cols_m1 number of columns in the first matrix
* @param rows_m2 dst number of rows in the second matrix
* @param cols_m2 number of columns in the second matrix
*/
void multiply_parallel(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

/**
* Multiplies the matrices by using 3 nested parallel for loops with standard blocked range
* @param m1 const reference of first matrix
* @param m2 const reference of second matrix
* @param m3 reference of the result matrix
* @param rows_m1 dst number of rows in first matrix
* @param cols_m1 number of columns in the first matrix
* @param rows_m2 dst number of rows in the second matrix
* @param cols_m2 number of columns in the second matrix
*/
void multiply_parallel_nested(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

/**
* Multiplies the matrices by using parallel for loop with 3D blocked range
* @param m1 const reference of first matrix
* @param m2 const reference of second matrix
* @param m3 reference of the result matrix
* @param rows_m1 dst number of rows in first matrix
* @param cols_m1 number of columns in the first matrix
* @param rows_m2 dst number of rows in the second matrix
* @param cols_m2 number of columns in the second matrix
*/
void multiply_parallel_3D(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);


/**
* Multiplies the matrix A with transposed matrix B by using parallel for loop with standard blocked range.
* @param m1 const reference of first matrix
* @param m2 const reference of second matrix
* @param m3 reference of the result matrix
* @param rows_m1 dst number of rows in first matrix
* @param cols_m1 number of columns in the first matrix
* @param rows_m2 dst number of rows in the second matrix
* @param cols_m2 number of columns in the second matrix
*/
void multiply_parallel_transposed(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

/**
* Multiplies the matrix A with transposed matrix B by using parallel for loop with 3D blocked range.
* @param m1 const reference of first matrix
* @param m2 const reference of second matrix
* @param m3 reference of the result matrix
* @param rows_m1 dst number of rows in first matrix
* @param cols_m1 number of columns in the first matrix
* @param rows_m2 dst number of rows in the second matrix
* @param cols_m2 number of columns in the second matrix
*/
void multiply_parallel_transposed_3d(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

/**
* Multiplies the matrix A with transposed matrix B by using parallel for loop with standard blocked range.
* Uses inner_product function to calculate each element in the resulting matrix.
* @param m1 const reference of first matrix
* @param m2 const reference of second matrix
* @param m3 reference of the result matrix
* @param rows_m1 dst number of rows in first matrix
* @param cols_m1 number of columns in the first matrix
* @param rows_m2 dst number of rows in the second matrix
* @param cols_m2 number of columns in the second matrix
*/
void mull_parallel_transp_inner_prod(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);


/**
* Multiplies the matrix A with transposed matrix B by using parallel for loop with 2D blocked range.
* Uses inner_product function to calculate each element in the resulting matrix.
* @param m1 const reference of first matrix
* @param m2 const reference of second matrix
* @param m3 reference of the result matrix
* @param rows_m1 dst number of rows in first matrix
* @param cols_m1 number of columns in the first matrix
* @param rows_m2 dst number of rows in the second matrix
* @param cols_m2 number of columns in the second matrix
*/
void mull_parallel_transp_inner_prod_2d(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);


/**
* Transposes the source matrix and writes the results into the destination matrix
* @param src const reference to a source matrix
* @param dst const reference to a destination matrix
* @param rows number of rows
* @param cols number of columns
*/
void pptranspose(const MyMatrix& src, MyMatrix& dst, const int rows, const int cols);

/**
* Loads the file with the correct result and checks if the numbers match
* @param result const reference for the matrix that we want to validate
* @param rows_m1 number of rows of the first matrix
* @param cols_m1 number of cols of the first matrix
* @param rows_m2 number of rows of the second matrix
* @param cols_m2 number of cols of the second matrix
* @returns true if the result is correct
*/
bool validate_pp_results(const MyMatrix& result, int rows_m1, int cols_m1, int rows_m2, int cols_m2);


/**
* Saves the result Matrix at the specified location
* @param outFilename const reference for a output filename
* @param res const reference for the result matrix
* @param rows dst number of rows in the matrix
* @param cols number of columns in the matrix
*/
void save_pp_result(const string& outFilename, const MyMatrix& res, int rows, int cols);





/**
* An exception that inherits runtime_error and is a parent class to other matrix exceptions
*/
struct PPMatrixException : public std::runtime_error {
	/**
	*Constructor that recieves an error message and calls the parent constructor
	* @param message error message that will be displayed
	*/
	PPMatrixException(const char* message) : runtime_error(message) {};
};

/**
* A struct that inherits MatrixException and is thrown when dimensions are incompatible during matrix multiplication
*/
struct PPIncompatibleDimensions : public PPMatrixException {
	/**
	*Constructor that calls the parent constructor with a message when matrix dimensions are incompatible
	*/
	PPIncompatibleDimensions() : PPMatrixException("Inner dimensions must be the same!") {};
};

/**
* A struct that inherits MatrixException and is thrown when data that is being loaded is invalid
*/
struct PPInvalidData : public PPMatrixException {
	/**
	* Constructor that calls the parent constructor with a message when data you are trying to load is invalid
	* @param filename name of the file where data is invalid
	*/
	PPInvalidData(string filename) : PPMatrixException(("Unable to load matrix data from " + filename + "\nbecause the data is invalid!").c_str()) {};
};






