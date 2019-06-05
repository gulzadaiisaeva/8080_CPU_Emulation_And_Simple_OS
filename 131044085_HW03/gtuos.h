#ifndef H_GTUOS
#define H_GTUOS

#include "8080emuCPP.h"
#include <fstream>
#include <vector>

enum SYS_CALL{
  PRINT_B = 0x04,
  PRINT_MEM = 0x03,
  READ_B = 0x07,
  READ_MEM = 0x02,
  PRINT_STR = 0x01,
  READ_STR = 0x08,
  LOAD_EXEC = 0x05,
  PROCESS_EXIT = 0x09,
  SET_QUANTUM = 0x06,
  PRINT_WHOLE = 0x0a,
  RAND_INT =  0x0C,
  WAIT = 0x0D,
  SIGNAL = 0x0E,
  SEND = 0x0F,
  READ = 0x010,
  GETCOND = 0x011

};


class GTUOS{  
 public:
  /*
   * Constructor and destructor are written
   * for file open,close processing
   */
  GTUOS(CPU8080& cpu);
  ~GTUOS();

  uint64_t handleCall(CPU8080  & cpu);
  uint64_t sysPrintB(CPU8080 & cpu);
  uint64_t sysPrintMem(CPU8080 & cpu);
  uint64_t sysReadB(CPU8080 & cpu);
  uint64_t sysReadMem(CPU8080 & cpu);
  uint64_t sysPrintStr(CPU8080 & cpu);
  uint64_t sysReadStr(CPU8080 & cpu);
  uint64_t loadExec(CPU8080& cpu);
  uint64_t processExit(CPU8080& cpu);
  uint64_t setQuantum(CPU8080& cpu);  
  uint64_t printWhole(CPU8080& cpu);

  uint64_t randInt(CPU8080& cpu);
  uint64_t wait(CPU8080& cpu);  
  uint64_t signal(CPU8080& cpu);

  uint64_t send(CPU8080& cpu);
  uint64_t read(CPU8080& cpu);
  uint64_t getcond(CPU8080& cpu);

  void writeContentsOfMemoryToFile(const CPU8080 & cpu);

  int mutexAddr = 40000;
  int fullAddr  = 40001;
  int emptyAddr = 40002;

  vector<int> localArray;
 private:
  /*These are used for file processing*/
  std::ifstream in;
  std::ofstream out;
};


/*
40000 = mutex
40001 = full
40002 = empty*/
#endif
