/**
Author: Marko Njegomir sw-38-2018
Project: Parallel Matrix Multiplication using Intel's TBB library
Date: 5/17/2020
*/
#pragma once

#include <iostream>
#include <tbb\tick_count.h>
#include <ctime>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <fstream>
#include <map>
#include <sstream>

using namespace std;
using namespace tbb;

/**
* Main starting point of the serial version of the program
* @param c int : number of arguments
* @param argv char* [] : matrix1filename, matrix2filename and outputfilename
* @returns int : 0 if no error like the regular main funcion
*/
int mullSerial(int c,char* argv[]);

/**
* Loads two matrices from the files, and then performs matrix multiplication multiple times.
* It checks if the results are valid, and writes the average time needed for the matrix multiplication
* int the results vector
* @param inputFile1 const reference to a first filename
* @param inputFile2 const reference to a second filename
* @param outFilename const reference to an output filename
* @param average_result_times reference to a vector that holds the average reult times
* @returns true if the results were correct
* @throws runtime_excetpion if the files can not be opened
*/
bool mull_two_matrices(const string& inputFile1, const string& inputFile2,const string& outFilename,std::vector<double>& average_result_times);

/**
* When no input arguments are given, the program uses this function to go trough a vector of predefined matrices
* It loads the matrices and multiplies them
* @param matrix_sizes const reference to a vector of matrix sizes that indicate what matrices will be loaded and multiplied
*/
void mull_all_matrices(const std::vector<std::pair<int, int>>& matrix_sizes);

/**
* Prints the average result times for given matrix sizes
* @param matrix_sizes const reference to a vector of matrix sizes that indicate what matrices will be printed in the header
* @param average_result_times const reference to a vector that holds average result times for apropriate matrix size
*/
void print_result_table(const std::vector<std::pair<int, int>>& matrix_sizes, const std::vector<double>& average_result_times);


/**
* Saves the matrix size as well as average times to the file
* @param matrix_sizes const reference to a vector of matrix sizes that indicate what matrices will be printed in the header
* @param average_result_times const reference to a vector that holds average result times for apropriate matrix size
*/
void save_average_times_serial(const std::vector<std::pair<int, int>>& matrix_sizes, const std::vector<double>& average_result_times);