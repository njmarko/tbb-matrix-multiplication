//#include "MullTask.h"
//
//MullTask::MullTask(MyParallelMatrix* _a, MyParallelMatrix* _b, MyParallelMatrix* _c)
//	:a(_a), b(_b), c(_c)
//{
//}
//
//task * MullTask::execute()
//{
//	for (size_t i = 0; i < a->getRowSize(); i++)
//	{
//		all_tasks.push_back(*new(allocate_child())RowAndCols(a, b, c, i));
//
//	}
//	set_ref_count(a->getRowSize() + 1);
//	spawn_and_wait_for_all(all_tasks);
//	return NULL;
//}
//
//RowAndCols::RowAndCols(MyParallelMatrix* _a, MyParallelMatrix* _b, MyParallelMatrix* _c, int _row) :
//	a(_a), b(_b), c(_c), row(_row)
//{
//}
//
//task * RowAndCols::execute()
//{
//	Matrix* m1 = &a->getData();
//	Matrix* m2 = &b->getData();
//	Matrix* m3 = &c->getData();
//	for (size_t i = 0; i < b->getColSize(); i++)
//	{
//		col_tasks.push_back(*new(allocate_child())RowAndCol(a, b, c, row, i));
//	}
//	set_ref_count(a->getColSize() + 1);
//	spawn_and_wait_for_all(col_tasks);
//	return NULL;
//}
//
//RowAndCol::RowAndCol(MyParallelMatrix * _a, MyParallelMatrix * _b, MyParallelMatrix * _c, int a_row, int b_col)
//	:a(_a), b(_b), c(_c), row(a_row), col(b_col)
//{
//}
//
//task * RowAndCol::execute()
//{
//	Matrix* m1 = &a->getData();
//	Matrix* m2 = &b->getData();
//	Matrix* m3 = &c->getData();
//	for (size_t i = 0; i < a->getRowSize(); i++)
//	{
//		(*m3)[row][col] += (*m1)[row][i] * (*m2)[i][col];
//	}
//	return NULL;
//}
