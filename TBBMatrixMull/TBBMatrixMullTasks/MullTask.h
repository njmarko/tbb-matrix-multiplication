//#pragma once
//
//#include <tbb\task.h>
//#include "MyParallelMatrix.h"
//
//using namespace std;
//using namespace tbb;
//
//class MullTask :public task
//{
//public:
//	MullTask(MyParallelMatrix* _a, MyParallelMatrix* _b, MyParallelMatrix* _c);
//	task* execute();
//
//private:
//	task_list all_tasks;
//	MyParallelMatrix* a;
//	MyParallelMatrix* b;
//	MyParallelMatrix* c;
//};
//
//
//class RowAndCols :public task {
//public:
//	RowAndCols(MyParallelMatrix* _a, MyParallelMatrix* _b, MyParallelMatrix* _c, int row);
//	task* execute();
//private:
//	task_list col_tasks;
//	MyParallelMatrix* a;
//	MyParallelMatrix* b;
//	MyParallelMatrix* c;
//	int row;
//};
//
//class RowAndCol :public task {
//public:
//	RowAndCol(MyParallelMatrix* _a, MyParallelMatrix* _b, MyParallelMatrix* _c, int row, int col);
//	task* execute();
//private:
//	MyParallelMatrix* a;
//	MyParallelMatrix* b;
//	MyParallelMatrix* c;
//	int row;
//	int col;
//};
//
