#include <iostream>
#include <vector>
const int width = 6;
const double V = 5.0;

int main()
{
    std::cin >> width;
    std::vector<std::vector<double>> phi(width, std::vector<double>(width, 0.0));

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 || i == width - 1 || j == 0 || j == width - 1)
            {
                phi[i][j] = V;
            }
            {
                continue;
            }
            else
            {
                phi[i][j] = (phi[i + 1][j] + phi[i - 1][j] + phi[i][j + 1] + phi[i][j - 1]) / 4.0;
            }
        }
    }
}

void jacobi_test_value_generate(int width, std::vector<std::vector<double>> &phi)
{
    for (int i = 0; i < width; i++)
    {
        phi[0][i] = V;
    }
}