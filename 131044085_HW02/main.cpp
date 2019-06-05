#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"

/* author Gulzada IISAEVA */

int main (int argc, char**argv)
{
	if (argc != 3){
		std::cerr << "Usage: prog exeFile debugOption\n";
		exit(1); 
	}
	remove( "output.txt" );
	remove( "kernel.mem" );
	int DEBUG = atoi(argv[2]);

	ofstream outfile;
    outfile.open("output.txt", ios_base::app);

	Memory mem(0x10000);
	CPU8080 theCPU(&mem);
	GTUOS	theOS(theCPU);
	uint64_t cycles=0;
	theCPU.ReadFileIntoMemoryAt(argv[1], 0);	

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

