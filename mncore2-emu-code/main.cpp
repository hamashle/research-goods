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
const int BLOCK_SIZE = 6;
const double CONVERGENCE_CRITERION = 1e-5;

void initialize_grid(int width, std::vector<std::vector<double>> &grid)
{
    for (int i = 0; i < width; i++)
    {
        grid[0][i] = V; // 上端の初期値を設定
        grid[1][i] = 1.0 * (i % 6);
        grid[3][i] = 3.0 * (i % 6);
        grid[4][i] = 4.0 * (i % 6);
    }
}

void print_grid(int width, const std::vector<std::vector<double>> &grid)
{
    for (const auto &row : grid)
    {
        for (const auto &cell : row)
        {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

void calculate_block(int start_row, int start_col, int block_size, std::vector<std::vector<double>> &grid, const std::vector<std::vector<double>> &old_grid)
{
    for (int i = start_row + 1; i < start_row + block_size - 1; i++)
    {
        for (int j = start_col + 1; j < start_col + block_size - 1; j++)
        {
            grid[i][j] = (grid[i + 1][j] + grid[i - 1][j] + grid[i][j + 1] + grid[i][j - 1]) / 4.0;
        }
    }
}

void exchange_boundaries(int width, std::vector<std::vector<double>> &grid)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {

            // 右から左へ
            if (i == 5 || i == 11 || i == 17 || i == 23 || i == 29 || i == 35 || i == 41)
            {
                grid[j][i] = grid[j][i + 2];
            } // 左から右へ
            else if (i == 6 || i == 12 || i == 18 || i == 24 || i == 30 || i == 36 || i == 42)
            {
                grid[j][i] = grid[j][i - 2];
            }
            else if (j == 5 || j == 11 || j == 17 || j == 23 || j == 29 || j == 35 || j == 41)
            {
                grid[j][i] = grid[j + 2][i];
            }
            else if (j == 6 || j == 12 || j == 18 || j == 24 || j == 30 || j == 36 || j == 42)
            {
                grid[j][i] = grid[j - 2][i];
            }
        }
    }
}

std::vector<std::vector<double>> jacobi()
{
    int width = 48;
    std::vector<std::vector<double>> grid(width, std::vector<double>(width, 0.0));
    std::vector<std::vector<double>> old_grid = grid;

    initialize_grid(width, grid);

    double delta = 1.0; // ループに入る前のチェック用

    // Perform the update
    while (delta > CONVERGENCE_CRITERION)
    {
        old_grid = grid;
        delta = 0.0;

        // Process each block
        for (int i = 0; i < width; i += BLOCK_SIZE)
        {
            for (int j = 0; j < width; j += BLOCK_SIZE)
            {
                calculate_block(i, j, BLOCK_SIZE, grid, old_grid);
            }
        }

        // Exchange boundaries
        exchange_boundaries(width, grid);

        // Calculate maximum difference (delta) for convergence check
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < width; j++)
            {
                delta = std::max(delta, std::abs(grid[i][j] - old_grid[i][j]));
            }
        }
    }
    return grid;
}

void mncore_kernel(double *LM0, double *LM1)
{
    auto result = jacobi();
    // int width = result.size();
    int column = 48;
    int row = 48;

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

    for (int i = 0; i < 48; i++)
    {
        LM0[i] = 5.0;
        LM1[i] = 0.0;
    }

    memcpy(LM0_ref, LM0, sizeof(double) * n);
    memcpy(LM1_ref, LM1, sizeof(double) * n);

    emu.execute_kernel("source-test.vsm", LM0, LM1, nvec);

    for (int i = 0; i < 100; i++)
    {
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
