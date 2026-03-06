#include <iostream>
#include "mpi.h"
using namespace std;

int main(int argc, char **argv){
    int rank;
    int *buf1, *buf2;
    MPI_Status status;
    int count = 10;
    int tag = 0;
    buf1 = new int[count];
    buf2 = new int[count];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0){
        MPI_Bsend(buf1, count, MPI_REAL, 1,tag, MPI_COMM_WORLD);
        MPI_Bsend(buf2, count, MPI_REAL, 1,tag, MPI_COMM_WORLD);
    }else if(rank == 1){
        MPI_Recv(buf1, count,MPI_REAL,0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(buf2, count,MPI_REAL,0, tag, MPI_COMM_WORLD, &status);
    }

    MPI_Finalize();
    return 0;
}