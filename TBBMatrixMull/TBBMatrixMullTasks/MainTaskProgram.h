/*
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

/*
* Main starting point of the serial version of the program
* @param int c : number of arguments
* @param char* argv[] : matrix1filename, matrix2filename and outputfilename
* @returns int : 0 if no error like the regular main funcion
*/
int mullTask(int c, char* argv[]);