#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"
#include <string.h>
#include <fstream> 

using namespace std;

#define CYCLE 10

GTUOS::GTUOS(CPU8080& cpu){
  cpu.memory->physicalAt(emptyAddr) = 50;
  cpu.memory->physicalAt(fullAddr) = 0;
  cpu.memory->physicalAt(mutexAddr) = 1;

}

GTUOS::~GTUOS(){
}

/*
  Handle the operating system calls
*/
uint64_t GTUOS::handleCall(CPU8080& cpu){
  //cout << "system" << endl;
  uint16_t cycles;

  int c = 0;
  if(1 || (cin && cout)){
    switch(cpu.state->a){
    case PRINT_B:
      cycles = sysPrintB(cpu);
      break;
    case PRINT_MEM:
      cycles = sysPrintMem(cpu);    
      break;
    case READ_B:
      cycles = sysReadB(cpu);
      break;
    case READ_MEM:
      cycles = sysReadMem(cpu);
      break;
    case PRINT_STR:
      cycles = sysPrintStr(cpu);
      break;
    case READ_STR:
      cycles = sysReadStr(cpu);
      break;
    case LOAD_EXEC:
      cycles = loadExec(cpu);
      break;
    case PROCESS_EXIT:
      cycles = processExit(cpu);
      break;
    case SET_QUANTUM:
	    break;
    case PRINT_WHOLE:
      cycles = printWhole(cpu);
      break;
    case RAND_INT:
      cycles = randInt(cpu);
      break;
    case WAIT:
      cycles = wait(cpu);
      break;
    case SIGNAL:
      cycles = signal(cpu);
      break;
    case SEND:
      cycles = send(cpu);
      break;
    case READ:
      cycles = read(cpu);
      break;
     case GETCOND:
      cycles = getcond(cpu);
      break;
    case 11:      
      for(int i = 0 ; i < 0x10000 ; ++i){
          if(i % 32 == 0){
            printf("\n");
            printf("%d - ",c++);
          }
          printf("%3d ",cpu.memory->at(i));
      }
          printf("\n");
          break;
    default:
      break;
    }
  }else{
    cout << "Not found input.txt or output.txt" << endl;
  }
  return cycles;
}
/*
 * Called when scheduling happened
 * Prints PID,PC,MEM_BASE Content and Address,SP
 **/
uint64_t GTUOS::printWhole(CPU8080& cpu){
  ofstream outMailbox,outLocal;
  uint16_t lastProcess = 0x00cffe;
  uint16_t ptableStart = 0x0d000;
  uint16_t ptableLen = 0x00100;

  uint16_t ptableAddr = ptableStart;
  uint8_t pid = cpu.memory->at(lastProcess);

  uint8_t tempPid = pid;
  while(tempPid > 0){
    ptableAddr += ptableLen;
    tempPid--;
  }
  uint16_t pc = (cpu.memory->at(ptableAddr + 10) << 8) | cpu.memory->at(ptableAddr + 9);
  uint16_t sp = (cpu.memory->at(ptableAddr + 8) << 8) | cpu.memory->at(ptableAddr + 7);
  uint16_t base = (cpu.memory->at(ptableAddr + 12) << 8) | cpu.memory->at(ptableAddr + 11);
  uint16_t baseContent = base + pc;
  uint16_t processNameAddr = (cpu.memory->at(ptableAddr + 17) << 8) | cpu.memory->at(ptableAddr + 16);
  
  cout << "\n----Context Scheduling----" << endl;
  printf("PID   :\t%4d | ",  pid);
  printf("PNAME    : ");

  while(cpu.memory->at(processNameAddr) != (uint8_t) 0){
    cout << cpu.memory->at(processNameAddr);
    processNameAddr++;
  }
  printf("\t| ");

  printf("PC    :\t%4x | ",  pc);
  printf("SP    :\t%4x | ",  sp);
  printf("BASE  :\t%4x | ",  base);
  printf("BASE_CONTENT  :\t%4x\n",  baseContent);
  cout << endl;

  
  outMailbox.open("mailbox.txt", ios_base::app);
  //cout<< "Mailbox content: ";
  for(int i=0; i<53;i++){
    outMailbox << (int)cpu.memory->physicalAt(mutexAddr+i) << " " ;
    //cout << (int)cpu.memory->physicalAt(mutexAddr+i+3) << " " ; 
  }
  outMailbox<< endl<< endl;
  //cout<< endl;
  outMailbox.close();

  outLocal.open("locallist.txt", ios_base::app);
  //cout<< "LocalList content: ";
  for(int i=0; i<localArray.size();i++){
    outLocal << localArray[i] << " " ;
    //cout << localArray[i] << " " ;
  }
  outLocal<< endl<< endl;
  //cout<< endl<< endl;
  outLocal.close();
  return CYCLE;
}

/*
 * Called to set the quantum time of scheduling
 */
uint64_t GTUOS::setQuantum(CPU8080& cpu){
  uint8_t quantum = cpu.state->b;
  cpu.setQuantum(quantum);
  return 0;
}

/*
 * Caleld when a process wants to exit
 * Removes process from the memory,
 * process table and reduces number of processes
 * by one
 */
uint64_t GTUOS::processExit(CPU8080& cpu){

  uint16_t baseProcessAddr = 0x00500;
  uint16_t processLen = 512;

  uint16_t ptableStart = 0x0d000;
  uint16_t ptableLen = 256;

  uint16_t ptableAddr = ptableStart;
  
  uint16_t currProcessAddr = ((Memory*)(cpu.memory))->getBaseRegister();
  uint16_t tempProcessAddr = currProcessAddr;

  uint16_t schedulerAddr = 0x00040;
  
  int i = 0;
  while(tempProcessAddr != baseProcessAddr){
    tempProcessAddr -=processLen;
    i++;
  }

  while(i > 0){
    ptableAddr += ptableLen;
    i--;
  }

  uint16_t lastProcess = 0x0cffe;
  uint16_t processCount = 0x0cfff;
  
  ((Memory*)(cpu.memory))->setBaseRegister(0);
  memset(&cpu.memory->at(currProcessAddr),0,processLen);
  memset(&cpu.memory->at(ptableAddr),0,ptableLen);

  //cpu.memory->at(lastProcess) = cpu.memory->at(lastProcess) - 1;
  cpu.memory->at(processCount) = cpu.memory->at(processCount) - 1;
  cpu.state->pc = schedulerAddr; //go back to scheduler
  return (CYCLE * 8);
}

/*
 * Loads a new process to the given address 
 **/
uint64_t GTUOS::loadExec(CPU8080& cpu){
  uint16_t fileNameAddr;
  uint16_t processStartAddr;
  
  char* fileName = (char* ) malloc(sizeof(uint8_t) * 64);

  fileNameAddr = ((uint16_t)cpu.state->b << 8) | cpu.state->c;

  int i = 0;
  while(cpu.memory->at(fileNameAddr) != (uint8_t) 0){
    fileName[i] = cpu.memory->at(fileNameAddr);
    fileNameAddr++;
    i++;
  }
  fileName[i] = 0;

  processStartAddr = ((uint16_t)cpu.state->h << 8) | cpu.state->l;
  
  cpu.ReadFileIntoMemoryAt(fileName, processStartAddr);
  //writeContentsOfMemoryToFile(cpu);
  return (CYCLE * 10);
}

/*
  Print the contents of register B
*/
uint64_t GTUOS::sysPrintB( CPU8080& cpu){
  //cout << "-";
  
  cout << (int)cpu.state->b;
  //printf("%d",(int)cpu.state->b);
  return CYCLE;
}
/*
  Print the content of memory pointed by B and C
  Calculate start address first then get the block
*/
uint64_t GTUOS::sysPrintMem( CPU8080& cpu){
  uint16_t fileNameAddr;
  fileNameAddr = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
  cout << (int)cpu.memory->at(fileNameAddr);
  //printf("%02x\n",(int)cpu.memory->at(fileNameAddr));
  return CYCLE;
}

/*
  Read an integer and put it to B
*/
uint64_t GTUOS::sysReadB( CPU8080& cpu){
  uint16_t num;
  cin >> num;
  cpu.state->b = num;
    
  return CYCLE;
}

/*
  Read an integer and put it to memory address pointed
  by B and C
*/
uint64_t GTUOS::sysReadMem( CPU8080& cpu){
    int readVal;
    uint16_t realValue;

    cin >> readVal;
    realValue = (uint16_t) readVal;

    if (readVal < 0 || readVal > 65535){
      cout << "Error! Bigger than 16 bits not accepted" << endl;
    }else{
      uint8_t leastSign = (uint8_t) realValue;
      uint8_t mostSign = (uint8_t) (realValue >> 8);
      cpu.state->c = leastSign;
      cpu.state->b = mostSign;
    }
    return CYCLE;
}

/*
  Print the null terminated string
  whose start address is stored cin B and C
*/
uint64_t GTUOS::sysPrintStr( CPU8080& cpu){
  uint16_t fileNameAddr;
  fileNameAddr = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
  
  while(cpu.memory->at(fileNameAddr) != (uint8_t) 0){
    cout << cpu.memory->at(fileNameAddr);
    fileNameAddr++;
  }
  return CYCLE;
}
/*
  Read string and put it to memory address
  pointed by B and C
*/
uint64_t GTUOS::sysReadStr( CPU8080& cpu){
  uint16_t fileNameAddr;
  string str;
  cin >> str;
  fileNameAddr = (((uint16_t)cpu.state->b) << 8) | cpu.state->c;
  for(int i = 0 ; i < (signed) str.length() ; ++i){
    cpu.memory->at(fileNameAddr) = str[i];
    fileNameAddr++;
  }
  cpu.memory->at(fileNameAddr) = (uint8_t)'\0'; 
  return CYCLE;
}

uint64_t GTUOS::randInt(CPU8080& cpu){
    uint8_t number;
    number = rand() % 256;
    cpu.state->b = number;
  return (CYCLE * 6);
}

uint64_t GTUOS::wait(CPU8080& cpu){
  

  int id = (int)cpu.state->c;
  int condVar = cpu.memory->physicalAt(mutexAddr+id);

  /*cout<< "System call WAIT (mutex=0, full=1, empty=2)"<<endl;
  cout<< "Cond id: "<<id<<endl;
  cout<< "Cond value: "<< condVar<<endl;*/

   if(condVar<=0){
        //cout << "status: busy"<<endl;
        cpu.dispatchScheduler();
   }
   else{
        //cout << "status: empty"<<endl;
        --condVar;
        cpu.memory->physicalAt(mutexAddr+id) = condVar;
        //cout<< "Cond value after -- : "<< (int)cpu.memory->physicalAt(mutexAddr+id)<<endl;
   }
    
  return (CYCLE * 20);

}  

uint64_t GTUOS::signal(CPU8080& cpu){
  int id = (int)cpu.state->c;
  int condVar = cpu.memory->physicalAt(mutexAddr+id);

  /*cout<< "System call SIGNAL(mutex=0, full=1, empty=2)"<<endl;
  cout<< "Cond id: "<<id<<endl;
  cout<< "Cond value: "<< condVar<<endl;*/
  ++condVar;
  cpu.memory->physicalAt(mutexAddr+id) = condVar;
  //cout<< "Cond value after ++ : "<< (int)cpu.memory->physicalAt(mutexAddr+id)<<endl;
  return (CYCLE * 20 );

}

uint64_t GTUOS::send(CPU8080& cpu){
  int randomNumber = cpu.state->b;
  int fullCounter = cpu.memory->physicalAt(fullAddr);
  /*cout<< "SEND"<< endl;
  cout<< "Random: " << randomNumber<<endl;
  cout<< "Full counter: "<< fullCounter<< endl;*/
  cpu.memory->physicalAt(mutexAddr+fullCounter+3) = randomNumber;
  return 0;

}
uint64_t GTUOS::read(CPU8080& cpu){
  int fullCounter = cpu.memory->physicalAt(fullAddr);
  int randomNumber =cpu.memory->physicalAt(mutexAddr+fullCounter+3);
  localArray.push_back(randomNumber);
  /*cout<< "READ"<< endl;
  cout<< "Random: " << randomNumber<<endl;
  cout<< "Full counter: " << fullCounter<< endl;*/
  cpu.memory->physicalAt(mutexAddr+fullCounter+3) = 0;
  
  return 0;
}

uint64_t GTUOS::getcond(CPU8080& cpu){
  uint8_t id = cpu.state->c;
  /*cout<< "getcond: ";
  cout<< " condID = " << (int)id ;
  cout<< ", value = " << (int)cpu.memory->physicalAt(mutexAddr+id) << endl;
  cpu.state->c = cpu.memory->physicalAt(mutexAddr+id);*/
  return 0;
}


void GTUOS::writeContentsOfMemoryToFile(const CPU8080 & cpu){
    ofstream output;
    char str[3];
    output.open("MicroKernel.mem");
    int i=0;
    for(i=0;i<4096;++i){
        output<<i*16<<" ";
        for(int j=0;j<16;++j) {
            sprintf(str,"%02x",cpu.memory->at(i*16+j));
            output << str << " ";
        }
        output<<"\n";
        
    }
    output.close();
}
