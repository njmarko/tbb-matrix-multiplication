/**
Author: Marko Njegomir sw-38-2018
Project: Serial Matrix Multiplication using Intel's TBB library
Date: 5/26/2020
*/

#include <iostream>
#include "MainTaskProgram.h"
#include <fstream>
#include <string>
#include <thread>

int main(int argc, char* argv[]) {

	std::cout << "Matrix multiplication using tasks\n";
	mullTask(argc, argv);


	return 0;
}