#include <iostream>
#include <string>
#include <vector>


class Data_object {
    public:
    Data_object* Left;
    Data_object* Right;
    Data_object* Up;
    Data_object* Down;
    Data_object* Column_header;

    Data_object(L, R, U, D, C) : Left(L), Right(R), Up(U), Down(D), Column_header(C) {}

    Data_object() : Left(nullptr), Right(nullptr), Up(nullptr), Down(nullptr), Column_header(nullptr) {}

}


class Column_object : public Data_object {
    public:
    std::string Name;
    int Size;

    Column_object(std::string name, int size) : Name(name), Size(size) {}

};


class Diagram {
    Data_object* Header;

    Diagram() : Header(nullptr) {}

    Diagram(std::vector<std::vector<bool>> matrix) {
        
        Header = new Data_object();

        // Initialize the nodes matrix

        std::vector<std::vector<Data_object>> data_objects(matrix.size() + 1, std::vector<Data_object>(matrix[0].size()));
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 1; j <= matrix[i].size(); j++) {
                data_objects[i][j] = new Data_object();
            }
        }

        // Fill the first row with Column_object instances and links them

        for (int j = 0; j < matrix[0].size(); j++) {
            Column_object* new_column_object = new Column_object(std::string("C") + std::to_string(j), 0);
            if (j == 0) {
                Header->Right = new_column_object;
                new_column_object->Left = Header;
            } else {
                data_objects[0][j - 1]->Right = new_column_object;
                new_column_object->Left = data_objects[0][j - 1];
            }
            if (j == matrix[0].size() - 1) {
                new_column_object->Right = Header;
                Header->Left = new_column_object;
            }
            data_objects[0][j] = new_column_object;
        }

        // Vertically link nodes

        for (int j = 0; j < matrix[0].size(); j++) {
             
        }
    }
}


