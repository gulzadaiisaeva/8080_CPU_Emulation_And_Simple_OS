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

  remove("mailbox.txt");
  remove("locallist.txt");
  remove("MicroKernel.mem");
  int DEBUG = atoi(argv[2]);

  Memory mem(0x10000);
  
  CPU8080 *theCPU = new CPU8080(&mem);
  GTUOS	theOS(*theCPU);
  
  theCPU->ReadFileIntoMemoryAt(argv[1], 0x0000);
  theCPU->setQuantum(250);
  do{
    if(DEBUG == 2)
      getchar();
    
    theCPU->Emulate8080p(DEBUG);
      if(theCPU->isSystemCall())
	theOS.handleCall(*theCPU);
  }while(!theCPU->isHalted());

  free(theCPU);

  
  return 0;
}
