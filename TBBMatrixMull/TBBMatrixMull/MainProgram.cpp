#include "MainProgram.h"
#include "MyMatrix.h"

int mullSerial(int argc, char * argv[])
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
		m1.print_matrix();

		m1.load_data(inFilename1);

		m1.print_matrix();

		MyMatrix m2;
		m2.load_data(inFilename2);
		m2.print_matrix();

		MyMatrix m3;
		tick_count t1 = tick_count::now();
		m3 = m1*m2;
		tick_count t2 = tick_count::now();
		cout << "Vreme izvrsavanja serijskog algoritma mnozenja: " << (t2 - t1).seconds() * 1000 << "ms.\n";
		m3.print_matrix();
	}
	catch (const MyMatrix::MatrixException&e)
	{
		std::cout << e.what() << endl;
	}

	return 0;
}
