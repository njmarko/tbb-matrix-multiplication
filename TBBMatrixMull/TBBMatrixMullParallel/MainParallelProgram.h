/*
Author: Marko Njegomir sw-38-2018
Project: Parallel Matrix Multiplication using Intel's TBB library
Date: 5/17/2020
*/
#pragma once

#include <iostream>
#include <tbb\tick_count.h>
#include "MyParallelMatrix.h"
#include "MullTask.h"
#include <numeric>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;
using namespace tbb;

/*
* Main starting point of the serial version of the program
* @param int c : number of arguments
* @param char* argv[] : matrix1filename, matrix2filename and outputfilename
* @returns int : 0 if no error like the regular main funcion
*/
int mullParallel(int c, char* argv[]);

/*
* Loads two matrices from the files, and then performs matrix multiplication multiple times.
* It checks if the results are valid, and writes the average time needed for the matrix multiplication
* int the results vector
* @param const reference to a first filename
* @param const reference to a second filename
* @param const reference to an output filename
* @param reference to a vector that holds the average reult times
* @param true if the results were correct
* @throws runtime_excetpion if the files can not be opened
*/
bool mull_two_pp_matrices(const string& inputFile1, const string& inputFile2, const string& outFilename, std::vector<double>& average_result_times);

/*
* When no input arguments are given, the program uses this function to go trough a vector of predefined matrices
* It loads the matrices and multiplies them
* @param const reference to a vector of matrix sizes that indicate what matrices will be loaded and multiplied
*/
void mull_all_pp_matrices(const std::vector<std::pair<int, int>>& matrix_sizes);

/*
* Prints the average result times for given matrix sizes
* @param const reference to a vector of matrix sizes that indicate what matrices will be printed in the header
* @param const reference to a vector that holds average result times for apropriate matrix size
*/
void print_pp_result_table(const std::vector<std::pair<int, int>>& matrix_sizes, const std::vector<double>& average_result_times);