#pragma once

class Instruction {
public:
    int rd;
    int rs;
    int rt;
    int op;
    int issueClock;
    int execClockBegin;
    int execClockEnd;
    int writebackClock;

    Instruction();
    Instruction(int, int, int, int);
};
