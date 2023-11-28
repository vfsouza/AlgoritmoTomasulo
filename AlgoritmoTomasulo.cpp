#include <iostream>
#include <iomanip>   
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "ReservationStation.h"
#include "RegisterStatus.h"
#include "Instruction.h"

using namespace std;

const int NumAddReservationStation = 1;
const int NumMultReservationStation = 2;
const int NumDivReservationStation = 2;

enum Operation {
    AddOp = 0,
    SubOp = 1,
    MultOp = 2,
    DivOp = 3,
};

const int AddLatency = 4;
const int MultLatency = 12;
const int DIV_lat = 38;

const int ISSUE_lat = 1;
const int WRITEBACK_lat = 1;

int Clock = 0;

bool Done = true;

int Total_WRITEBACKS = 0;
int currentInst_ISSUE = 0;

const int ZERO_REG = 5000;
const int RegStatusEmpty = 1000;
const int OperandAvailable = 1001;
const int OperandInit = 1002;

int ISSUE(vector<Instruction>& Inst,
          vector<ReservationStation>& ResStat,
          vector<RegisterStatus>& RegStat,
          vector<int>& Register);
void EXECUTE(vector<Instruction>& Inst,
             vector<ReservationStation>& ResStat,
             vector<RegisterStatus>& RegStat,
             vector<int>& Register);
void WRITEBACK(vector<Instruction>& Inst,
               vector<ReservationStation>& ResStat,
               vector<RegisterStatus>& RegStat,
               vector<int>& Register);

Instruction parseLine(string line);
void printRegisterStatus(vector<RegisterStatus> );
void printReservationStations(vector<ReservationStation> );
void printRegisters(vector<int> );
void printInstructions(vector<Instruction> );
void printTimingTable(vector<Instruction> );

int main(int argc, char* argv[])
{
    string arq = "instrucoes.txt";
    ifstream file(arq);

    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo " << arq << endl;
        return 1;
    }

    string instruction;
    
    vector<Instruction> inst = {};
    
    while (getline(file, instruction)) {
        Instruction newInst = parseLine(instruction);
        inst.push_back(newInst);
    }
    
    ReservationStation ADD1(AddOp, OperandInit);
    ReservationStation MULT1(MultOp, OperandInit), MULT2(MultOp, OperandInit);
    ReservationStation DIV1(DivOp, OperandInit), DIV2(DivOp, OperandInit);

    vector<ReservationStation> ResStation = {ADD1, MULT1, MULT2, DIV1, DIV2};

    RegisterStatus F0(RegStatusEmpty), F1(RegStatusEmpty),
                   F2(RegStatusEmpty), F3(RegStatusEmpty),
                   F4(RegStatusEmpty), F5(RegStatusEmpty),
                   F6(RegStatusEmpty), F7(RegStatusEmpty),
                   F8(RegStatusEmpty), F9(RegStatusEmpty),
                   F10(RegStatusEmpty), F11(RegStatusEmpty),
                   F12(RegStatusEmpty);

    vector<RegisterStatus> RegisterStatus = {F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12};

    vector<int> Register = {ZERO_REG, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    cout << "INITIAL VALUES:" << endl;
    printInstructions(inst);
    printReservationStations(ResStation);
    printRegisters(Register);
    printRegisterStatus(RegisterStatus);
    cout << endl;

    do {
        Clock++;

        ISSUE(inst, ResStation, RegisterStatus, Register);
        EXECUTE(inst, ResStation, RegisterStatus, Register);
        WRITEBACK(inst, ResStation, RegisterStatus, Register);

        printRegisters(Register);
        printTimingTable(inst);
        cout << endl;

        Done = false;

        if (Total_WRITEBACKS == inst.size()) {
            Done = true;
        }
        cout << endl;
    } while (!Done);
    
    return 0;
}

int ISSUE(vector<Instruction>& INST, vector<ReservationStation>& RESSTATION, vector<RegisterStatus>& REGSTATUS, vector<int>& REG) {

    int r = 0;
    bool rsFree = false;

    if (currentInst_ISSUE >= INST.size()) {
        return 0;
    }

    r = INST[currentInst_ISSUE].op;

    int RSAddStart = NumAddReservationStation - NumAddReservationStation;
    int RSAddEnd = NumAddReservationStation;
    int RSSubStart = NumAddReservationStation - NumAddReservationStation;
    int RSSubEnd = NumAddReservationStation;
    int RSMultStart = NumAddReservationStation;
    int RSMultEnd = NumAddReservationStation + NumMultReservationStation;
    int RSDivStart = NumAddReservationStation + NumMultReservationStation;
    int RSDivEnd = NumAddReservationStation + NumMultReservationStation + NumDivReservationStation;

    switch (r) {
        case AddOp:
            for (int i = RSAddStart; i < RSAddEnd; i++) {
                if (!RESSTATION[i].busy) {
                    r = i;
                    currentInst_ISSUE++;
                    RESSTATION[i].op = AddOp;
                    rsFree = true;
                    break;
                }
                
            }

            if (!rsFree) {
                return 1;
            }
            break;
        
        case SubOp:
            for (int i = RSSubStart; i < RSSubEnd; i++) {
                if (!RESSTATION[i].busy) {
                    r = i;
                    currentInst_ISSUE++;
                    RESSTATION[i].op = SubOp;
                    rsFree = true;
                    break;
                }
                
            }

            if (!rsFree) {
                return 1;
            }
            break;
        
        case MultOp:
            for (int i = RSMultStart; i < RSMultEnd; i++) {
                if (!RESSTATION[i].busy) {
                    r = i;
                    currentInst_ISSUE++;
                    RESSTATION[i].op = MultOp;
                    rsFree = true;
                    break;
                }
                    
            }

            if (!rsFree) {
                return 1;
            }
            break;

        case DivOp:
            for (int i = RSDivStart; i < RSDivEnd; i++) {
                if (!RESSTATION[i].busy) {
                    r = i;
                    currentInst_ISSUE++;
                    RESSTATION[i].op = DivOp;
                    rsFree = true;
                    break;
                }
                        
            }

            if (!rsFree) {
                return 1;
            }
            break;

        default:
            break;
    }

    if (REGSTATUS[INST[currentInst_ISSUE - 1].rs].Qi == RegStatusEmpty) {
        RESSTATION[r].Vj = REG[INST[currentInst_ISSUE - 1].rs];
        RESSTATION[r].Qj = OperandAvailable;
    } else {
        RESSTATION[r].Qj = REGSTATUS[INST[currentInst_ISSUE - 1].rs].Qi;
    }

    if(REGSTATUS[INST[currentInst_ISSUE - 1].rt].Qi == RegStatusEmpty){
        RESSTATION[r].Vk = REG[INST[currentInst_ISSUE - 1].rt];
        RESSTATION[r].Qk = OperandAvailable;
    }
    else{
        RESSTATION[r].Qk = REGSTATUS[INST[currentInst_ISSUE - 1].rt].Qi;
    }

    RESSTATION[r].busy = true;
    RESSTATION[r].issueLatency = 0;

    RESSTATION[r].instNum = currentInst_ISSUE - 1;

    INST[currentInst_ISSUE - 1].issueClock = Clock;

    REGSTATUS[INST[currentInst_ISSUE - 1].rd].Qi = r;
    return 2;
}

void EXECUTE(vector<Instruction>& INST, vector<ReservationStation>& RESSTATION, vector<RegisterStatus>& REGSTATUS, vector<int>& REG) {

    for (int r = 0; r < RESSTATION.size(); r++) {
        if (RESSTATION[r].busy == true) {
            if (RESSTATION[r].issueLatency >= ISSUE_lat) {
                if (RESSTATION[r].Qj == OperandAvailable && RESSTATION[r].Qk == OperandAvailable) {
                    if (INST[RESSTATION[r].instNum].execClockBegin == 0) 
                        INST[RESSTATION[r].instNum].execClockBegin = Clock;
                    
                    RESSTATION[r].latency++;

                    switch (RESSTATION[r].op) {
                        case AddOp:
                            if(RESSTATION[r].latency == AddLatency){
                                RESSTATION[r].result = RESSTATION[r].Vj + RESSTATION[r].Vk;
                                
                                RESSTATION[r].completed = true;
                                RESSTATION[r].latency = 0;
                                
                                INST[RESSTATION[r].instNum].execClockEnd = Clock;
                                
                                RESSTATION[r].issueLatency = 0;
                            }
                            break;
                        case SubOp:
                            if(RESSTATION[r].latency == AddLatency){
                                RESSTATION[r].result = RESSTATION[r].Vj - RESSTATION[r].Vk;
                                RESSTATION[r].completed = true;
                                RESSTATION[r].latency = 0;
                                
                                INST[RESSTATION[r].instNum].execClockEnd = Clock;
                                
                                RESSTATION[r].issueLatency = 0;
                            }
                        case MultOp:
                            if(RESSTATION[r].latency == MultLatency){
                                RESSTATION[r].result = RESSTATION[r].Vj * RESSTATION[r].Vk;
                                RESSTATION[r].completed = true;
                                RESSTATION[r].latency = 0;
                                
                                INST[RESSTATION[r].instNum].execClockEnd = Clock;
                                
                                RESSTATION[r].issueLatency = 0;
                            }
                        case DivOp:
                            if(RESSTATION[r].latency == DIV_lat){
                                RESSTATION[r].result = RESSTATION[r].Vj / RESSTATION[r].Vk;
                                RESSTATION[r].completed = true;
                                RESSTATION[r].latency = 0;
                                
                                INST[RESSTATION[r].instNum].execClockEnd = Clock;
                                
                                RESSTATION[r].issueLatency = 0;
                            }
                        default:
                            break;
                    }
                }
            }
            else RESSTATION[r].issueLatency++;
        }
    }
}

void WRITEBACK(vector<Instruction>& INST, vector<ReservationStation>& RESSTATION, vector<RegisterStatus>& REGSTATUS, vector<int>& REG){
    
    for(int r=0;r<RESSTATION.size();r++){
        if(RESSTATION[r].completed){
            if(RESSTATION[r].writebackLatency == WRITEBACK_lat){
                if(INST[RESSTATION[r].instNum].writebackClock == 0)
                    INST[RESSTATION[r].instNum].writebackClock = Clock;
                
                for(int x=0;x < REG.size();x++) {
                    if (REGSTATUS[x].Qi == r) {
                        REG[x] = RESSTATION[r].result;
                        REGSTATUS[x].Qi = RegStatusEmpty;
                    }
                }
                
                for(int y=0;y<RESSTATION.size();y++){
                    if(RESSTATION[y].Qj==r){
                        RESSTATION[y].Vj=RESSTATION[r].result;
                        RESSTATION[y].Qj=OperandAvailable;
                    }
                    if(RESSTATION[y].Qk==r){
                        RESSTATION[y].Vk=RESSTATION[r].result;
                        RESSTATION[y].Qk=OperandAvailable;
                    }
                }
                
                RESSTATION[r].completed = false;
                RESSTATION[r].busy = false;
                RESSTATION[r].Qj = OperandInit;
                RESSTATION[r].Qk = OperandInit;
                RESSTATION[r].Vj = 0;
                RESSTATION[r].Vk = 0;
                RESSTATION[r].writebackLatency = 0;
                Total_WRITEBACKS++;
            }
            else
                RESSTATION[r].writebackLatency++;
        }
    }
}

void printRegisterStatus(vector<RegisterStatus> RegisterStatusVector){
    cout << "Register Status: " << endl;
    for(int i=0; i < RegisterStatusVector.size(); i++)
        cout << RegisterStatusVector[i].Qi << ' ';
    cout << endl;
}
void printReservationStations(vector<ReservationStation> RSV){
    for(int i = 0; i < RSV.size(); i++)
        cout << "RS #: " << i << "  Busy: " << RSV[i].busy << "  op: "<<
                RSV[i].op << "  Vj: " << RSV[i].Vj << "  Vk: " <<
                RSV[i].Vk << "  Qj: " << RSV[i].Qj << "  Qk: " <<
                RSV[i].Qk << endl;
}
void printRegisters(vector<int> RegistersVector){
    cout << "Register Content:" << endl;
    for(int i = 0; i < RegistersVector.size(); i++)
        cout << RegistersVector[i] << ' ';
    cout << endl;
}
void printInstructions(vector<Instruction> IV){
    for(int i=0; i<IV.size(); i++)
        cout << "Instruction #: " << i << "  Operation: " <<
                IV[i].op << "  " <<
                IV[i].rd << " <- " << IV[i].rs << " op " <<
                IV[i].rt << endl;
}
void printTimingTable(vector<Instruction> INST){
    string separator = "      ";
    cout << "Inst" << separator;
    cout << "Issue" << separator;
    cout << "Execute" << separator;
    cout << "WB" << separator;
    cout << "SystemClock" << endl;
    cout << "                                          " <<  Clock;
    cout << endl;
    cout << separator;
    cout << endl;
    // Define Row Labels and values
    for(int i = 0; i < INST.size(); i++){
        cout << i  << "         ";
        if (INST[i].issueClock < 10) {
            cout << INST[i].issueClock  << "          ";
            cout << INST[i].execClockBegin <<  "-";
            cout << INST[i].execClockEnd << "          ";
            cout << INST[i].writebackClock << "          ";
        } else {
            cout << INST[i].issueClock  << "         ";
            cout << INST[i].execClockBegin <<  "-";
            cout << INST[i].execClockEnd << "        ";
            cout << INST[i].writebackClock << "        ";
        }
        cout << endl;
    }
}

Instruction parseLine(string line) {
    std::istringstream iss(line);
    std::string op, f1, f2, f3;

    iss >> op >> f1 >> f2 >> f3;

    int operand1 = std::stoi(f1.substr(1));
    int operand2 = std::stoi(f2.substr(1));
    int operand3 = std::stoi(f3.substr(1));

    Operation operation;
    if (op == "ADD") {
        operation = AddOp;
    } else if (op == "SUB") {
        operation = SubOp;
    } else if (op == "MULT") {
        operation = MultOp;
    } else if (op == "DIV") {
        operation = MultOp;
    }

    return Instruction(operand1, operand2, operand3, operation);
}

