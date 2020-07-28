/**
Author: Marko Njegomir sw-38-2018
Project: Parallel Matrix Multiplication using Intel's TBB Tasks
Date: 5/17/2020
*/
#pragma once

#include <iostream>
#include <tbb\tick_count.h>
#include "MyTaskMatrix.h"
#include "MullTask.h"

using namespace std;
using namespace tbb;

/**
* Main starting point of the serial version of the program
* @param c int : number of arguments
* @param argv char* [] : matrix1filename, matrix2filename and outputfilename
* @returns int : 0 if no error like the regular main funcion
*/
int mullTask(int c, char* argv[]);

/**
* Loads two matrices from the files, and then performs matrix multiplication multiple times.
* It checks if the results are valid, and writes the average time needed for the matrix multiplication
* int the results vector
* @param inputFile1 const reference to a first filename
* @param inputFile2 const reference to a second filename
* @param outFilename const reference to an output filename
* @param average_result_times reference to a vector that holds the average reult times
* @param type type of task
* @returns true if the results were correct
* @throws runtime_excetpion if the files can not be opened
*/
bool mull_two_task_matrices(const string& inputFile1, const string& inputFile2, const string& outFilename, std::vector<double>& average_result_times, TaskType type);


/**
* When no input arguments are given, the program uses this function to go trough a vector of predefined matrices
* It loads the matrices and multiplies them
* @param matrix_sizes const reference to a vector of matrix sizes that indicate what matrices will be loaded and multiplied
*/
void mull_all_task_matrices(const std::vector<std::pair<int, int>>& matrix_sizes);

/**
* Prints the average result times for given matrix sizes
* @param matrix_sizes const reference to a vector of matrix sizes that indicate what matrices will be printed in the header
* @param avg_res_times_single_elem const reference to a vector that holds average result times for apropriate matrix size for siglne element function
* @param avg_res_times_single_row const reference to a vector that holds average result times for apropriate matrix size for single row function
* @param avg_res_times_distributed const reference to a vector that holds average result times for apropriate matrix size for distributed rows function
* @param const std::vector<double>& avg_res_times_map_reduce_single_row const reference to a vector that holds average result times for apropriate matrix size for map reduce single row function
*/
void print_task_result_table(const std::vector<std::pair<int, int>>& matrix_sizes,
	const std::vector<double>& avg_res_times_single_elem, const std::vector<double>& avg_res_times_single_row, const std::vector<double>& avg_res_times_distributed,
	const std::vector<double>& avg_res_times_map_reduce_single_row);



/**
* Saves the matrix size as well as average times to the file
* @param matrix_sizes const reference to a vector of matrix sizes that indicate what matrices will be printed in the header
* @param avg_res_times_single_elem const reference to a vector that holds average result times for apropriate matrix size for siglne element function
* @param avg_res_times_single_row const reference to a vector that holds average result times for apropriate matrix size for single row function
* @param avg_res_times_distributed const reference to a vector that holds average result times for apropriate matrix size for distributed rows function
* @param const std::vector<double>& avg_res_times_map_reduce_single_row const reference to a vector that holds average result times for apropriate matrix size for map reduce single row function
*/
void save_average_times_tasks(const std::vector<std::pair<int, int>>& matrix_sizes,
	const std::vector<double>& avg_res_times_single_elem, const std::vector<double>& avg_res_times_single_row, const std::vector<double>& avg_res_times_distributed,
	const std::vector<double>& avg_res_times_map_reduce_single_row);