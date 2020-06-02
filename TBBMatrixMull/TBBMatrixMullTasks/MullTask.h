#pragma once

#include <tbb\task.h>
#include "MyTaskMatrix.h"

using namespace std;
using namespace tbb;


/*
* Different task types
*/
enum TaskType
{
	single_element=0,
	signle_row,
	distributed
};

/*
* Converts the task type to string
* @param enum type of the task
* @returns string representation of the type of task
*/
string taskTypeToStr(TaskType t);


/*
*
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