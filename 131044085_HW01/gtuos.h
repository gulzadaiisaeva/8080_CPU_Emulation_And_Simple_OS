#ifndef H_GTUOS
#define H_GTUOS

/* author Gulzada IISAEVA */

#include "8080emuCPP.h"
#include "8080emuCPP.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#define printBcycle 10
#define printMemCycle 10
#define readBcycle 10
#define readMemCycle 10


using namespace std;



class GTUOS{
	public:
		uint64_t handleCall(const CPU8080 & cpu);
		uint16_t print_b(const CPU8080 & cpu);
	    uint16_t print_mem(const CPU8080 & cpu);
	    uint16_t read_b(const CPU8080 & cpu);
	    uint16_t read_mem(const CPU8080 & cpu);
	    uint16_t print_str(const CPU8080 & cpu);
	    uint16_t read_str(const CPU8080 & cpu);
};

#endif
