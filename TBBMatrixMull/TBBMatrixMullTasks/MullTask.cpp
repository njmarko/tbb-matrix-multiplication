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

SingleRow::SingleRow(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix & _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2, int _a_row)
	:m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2), a_row(_a_row)
{
}

task * SingleRow::execute()
{
	for (size_t b_col = 0; b_col < cols_m2; b_col++)
	{
		MyMatrix::const_iterator cit_m1_beg = m1.cbegin() + a_row*cols_m1;
		MyMatrix::const_iterator cit_m1_end = m1.cbegin() + (a_row + 1)*cols_m1;
		MyMatrix::const_iterator cit_m2_beg = m2.cbegin() + b_col*rows_m2;
		m3[a_row*rows_m1 + b_col] = std::inner_product(cit_m1_beg, cit_m1_end, cit_m2_beg, 0);
	}
	return NULL;
}

MullSingleRowTask::MullSingleRowTask(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix & _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
	:m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2)
{
}

task * MullSingleRowTask::execute()
{
	for (size_t i = 0; i < rows_m1; i++)
	{
		all_tasks.push_back(*new(allocate_child())SingleRow(m1, m2, m3, rows_m1, cols_m1, rows_m2, cols_m2, i));

	}
	set_ref_count(rows_m1 + 1);
	spawn_and_wait_for_all(all_tasks);
	return NULL;
}

MullDistributedTask::MullDistributedTask(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix & _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
	:m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2)
{
}

task * MullDistributedTask::execute()
{
	const int processor_count = std::thread::hardware_concurrency();
	int inc = rows_m1 < processor_count ? 1 : (int)rows_m1 / processor_count;
	int i = 0;
	int counter = 0;
	while(i < rows_m1)
	{
		if (i+inc > rows_m1)
		{
			all_tasks.push_back(*new(allocate_child())RowsInRange(m1, m2, m3, rows_m1, cols_m1, rows_m2, cols_m2, i, rows_m1));
		}
		else {
			all_tasks.push_back(*new(allocate_child())RowsInRange(m1, m2, m3, rows_m1, cols_m1, rows_m2, cols_m2, i, i + inc));
		}
		++counter;
		i += inc;
	}
	set_ref_count(counter + 1);
	spawn_and_wait_for_all(all_tasks);
	return NULL;
}

RowsInRange::RowsInRange(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix & _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2, int _row_start, int _row_end)
	:m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2),row_start(_row_start), row_end(_row_end)
{
}

task * RowsInRange::execute()
{
	for (int a_row = row_start; a_row < row_end; a_row++)
	{
		for (size_t b_col = 0; b_col < cols_m2; b_col++)
		{
			MyMatrix::const_iterator cit_m1_beg = m1.cbegin() + a_row*cols_m1;
			MyMatrix::const_iterator cit_m1_end = m1.cbegin() + (a_row + 1)*cols_m1;
			MyMatrix::const_iterator cit_m2_beg = m2.cbegin() + b_col*rows_m2;
			m3[a_row*rows_m1 + b_col] = std::inner_product(cit_m1_beg, cit_m1_end, cit_m2_beg, 0);
		}
	}
	return NULL;
}

string taskTypeToStr(TaskType t)
{
	switch (t)
	{
	case single_element: return "Single element";
	case signle_row: return "Single row";
	case distributed: return "Distributed";
	case map_reduce: return "Map reduce";
	default: return "NoType";
	}
}

MapReduceSingleRow::MapReduceSingleRow(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix & _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2, int _a_row)
	:m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2), a_row(_a_row)
{
}

task * MapReduceSingleRow::execute()
{
	for (size_t b_col = 0; b_col < cols_m2; b_col++)
	{
		m3[a_row*rows_m1 + b_col] = tbb::parallel_reduce(
			tbb::blocked_range<int>(0, cols_m1),
			0,
			[&](tbb::blocked_range<int> r, int running_total)
		{
			for (int i = r.begin(); i<r.end(); ++i)
			{
				running_total = std::plus<int>()(running_total,
					std::multiplies<int>()(m1[a_row*cols_m1 + i], m2[b_col*rows_m2 + i]));
				//running_total += m1[a_row*cols_m1 + i] * m2[b_col*rows_m2 + i];
			}

			return running_total;
		}, std::plus<int>());

	}
	return NULL;
}

MapReduceSingleRowTask::MapReduceSingleRowTask(const MyMatrix & _m1, const MyMatrix & _m2, MyMatrix & _m3, const int _rows_m1, const int _cols_m1, const int _rows_m2, const int _cols_m2)
	:m1(_m1), m2(_m2), m3(_m3), rows_m1(_rows_m1), cols_m1(_cols_m1), rows_m2(_rows_m2), cols_m2(_cols_m2)
{
}

task * MapReduceSingleRowTask::execute()
{
	for (size_t i = 0; i < rows_m1; i++)
	{
		all_tasks.push_back(*new(allocate_child())MapReduceSingleRow(m1, m2, m3, rows_m1, cols_m1, rows_m2, cols_m2, i));

	}
	set_ref_count(rows_m1 + 1);
	spawn_and_wait_for_all(all_tasks);
	return NULL;
}
