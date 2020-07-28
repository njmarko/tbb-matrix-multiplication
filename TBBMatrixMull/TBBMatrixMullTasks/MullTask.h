#pragma once

#include <tbb\task.h>
#include "MyTaskMatrix.h"
#include <tbb\parallel_reduce.h>
#include <tbb\parallel_for.h>

using namespace std;
using namespace tbb;


/**
* Different task types
*/
enum TaskType
{
	single_element=0,
	signle_row,
	distributed,
	map_reduce
};

/**
* Converts the task type to string
* @param t enum type of the task
* @returns string representation of the type of task
*/
string taskTypeToStr(TaskType t);


/**
* Root task that will result in the creation of
* one task per element in the resulting matrix
*/
class MullSingleElemTask :public task
{
public:
	MullSingleElemTask(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2);
	task* execute();

private:
	task_list all_tasks;
	const MyMatrix& m1;
	const MyMatrix& m2;
	MyMatrix& m3;
	const int rows_m1;
	const int cols_m1;
	const int rows_m2;
	const int cols_m2;
};

/**
* Task that represents one row, and will create a task for every element in the row
*/
class RowAndCols :public task {
public:
	RowAndCols(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2, int _a_row);
	task* execute();
private:
	task_list col_tasks;
	const MyMatrix& m1;
	const MyMatrix& m2;
	MyMatrix& m3;
	const int rows_m1;
	const int cols_m1;
	const int rows_m2;
	const int cols_m2;
	int a_row;
};

/**
* Task that represents a single element, and will calculate the 
* value of that element
*/
class RowAndCol :public task {
public:
	RowAndCol(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2, int _a_row, int _b_col);
	task* execute();
private:
	const MyMatrix& m1;
	const MyMatrix& m2;
	MyMatrix& m3;
	const int rows_m1;
	const int cols_m1;
	const int rows_m2;
	const int cols_m2;
	int a_row;
	int b_col;
};

/**
* Task that will create one task per row
* and that child task will then calculate the values for all the elements in the row
*/
class MullSingleRowTask :public task
{
public:
	MullSingleRowTask(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2);
	task* execute();

private:
	task_list all_tasks;
	const MyMatrix& m1;
	const MyMatrix& m2;
	MyMatrix& m3;
	const int rows_m1;
	const int cols_m1;
	const int rows_m2;
	const int cols_m2;
};

/**
* Task that will calculate all the values for a single row
*/
class SingleRow :public task {
public:
	SingleRow(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2, int _a_row);
	task* execute();
private:
	task_list col_tasks;
	const MyMatrix& m1;
	const MyMatrix& m2;
	MyMatrix& m3;
	const int rows_m1;
	const int cols_m1;
	const int rows_m2;
	const int cols_m2;
	int a_row;
};

/**
* This task will create a certain amount of row tasks
* determined by the formula number_of_rows diveded by number of cores in CPU
*/
class MullDistributedTask :public task
{
public:
	MullDistributedTask(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2);
	task* execute();

private:
	task_list all_tasks;
	const MyMatrix& m1;
	const MyMatrix& m2;
	MyMatrix& m3;
	const int rows_m1;
	const int cols_m1;
	const int rows_m2;
	const int cols_m2;
};

/**
* This task will calculate the values for all the rows assigned to it
*/
class RowsInRange :public task {
public:
	RowsInRange(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2, int _row_start, int _row_end);
	task* execute();
private:
	const MyMatrix& m1;
	const MyMatrix& m2;
	MyMatrix& m3;
	const int rows_m1;
	const int cols_m1;
	const int rows_m2;
	const int cols_m2;
	int row_start;
	int row_end;
};


/**
* Task that will create one task per row
* and that child task will then calculate the values for all the elements in the row by using map reduce implemented with tbb reduce
*/
class MapReduceSingleRowTask :public task
{
public:
	MapReduceSingleRowTask(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2);
	task* execute();

private:
	task_list all_tasks;
	const MyMatrix& m1;
	const MyMatrix& m2;
	MyMatrix& m3;
	const int rows_m1;
	const int cols_m1;
	const int rows_m2;
	const int cols_m2;
};

/**
* Task that will calculate all the values for a single row by using parallel map reduce implemented with tbb reduce
*/
class MapReduceSingleRow :public task {
public:
	MapReduceSingleRow(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix& _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2, int _a_row);
	task* execute();
private:
	task_list col_tasks;
	const MyMatrix& m1;
	const MyMatrix& m2;
	MyMatrix& m3;
	const int rows_m1;
	const int cols_m1;
	const int rows_m2;
	const int cols_m2;
	int a_row;
};
