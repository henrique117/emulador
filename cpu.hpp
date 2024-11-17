#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>
#include <cstdint>

using Byte = uint8_t;       // byte (8 bits)
using Word = uint16_t;      // word = 2 bytes (16 bits)

class Memory {

    private:
        static constexpr size_t MEM_SIZE = 512;         // memory size
        Word memory[MEM_SIZE];                          // memory array

    public:
        Memory() {                                      // boot memory with all 0
            for(size_t i = 0; i < MEM_SIZE; i++) {
                memory[i] = 0;
            }
        }

        void LOAD_PROGRAM(const Word* program, size_t size, size_t offset = 0) {    // load the program from main function
            if(offset + size <= MEM_SIZE) {
                for (size_t i = 0; i < size; i++) {
                    memory[offset + i] = program[i];
                }
            } else {
                throw std::out_of_range("The program reaches adresses out of the memory range!");
            }
        }

        Word FETCH_BYTE(size_t adress) const {      // fetch instructions from the memory
            if (adress < MEM_SIZE) {
                return memory[adress];
            } else {
                throw std::out_of_range("Adress out of the memory range!");
            }
        }

        void STORE_BYTE(size_t adress, Word value) {    // store bytes in memory if needed
            if (adress < MEM_SIZE) {
                memory[adress] = value;
            } else {
                throw std::out_of_range("Adress out of the memory range!");
            }
        }

};

class CPU {

    private:
        Byte PC;        // program counter
        Word A, X, Y;   // registers | GPR: X, Y | A: For saving results
        Memory* memory; // alocate a memory for the CPU

    public:
        CPU(Memory* mem) : PC(0x00), A(0), X(0), Y(0), memory(mem) {}   // constructor of the CPU | initialize all the registers as 0 | point the PC to the first adress

        void LOAD_REG_X() {     // load the X register
            this->X = this->DECODE_VALUE();
        }

        void LOAD_REG_Y() {     // load the Y register
            this->Y = this->DECODE_VALUE();
        }

        void ADD_TO_X() {       // add to the X register
            this->X += this->DECODE_VALUE();
        }

        void ADD_TO_Y() {       // add to the Y register
            this->Y += this->DECODE_VALUE();
        }

        void SUM_X_Y() {        // sum the X reg with the Y reg and then store the result in A register
            this->A = this->X + this->Y;
        }

        void PRINT_REGISTERS_VALUES() {     // check the registers values
            std::cout << "RX: " << int(X) << std::endl <<  "RY: " << int(Y) << std::endl <<  "RA: " << int(A) << std::endl;
        }

        Word DECODE_INSTRUCTION() {         // decode the instruction to get the OPCODE (fisrt 8 bits)
            Word instruction = memory->FETCH_BYTE(PC);

            return (instruction & 0xFF00) >> 8;
        }

        Word DECODE_VALUE() {               // decode the instruction to get the value of the instruction (last 8 bits)
            Word instruction = memory->FETCH_BYTE(PC);

            return instruction & 0x00FF;
        }

        void EXECUTE() {                    // execute the instructions pointed by the PC
            Word opcode = this->DECODE_INSTRUCTION();

            switch (opcode) {
                case 0x0001:
                    this->LOAD_REG_X();
                    break;

                case 0x0002:
                    this->LOAD_REG_Y();
                    break;

                case 0x0003:
                    this->ADD_TO_X();
                    break;

                case 0x0004:
                    this->ADD_TO_Y();
                    break;

                case 0x0005:
                    this->SUM_X_Y();
                    break;

                case 0x00FF:
                    std::cout << "Program ended by HALT instruction!" << std::endl;
                    break;
                
                default:
                    throw std::out_of_range("Unknown instruction!");
                    break;
            }
        }

        void RUN(size_t cycles) {           // run the cycles
            for (size_t i = 0; i < cycles; i++) {
                this->EXECUTE();
                this->PC++;
            }
        }

};

#endif