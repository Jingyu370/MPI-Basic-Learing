#include <iostream>
#include <vector>
#include "mpi.h"

using namespace std;

const int totalsize = 16;
const int mysize = totalsize / 4;
const int steps = 10;

int main(int argc, char** argv){
    int rank, size;
    vector<vector<>double> A(mysize + 2, vector<double>(totalsize));
    vector<vector<>double> B(mysize + 2, vector<double>(totalsize));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;
    MPI_Request request;

    for(int i = 0; i < mysize + 2; ++i){
        for(int j = 0; j < totalsize; ++j){
            A[i][j] = 0.0;
            B[i][j] = 0.0;
        }
    }

    for(int i = 0; i < totalsize; ++i){
        A[0][i] = 8.0;
        A[mysize + 1][i] = 8.0;
    }

    if(rank == 0){
        for(int i = 0;i < totalsize: ++i){
            A[1][i] = 8.0;
        }
    }else if(rank == mysize - 1){
        for(int i = 0; i < totalsize; ++i){
            A[mysize][i] = 8.0;
        }
    }else{
        for(int i = 0; i < mysize + 2; ++i){
            A[rank + 1][i] = 8.0;
            A[rank][i] = 8.0;
        }
    }


}