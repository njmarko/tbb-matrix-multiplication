/**
Author: Marko Njegomir sw-38-2018
Project: Serial Matrix Multiplication using Intel's TBB library
Date: 5/26/2020
*/

#include <iostream>
#include "MainParallelProgram.h"
#include <fstream>
#include <string>
#include <thread>

int main(int argc, char* argv[]) {

	std::cout << "Parallel matrix multiplication\n";
	mullParallel(argc, argv);


	return 0;
}