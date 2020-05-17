/*
Author: Marko Njegomir sw-38-2018
Project: Parallel Matrix Multiplication using Intel's TBB library
Date: 5/17/2020
*/
#pragma once

#include <iostream>
#include <tbb\tick_count.h>

using namespace std;
using namespace tbb;

int mullSerial(int c,char* argv[]);