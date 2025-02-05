#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

const double V = 5.0;
const int BLOCK_SIZE = 6;
const double CONVERGENCE_CRITERION = 1e-5;

void initialize_grid(int width, std::vector<std::vector<double>> &grid)
{
    for (int i = 0; i < width; i++)
    {
        grid[0][i] = V; // 上端の初期値を設定
        grid[0][i] = V;
        grid[1][i] = 1.0 * i;
        grid[3][i] = 3.0 * i;
        grid[4][i] = 4.0 * i;
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
void save_grid_to_file(int width, const std::vector<std::vector<double>> &grid, const std::string &filename)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        for (const auto &row : grid)
        {
            for (const auto &cell : row)
            {
                file << cell << std::endl;
            }
        }
        file.close();
    }
    else
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }
}

int main()
{
    int width = 48;

    std::vector<std::vector<double>> grid(width, std::vector<double>(width, 0.0));
    std::vector<std::vector<double>> old_grid = grid;

    initialize_grid(width, grid);

    print_grid(width, grid);

    double delta = 1.0; // 初期の変化量
    int iteration = 0;

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

        iteration++;
        std::cout << "Iteration " << iteration << ", delta = " << delta << std::endl;
    }

    std::cout << "Final grid after " << iteration << " iterations:" << std::endl;
    print_grid(width, grid);

    save_grid_to_file(width, grid, "final_grid.txt");

    return 0;
}
