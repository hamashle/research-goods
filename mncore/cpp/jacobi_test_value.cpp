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
    for (int i = 0; i < width; i++)
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
    int width;
    std::cout << "Enter grid width: ";
    std::cin >> width;
    int loop_counter = 0;
    std::vector<std::vector<double>> grid(width, std::vector<double>(width, 0.0));
    std::vector<std::vector<double>> new_grid(width, std::vector<double>(width, 0.0));

    initialize_grid(width, grid);

    std::cout << "Initialized grid grid:" << std::endl;
    print_grid(width, grid);

    double delta = 1.0; // ループに入る前のチェック用

    // Perform the update
    while (delta > convergence_criterion)
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (i == 0 || i == width - 1 || j == 0 || j == width - 1)
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
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < width; j++)
            {
                delta = std::max(delta, std::abs(grid[i][j] - new_grid[i][j]));
            }
        }

        grid.swap(new_grid);
        loop_counter++;
    }

    // Print the updated grid
    std::cout << "Updated grid grid:" << std::endl;
    print_grid(width, grid);
    std::cout << "Number of iterations: " << loop_counter << std::endl;
    return 0;
}
