#include <iostream>
#include <string>
#include <vector>
#include <random>


class Data_object {
    public:
    Data_object* Left;
    Data_object* Right;
    Data_object* Up;
    Data_object* Down;
    Data_object* Column_header;
    int id;

    Data_object() : Left(nullptr), Right(nullptr), Up(nullptr), Down(nullptr), Column_header(nullptr) {
        id = rand() % 1000000; // Random ID for tracking purposes
    }

    virtual ~Data_object() {} // Make Base polymorphic (enables RTTI)

};


class Column_object : public Data_object {
    public:
    std::string Name;
    int Size;

    Column_object(std::string name, int size) : Name(name), Size(size) {}

};


class Diagram {
    public:
    Data_object* Header;

    Diagram() : Header(nullptr) {}

    Diagram(std::vector<std::vector<bool>> matrix) {
        
        Header = new Data_object();
        Header->id = 0;

        // Initialize the nodes matrix
        std::vector<std::vector<Data_object*>> data_objects(matrix.size() + 1, std::vector<Data_object*>(matrix[0].size()));
        for (int i = 1; i <= matrix.size(); i++) {
            for (int j = 0; j < matrix[0].size(); j++) {
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
            Data_object* column = data_objects[0][j];
            for (int i = 1; i <= matrix.size(); i++) {
                data_objects[i][j]->Column_header = column;
                data_objects[i][j]->Up = data_objects[i-1][j];
                data_objects[i-1][j]->Down = data_objects[i][j];
            }
            int i = data_objects.size() - 1;
            data_objects[i][j]->Down = column;
            column->Up = data_objects[i][j];
        }

        // Horizontally link nodes
        for (int i = 1; i <= matrix.size(); i++) {
            for (int j = 1; j < matrix.size(); j++) {
                data_objects[i][j]->Left = data_objects[i][j-1];
                data_objects[i][j-1]->Right = data_objects[i][j];
            }
            int j = data_objects[0].size() - 1;
            data_objects[i][j]->Right = data_objects[i][0];
            data_objects[i][0]->Left = data_objects[i][j];
        }
    }

    bool Circularity_test(){
        Data_object* current_node = this->Header->Right;
        int counter = 0;
        while (current_node != this->Header && counter < 20){
            Data_object* vertical_node = current_node->Down;
            while (vertical_node != current_node){
                if (dynamic_cast<Column_object*>(vertical_node)){
                    std::cout << "Checking Header node with ID: " << vertical_node->id << std::endl;
                } else {
                    std::cout << "Checking node with ID: " << vertical_node->id << std::endl;
                }
                std::cout << vertical_node->Down->id << "   " << vertical_node->Up->id << std::endl;
                std::cout << counter << std::endl;
                vertical_node = vertical_node->Down;
            }
            counter++;
        }

        counter = 0;

        while (current_node != this->Header && counter < 20){
            Data_object* horizontal_node = current_node->Right;
            while (horizontal_node != current_node){
                if (dynamic_cast<Column_object*>(horizontal_node)){
                    std::cout << "Checking Header node with ID: " << horizontal_node->id << std::endl;
                } else {
                    std::cout << "Checking node with ID: " << horizontal_node->id << std::endl;
                }
                std::cout << counter << std::endl;
                horizontal_node = horizontal_node->Right;
            }
            counter++;
        }

        return true;
    }

};


