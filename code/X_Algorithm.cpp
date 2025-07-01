#include "Classes.cpp"

int main() {
    Diagram test_diagram({
        {1, 0, 1},
        {0, 1, 0},
        {1, 0, 1}
    }, {"Column"});

    std::cout << test_diagram.partial_print() << std::endl;

    return 0;
}
