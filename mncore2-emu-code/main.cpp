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

const int n = 2304; // 48 * 48

double LM0[n];
double LM1[n];

double LM0_ref[n];
double LM1_ref[n];

const int nvec = 48;

const double V = 5.0;
const double convergence_criterion = 1e-5;

void initialize_grid(int width, std::vector<std::vector<double>> &grid)
{
    for (int i = 0; i < width; i++)
    {

        grid[0][i] = V;
    }
}

std::vector<std::vector<double>> jacobi()
{
    int colmn = 34;
    int row = 34;
    std::vector<std::vector<double>> grid(row, std::vector<double>(colmn, 0.0));
    std::vector<std::vector<double>> new_grid(row, std::vector<double>(colmn, 0.0));

    initialize_grid(colmn, grid);

    double delta = 1.0; // ループに入る前のチェック用

    // Perform the update
    while (delta > convergence_criterion)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < colmn; j++)
            {
                if (i == 0 || i == row - 1 || j == 0 || j == colmn - 1)
                {
                    new_grid[i][j] = grid[i][j];
                }
                else
                {
                    new_grid[i][j] = (grid[i + 1][j] + grid[i - 1][j] + grid[i][j + 1] + grid[i][j - 1]) / 4.0;
                }
            }
        }
        delta = 0.0;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < colmn; j++)
            {
                delta = std::max(delta, std::abs(grid[i][j] - new_grid[i][j]));
            }
        }

        grid.swap(new_grid);
    }
    return new_grid;
}

void mncore_kernel(double *LM0, double *LM1)
{
    auto result = jacobi();
    //int width = result.size();
    int column = 34;
    int row = 34;

    // 2次元配列を1次元配列に変換してLM1に格納
    int index = 0;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            LM1[index++] = result[i][j];
        }
    }
}

int main()
{
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<> distr(0.0, 0.5 * M_PI); // 0.0からπ/2までの一様乱数 -> 今回は、5で初期化するので、ここはいらんかも

    std::cerr << std::scientific;
    std::cerr << std::setprecision(15);

    for (int i = 0; i < 34; i++)
    {
        LM0[i] = 5.0;
        LM1[i] = 0.0;
    }

    memcpy(LM0_ref, LM0, sizeof(double) * n);
    memcpy(LM1_ref, LM1, sizeof(double) * n);

    emu.execute_kernel("source-test.vsm", LM0, LM1, nvec);

    for(int i = 0; i < 100; i++) {
        std::cerr << "LM0[" << i << "] = " << LM0[i] << ", LM1[" << i << "] = " << LM1[i] << std::endl;
    }

    // CPU側で計算したものの値をLM1_ref, LM2_refに入れる
    mncore_kernel(LM0_ref, LM1_ref);

    // MN-Coreの結果とCPU側の結果を比較する
    for (int i = 0; i < 2304; i++)
    {
        double b_emu = LM0[i];
        double b_ref = LM1_ref[i];
        double diff = (b_emu - b_ref) / b_ref;
        std::cout << i << " " << b_ref << " " << b_emu << " " << diff << "\n";
    }
    std::cout << std::flush;
}
