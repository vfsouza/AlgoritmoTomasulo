#pragma once

#include <iostream>
#include <vector>
#include "Helper.h"
#include <sstream>
#include "Instruction.h"
#include "RegisterStatus.h"
#include "ReservationStation.h"

using namespace std;

class Helper {
public:
    
    // Printar os status dos registradores
    void static printRegisterStatus(vector<RegisterStatus> );
    // Printar as reservation stations
    void static printReservationStations(vector<ReservationStation> );
    // Printar os registradores
    void static printRegisters(vector<int> );
    // Printar as instrucoes
    void static printInstructions(vector<Instruction> );
    // Printar a tabela de clock
    void static printTimingTable(vector<Instruction>, int);
};
