/**
Author: Marko Njegomir sw-38-2018
Project: Serial Matrix Multiplication using Intel's TBB library
Date: 5/26/2020
*/

#include <iostream>
#include "MainProgram.h"
#include <fstream>
#include <string>
#include <thread>

int main(int argc, char* argv[]) {

	std::cout << "Serial matrix multiplication\n";
	mullSerial(argc,argv);


	return 0;
}