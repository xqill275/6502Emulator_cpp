#include <iostream>
#include "cpu.h"
#include "assembler.h"


int main() {
    cpu testCpu;

    testCpu.loadFile("TestFile.txt");
    int programSize = testCpu.getProgramSize();

    testCpu.run(programSize, true);


    return 0;
}