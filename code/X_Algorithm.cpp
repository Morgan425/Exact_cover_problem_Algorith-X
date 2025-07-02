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

    test_diagram.print_sizes();

    //test_diagram.search(0);
    test_diagram.cover_column(static_cast<Column_object*>(test_diagram.Header->Right));
    test_diagram.uncover_column(static_cast<Column_object*>(test_diagram.Header->Right));
    // test_diagram.cover_column(static_cast<Column_object*>(test_diagram.Header->Right->Right));
    // test_diagram.uncover_column(static_cast<Column_object*>(test_diagram.Header->Right->Right));
    // test_diagram.cover_column(static_cast<Column_object*>(test_diagram.Header->Right->Right->Right));
    // test_diagram.uncover_column(static_cast<Column_object*>(test_diagram.Header->Right->Right->Right));
    // test_diagram.cover_column(static_cast<Column_object*>(test_diagram.Header->Right->Right->Right->Right));
    // test_diagram.uncover_column(static_cast<Column_object*>(test_diagram.Header->Right->Right->Right->Right));

    test_diagram.print_sizes();

    return 0;
}
