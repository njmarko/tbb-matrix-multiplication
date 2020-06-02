#include "MainTaskProgram.h"


int mullTask(int argc, char * argv[])
{
	char* inFilename1;
	char* inFilename2;
	char* outFilename;

	if (argc != 4) {
		std::cout << "Wrong number of input arguments!\n";
		return -1;
	}

	inFilename1 = argv[1];
	inFilename2 = argv[2];

	try
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



		MyMatrix m3(rows_m1*cols_m2, 0);

		tick_count t1 = tick_count::now();
		MyMatrix m2_transposed(rows_m2*cols_m2);
		transpose(m2, m2_transposed, rows_m2, cols_m2);
		MullSingleElemTask& task = *new(task::allocate_root()) MullSingleElemTask(m1, m2_transposed, m3,rows_m1,cols_m1,rows_m2,cols_m2);
		task::spawn_root_and_wait(task);
		tick_count t2 = tick_count::now();
		cout << "Time taken for parallel multiplication with tasks: " << (t2 - t1).seconds() * 1000 << "ms.\n";
		if (rows_m1 < 10 && cols_m2 < 10)
			print_matrix(m3, rows_m1, cols_m2);

	}
	catch (const MatrixException&e)
	{
		std::cout << e.what() << endl;
	}

	return 0;
}
