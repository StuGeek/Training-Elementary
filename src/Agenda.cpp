#include "../include/AgendaUI.hpp"
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

AgendaUI aui;

void Stop(int signo) {
    aui.~AgendaUI();
    _exit(0);
}

int main() {
    aui.OperationLoop();
}