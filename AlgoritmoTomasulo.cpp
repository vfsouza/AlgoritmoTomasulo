#include <iostream>
#include <iomanip>   
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "Helper.h"
#include "ReservationStation.h"
#include "RegisterStatus.h"
#include "Instruction.h"

using namespace std;

// Numero de Reservation Stations de ADD
const int NumAddReservationStation = 3;
// Numero de Reservation Stations de Mult
const int NumMultReservationStation = 4;
// Numero de Reservation Stations de Div
const int NumDivReservationStation = 2;

// Numero das Operacoes
enum Operation {
    AddOp = 0,
    SubOp = 1,
    MultOp = 2,
    DivOp = 3,
};

struct OpTime {
    int startTime;
    int endTime;
} OpTime;

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
const int RegStatusEmpty = 1000;
// Status de Operador Disponivel
const int OperandAvailable = 1001;
// Status de Operador Iniciado
const int OperandInit = 1002;

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

// Seta os tempos de cada operacao
void SetOpTimes(struct OpTime &AddTime, struct OpTime &SubTime, struct OpTime &MultTime, struct OpTime &DivTime);
// Faz a operacao da Reservation Station
void execOperation(ReservationStation &rs, Operation op);
// Reseta os valores da Reservation Station quando for concluida sua operacao
void resetReservationStation(ReservationStation &rs);
// Tratar a linha de instrucao do txt
Instruction parseLine(string line);


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
    ReservationStation ADD1(AddOp, OperandInit), ADD2(AddOp, OperandInit), ADD3(AddOp, OperandInit);
    ReservationStation MULT1(MultOp, OperandInit), MULT2(MultOp, OperandInit), MULT3(MultOp, OperandInit), MULT4(MultOp, OperandInit);
    ReservationStation DIV1(DivOp, OperandInit), DIV2(DivOp, OperandInit);

    // Vetor das reservation stations
    vector<ReservationStation> ResStation = {ADD1, ADD2, ADD3, MULT1, MULT2, MULT3, MULT4, DIV1, DIV2};

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
    
    Helper::printInstructions(inst);
    Helper::printReservationStations(ResStation);
    Helper::printRegisters(Register);
    Helper::printRegisterStatus(RegisterStatus);
    cout << endl;

    // Execucao das instrucoes
    do {
        Clock++;

        ISSUE(inst, ResStation, RegisterStatus, Register);
        EXECUTE(inst, ResStation, RegisterStatus, Register);
        WRITEBACK(inst, ResStation, RegisterStatus, Register);

        Helper::printRegisters(Register);
        Helper::printTimingTable(inst, Clock);
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

    struct OpTime AddTime, SubTime, MultTime, DivTime;
    SetOpTimes(AddTime, SubTime, MultTime, DivTime);

    switch (r) {
        // Issue da operacao ADD
        case AddOp:
            for (int i = AddTime.startTime; i < AddTime.endTime; i++) {
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
            for (int i = SubTime.startTime; i < SubTime.endTime; i++) {
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
            for (int i = MultTime.startTime; i < MultTime.endTime; i++) {
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
            for (int i = DivTime.startTime; i < DivTime.endTime; i++) {
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
                                execOperation(RESSTATION[r], AddOp);
                                INST[RESSTATION[r].instNum].execClockEnd = Clock;
                            }
                            break;
                        // Execucao da operacao SUB
                        case SubOp:
                            if(RESSTATION[r].latency == AddLatency){
                                execOperation(RESSTATION[r], SubOp);
                                INST[RESSTATION[r].instNum].execClockEnd = Clock;
                            }
                        // Execucao da operacao MULT
                        case MultOp:
                            if(RESSTATION[r].latency == MultLatency){
                                execOperation(RESSTATION[r], MultOp);
                                INST[RESSTATION[r].instNum].execClockEnd = Clock;
                            }

                        // Execucao da operacao DIV
                        case DivOp:
                            if(RESSTATION[r].latency == DivLatency){
                                execOperation(RESSTATION[r], DivOp);
                                INST[RESSTATION[r].instNum].execClockEnd = Clock;
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
    
    for(int r = 0; r < RESSTATION.size(); r++){
        if(RESSTATION[r].completed){
            if(RESSTATION[r].writebackLatency == WritebackLatency){
                if(INST[RESSTATION[r].instNum].writebackClock == 0)
                    INST[RESSTATION[r].instNum].writebackClock = Clock;
                
                for(int x = 0; x < REG.size();x++) {
                    if (REGSTATUS[x].Qi == r) {
                        REG[x] = RESSTATION[r].result;
                        REGSTATUS[x].Qi = RegStatusEmpty;
                    }
                }
                
                for(int y = 0; y < RESSTATION.size();y++){
                    if(RESSTATION[y].Qj==r){
                        RESSTATION[y].Vj = RESSTATION[r].result;
                        RESSTATION[y].Qj = OperandAvailable;
                    }
                    if(RESSTATION[y].Qk==r){
                        RESSTATION[y].Vk = RESSTATION[r].result;
                        RESSTATION[y].Qk = OperandAvailable;
                    }
                }

                resetReservationStation(RESSTATION[r]);
                TotalWritebacks++;
            }
            else
                RESSTATION[r].writebackLatency++;
        }
    }
}

void resetReservationStation(ReservationStation& rs) {
    rs.completed = false;
    rs.busy = false;
    rs.Qj = OperandInit;
    rs.Qk = OperandInit;
    rs.Vj = 0;
    rs.Vk = 0;
    rs.writebackLatency = 0;
}

void execOperation(ReservationStation& rs, Operation op) {
    switch (op) {
        case AddOp:
            rs.result = rs.Vj + rs.Vk;
            break;
        case SubOp:
            rs.result = rs.Vj - rs.Vk;
            break;
        case MultOp:
            rs.result = rs.Vj * rs.Vk;
            break;
        case DivOp:
            rs.result = rs.Vj / rs.Vk;
            break;
        default:
            break;
    }
    rs.completed = true;
    rs.latency = 0;
    rs.issueLatency = 0;
}

void SetOpTimes(struct OpTime& AddTime, struct OpTime& SubTime, struct OpTime& MultTime, struct OpTime& DivTime) {
    AddTime.startTime = NumAddReservationStation - NumAddReservationStation;
    AddTime.endTime = NumAddReservationStation;

    SubTime.startTime = NumAddReservationStation - NumAddReservationStation;
    SubTime.endTime = NumAddReservationStation;

    MultTime.startTime = NumAddReservationStation;
    MultTime.endTime = NumAddReservationStation + NumMultReservationStation;
    
    DivTime.startTime = NumAddReservationStation + NumMultReservationStation;
    DivTime.endTime = NumAddReservationStation + NumMultReservationStation + NumDivReservationStation;
}


