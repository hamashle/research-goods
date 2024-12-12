#include <iostream>
#include <vector>

const double V = 5.0;

void initialize_phi(int width, std::vector<std::vector<double>> &phi)
{
    for (int i = 0; i < width; i++)
    {

        phi[0][i] = V;
    }
}

void print_phi(int width, const std::vector<std::vector<double>> &phi)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << phi[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    int width;
    std::cout << "Enter grid width: ";
    std::cin >> width;

    std::vector<std::vector<double>> phi(width, std::vector<double>(width, 0.0));

    initialize_phi(width, phi);

    std::cout << "Initialized phi grid:" << std::endl;
    print_phi(width, phi);

    // Perform the update
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 || i == width - 1 || j == 0 || j == width - 1)
            {
                continue;
            }
            else
            {
                phi[i][j] = (phi[i + 1][j] + phi[i - 1][j] + phi[i][j + 1] + phi[i][j - 1]) / 4.0;
            }
        }
    }

    // Print the updated grid
    std::cout << "Updated phi grid:" << std::endl;
    print_phi(width, phi);

    return 0;
}
