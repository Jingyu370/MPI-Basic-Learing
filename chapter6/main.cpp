#include <iostream>
#include <cstring>
#include "mpi.h"
using namespace std;

int main(int argc, char** argv) {
    MPI_Status status;
    char message[20];
    int myrank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank == 0) {
        strcpy(message, "Hello, process 1");
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, 99, MPI_COMM_WORLD);
        // cout << "send:" << message << endl; 
    } else if (myrank == 1) {
        MPI_Recv(message, 20, MPI_CHAR, 0, 99, MPI_COMM_WORLD, &status);
        cout << "received:" << message << endl;
    }

    MPI_Finalize();
    return 0;
}