#include "Classes.cpp"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <chrono>

// Chat-GPT generated function to convert a csv of <int> to a matrix of <int>
std::vector<std::vector<int>> loadCSVToVector(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open the file: " + filename);
    }

    std::vector<std::vector<int>> data;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<int> row;

        while (std::getline(ss, token, ',')) {
            try {
                row.push_back(std::stoi(token));
            } catch (const std::invalid_argument& e) {
                throw std::runtime_error("Invalid integer in CSV: \"" + token + "\"");
            }
        }

        if (!row.empty()) {
            data.push_back(row);
        }
    }

    return data;
}


void sortMatrix(std::vector<std::vector<int>>& matrix) {
    // First: sort each row individually
    for (auto& row : matrix) {
        std::sort(row.begin(), row.end());
    }
    // Then: sort the matrix (vector of vectors) lexicographically
    std::sort(matrix.begin(), matrix.end());
}

// Allows to display a matrix (vector<vector<int>>), and to display it sorted if need be (the benchmarking solutions were often not ordered in the same way)
void print_matrix(const std::vector<std::vector<int>>& matrix, bool sorted = false) {
    
        std::vector<std::vector<int>> matrixToPrint = matrix;
    if (sorted) {
        matrixToPrint = matrix;
        sortMatrix(matrixToPrint);
    }
    std::cout << std::endl;
    for (const auto& row : matrixToPrint) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

// Function that test if the solution found with the Diagram class works correctly on a specified problem
bool Benchmark(std::string fileroot){
    // The result from Diagram is comuted then sorted to have the same ordering 
    Diagram diagram(loadCSVToVector(fileroot + ".csv"));
    diagram.search(0);

    auto SolutionsSorted = diagram.Solutions;
    sortMatrix(SolutionsSorted);

    // The expected result is extracted and sorted
    auto ExpectedSolutions = loadCSVToVector(fileroot + "-solutions.csv");
    sortMatrix(ExpectedSolutions);

    // the function returns true if it works correctly
    return SolutionsSorted == ExpectedSolutions;
}

int main() {
    
    std::vector<std::string> pentominosProblems = {
        "2-5-6",
        "3-20",
        "4-15",
        "5-12",
        "6-10",
        "8-8",
        "8-9"
    };

    // Benchmark for each data that is available
    auto start = std::chrono::high_resolution_clock::now();
    auto lap = start;
    for (const auto& problem : pentominosProblems) {
        std::string fileRoot = "data/pentominos-" + problem;
        if (Benchmark(fileRoot)) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - lap).count();
            std::cout << "Benchmark for " << fileRoot << " passed. Time taken: " << duration << " ms" << std::endl;
            lap = std::chrono::high_resolution_clock::now();
        } else {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - lap).count();
            std::cout << "Benchmark for " << fileRoot << " failed. Time taken: " << duration << " ms" << std::endl;
            lap = std::chrono::high_resolution_clock::now();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout <<"Total time: " << duration/1000 << "." << duration%1000 << std::endl;
    return 0;
}
