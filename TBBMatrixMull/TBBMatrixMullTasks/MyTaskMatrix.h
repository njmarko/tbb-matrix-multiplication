#pragma once
/**
Author: Marko Njegomir sw-38-2018
Project: Matrix Multiplication using Intel's TBB Tasks 
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

/**
* Prints the matrix
* @param m const reference to a matrix
* @param rows number of rows
* @param cols number of columns
*/
void print_matrix_tasks(const MyMatrix& m, const int rows, const int cols);

/**
* Loads the data from file into the appropriate matrix, and sets the rows and cols values
* @param filename const reference to a name of the file that is being loaded
* @param m reference to a matrix where the data will be stored
* @param rows reference to a variable that represents matrix rows
* @param cols reference to a variable that represents matrix columns
* @returns true if the operation was successful
*/
bool load_data_tasks(const std::string& filename, MyMatrix& m, int& rows, int& cols);

/**
* Transposes the source matrix and writes the results into the destination matrix
* @param src const reference to a source matrix
* @param dst const reference to a destination matrix
* @param rows number of rows
* @param cols number of columns
*/
void task_transpose(const MyMatrix& src, MyMatrix& dst, const int rows, const int cols);

/**
* Loads the file with the correct result and checks if the numbers match
* @param result const reference for the matrix that we want to validate
* @param rows_m1 number of rows of the first matrix
* @param cols_m1 number of cols of the first matrix
* @param rows_m2 number of rows of the second matrix
* @param cols_m2 number of cols of the second matrix
* @returns true if the result is correct
*/
bool valid_task_results(const MyMatrix& result, int rows_m1, int cols_m1, int rows_m2, int cols_m2);

/**
* Saves the result Matrix at the specified location
* @param outFilename const reference for a output filename
* @param res const reference for the result matrix
* @param rows dst number of rows in the matrix
* @param cols number of columns in the matrix
*/
bool save_task_results(const string& outFilename, const MyMatrix& res, int rows, int cols);

/**
* An exception that inherits runtime_error and is a parent class to other matrix exceptions
*/
struct TaskMatrixException : public std::runtime_error {
	/**
	*Constructor that recieves an error message and calls the parent constructor
	* @param message error message that will be displayed
	*/
	TaskMatrixException(const char* message) : runtime_error(message) {};
};

/**
* A struct that inherits MatrixException and is thrown when dimensions are incompatible during matrix multiplication
*/
struct TaskIncompatibleDimensions : public TaskMatrixException {
	/**
	*Constructor that calls the parent constructor with a message when matrix dimensions are incompatible
	*/
	TaskIncompatibleDimensions() : TaskMatrixException("Inner dimensions must be the same!") {};
};

/**
* A struct that inherits MatrixException and is thrown when data that is being loaded is invalid
*/
struct TaskInvalidData : public TaskMatrixException {
	/**
	*Constructor that calls the parent constructor with a message when data you are trying to load is invalid
	* @param filename name of the file where the the invalid data is
	*/
	TaskInvalidData(string filename) : TaskMatrixException(("Unable to load matrix data from " + filename + "\nbecause the data is invalid!").c_str()) {};
};
