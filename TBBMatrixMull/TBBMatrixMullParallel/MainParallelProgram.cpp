#include "MainParallelProgram.h"


int mullParallel(int argc, char * argv[])
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
		MyParallelMatrix* m1 = new MyParallelMatrix();


		m1->load_data(inFilename1);
		//m1.print_matrix();
		//m1.print_matrix();

		MyParallelMatrix* m2 = new MyParallelMatrix();
		m2->load_data(inFilename2);
		//m2->print_matrix();

		MyParallelMatrix* m3 = new MyParallelMatrix(m1->getRowSize(),m2->getColSize());

		tick_count t1 = tick_count::now();
		MullTask& task = *new(task::allocate_root()) MullTask(m1, m2, m3);
		task::spawn_root_and_wait(task);
		tick_count t2 = tick_count::now();
		m3->print_matrix();
		cout << "Vreme izvrsavanja paralelnog algoritma mnozenja: " << (t2 - t1).seconds() * 1000 << "ms.\n";
		
	}
	catch (const MyParallelMatrix::MatrixException&e)
	{
		std::cout << e.what() << endl;
	}

	return 0;
}
