#include <iostream>
#include "mpi.h"
using namespace std;

#define SIZE 10
static int src = 0;
static int dest = 1;

int main(int argc, char **argv){
    int rank = 0;
    int act_size = 5;
    int flag, np, rval;
    int buffer[SIZE];

    MPI_Status status, status1, status2;
    MPI_Init(&argc ,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(np != 2){
        cerr << "*** This program uses exactly 2 processes! ***" <<endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    act_size = 5;

    if(rank == src){
        act_size = 1;
        MPI_Ssend(buffer, act_size, MPI_INT, dest, 1,MPI_COMM_WORLD, );
    }
    return 0;
}