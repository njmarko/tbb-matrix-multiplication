#include "MainProgram.h"
#include "MyMatrix.h"



int mullSerial(int argc, char * argv[])
{
	// set the seed for pseudorandom numbers that are used for generating data
	srand(time(NULL));
	// dimensions of the matrices that will be created and tested
	vector<int> matrix_sizes = {10,25,50,100,250,500,1000,2000,4000};
	try
	{
		//create_matrix_files(matrix_sizes);
		char* inFilename1;
		char* inFilename2;
		char* outFilename;

		if (argc != 4) {
			throw runtime_error("Wrong number of input arguments!\n");
		}

		inFilename1 = argv[1];
		inFilename2 = argv[2];

		MyMatrix m1;
		int rows_m1 = 0;
		int cols_m1 = 0;

		load_data(inFilename1, m1, rows_m1,cols_m1);

		if(rows_m1 < 10 && cols_m1 < 10)
			print_matrix(m1,rows_m1,cols_m1);

		MyMatrix m2;
		int rows_m2 = 0;
		int cols_m2 = 0;
		load_data(inFilename2,m2,rows_m2,cols_m2);
		if (rows_m2 < 10 && cols_m2 < 10)
			print_matrix(m2, rows_m2, cols_m2);

		MyMatrix m3(rows_m1*cols_m2,0);
		tick_count t1 = tick_count::now();
		mull_serial_transp_inner_prod(m1,m2,m3,rows_m1,cols_m1,rows_m2, cols_m2);
		tick_count t2 = tick_count::now();
		cout << "Time taken for serial multiplication: " << (t2 - t1).seconds() * 1000 << "ms.\n";
		if (rows_m1 < 10 && cols_m2 < 10)
			print_matrix(m3, rows_m1, cols_m2);
	}
	catch (const runtime_error&e)
	{
		std::cout << e.what() << endl;
	}

	return 0;
}
