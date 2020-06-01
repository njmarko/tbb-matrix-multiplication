#pragma once
/*
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

using namespace std;
using namespace tbb;

/*
* Represents 1D matrix
*/
typedef std::vector<int> MyMatrix;

class PPMatrixMull;
class PPMatrixMullColls;
class PPMatrixMullRows;


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

class PPMatrixMullTransposedCrossProduct2D {
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
	PPMatrixMullTransposedCrossProduct2D(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3,
		const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
		:m1(_m1), m2_transposed(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2)
	{}
};


bool load_data(const std::string& filename, MyMatrix& m, int& rows, int& cols);

void print_matrix(const MyMatrix& m, const int rows, const int cols);

void multiply_parallel(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

void multiply_parallel_rows_cols(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

void multiply_parallel_3D(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

void multiply_parallel_transposed(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

void multiply_parallel_transposed_3d(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

void mull_parallel_transp_inner_prod(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

void transpose(const MyMatrix& src, MyMatrix& dst, const int rows, const int cols);

/*
* An exception that inherits runtime_error and is a parent class to other matrix exceptions
*/
struct MatrixException : public std::runtime_error {
	/*
	*Constructor that recieves an error message and calls the parent constructor
	* @param error message that will be displayed
	*/
	MatrixException(const char* message) : runtime_error(message) {};
};

/*
* A struct that inherits MatrixException and is thrown when dimensions are incompatible during matrix multiplication
*/
struct IncompatibleDimensions : public MatrixException {
	/*
	*Constructor that calls the parent constructor with a message when matrix dimensions are incompatible
	*/
	IncompatibleDimensions() : MatrixException("Inner dimensions must be the same!") {};
};

/*
* A struct that inherits MatrixException and is thrown when data that is being loaded is invalid
*/
struct InvalidData : public MatrixException {
	/*
	*Constructor that calls the parent constructor with a message when data you are trying to load is invalid
	*/
	InvalidData(string filename) : MatrixException(("Unable to load matrix data from " + filename + "\nbecause the data is invalid!").c_str()) {};
};






