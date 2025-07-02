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
        std::cout << "Node IDs: " << id1 << ", " << id2 << std::endl;
    }



};


class Column_object : public Data_object {
    public:
    std::string Name;
    int Size;

    Column_object(std::string name, int size, int id) : Name(name), Size(size), Data_object(0, id) {}

    void print(){
        if (id2) {
            std::cout << "Header Node ID: " << id2 << ", Name: " << Name  << ", Size:" << Size << std::endl;
        } else {
            std::cout << "HEADER NODE" << std::endl;
        }
    }

};


class Diagram {
    public:
    Data_object* Header;
    std::vector<Data_object*> solution;

    Diagram() : Header(nullptr) {}

    Diagram(std::vector<std::vector<bool>> matrix, std::vector<std::string> column_names = {}) {
        
        Header = new Column_object("HEADER", 0, 0); // Create the header node

        // Initialize the nodes matrix
        std::vector<std::vector<Data_object*>> data_objects(matrix.size() + 1, std::vector<Data_object*>(matrix[0].size()));
        for (int i = 1; i <= matrix.size(); i++) {
            for (int j = 0; j < matrix[0].size(); j++) {
                data_objects[i][j] = new Data_object(i, j + 1); // Assign unique IDs to each Data_object
            }
        }

        // Fill the first row with Column_object instances and links them
        if (column_names.size() < matrix[0].size()) {
            std::cerr << "Warning: Not enough column names provided. Default names will be used for missing columns." << std::endl << std::endl;
        }
        for (int j = 0; j < matrix[0].size(); j++) {
            std::string column_name;
            if (j < column_names.size()) {
                // Use the provided column name if available
                column_name = column_names[j];
            } else {
                // Default naming if no names are provided
                column_name = "C" + std::to_string(j + 1);
            }
            Column_object* new_column_object = new Column_object(column_name, 0, j + 1);
            data_objects[0][j] = new_column_object;
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

                if (matrix[vertical_node->id1 - 1][vertical_node->id2 - 1]){
                    Column_object* column_header = static_cast<Column_object*>(vertical_node->Column_header);
                    column_header->Size++;
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
            current_node = current_node->Right;
        }
    }

    ~Diagram() {
        // Clean up the allocated memory
        Data_object* current_node = Header->Right;
        while (current_node != Header) {
            Data_object* vertical_node = current_node->Down;
            while (vertical_node != current_node) {
                Data_object* node_to_delete = vertical_node;
                vertical_node = vertical_node->Down;
                delete node_to_delete;
            }
            Data_object* header_to_delete = current_node;
            current_node = current_node->Right;
            delete header_to_delete;
        }
        delete Header; // Delete the header node
    }

    void matrix_print() {
        for (Data_object* current_node = this->Header->Right; current_node != this->Header; current_node = current_node->Right) {
            std::cout << static_cast<Column_object*>(current_node)->Name << ": ";
            for (Data_object* vertical_node = current_node->Down; vertical_node != current_node; vertical_node = vertical_node->Down) {
                std::cout << "(" << vertical_node->id1 << ", " << vertical_node->id2 << ") ";
            }
            std::cout << std::endl;
        }
    }

    bool partial_link_print(){
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


    Column_object* min_size_column(){
        Column_object* min_column = static_cast<Column_object*>(this->Header->Right);
        int min_size = min_column->Size;
        Column_object* current_col = static_cast<Column_object*>(min_column->Right);
        while (current_col != this->Header){
            if (current_col->Size < min_size){
                min_size = current_col->Size;
                min_column = current_col;
            }
            current_col = static_cast<Column_object*>(current_col->Right);
        }
        return min_column;
    }


    Column_object* cover_column(Column_object* column) {
        column->Left->Right = column->Right;
        column->Right->Left = column->Left;
        for (Data_object* i = column->Down; i != column; i = i->Down) {
            for (Data_object* j = i->Right; j != i; j = j->Right) {
                j->Up->Down = j->Down;
                j->Down->Up = j->Up;
                static_cast<Column_object*>(j->Column_header)->Size--;
            }
        }
        return column;
    }


    void uncover_column(Column_object* column) {
        for (Data_object* i = column->Up; i != column; i = i->Up) {
            for (Data_object* j = i->Left; j != i; j = j->Left) {
                static_cast<Column_object*>(j->Column_header)->Size++;
                j->Up->Down = j;
                j->Down->Up = j;
            }
        }
        column->Left->Right = column;
        column->Right->Left = column;
    }

    void print_solution() {
        for (Data_object* obj : solution) {
            std::cout << static_cast<Column_object*>(obj->Column_header)->Name << " ";
            for (Data_object* j = obj->Right; j != obj; j = j->Right) {
                std::cout << static_cast<Column_object*>(j->Column_header)->Name << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void print_sizes() {
        Data_object* current_node = this->Header->Right;
        std::cout << "Sizes:    ";
        while (current_node != this->Header) {
            Column_object* column = static_cast<Column_object*>(current_node);
            std::cout <<  column->Name << ": " << column->Size << ", ";
            current_node = current_node->Right;
        }
        std::cout << std::endl;
    }



    void search(int k) {
        
        if (this->Header->Right == this->Header) {
            std::cout << "Solution found: " << std::endl;
            print_solution();
            return;
        }

        Column_object* column = min_size_column();
        cover_column(column);

        for (Data_object* i = column->Down; i != column; i = i->Down) {
            solution.push_back(i);
            for (Data_object* j = i->Right; j != i; j = j->Right) {
                cover_column(static_cast<Column_object*>(j->Column_header));
            }

            this->search(k + 1);

            for (Data_object* j = i->Left; j != i; j = j->Left) {
                uncover_column(static_cast<Column_object*>(j->Column_header));
            }
            solution.pop_back();
        }

        uncover_column(column);
    }


};


