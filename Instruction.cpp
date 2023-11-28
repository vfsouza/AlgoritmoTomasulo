#include "Instruction.h"

Instruction::Instruction() {
    rd = 0;
    rs = 0;
    rt = 0;
    op = 0;
    issueClock = 0;
    execClockBegin = 0;
    execClockEnd = 0;
    writebackClock = 0;
}

Instruction::Instruction(int RD, int RS, int RT, int OP) {
    rd = RD;
    rs = RS;
    rt = RT;
    op = OP;
    issueClock = 0;
    execClockBegin = 0;
    execClockEnd = 0;
    writebackClock = 0;
}
