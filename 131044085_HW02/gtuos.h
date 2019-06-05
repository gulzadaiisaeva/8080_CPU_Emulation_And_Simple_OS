#ifndef H_GTUOS
#define H_GTUOS

/* author Gulzada IISAEVA */

#include "8080emuCPP.h"
#include "8080emuCPP.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include<vector>
#include "memory.h"


#define printBcycle 10
#define printMemCycle 10
#define readBcycle 10
#define readMemCycle 10
#define loadExec 100
#define setQuant 7
#define processExit 80


using namespace std;

typedef struct process {
	string name;
	uint64_t id;
	uint32_t adress;
	int mem_address;
	int stack_address;

} process;


class GTUOS{
	public:
		GTUOS( CPU8080 & cpu);
		uint64_t handleCall( CPU8080 & cpu);
		uint16_t print_b(const CPU8080 & cpu);
	    uint16_t print_mem(const CPU8080 & cpu);
	    uint16_t read_b(const CPU8080 & cpu);
	    uint16_t read_mem(const CPU8080 & cpu);
	    uint16_t print_str(const CPU8080 & cpu);
	    uint16_t read_str(const CPU8080 & cpu);
	    uint16_t load_exec( CPU8080 & cpu);
	    uint16_t set_quantum(CPU8080 & cpu);
	    uint16_t process_exit(CPU8080 & cpu);

	    void print(CPU8080 & cpu);
	    void copy_table(CPU8080 & cpu,int from, int to);
	    void create_main_table(CPU8080 & cpu,int process_addres);
	    void create_process_table(CPU8080 & cpu,int process_addres,int baseAddr);
		void setCPUState(CPU8080 & cpu,int process_addres, uint16_t baseReg);
		void Read(MemoryBase *memory, State8080 *state, uint8_t *high, uint8_t *low) ;
		void Save(MemoryBase *memory, State8080 *state, uint8_t high, uint8_t low);
		void WriteMem(MemoryBase *memory, uint16_t address, uint8_t value);
		void clearTable(CPU8080 & cpu,int process_addres);

	    void processCreate(CPU8080 & cpu,uint16_t SpAddress,char *name,int memAddress,int stack);
	    void setPC(uint16_t paramPC,CPU8080 & cpu);
	    void switchInfo(process oldP, process newP);
	    void writeContentsOfMemoryToFile(const CPU8080 & cpu,const string filename);
	    void handler(CPU8080 & cpu);
	    void printSwitch(CPU8080 & cpu);

	    vector<process> processTable;
	    int tableAddress=64000;
	    int currentTableAdrress=14;
	    uint8_t counterId=0;
	    int spointer=25000;
	    int currentSpointer=600;
	    int tempStack=65504;  //FFE0 
	    int tempBase= 65514;  //FFEA d base
	    int tempPC =  65524;  //FFF4 h

};

#endif

