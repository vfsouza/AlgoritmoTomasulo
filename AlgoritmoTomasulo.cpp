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

// Numero de Reservation Stations de ADD
const int NumAddReservationStation = 1;
// Numero de Reservation Stations de Mult
const int NumMultReservationStation = 2;
// Numero de Reservation Stations de Div
const int NumDivReservationStation = 2;

// Numero das Operacoes
enum Operation {
    AddOp = 0,
    SubOp = 1,
    MultOp = 2,
    DivOp = 3,
};

// Latencia da op de ADD
const int AddLatency = 4;
// Latencia da op de MULT
const int MultLatency = 12;
// Latencia da op de DIV
const int DivLatency = 38;
// Lantecia da ISSUE
const int IssueLatency = 1;
// Latencia de WRITEBACK
const int WritebackLatency = 1;

// Clock total
int Clock = 0;

// Condicao de teste completo
bool Completed = true;

// Total de writebacks efetuados
int TotalWritebacks = 0;
// Issue da instrucao que esta sendo feita
int CurrentInstIssue = 0;

// Registrador zero
const int RegZero = 5000;
// Status de registrador vazio
const int RegStatusEmpty = 1;
// Status de Operador Disponivel
const int OperandAvailable = 1;
// Status de Operador Iniciado
const int OperandInit = 1;

int ISSUE(vector<Instruction>& Instructions,
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

// Tratar a linha de instrucao do txt
Instruction parseLine(string line);
// Printar os status dos registradores
void printRegisterStatus(vector<RegisterStatus> );
// Printar as reservation stations
void printReservationStations(vector<ReservationStation> );
// Printar os registradores
void printRegisters(vector<int> );
// Printar as instrucoes
void printInstructions(vector<Instruction> );
// Printar a tabela de clock
void printTimingTable(vector<Instruction> );

int main(int argc, char* argv[])
{
    // Arquivo para ser lido
    string arq = "instrucoes.txt";
    ifstream file(arq);

    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo " << arq << endl;
        return 1;
    }

    // Linha da instrucao a ser lida
    string instruction;

    // Vetor de instrucoes
    vector<Instruction> inst = {};
    
    while (getline(file, instruction)) {
        Instruction newInst = parseLine(instruction);
        inst.push_back(newInst);
    }

    // Adicionando as reservation stations
    // Para atualizar esse numero deve-se atualizar as variaveis globais tambem
    // NumAddReservationStation = 1
    // NumMultReservationStation = 2
    // NumDivReservationStation = 2
    ReservationStation ADD1(AddOp, OperandInit);
    ReservationStation MULT1(MultOp, OperandInit), MULT2(MultOp, OperandInit);
    ReservationStation DIV1(DivOp, OperandInit), DIV2(DivOp, OperandInit);

    // Vetor das reservation stations
    vector<ReservationStation> ResStation = {ADD1, MULT1, MULT2, DIV1, DIV2};

    // Status do registradores
    RegisterStatus F0(RegStatusEmpty), F1(RegStatusEmpty),
                   F2(RegStatusEmpty), F3(RegStatusEmpty),
                   F4(RegStatusEmpty), F5(RegStatusEmpty),
                   F6(RegStatusEmpty), F7(RegStatusEmpty),
                   F8(RegStatusEmpty), F9(RegStatusEmpty),
                   F10(RegStatusEmpty), F11(RegStatusEmpty),
                   F12(RegStatusEmpty);

    // Vetor dos status dos registrador
    vector<RegisterStatus> RegisterStatus = {F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12};

    // Conteudo dos registradores
    vector<int> Register = {RegZero, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    // Printar o estado inicial do sistema
    cout << "INITIAL VALUES:" << endl;
    printInstructions(inst);
    printReservationStations(ResStation);
    printRegisters(Register);
    printRegisterStatus(RegisterStatus);
    cout << endl;

    // Execucao das instrucoes
    do {
        Clock++;

        ISSUE(inst, ResStation, RegisterStatus, Register);
        EXECUTE(inst, ResStation, RegisterStatus, Register);
        WRITEBACK(inst, ResStation, RegisterStatus, Register);

        printRegisters(Register);
        printTimingTable(inst);
        cout << endl;

        Completed = false;

        if (TotalWritebacks == inst.size()) {
            Completed = true;
        }
        cout << endl;
    } while (!Completed);
    
    return 0;
}

int ISSUE(vector<Instruction>& Instructions, vector<ReservationStation>& RegistrationStations, vector<RegisterStatus>& RegisterStatus, vector<int>& Register) {

    int r = 0;
    bool rsFree = false;

    if (CurrentInstIssue >= Instructions.size()) {
        return 0;
    }

    r = Instructions[CurrentInstIssue].op;

    int AddStart = NumAddReservationStation - NumAddReservationStation;
    int AddEnd = NumAddReservationStation;
    int SubStart = NumAddReservationStation - NumAddReservationStation;
    int SubEnd = NumAddReservationStation;
    int MultStart = NumAddReservationStation;
    int MultEnd = NumAddReservationStation + NumMultReservationStation;
    int DivStart = NumAddReservationStation + NumMultReservationStation;
    int DivEnd = NumAddReservationStation + NumMultReservationStation + NumDivReservationStation;

    switch (r) {
        // Issue da operacao ADD
        case AddOp:
            for (int i = AddStart; i < AddEnd; i++) {
                if (!RegistrationStations[i].busy) {
                    r = i;
                    CurrentInstIssue++;
                    RegistrationStations[i].op = AddOp;
                    rsFree = true;
                    break;
                }
                
            }

            if (!rsFree) {
                return 1;
            }
            break;

        // Issue da operacao SUB
        case SubOp:
            for (int i = SubStart; i < SubEnd; i++) {
                if (!RegistrationStations[i].busy) {
                    r = i;
                    CurrentInstIssue++;
                    RegistrationStations[i].op = SubOp;
                    rsFree = true;
                    break;
                }
                
            }

            if (!rsFree) {
                return 1;
            }
            break;

        // Issue da operacao MULT
        case MultOp:
            for (int i = MultStart; i < MultEnd; i++) {
                if (!RegistrationStations[i].busy) {
                    r = i;
                    CurrentInstIssue++;
                    RegistrationStations[i].op = MultOp;
                    rsFree = true;
                    break;
                }
                    
            }

            if (!rsFree) {
                return 1;
            }
            break;

        // Issue da operacao DIV
        case DivOp:
            for (int i = DivStart; i < DivEnd; i++) {
                if (!RegistrationStations[i].busy) {
                    r = i;
                    CurrentInstIssue++;
                    RegistrationStations[i].op = DivOp;
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

    if (RegisterStatus[Instructions[CurrentInstIssue - 1].rs].Qi == RegStatusEmpty) {
        RegistrationStations[r].Vj = Register[Instructions[CurrentInstIssue - 1].rs];
        RegistrationStations[r].Qj = OperandAvailable;
    } else {
        RegistrationStations[r].Qj = RegisterStatus[Instructions[CurrentInstIssue - 1].rs].Qi;
    }

    if(RegisterStatus[Instructions[CurrentInstIssue - 1].rt].Qi == RegStatusEmpty){
        RegistrationStations[r].Vk = Register[Instructions[CurrentInstIssue - 1].rt];
        RegistrationStations[r].Qk = OperandAvailable;
    }
    else{
        RegistrationStations[r].Qk = RegisterStatus[Instructions[CurrentInstIssue - 1].rt].Qi;
    }

    RegistrationStations[r].busy = true;
    RegistrationStations[r].issueLatency = 0;

    RegistrationStations[r].instNum = CurrentInstIssue - 1;

    Instructions[CurrentInstIssue - 1].issueClock = Clock;

    RegisterStatus[Instructions[CurrentInstIssue - 1].rd].Qi = r;
    return 2;
}

void EXECUTE(vector<Instruction>& INST, vector<ReservationStation>& RESSTATION, vector<RegisterStatus>& REGSTATUS, vector<int>& REG) {

    for (int r = 0; r < RESSTATION.size(); r++) {
        if (RESSTATION[r].busy == true) {
            if (RESSTATION[r].issueLatency >= IssueLatency) {
                if (RESSTATION[r].Qj == OperandAvailable && RESSTATION[r].Qk == OperandAvailable) {
                    if (INST[RESSTATION[r].instNum].execClockBegin == 0) 
                        INST[RESSTATION[r].instNum].execClockBegin = Clock;
                    
                    RESSTATION[r].latency++;

                    switch (RESSTATION[r].op) {
                        // Execucao da operacao ADD
                        case AddOp:
                            if(RESSTATION[r].latency == AddLatency){
                                RESSTATION[r].result = RESSTATION[r].Vj + RESSTATION[r].Vk;
                                
                                RESSTATION[r].completed = true;
                                RESSTATION[r].latency = 0;
                                
                                INST[RESSTATION[r].instNum].execClockEnd = Clock;
                                
                                RESSTATION[r].issueLatency = 0;
                            }
                            break;
                        // Execucao da operacao SUB
                        case SubOp:
                            if(RESSTATION[r].latency == AddLatency){
                                RESSTATION[r].result = RESSTATION[r].Vj - RESSTATION[r].Vk;
                                RESSTATION[r].completed = true;
                                RESSTATION[r].latency = 0;
                                
                                INST[RESSTATION[r].instNum].execClockEnd = Clock;
                                
                                RESSTATION[r].issueLatency = 0;
                            }
                        // Execucao da operacao MULT
                        case MultOp:
                            if(RESSTATION[r].latency == MultLatency){
                                RESSTATION[r].result = RESSTATION[r].Vj * RESSTATION[r].Vk;
                                RESSTATION[r].completed = true;
                                RESSTATION[r].latency = 0;
                                
                                INST[RESSTATION[r].instNum].execClockEnd = Clock;
                                
                                RESSTATION[r].issueLatency = 0;
                            }

                        // Execucao da operacao DIV
                        case DivOp:
                            if(RESSTATION[r].latency == DivLatency){
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
            if(RESSTATION[r].writebackLatency == WritebackLatency){
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
                TotalWritebacks++;
            }
            else
                RESSTATION[r].writebackLatency++;
        }
    }
}

void printRegisterStatus(vector<RegisterStatus> RegisterStatus){
    cout << "Status dos registradores: " << endl;
    for(int i = 0; i < RegisterStatus.size(); i++) {
        cout << RegisterStatus[i].Qi << ' ';
    }
    cout << endl;
}

void printReservationStations(vector<ReservationStation> ReservationStations){
    for(int i = 0; i < ReservationStations.size(); i++) {
        cout << "RS #: " << i << "  Busy: " << ReservationStations[i].busy << "  op: "<<
                ReservationStations[i].op << "  Vj: " << ReservationStations[i].Vj << "  Vk: " <<
                ReservationStations[i].Vk << "  Qj: " << ReservationStations[i].Qj << "  Qk: " <<
                ReservationStations[i].Qk << endl;
    }
}

void printRegisters(vector<int> Registers){
    cout << "Register Content:" << endl;
    for(int i = 0; i < Registers.size(); i++) {
        cout << Registers[i] << ' ';
    }
    cout << endl;
}
void printInstructions(vector<Instruction> IV){
    for(int i=0; i<IV.size(); i++) {
        cout << "Instruction #: " << i << "  Operation: " <<
                IV[i].op << "  " <<
                IV[i].rd << " <- " << IV[i].rs << " op " <<
                IV[i].rt << endl;
    }
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

    // ADD F1, F2, F3
    iss >> op >> f1 >> f2 >> f3;

    int operand1 = std::stoi(f1.substr(1)); // F1 -> 1
    int operand2 = std::stoi(f2.substr(1)); // F2 -> 2
    int operand3 = std::stoi(f3.substr(1)); // F3 -> 3

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

