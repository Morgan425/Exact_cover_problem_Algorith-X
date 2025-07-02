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


    test_diagram.search(0);

    return 0;
}
