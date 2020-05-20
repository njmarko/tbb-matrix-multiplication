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
	* @throws
	*/
	friend MyMatrix operator*(const MyMatrix& m1, const MyMatrix& m2);

	/*
	* Asignment operator that copies data and matrix size
	* @param matrix that is being copied
	* @returns reference to this matrix
	*/
	MyMatrix& operator=(const MyMatrix& m2);


private:
	std::vector<std::vector<int>> m;
	unsigned int rows;
	unsigned int cols;
};




