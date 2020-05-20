#include "MainProgram.h"
#include "MyMatrix.h"

int mullSerial(int argc, char * argv[])
{
	char* inFilename1;
	char* inFilename2;
	char* outFilename;

	if (argc != 4) {
		std::cout << "Wrong number of elements\n";
		return -1;
	}

	inFilename1 = argv[1];
	inFilename2 = argv[2];

	MyMatrix m1;
	m1.print_matrix();
	
	m1.load_data(inFilename1);
	
	m1.print_matrix();

	MyMatrix m2;
	m2.load_data(inFilename2);
	m2.print_matrix();

	MyMatrix m3 = m1*m2;
	m3.print_matrix();
	

	
	return 0;
}