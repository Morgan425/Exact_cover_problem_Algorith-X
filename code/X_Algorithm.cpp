#include "Classes.cpp"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>

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

bool Benchmark(std::string fileroot){
    Diagram diagram(loadCSVToVector(fileroot + ".csv"));
    diagram.search(0);

    auto SolutionsSorted = diagram.Solutions;
    sortMatrix(SolutionsSorted);

    auto ExpectedSolutions = loadCSVToVector(fileroot + "-solutions.csv");
    sortMatrix(ExpectedSolutions);

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

    for (const auto& problem : pentominosProblems) {
        std::string fileRoot = "data/pentominos-" + problem;
        try {
            if (Benchmark(fileRoot)) {
                std::cout << "Benchmark for " << fileRoot << " passed." << std::endl;
            } else {
                std::cout << "Benchmark for " << fileRoot << " failed." << std::endl;
            }
        } catch (const std::runtime_error& e) {
            std::cerr << "Error processing " << fileRoot << ": " << e.what() << std::endl;
        }
    }

    return 0;
}
