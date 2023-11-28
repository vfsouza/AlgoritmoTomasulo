#include "Helper.h"

// Numero das Operacoes
enum Operation {
    AddOp = 0,
    SubOp = 1,
    MultOp = 2,
    DivOp = 3,
};

void Helper::printRegisterStatus(vector<RegisterStatus> RegisterStatus){
    cout << "Status dos registradores: " << endl;
    for(int i = 0; i < RegisterStatus.size(); i++) {
        cout << RegisterStatus[i].Qi << ' ';
    }
    cout << endl;
}

void Helper::printReservationStations(vector<ReservationStation> ReservationStations){
    for(int i = 0; i < ReservationStations.size(); i++) {
        cout << "RS #: " << i << "  Busy: " << ReservationStations[i].busy << "  op: "<<
                ReservationStations[i].op << "  Vj: " << ReservationStations[i].Vj << "  Vk: " <<
                ReservationStations[i].Vk << "  Qj: " << ReservationStations[i].Qj << "  Qk: " <<
                ReservationStations[i].Qk << endl;
    }
}

void Helper::printRegisters(vector<int> Registers){
    cout << "Register Content:" << endl;
    for(int i = 0; i < Registers.size(); i++) {
        cout << Registers[i] << ' ';
    }
    cout << endl;
}

void Helper::printInstructions(vector<Instruction> IV){
    for(int i = 0; i < IV.size(); i++) {
        cout << "Instruction #: " << i << "  Operation: " <<
                IV[i].op << "  " <<
                IV[i].rd << " <- " << IV[i].rs << " op " <<
                IV[i].rt << endl;
    }
}

void Helper::printTimingTable(vector<Instruction> INST, int Clock){
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