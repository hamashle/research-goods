#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <iomanip>
#include <random>

#include "emu.h"

struct pe_emulator emu;

const int n = 2048;

double LM0[n];
double LM1[n];

double LM0_ref[n];
double LM1_ref[n];

const int nvec = 16;

void mncore_kernel(double *LM0, double *LM1)
{
    for (int i = 0; i < nvec; i++)
    {
        LM1[i] = cos(LM0[i]);
    }
}

int main()
{
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<> distr(0.0, 0.5 * M_PI);

    std::cerr << std::scientific;
    std::cerr << std::setprecision(15);

    for (int i = 0; i < nvec; i++)
    {
        LM0[i] = distr(mt);
        LM1[i] = 0.0;
    }

    memcpy(LM0_ref, LM0, sizeof(double) * n);
    memcpy(LM1_ref, LM1, sizeof(double) * n);

    emu.execute_kernel("source.vsm", LM0, LM1, nvec);

    mncore_kernel(LM0_ref, LM1_ref);

    for (int i = 0; i < nvec; i++)
    {
        double b_emu = LM1[i];
        double b_ref = LM1_ref[i];
        double diff = (b_emu - b_ref) / b_ref;
        std::cout << i << " " << b_ref << " " << b_emu << " " << diff << "\n";
    }
    std::cout << std::flush;
}
