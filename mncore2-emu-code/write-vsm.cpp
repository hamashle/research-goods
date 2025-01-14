#include <iostream>
#include <fstream>
#include <string>

void writeFilesAlternately(const std::string &file1, const std::string &file2, const std::string &outputFile, int iterations)
{
    // Open the input files
    std::ifstream inputFile1(file1);
    std::ifstream inputFile2(file2);

    if (!inputFile1.is_open() || !inputFile2.is_open())
    {
        std::cerr << "Error: Unable to open input files." << std::endl;
        return;
    }

    // Read the content of the input files into strings
    std::string content1((std::istreambuf_iterator<char>(inputFile1)), std::istreambuf_iterator<char>());
    std::string content2((std::istreambuf_iterator<char>(inputFile2)), std::istreambuf_iterator<char>());

    // Close the input files as we no longer need them
    inputFile1.close();
    inputFile2.close();

    // Open the output file
    std::ofstream outputFileStream(outputFile);
    if (!outputFileStream.is_open())
    {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return;
    }

    // Write the contents alternately to the output file
    for (int i = 0; i < iterations; ++i)
    {
        outputFileStream << content1;
        outputFileStream << content2;
    }

    // Close the output file
    outputFileStream.close();

    std::cout << "Files written alternately to " << outputFile << " successfully." << std::endl;
}

int main()
{
    std::string file1 = "jacobi.txt";
    std::string file2 = "halo-exchange.txt";
    std::string outputFile = "source-test.vsm";
    int iterations;

    std::cout << "Enter the number of times to write files alternately: ";
    std::cin >> iterations;

    if (iterations <= 0)
    {
        std::cerr << "Error: Number of iterations must be positive." << std::endl;
        return 1;
    }

    writeFilesAlternately(file1, file2, outputFile, iterations);

    return 0;
}
