#pragma once

class ReservationStation {
public:
    bool busy;
    int Qj;
    int Qk;
    int Vj;
    int Vk;
    int latency;
    int op;
    int result;
    bool completed;
    int instNum;
    int issueLatency;
    int writebackLatency;

public:
    ReservationStation();
    ReservationStation(int, int);
};
