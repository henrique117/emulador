#include <iostream>
#include "cpu.hpp"

int main() {
    Memory mem;

    Word program[] = {0x01FF, 0x020A, 0x03FF, 0x0500, 0xFF00};
    size_t programSize = sizeof(program) / sizeof(Byte);

    mem.LOAD_PROGRAM(program, programSize);

    CPU cpu(&mem);

    cpu.RUN(5);
    cpu.PRINT_REGISTERS_VALUES();

    return 0;
}