#include "Classes.cpp"

int main() {
    Diagram test_diagram({
    {0, 0, 1, 0, 1, 1, 0},
    {1, 0, 0, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 1},
    {0, 0, 0, 1, 1, 0, 1}
    }, {"A", "B", "C", "D", "E", "F", "G"});

    std::cout << test_diagram.partial_print() << std::endl;

    return 0;
}
