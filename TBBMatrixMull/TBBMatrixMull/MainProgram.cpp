#include "MainProgram.h"
#include "MyMatrix.h"



int mullSerial(int argc, char * argv[])
{
	// set the seed for pseudorandom numbers that are used for generating data
	srand(time(NULL));
	// dimensions of the matrices that will be created and tested
	std::vector<std::pair<int, int>> matrix_sizes = { {10,10},{ 25 ,25 },{ 64 ,64 },{ 100 ,100 },{ 256 ,256 },{400,400},{ 512 ,512 },{ 1000 ,1000 },{ 1536,1536},{2000,2000} };
	try
	{
		//create_matrix_files(matrix_sizes);

		char* inFilename1;
		char* inFilename2;
		char* outFilename;

		// Program either has no arguments or it has 4 arguments (inputfile1, inptufile2, outputfiles)
		if (argc !=4 && argc != 1) {
			throw runtime_error("Wrong number of input arguments!\n");
		}


		if (argc == 4) // when the filenames are givens
		{
			std::vector<double> average_result_times;
			inFilename1 = argv[1];
			inFilename2 = argv[2];
			outFilename = argv[3];
			if (mull_two_matrices(inFilename1, inFilename2, outFilename, average_result_times))
			{
				cout << "Average time for serial multiplication: " << average_result_times[0] << endl;
			}		
		}
		if (argc == 1) // when there are no arguments, go trough the matrix_sizes vector
		{
			mull_all_matrices(matrix_sizes);
		}
	}
	catch (const runtime_error&e)
	{
		std::cout << e.what() << endl;
	}

	return 0;
}

void mull_all_matrices(const std::vector<std::pair<int, int>>& matrix_sizes) {

	std::vector<double> average_result_times;

	for each (pair<int,int> var in matrix_sizes)
	{
		string inFilename1 = "../TestData/" + to_string(var.first) + "x" + to_string(var.second) + ".txt";
		string inFilename2 = "../TestData/" + to_string(var.first) + "x" + to_string(var.second) + ".txt";
		string outFilename = "../MultiplicationResults/serial/" + 
			to_string(var.first) + "x" + to_string(var.second) + "mull"+
			to_string(var.first) + "x" + to_string(var.second) + ".txt";
		mull_two_matrices(inFilename1, inFilename2, outFilename, average_result_times);
	}

	print_result_table(matrix_sizes, average_result_times);
}

void print_result_table(const std::vector<std::pair<int, int>>& matrix_sizes, const std::vector<double>& average_result_times)
{
	//std::stringstream ss;
	cout << endl;
	cout << string(15 + 10 * matrix_sizes.size(), '=');
	cout << endl;
	cout.width(15);
	cout << "Algoritham\\Size"; 
	for each (pair<int, int> var in matrix_sizes)
	{
		cout.width(10);
		cout << to_string(var.first )+ "x" + to_string(var.second);
	}
	cout << endl;
	cout << string(15 + 10 * matrix_sizes.size(), '=');
	cout << endl;
	cout.width(15);
	cout << "Serial";
	for each (double var in average_result_times)
	{
		cout.width(10);
		cout << var;
	}
	cout << endl;
	cout << string(15 + 10 * matrix_sizes.size(), '-');
	cout << endl;

}

bool mull_two_matrices(const string & inFilename1, const string & inFilename2, const string & outFilename, std::vector<double>& average_result_times)
{
	MyMatrix m1;
	int rows_m1 = 0;
	int cols_m1 = 0;

	load_data(inFilename1, m1, rows_m1, cols_m1);

	if (rows_m1 < 10 && cols_m1 < 10)
		print_matrix(m1, rows_m1, cols_m1);

	MyMatrix m2;
	int rows_m2 = 0;
	int cols_m2 = 0;
	load_data(inFilename2, m2, rows_m2, cols_m2);
	if (rows_m2 < 10 && cols_m2 < 10)
		print_matrix(m2, rows_m2, cols_m2);

	MyMatrix m3;

	vector<double> times;
	tick_count t1;
	tick_count t2;
	for (int i = 0; i < 5; i++)
	{
		m3 = MyMatrix(rows_m1*cols_m2, 0);
		t1 = tick_count::now();
		mull_serial_transp_inner_prod(m1, m2, m3, rows_m1, cols_m1, rows_m2, cols_m2);
		t2 = tick_count::now();
		//if (!validate_results(m3, rows_m1, cols_m1, rows_m2, cols_m2))
		//{
		//	cout << "Invalid results for matrix multiplication of " <<
		//		to_string(rows_m1) + "x" + to_string(cols_m1) + " and " +
		//		to_string(rows_m1) + "x" + to_string(cols_m1) << " matrices\n";
		//	return false;
		//}

		times.push_back((t2 - t1).seconds() * 1000); // time in ms
	}
	double avg = std::accumulate(times.cbegin(), times.cend(), 0.0) / times.size();
	cout << "Average time taken for serial multiplication of "
		<< to_string(rows_m1) << "x" << to_string(cols_m1) << " and " 
		<< to_string(rows_m2) << "x" << to_string(cols_m2) << " matrices "
		<<" : " << avg << "ms.\n";
	if (rows_m1 < 10 && cols_m2 < 10)
		print_matrix(m3, rows_m1, cols_m2);
	cout << "Results are valid!\n";
	average_result_times.emplace_back(avg);
	save_result(outFilename, m3, rows_m1, cols_m2);
	return true;
}
