The point of this project is to so make a solver for the **exact cover problem**. This problem is about finding a subset of rows in a boolean matrix that so that there is exactly only one 1 for each column. This problem is very potent as it can be extrapolated to much more complicated problems such as pentaminoes or other, simply by translating the problem to a boolean matrix

This project was vastly based on the work of **Donald Knuth** as the algorithm used is his **"Algorithm X"**

The **code** directory is comprised of **Classes.cpp** and **Algorithm X**. The first file allows to create all the classes needed, primarly Diagram. The latter is comprised of the main() function, which goal is to benchmark on the given data an this interpretation of Algorithm X

The **data** directory contains all the problems and solutions used for benchmarking

Donald Knuth's papers used to make this are present in the **resources** direactory.