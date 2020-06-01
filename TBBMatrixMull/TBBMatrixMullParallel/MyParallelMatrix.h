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
#include <tbb\task.h>
#include <algorithm>
#include <numeric>

using namespace std;
using namespace tbb;

/*
* Represents 1D matrix
*/
typedef std::vector<int> MyMatrix;

bool load_data(const std::string& filename, MyMatrix& m, int& rows, int& cols);

void print_matrix(const MyMatrix& m, const int rows, const int cols);

void multiply_serial(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

void multiply_serial_transposed(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

void mull_serial_transp_inner_prod(const MyMatrix & m1, const MyMatrix & m2, MyMatrix& m3, const int rows_m1, const int cols_m1, const int rows_m2, const int cols_m2);

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






