#include "ReservationStation.h"

ReservationStation::ReservationStation() {
    busy = false;
    op = 0;
    latency = 0;
    result = 0;
    completed = false;
    Qj = 1;
    Qk = 1;
    Vj = 0;
    Vk = 0;
    instNum = 100000;
    issueLatency = 0;
    writebackLatency = 0;
}

ReservationStation::ReservationStation(int OP, int RSoperandAvailable){
    busy = false;
    op = OP;
    latency = 0;
    result = 0;
    completed = false;
    Qj = RSoperandAvailable;
    Qk = RSoperandAvailable;
    Vj = 0;
    Vk = 0;
    instNum = 100000;
    issueLatency = 0;
    writebackLatency = 0;

}