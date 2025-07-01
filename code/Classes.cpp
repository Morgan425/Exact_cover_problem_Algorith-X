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
    int id1;
    int id2;

    Data_object(int row, int col) : Left(nullptr), Right(nullptr), Up(nullptr), Down(nullptr), Column_header(nullptr), id1(row), id2(col) {}

    virtual void print(){
        if (id1 || id2) {
            std::cout << "Node IDs: " << id1 << ", " << id2 << std::endl;
        } else {
            std::cout << "HEADER NODE" << std::endl;
        }
    }



};


class Column_object : public Data_object {
    public:
    std::string Name;
    int Size;

    Column_object(std::string name, int size, int id) : Name(name), Size(size), Data_object(0, id) {
        id1 = 0; // Column objects do not have row IDs
        id2 = 0; // Column objects do not have column IDs

    }

    void print(){
        std::cout << "Header Node ID: " << id2 << std::endl;
    }

};


class Diagram {
    public:
    Data_object* Header;

    Diagram() : Header(nullptr) {}

    Diagram(std::vector<std::vector<bool>> matrix) {
        
        Header = new Data_object(0, 0); // Create the header node

        // Initialize the nodes matrix
        std::vector<std::vector<Data_object*>> data_objects(matrix.size() + 1, std::vector<Data_object*>(matrix[0].size()));
        int id_base = 1;
        while (id_base < matrix[0].size()) {
            id_base *= 10;
        }
        for (int i = 1; i <= matrix.size(); i++) {
            for (int j = 0; j < matrix[0].size(); j++) {
                data_objects[i][j] = new Data_object(i+1, j+1); // Assign unique IDs to each Data_object
            }
        }

        // Fill the first row with Column_object instances and links them
        for (int j = 0; j < matrix[0].size(); j++) {
            Column_object* new_column_object = new Column_object(std::string("C") + std::to_string(j), 0, j + 1);
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
            for (int j = 1; j < matrix[0].size(); j++) {
                data_objects[i][j]->Left = data_objects[i][j-1];
                data_objects[i][j-1]->Right = data_objects[i][j];
            }
            int j = data_objects[0].size() - 1;
            data_objects[i][j]->Right = data_objects[i][0];
            data_objects[i][0]->Left = data_objects[i][j];
        }



        // Remove the 0 nodes
        Data_object* current_node = this->Header->Right;
        while (current_node != this->Header){
            Data_object* vertical_node = current_node->Down;
            while (vertical_node != current_node){

                if (matrix[vertical_node->id1][vertical_node->id2]){
                    vertical_node = vertical_node->Down;
                } else {
                    // Remove the node
                    vertical_node->Up->Down = vertical_node->Down;
                    vertical_node->Down->Up = vertical_node->Up;
                    vertical_node->Left->Right = vertical_node->Right;
                    vertical_node->Right->Left = vertical_node->Left;
                    Data_object* node_to_delete = vertical_node;
                    vertical_node = vertical_node->Down;
                    delete node_to_delete;
                }
            }
            std::cout << std::endl;
            current_node = current_node->Right;
        }    

    }

    bool print(){
        std::cout << "Vertical links:" << std::endl;
        Data_object* current_node = this->Header->Right;
        while (current_node != this->Header){
            Data_object* vertical_node = current_node->Down;
            vertical_node->print();
            while (vertical_node != current_node){
                vertical_node = vertical_node->Down;
                vertical_node->print();
            }
            std::cout << std::endl;
            current_node = current_node->Right;
        }

        Data_object* base_node = this->Header->Right;
        std::cout << std::endl << std::endl << "Horizontal links:" << std::endl;
        current_node = base_node->Down;
        

        while (current_node != base_node){
            Data_object* horizontal_node = current_node->Right;
            horizontal_node->print();
            while (horizontal_node != current_node){
                horizontal_node = horizontal_node->Right;
                horizontal_node->print();
            }
            std::cout << std::endl;
            current_node = current_node->Down;
        }

        return true;
    }

};


