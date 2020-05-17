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
private:
	vector<vector<int>> m;
public:
	/*Default constructor*/
	MyMatrix() {};

	/*
	* Load matrix from the file. Matrix dimensions can vary
	* @params string filename
	* @returns bool true if the load is successfull
	*/
	bool load_data(string filename);

	/*
	* Prints the matrix
	*/
	void print_matrix();
};