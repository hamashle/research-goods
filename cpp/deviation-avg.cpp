#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <cmath>

bool is_valid_number(double value)
{
    return value != -1 && !std::isnan(value);
}

int main()
{
    std::ifstream file("/Users/user/workspace/research/research-goods/mncore2-emu-code/1000.txt");
    if (!file)
    {
        std::cerr << "Error: Cannot open file." << std::endl;
        return 1;
    }

    std::string line;
    double sum = 0.0;
    int count = 0;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        double col1, col2, col3, error;
        if (iss >> col1 >> col2 >> col3 >> error)
        {
            std::cout << "Read error value: " << error << std::endl; // デバッグ出力

            if (is_valid_number(error))
            {
                sum += error;
                count++;
            }
        }
        else
        {
            std::cerr << "Error reading line: " << line << std::endl;
        }
    }

    file.close();

    if (count > 0)
    {
        double average = sum / count;
        std::cout << "Average error (excluding -1 and nan): " << average << std::endl;
    }
    else
    {
        std::cout << "No valid error values found." << std::endl;
    }
    std::cout << "Total number of valid error values: " << count << std::endl;
    return 0;
}
