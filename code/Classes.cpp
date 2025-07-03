#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>


class Data_object { //The class for my nodes of my doubly linked toroidal list, as named in Knuth's paper
    public:
    Data_object* Left;
    Data_object* Right;
    Data_object* Up;
    Data_object* Down;
    Data_object* Column_header;
    // Nodes are identified by an id couple that remembers where it is in the original matrix, for tracking purposes
    int id1;
    int id2;

    Data_object(int row, int col) : Left(nullptr), Right(nullptr), Up(nullptr), Down(nullptr), Column_header(nullptr), id1(row), id2(col) {}

    virtual void print(){
        std::cout << "Node IDs: " << id1 << ", " << id2 << std::endl;
    }



};


class Column_object : public Data_object { // The class for my column headers, to add Size and Name to them
    public:
    std::string Name;
    int Size;

    Column_object(std::string name, int size, int id) : Name(name), Size(size), Data_object(0, id) {} // Column headers are considered as the 0th row of the matrix, while the header node is (0, 0)

    void print(){
        if (id2) {
            std::cout << "Header Node ID: " << id2 << ", Name: " << Name  << ", Size:" << Size << std::endl;
        } else {
            std::cout << "HEADER NODE" << std::endl;
        }
    }

};


class Diagram { // The class that will allow to run the search algorithm on, a doubly linked toroidal list
    public:
    Data_object* Header;
    std::vector<Data_object*> solution; // The current solution that is being explored by the search() function
    std::vector<std::vector<int>> Solutions; // The already found Solutions, to which is added *solution* when it works (The data type is <int> to be on par with the benchmarking data)

    Diagram() : Header(nullptr) {}

    Diagram(std::vector<std::vector<int>> matrix, std::vector<std::string> column_names = {}) { // I build my diagram from a boolean matrix 
        
        Header = new Column_object("HEADER", 0, 0); // Create the header node

        // Initialize a matrix to store all the Dat_objects, which allows me to keep them on hand to then link them
        // It is like a canvass that will be discarded when the diagram is finished
        std::vector<std::vector<Data_object*>> data_objects(matrix.size() + 1, std::vector<Data_object*>(matrix[0].size()));
        for (int i = 1; i <= matrix.size(); i++) {
            for (int j = 0; j < matrix[0].size(); j++) {
                data_objects[i][j] = new Data_object(i, j + 1); // Assign unique IDs to each Data_object
            }
        }

        // Fill the first row of data_objects with Column_object instances and links them
        if (column_names.size() < matrix[0].size() && !column_names.empty()) {
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
                    // Size is calculated here for each column
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
        // Clean up the allocated memory (Copilot-generated)
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


    bool partial_link_print(){ // Was initially a test to see if the list is indeed toroidal in all directions (correctly prints when used before removing the 0s)

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


    Column_object* min_size_column(){ // Returns the Column header of the column with the lowest Size
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


    Column_object* cover_column(Column_object* column) { // Cover a column as described in Knuth's algorithm, not by deleting it but by linking the list around it
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


    void uncover_column(Column_object* column) { // Uncover a column as described in Knuth's algorithm, by linking the list (which was not deleted) back to the column
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

    void print_solution() { // prints a solution the same way as in Knuth's paper (was at some point used with ids to understand the benchmark solutions)
        for (Data_object* obj : solution) {
            std::cout << static_cast<Column_object*>(obj->Column_header)->Name << " ";
            // std::cout << "(" << obj->id1 << ", " << obj->id2 << "); ";
            for (Data_object* j = obj->Right; j != obj; j = j->Right) {
                std::cout << static_cast<Column_object*>(j->Column_header)->Name << " ";
                // std::cout << "(" << j->id1 << ", " << j->id2 << "); ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }



    void print_sizes() { // Was used to debug at some point
        Data_object* current_node = this->Header->Right;
        std::cout << "Sizes:    ";
        while (current_node != this->Header) {
            Column_object* column = static_cast<Column_object*>(current_node);
            std::cout <<  column->Name << ": " << column->Size << ", ";
            current_node = current_node->Right;
        }
        std::cout << std::endl;
    }



    void search(int k) { // Algorithm X search function, as described in Knuth's paper
        
        // Add the solution to already found solutions (converts it to [<int> = row's index], from a list of column names as solutions are explained in Knuth's paper)
        if (this->Header->Right == this->Header) {
            // std::cout << "Solution found: " << std::endl;
            std::vector<int> solution_ids;
            for (Data_object* obj : solution) {
                solution_ids.push_back(obj->id1 - 1); // Store the row index (id1 - 1)
            }
            Solutions.push_back(solution_ids);
            // print_solution();
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


