#include <iostream>
#include <cstdint>

using namespace std;

enum Opcodes {
    LOAD = 1,
    ADD = 2,
    SUB = 3,
    HALT = 99
};

class CPU {
    private:
        int8_t A;
        int16_t PC;
        bool running;
        size_t memorySize;
        int8_t* memory;

    public:
        
        CPU(size_t size) : A(0), PC(0), running(true), memorySize(size) {
            memory = new int8_t[memorySize]();
        }

        ~CPU() {
            delete[] memory;
        }

        void loadMemory(const int8_t* program, size_t programSize) {
            if (programSize > memorySize) {
                std::cerr << "Programa excede o tamanho da memória!" << std::endl;
                return;
            }

            for (size_t i = 0; i < programSize; i++) {
                memory[i] = program[i];
            } 
        }

        void executeCycle() {
            if (PC >= memorySize) {
                std::cerr << "PC fora dos limites da memória!" << std::endl;
                running = false;
                return;
            }

            int8_t opcode = memory[PC++];

            switch (opcode) {
                case LOAD:
                    A = memory[PC++];
                    break;

                case ADD:
                    A += memory[PC++];
                    break;

                case SUB:
                    A -= memory[PC++];
                    break;

                case HALT:
                    running = false;
                    break;
                
                default:
                    std::cerr << "Intrução desconhecida: " << int(opcode) << std::endl;
                    running = false;
                    break;
            }
        }

        void run() {
            while(running) {
                executeCycle();
            }
        }

        int8_t getRegistredValue() {
            return A;
        }
    
};

int main() {
    
    size_t memorySize = 32;
    int8_t* program = new int8_t[memorySize]();

    program[0] = LOAD;
    program[1] = 10;
    program[2] = ADD;
    program[3] = 5;
    program[4] = SUB;
    program[5] = 3;
    program[6] = HALT;

    CPU cpu(memorySize);

    cpu.loadMemory(program, memorySize);
    cpu.run();

    std::cout << "Resultado no registrador A: " << int(cpu.getRegistredValue()) << std::endl;

    return 0;
}