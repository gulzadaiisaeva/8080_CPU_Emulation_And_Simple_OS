#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"

	// This is just a sample main function, you should rewrite this file to handle problems 
	// with new multitasking and virtual memory additions.
int main (int argc, char**argv)
{
	if (argc != 3){
		std::cerr << "Usage: prog exeFile debugOption\n";
		exit(1); 
	}
	remove( "output.txt" );
	int DEBUG = atoi(argv[2]);
	ofstream outfile;
    outfile.open("output.txt", ios_base::app);

	memory mem;
	CPU8080 theCPU(&mem);
	GTUOS	theOS;
	uint64_t cycles=0;
	theCPU.ReadFileIntoMemoryAt(argv[1], 0x0000);	
 
	do	
	{
		cycles+=theCPU.Emulate8080p(DEBUG);
		if(theCPU.isSystemCall())
			cycles+=theOS.handleCall(theCPU);
		if(DEBUG ==2){
			std::cin.get();
		}
	}	while (!theCPU.isHalted());

	outfile<<"\nTotal Number Of Cycles: "<<cycles<<endl;
	cout<<"\nTotal Number Of Cycles: "<<cycles<<endl;
	outfile.close();
	return 0;
}

