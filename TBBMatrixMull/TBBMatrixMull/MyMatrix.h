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

using namespace std;

/*
* Class that represents a matrix by using a two dimensional vector and implements methods for loading the data,
* printing the data and other basic functionalities like addition, multiplication...
*/
class MyMatrix {

public:
	/*Default constructor*/
	MyMatrix();

	/*
	* Constructor that takes matrix dimensions and initializes the matrix with zeroes
	* @param number of rows
	* @param number of columns
	*/
	MyMatrix(unsigned int _rows, unsigned int _cols);

	/*
	* Copy constructor that copies matrix data and rows and column sizes
	* @param Matrix that is being copied
	*/
	MyMatrix(MyMatrix& m1);

	/*
	* Load matrix from the file. Matrix dimensions can vary
	* @param string filename
	* @returns bool true if the load is successfull
	* @throws MyMatrix::InvalidData thrown if data you were trying to load is not valid
	*/
	bool load_data(string filename);

	/*
	* Prints the matrix
	*/
	void print_matrix();

	/*
	* Serially multiply 2 matrices and return the resulting matrix
	* @param left side matrix whose dimensions are r1xc1
	* @param right side matrix whose dimensions are r2xc2
	* @returns matrix whose dimensions are c1xr2
	* @throws MyMatrix::IncompatibleDimensions thrown if inner matrix dimensions are not compatible
	*/
	friend MyMatrix operator*(const MyMatrix& m1, const MyMatrix& m2);

	/*
	* Asignment operator that copies data and matrix size
	* @param matrix that is being copied
	* @returns reference to this matrix
	*/
	MyMatrix& operator=(const MyMatrix& m2);

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

private:
	/*
	* A two dimensional vector of ints that holds the matrix data
	*/
	std::vector<std::vector<int>> m;

	/*
	* Number of rows in the matrix
	*/
	unsigned int rows;

	/*
	* Number of columns in the matrix
	*/
	unsigned int cols;
};




