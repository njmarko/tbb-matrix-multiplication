/*
Author: Marko Njegomir sw-38-2018
Project: Serial Matrix Multiplication using Intel's TBB library
Date: 5/11/2020
*/

#include <iostream>
#include "MainProgram.h"

int main(int argc, char* argv[]) {

	mullSerial(argc,argv);
	std::cout << "Hello World serial!\n";

	return 0;
}