#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"

/* author Gulzada IISAEVA */

uint64_t GTUOS::handleCall(const CPU8080 & cpu){
	uint64_t cycle=0;
	switch (cpu.state->a){
        case 1:
            cycle=print_str(cpu);
            break;
        case 2:
            cycle=read_mem(cpu);
            break;
        case 3:
            cycle=print_mem(cpu);
            break;
        case 4:
            cycle=print_b(cpu);
            break;
        case 7:
            cycle=read_b(cpu);
            break;
        case 8:
            cycle=read_str(cpu);
            break;
        default:
            std::cout <<  "Unimplemented OS call"<<endl;
            break;
	}
	return cycle;
}


uint16_t GTUOS::print_b(const CPU8080 & cpu){
    ofstream outfile;
    outfile.open("output.txt", ios_base::app);
	int b=int(cpu.state->b);
	cout<< "System Call PRINT_B   :   " <<b<<endl;
    outfile << "System Call PRINT_B   :   " <<b<<endl;
    outfile.close();
	return printBcycle;

}
uint16_t GTUOS::print_mem(const CPU8080 & cpu){
    ofstream outfile;
    outfile.open("output.txt", ios_base::app);
	uint16_t addr = cpu.state->b;
    addr = (addr<<8) | cpu.state->c;

    cout<< "System Call PRINT_MEM :   " ;
    cout<<int(cpu.memory->at(addr))<<endl;
    outfile<< "System Call PRINT_MEM :   " ;
    outfile<<int(cpu.memory->at(addr))<<endl;
    outfile.close();
	return printMemCycle;
}
uint16_t GTUOS::read_b(const CPU8080 & cpu){
    ofstream outfile;
    outfile.open("output.txt", ios_base::app);
	int b;
    cout<< "Enter a value for B: " <<endl;
    outfile<< "Enter a value for B: " <<endl;
	cin>>b;
	cpu.state->b = b;
    outfile.close();
	return readBcycle;
}
uint16_t GTUOS::read_mem(const CPU8080 & cpu){
    ofstream outfile;
    outfile.open("output.txt", ios_base::app);
	int value;
    uint16_t addr;
    outfile<< "Enter a value for BC: " <<endl;
   	cout<< "Enter a value for BC: " <<endl;
    cin>>value;
    addr = (cpu.state->b << 8) | cpu.state->c;
    cpu.memory->at(addr) = value;
    outfile.close();
	return readMemCycle;
}
uint16_t GTUOS::print_str(const CPU8080 & cpu){
    ofstream outfile;
    outfile.open("output.txt", ios_base::app);
	uint16_t addr = (cpu.state->b << 8) | cpu.state->c;
    uint16_t printStrCycle = 0;
	addr = (cpu.state->b << 8) | cpu.state->c;
    outfile<< "System Call PRINT_STR :\n" ;
    cout<< "System Call PRINT_STR :\n" ;
    while( cpu.memory->at(addr) != '\0' ){
        cout << cpu.memory->at(addr) ;
        ++addr;
        ++printStrCycle;
    }
	outfile.close();
	return printStrCycle*10;
}
uint16_t GTUOS::read_str(const CPU8080 & cpu){
    ofstream outfile;
    outfile.open("output.txt", ios_base::app);
	string value;
	uint16_t addr = cpu.state->b;
    uint16_t readStrCycle;
	addr = (addr<<8) | cpu.state->c;
    outfile<< "System Call READ_STR"<<endl;
	cout<< "System Call READ_STR"<<endl;
    cin>> value;

    for(int i=0;i<value.size();++i )
    {
        cpu.memory->at(addr)=value[i];
        ++addr;
	}
    readStrCycle= value.size()*10;
    outfile.close();
	return readStrCycle;
}
