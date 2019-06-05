#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"


/* author Gulzada IISAEVA */

GTUOS::GTUOS( CPU8080 & cpu){
    uint16_t adress;
    process mainProcess;
    mainProcess.id=(int)counterId;
    mainProcess.name="init";
    counterId++;
    mainProcess.adress=0;
    mainProcess.mem_address=tableAddress;
    mainProcess.stack_address=61440;
    processTable.insert(processTable.begin(),mainProcess);
    create_main_table(cpu, tableAddress);
    //print(cpu);
}
uint64_t GTUOS::handleCall( CPU8080 & cpu){
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
        case 5:
            cycle=load_exec(cpu);
            break;
        case 6:
            cycle=set_quantum(cpu);
            break;
        case 7:
            cycle=read_b(cpu);
            break;
        case 8:
            cycle=read_str(cpu);
            break;
        case 9:
            cycle=process_exit(cpu);
            break;
        case 15:
            handler(cpu);
            break;
        default:
            std::cout <<  "Unimplemented OS call"<<endl;
            break;
	}
	return cycle;
}
uint16_t GTUOS::load_exec( CPU8080 & cpu){
  
    cout<< "System Call LOAD_EXEC"<<endl;
    uint16_t fAddress =(cpu.state->b << 8) | cpu.state->c;
    uint16_t startAddress =(cpu.state->h << 8) | cpu.state->l;
    char filename[255];

    int i=0;
    while( cpu.memory->at(fAddress) != '\0' ){
        filename[i]=cpu.memory->at(fAddress);
        i++;
        ++fAddress;
    }
    filename[i]='\0';
    processCreate(cpu,startAddress,filename,tableAddress+currentTableAdrress,spointer);
    create_process_table(cpu,tableAddress+currentTableAdrress,startAddress);
    currentTableAdrress+=14;
    spointer-=100;
    cpu.ReadFileIntoMemoryAt(filename,startAddress);
    //writeContentsOfMemoryToFile(cpu,"kernel.mem");
    return loadExec;
}
void GTUOS::print(CPU8080 & cpu){
    cout<<"\n************All process ************************\n";
    cout<<"************************************************\n";
    for (int i = 0; i < processTable.size(); ++i)
    {
        cout<< "id       : " << processTable[i].id<<endl;   
        cout<< "name     : " << processTable[i].name<<endl;
        cout<< "pc       : " <<((cpu.memory->physicalAt(processTable[i].mem_address+10) << 8 ) | (cpu.memory->physicalAt(processTable[i].mem_address+9)))<< endl;
        cout<< "address  : " << processTable[i].adress<<endl;
        cout<< "base reg : " << ((cpu.memory->physicalAt(processTable[i].mem_address+12) << 8 ) | (cpu.memory->physicalAt(processTable[i].mem_address+11)))<< endl;
        cout<< "sp       : " <<  ((cpu.memory->physicalAt(processTable[i].mem_address+8) << 8 ) | (cpu.memory->physicalAt(processTable[i].mem_address+7))) << endl;
        cout<<"************************************************\n";
        cout<<"************************************************\n";
        cout<< endl;
    }

    cout<< endl;


}

uint16_t GTUOS::set_quantum(CPU8080 & cpu){
    ofstream outfile;
    outfile.open("output.txt", ios_base::app);
    outfile << "System Call SET_QUANTUM   :   "<<endl;
    uint8_t b = cpu.state->b;
    cpu.setQuantum(b);
    outfile.close();
    cout << "System Call SET_QUANTUM   :   "<<endl;
    return setQuant;
}
uint16_t GTUOS::process_exit(CPU8080 & cpu){
    clearTable(cpu,processTable[processTable.size()-2].mem_address);
    processTable.pop_back();
    return processExit;
}

void GTUOS::clearTable(CPU8080 & cpu,int process_addres){
    cpu.state->a = cpu.memory->physicalAt(process_addres+0);
    cpu.state->b = cpu.memory->physicalAt(process_addres+1);
    cpu.state->c = cpu.memory->physicalAt(process_addres+2);
    cpu.state->d = cpu.memory->physicalAt(process_addres+3);
    cpu.state->e = cpu.memory->physicalAt(process_addres+4);
    cpu.state->h = cpu.memory->physicalAt(process_addres+5);
    cpu.state->l = cpu.memory->physicalAt(process_addres+6);
    cpu.state->sp=(cpu.memory->physicalAt(process_addres+8) << 8 ) | (cpu.memory->physicalAt(process_addres+7));
    cpu.state->pc=(cpu.memory->physicalAt(process_addres+10) << 8 ) | (cpu.memory->physicalAt(process_addres+9));
    ((Memory *) (cpu.memory))->setBaseRegister( ((cpu.memory->physicalAt(process_addres+12) << 8 ) | (cpu.memory->physicalAt(process_addres+11))) );
    *(unsigned char *)&cpu.state->cc = cpu.memory->at( process_addres + 13 );
}  

void GTUOS::copy_table(CPU8080 & cpu,int from, int to){
    cpu.memory->physicalAt(to+0)=cpu.memory->physicalAt(from+0) ;
    cpu.memory->physicalAt(to+1)=cpu.memory->physicalAt(from+1) ;
    cpu.memory->physicalAt(to+2)=cpu.memory->physicalAt(from+2) ;
    cpu.memory->physicalAt(to+3)= cpu.memory->physicalAt(from+3) ;
    cpu.memory->physicalAt(to+4)= cpu.memory->physicalAt(from+4) ;
    cpu.memory->physicalAt(to+5)= cpu.memory->physicalAt(from+5) ;
    cpu.memory->physicalAt(to+6)=cpu.memory->physicalAt(from+6) ;
    cpu.memory->physicalAt(to+8)=cpu.memory->physicalAt(from+8) ;
    cpu.memory->physicalAt(to+7)=cpu.memory->physicalAt(from+7) ;
    cpu.memory->physicalAt(to+10)=cpu.memory->physicalAt(from+10) ;
    cpu.memory->physicalAt(to+9)=cpu.memory->physicalAt(from+9) ;
    cpu.memory->physicalAt(to+12)=cpu.memory->physicalAt(from+12) ;
    cpu.memory->physicalAt(to+11)=cpu.memory->physicalAt(from+11) ;
    cpu.memory->physicalAt(to+13)=cpu.memory->physicalAt(from+13) ;

}  
void GTUOS::create_process_table(CPU8080 & cpu,int process_addres, int baseAddr){\
    cpu.memory->physicalAt(process_addres+0) = 0;
    cpu.memory->physicalAt(process_addres+1) = 0;
    cpu.memory->physicalAt(process_addres+2) = 0;
    cpu.memory->physicalAt(process_addres+3) = 0;
    cpu.memory->physicalAt(process_addres+4) = 0;
    cpu.memory->physicalAt(process_addres+5) = 0;
    cpu.memory->physicalAt(process_addres+6) = 0;

    cpu.memory->physicalAt(process_addres+8) = ((spointer) >> 8) & 0xff;
    cpu.memory->physicalAt(process_addres+7) = ((spointer)  & 0xff);
    
    cpu.memory->physicalAt(process_addres+10) =0;
    cpu.memory->physicalAt(process_addres+9) = 0;

    cpu.memory->physicalAt(process_addres+12) = (baseAddr >> 8) & 0xff;
    cpu.memory->physicalAt(process_addres+11) = (baseAddr & 0xff);
    cpu.memory->physicalAt(process_addres+13) =  cpu.memory->physicalAt(tableAddress+13);

}  
void GTUOS::create_main_table(CPU8080 & cpu,int process_addres){
    uint16_t base = ((Memory *)cpu.memory)->getBaseRegister();
    uint16_t limit = ((Memory *)cpu.memory)->getLimitRegister();
    cpu.memory->physicalAt(process_addres+0) = cpu.state->a;
    cpu.memory->physicalAt(process_addres+1) = cpu.state->b;
    cpu.memory->physicalAt(process_addres+2) = cpu.state->c;
    cpu.memory->physicalAt(process_addres+3) = cpu.state->d;
    cpu.memory->physicalAt(process_addres+4) = cpu.state->e;
    cpu.memory->physicalAt(process_addres+5) = cpu.state->h;
    cpu.memory->physicalAt(process_addres+6) = cpu.state->l;
    cpu.memory->physicalAt(process_addres+8) = (cpu.state->sp >> 8) & 0xff;
    cpu.memory->physicalAt(process_addres+7) = (cpu.state->sp & 0xff);
    cpu.memory->physicalAt(process_addres+10)= (cpu.state->pc >> 8) & 0xff;
    cpu.memory->physicalAt(process_addres+9) = (cpu.state->pc & 0xff);
    cpu.memory->physicalAt(process_addres+12)= (base >> 8) & 0xff;
    cpu.memory->physicalAt(process_addres+11)= (base & 0xff);
    cpu.memory->physicalAt(process_addres+13)= *(unsigned char *)&cpu.state->cc;
} 
void GTUOS::setCPUState(CPU8080 & cpu,int process_addres, uint16_t baseReg){
    
    //cout<<"state sp    :" << cpu.state->sp<< endl;
    int tempSp=cpu.state->sp;
    cpu.state->sp = (cpu.memory->physicalAt(process_addres+8) << 8 ) | (cpu.memory->physicalAt(process_addres+7));
    Save(cpu.memory, cpu.state, cpu.memory->physicalAt(process_addres+1), cpu.memory->physicalAt(process_addres+2));
    Save(cpu.memory, cpu.state, cpu.memory->physicalAt(process_addres+3), cpu.memory->physicalAt(process_addres+4));
    Save(cpu.memory, cpu.state, cpu.memory->physicalAt(process_addres+5), cpu.memory->physicalAt(process_addres+6));
    Save(cpu.memory, cpu.state, cpu.memory->physicalAt(process_addres+0), cpu.memory->physicalAt(process_addres+13));
    cpu.memory->physicalAt(process_addres+8) = ((cpu.state->sp) >> 8) & 0xff;
    cpu.memory->physicalAt(process_addres+7) = ((cpu.state->sp)  & 0xff);
    cpu.state->sp =tempSp;

    cpu.memory->physicalAt(tempStack+1) =cpu.memory->physicalAt(process_addres+8) ;
    cpu.memory->physicalAt(tempStack) = cpu.memory->physicalAt(process_addres+7);

    cpu.memory->physicalAt(tempBase+1) = cpu.memory->physicalAt(process_addres+12) ;
    cpu.memory->physicalAt(tempBase) = cpu.memory->physicalAt(process_addres+11);

    cpu.memory->physicalAt(tempPC+1) = cpu.memory->physicalAt(process_addres+10);
    cpu.memory->physicalAt(tempPC) = cpu.memory->physicalAt(process_addres+9);

   

    /*cout<<"from which adres    :" << process_addres<< endl;
    cout<<"state sp    :" << cpu.state->sp<< endl;
    cout<<"pc in process table :"<<((cpu.memory->physicalAt(process_addres+10) << 8 ) | (cpu.memory->physicalAt(process_addres+9)))<<   endl;
    cout<<"assemly stack       :"<<((cpu.memory->physicalAt(tempStack+1) << 8 ) | (cpu.memory->physicalAt(tempStack)))<< endl;
    cout<<"assemly pc          : "<<((cpu.memory->physicalAt(tempPC+1) << 8 ) | (cpu.memory->physicalAt(tempPC))) << endl;
    cout<<"assemly base        :"<<((cpu.memory->physicalAt(65514+1) << 8 ) | (cpu.memory->physicalAt(65514)))<<  endl;
    */
}   

void GTUOS::handler(CPU8080 & cpu){
    if(processTable.size()>1){
        printSwitch(cpu);
        copy_table(cpu,cpu.int_buffer,processTable[processTable.size()-1].mem_address);
        setCPUState(cpu,processTable[processTable.size()-2].mem_address,processTable[processTable.size()-2].mem_address);
        processTable.insert(processTable.begin(),processTable[processTable.size()-1]);
        processTable.pop_back();
       
    }
    else if(processTable.size()==1){
        *cpu.lastOpcode = 0x76;
    }
}

void GTUOS::Save(MemoryBase *memory, State8080 *state, uint8_t high, uint8_t low) {
  WriteMem(memory, state->sp - 1, high);
  WriteMem(memory, state->sp - 2, low);
  state->sp = state->sp - 2;
 
}

void GTUOS::WriteMem(MemoryBase *memory, uint16_t address, uint8_t value) {
    memory->at(address) = value;
}

void GTUOS::Read(MemoryBase *memory, State8080 *state, uint8_t *high, uint8_t *low) {
      *low = memory->at(state->sp);
      *high = memory->at(state->sp + 1);
      state->sp += 2;
}
void GTUOS::setPC(uint16_t paramPC,CPU8080 & cpu){
    cpu.state->pc=paramPC;
}
uint16_t GTUOS::print_b(const CPU8080 & cpu){
    ofstream outfile;
    outfile.open("output.txt", ios_base::app);
	int b=int(cpu.state->b);
	cout<< b<<endl;
    outfile b<<endl;
    outfile.close();
	return printBcycle;
}
uint16_t GTUOS::print_mem(const CPU8080 & cpu){
    ofstream outfile;
    outfile.open("output.txt", ios_base::app);
	uint16_t addr = cpu.state->b;
    addr = (addr<<8) | cpu.state->c;

    cout<<int(cpu.memory->at(addr))<<endl;
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
void GTUOS::processCreate(CPU8080 & cpu,uint16_t SpAddress , char *name, int memAddress,int stack){
    process newProcess;
    newProcess.name=name;
    newProcess.id=(int)counterId;
    newProcess.adress=SpAddress;
    newProcess.mem_address=memAddress;
    newProcess.stack_address=stack;
    processTable.insert(processTable.begin(),newProcess);
    counterId++;
}
void GTUOS::writeContentsOfMemoryToFile(const CPU8080 & cpu,const string filename){
    ofstream output;
    char str[3];
    output.open(filename.c_str());
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


void GTUOS::printSwitch(CPU8080 & cpu){
    
    cout<<"\n************Current process **********************************************************************************\n";
    cout<<"*****************************************************************************************************************\n";
    
    int index=processTable.size()-1;
    cout<< "id: " << processTable[index].id<<" | ";   
    cout<< "name: " << processTable[index].name<<" | ";
    cout<< "pc: " <<((cpu.memory->physicalAt(processTable[index].mem_address+10) << 8 ) | (cpu.memory->physicalAt(processTable[index].mem_address+9)))<< " | ";
    cout<< "address: " << processTable[index].adress<<" | ";
    cout<< "base reg: " << ((cpu.memory->physicalAt(processTable[index].mem_address+12) << 8 ) | (cpu.memory->physicalAt(processTable[index].mem_address+11)))<< " | ";
    cout<< "sp: " <<  ((cpu.memory->physicalAt(processTable[index].mem_address+8) << 8 ) | (cpu.memory->physicalAt(processTable[index].mem_address+7))) << endl;
    cout<<"*****************************************************************************************************************\n";
    cout<< endl;
    cout<<"\n************Next process **************************************************************************************\n";
    cout<<"*****************************************************************************************************************\n";
    
    index=processTable.size()-2;
    cout<< "id: " << processTable[index].id<<" | ";   
    cout<< "name: " << processTable[index].name<<" | ";
    cout<< "pc: " <<((cpu.memory->physicalAt(processTable[index].mem_address+10) << 8 ) | (cpu.memory->physicalAt(processTable[index].mem_address+9)))<< " | ";
    cout<< "address: " << processTable[index].adress<<" | ";
    cout<< "base reg: " << ((cpu.memory->physicalAt(processTable[index].mem_address+12) << 8 ) | (cpu.memory->physicalAt(processTable[index].mem_address+11)))<< " | ";
    cout<< "sp: " <<  ((cpu.memory->physicalAt(processTable[index].mem_address+8) << 8 ) | (cpu.memory->physicalAt(processTable[index].mem_address+7))) << endl;
    cout<<"****************************************************************************************************************\n";
    cout<< endl;


}


