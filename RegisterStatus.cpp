#include "RegisterStatus.h"

RegisterStatus::RegisterStatus() {
    busy = false;
    Qi = 0;
}

RegisterStatus::RegisterStatus(int RegStatusEmpty) {
    Qi = RegStatusEmpty;
}

