#include "MullTask.h"




MullSingleElemTask::MullSingleElemTask(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix & _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
:m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2)
{
}

task * MullSingleElemTask::execute()
{
	for (size_t i = 0; i < rows_m1; i++)
	{
		all_tasks.push_back(*new(allocate_child())RowAndCols(m1, m2, m3,rows_m1,cols_m1,rows_m2,cols_m2, i));

	}
	set_ref_count(rows_m1 + 1);
	spawn_and_wait_for_all(all_tasks);
	return NULL;
}


RowAndCols::RowAndCols(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix & _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2, int _a_row)
	: m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2), a_row(_a_row)
{
}

task * RowAndCols::execute()
{

	for (size_t i = 0; i < cols_m2; i++)
	{
		col_tasks.push_back(*new(allocate_child())RowAndCol(m1, m2, m3,rows_m1, cols_m1, rows_m2, cols_m2, a_row, i));
	}
	set_ref_count(cols_m2 + 1);
	spawn_and_wait_for_all(col_tasks);
	return NULL;
}


RowAndCol::RowAndCol(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix & _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2, int _a_row, int _b_col)
	: m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2), a_row(_a_row),b_col(_b_col)
{
}

task * RowAndCol::execute()
{

	MyMatrix::const_iterator cit_m1_beg = m1.cbegin() + a_row*cols_m1;
	MyMatrix::const_iterator cit_m1_end = m1.cbegin() + (a_row + 1)*cols_m1;
	MyMatrix::const_iterator cit_m2_beg = m2.cbegin() + b_col*rows_m2;
	m3[a_row*rows_m1 + b_col] = std::inner_product(cit_m1_beg, cit_m1_end, cit_m2_beg, 0);
	return NULL;
}
