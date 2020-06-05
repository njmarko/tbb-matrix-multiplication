#include "MainTaskProgram.h"


int mullTask(int argc, char * argv[])
{
	// set the seed for pseudorandom numbers that are used for generating data
	srand(time(NULL));
	// dimensions of the matrices that will be created and tested
	std::vector<std::pair<int, int>> matrix_sizes = { { 10,10 },{ 25 ,25 },{ 64 ,64 },{ 100 ,100 },{ 256 ,256 },{ 400,400 },{ 512 ,512 },{ 1000 ,1000 },{ 1536,1536 },{ 2000,2000 } };
	try
	{
		//create_matrix_files(matrix_sizes);

		char* inFilename1;
		char* inFilename2;
		char* outFilename;

		// Program either has no arguments or it has 4 arguments (inputfile1, inptufile2, outputfiles)
		if (argc != 4 && argc != 1) {
			throw runtime_error("Wrong number of input arguments!\n");
		}

		if (argc == 4) // when the filenames are givens
		{
			std::vector<double> average_result_times;
			inFilename1 = argv[1];
			inFilename2 = argv[2];
			outFilename = argv[3];
			if (mull_two_task_matrices(inFilename1, inFilename2, outFilename, average_result_times,TaskType::distributed))
			{
				cout << "Average time for multiplication by using tbb tasks: " << average_result_times[0] << endl;
			}
		}
		if (argc == 1) // when there are no arguments, go trough the matrix_sizes vector
		{
			mull_all_task_matrices(matrix_sizes);
		}
	}
	catch (const runtime_error&e)
	{
		std::cout << e.what() << endl;
	}
	return 0;
}

bool mull_two_task_matrices(const string & inputFile1, const string & inputFile2, const string & outFilename, std::vector<double>& average_result_times, TaskType type)
{
	MyMatrix m1;
	int rows_m1 = 0;
	int cols_m1 = 0;

	load_data_tasks(inputFile1, m1, rows_m1, cols_m1);

	if (rows_m1 < 10 && cols_m1 < 10)
		print_matrix_tasks(m1, rows_m1, cols_m1);

	MyMatrix m2;
	int rows_m2 = 0;
	int cols_m2 = 0;
	load_data_tasks(inputFile2, m2, rows_m2, cols_m2);
	if (rows_m2 < 10 && cols_m2 < 10)
		print_matrix_tasks(m2, rows_m2, cols_m2);

	MyMatrix m3;

	vector<double> times;
	for (int i = 0; i < 5; i++)
	{
		m3 = MyMatrix(rows_m1*cols_m2, 0);
		tick_count t1 = tick_count::now();

		MyMatrix m2_transposed(rows_m2*cols_m2);
		task_transpose(m2, m2_transposed, rows_m2, cols_m2);
		if (type==TaskType::single_element)
		{
			MullSingleElemTask& task = *new(task::allocate_root()) MullSingleElemTask(m1, m2_transposed, m3, rows_m1, cols_m1, rows_m2, cols_m2);
			task::spawn_root_and_wait(task);
		}
		else if (type == TaskType::signle_row)
		{
			MullSingleRowTask& task = *new(task::allocate_root()) MullSingleRowTask(m1, m2_transposed, m3, rows_m1, cols_m1, rows_m2, cols_m2);
			task::spawn_root_and_wait(task);
		}
		else if (type == TaskType::distributed)
		{
			MullDistributedTask& task = *new(task::allocate_root()) MullDistributedTask(m1, m2_transposed, m3, rows_m1, cols_m1, rows_m2, cols_m2);
			task::spawn_root_and_wait(task);
		}
		tick_count t2 = tick_count::now();
		if (!valid_task_results(m3, rows_m1, cols_m1, rows_m2, cols_m2))
		{
			cout << "Invalid results for matrix multiplication of " <<
				to_string(rows_m1) + "x" + to_string(cols_m1) + " and " +
				to_string(rows_m1) + "x" + to_string(cols_m1) << " matrices\n";
			return false;
		}

		times.push_back((t2 - t1).seconds() * 1000); // time in ms
	}
	double avg = std::accumulate(times.cbegin(), times.cend(), 0.0) / times.size();


	cout << taskTypeToStr(type) << " task time taken for matrix multiplication of "
		<< to_string(rows_m1) << "x" << to_string(cols_m1) << " and "
		<< to_string(rows_m2) << "x" << to_string(cols_m2) << " matrices "
		<< " : " << avg << "ms.\n";

	if (rows_m1 < 10 && cols_m2 < 10)
		print_matrix_tasks(m3, rows_m1, cols_m2);
	cout << "Results are valid!\n";
	average_result_times.emplace_back(avg);
	save_task_results(outFilename, m3, rows_m1, cols_m2);
	return true;
}

void mull_all_task_matrices(const std::vector<std::pair<int, int>>& matrix_sizes)
{
	std::vector<double> avg_res_times_single_elem;
	std::vector<double> avg_res_times_single_row;
	std::vector<double> avg_res_times_distributed;

	for each (pair<int, int> var in matrix_sizes)
	{
		string inFilename1 = "../TestData/" + to_string(var.first) + "x" + to_string(var.second) + ".txt";
		string inFilename2 = "../TestData/" + to_string(var.first) + "x" + to_string(var.second) + ".txt";
		string outFilenameSignleElem = "../MultiplicationResults/parallelTasksSignleElem/" +
			to_string(var.first) + "x" + to_string(var.second) + "mull" +
			to_string(var.first) + "x" + to_string(var.second) + ".txt";
		mull_two_task_matrices(inFilename1, inFilename2, outFilenameSignleElem, avg_res_times_single_elem, TaskType::single_element);
		string outFilenameSignleRow = "../MultiplicationResults/parallelTasksRows/" +
			to_string(var.first) + "x" + to_string(var.second) + "mull" +
			to_string(var.first) + "x" + to_string(var.second) + ".txt";
		mull_two_task_matrices(inFilename1, inFilename2, outFilenameSignleRow, avg_res_times_single_row, TaskType::signle_row);
		string outFilenameDistributed = "../MultiplicationResults/parallelDistributed/" +
			to_string(var.first) + "x" + to_string(var.second) + "mull" +
			to_string(var.first) + "x" + to_string(var.second) + ".txt";
		mull_two_task_matrices(inFilename1, inFilename2, outFilenameDistributed, avg_res_times_distributed, TaskType::distributed);
	}

	print_task_result_table(matrix_sizes, avg_res_times_single_elem, avg_res_times_single_row, avg_res_times_distributed);
	save_average_times_tasks(matrix_sizes, avg_res_times_single_elem, avg_res_times_single_row, avg_res_times_distributed);
}

void print_task_result_table(const std::vector<std::pair<int, int>>& matrix_sizes, 
	const std::vector<double>& avg_res_times_single_elem, const std::vector<double>& avg_res_times_single_row, const std::vector<double>& avg_res_times_distributed)
{
	//std::stringstream ss;
	cout << endl;
	cout << string(20 + 10 * matrix_sizes.size(), '=');
	cout << endl;
	cout.width(20);
	cout << "Algorithm\\Size";
	for each (pair<int, int> var in matrix_sizes)
	{
		cout.width(10);
		cout << to_string(var.first) + "x" + to_string(var.second);
	}
	cout << endl;
	cout << string(20 + 10 * matrix_sizes.size(), '=');
	cout << endl;

	cout.width(20);
	cout << "Tasks Element";
	for each (double var in avg_res_times_single_elem)
	{
		cout.width(10);
		cout << var;
	}
	cout << endl;
	cout << string(20 + 10 * matrix_sizes.size(), '-');
	cout << endl;

	cout.width(20);
	cout << "Tasks Rows";
	for each (double var in avg_res_times_single_row)
	{
		cout.width(10);
		cout << var;
	}
	cout << endl;
	cout << string(20 + 10 * matrix_sizes.size(), '-');
	cout << endl;

	cout.width(20);
	cout << "Tasks Distributed";
	for each (double var in avg_res_times_distributed)
	{
		cout.width(10);
		cout << var;
	}
	cout << endl;
	cout << string(20 + 10 * matrix_sizes.size(), '-');
	cout << endl;
}

void save_average_times_tasks(const std::vector<std::pair<int, int>>& matrix_sizes, 
	const std::vector<double>& avg_res_times_single_elem, const std::vector<double>& avg_res_times_single_row, const std::vector<double>& avg_res_times_distributed)
{
	string outFilename = "../MultiplicationResults/timesTasks/AverageTasksMullTimes.txt";
	ofstream out(outFilename);
	for each (pair<int, int> var in matrix_sizes)
	{
		out << var.first << " ";
	}
	out << endl;
	for each (double var in avg_res_times_single_elem)
	{
		out << var << " ";
	}
	out << endl;
	for each (double var in avg_res_times_single_row)
	{
		out << var << " ";
	}
	out << endl;
	for each (double var in avg_res_times_distributed)
	{
		out << var << " ";
	}
	out << endl;
	out.flush();
	out.close();
}
