#include <iostream>
#include <vector>

const double V = 5.0;
const double convergence_criterion = 1e-5;

void initialize_grid(int width, std::vector<std::vector<double>> &grid)
{
    for (int i = 0; i < width; i++)
    {

        grid[0][i] = V;
    }
}

void print_grid(int width, const std::vector<std::vector<double>> &grid)
{
    for (int i = 0; i < width + 1; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    int column;
    int row;
    int loop;
    std::cout << "Enter grid width: ";
    std::cout << "Enter loop:" << std::endl;
    std::cin >> column;
    std::cin >> loop;
    row = column + 1;
    int loop_counter = 0;
    std::vector<std::vector<double>> grid(row, std::vector<double>(column, 0.0));
    std::vector<std::vector<double>> new_grid(row, std::vector<double>(column, 0.0));

    initialize_grid(column, grid);

    std::cout << "Initialized grid grid:" << std::endl;
    print_grid(column, grid);

    double delta = 1.0; // ループに入る前のチェック用

    // Perform the update
    while (loop > loop_counter)
    {
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                if (i == 0 || i == row - 1 || j == 0 || j == column - 1)
                {
                    // new_
                    grid[i][j] = grid[i][j];
                }
                else
                {
                    // new_
                    grid[i][j] = (grid[i + 1][j] + grid[i - 1][j] + grid[i][j + 1] + grid[i][j - 1]) / 4.0;
                }
            }
        }

        delta = 0.0;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                delta = std::max(delta, std::abs(grid[i][j] - new_grid[i][j]));
            }
        }

        grid.swap(new_grid);
        loop_counter++;
    }

    // Print the updated grid
    std::cout << "Updated grid grid:" << std::endl;
    print_grid(column, grid);
    std::cout << "Number of iterations: " << loop_counter << std::endl;
    return 0;
}
