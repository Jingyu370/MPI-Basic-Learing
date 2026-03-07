#include <iostream>
#include <vector>
#include "mpi.h"
using namespace std;

int main(int argc, char **argv){
    int rank ,size, sbuf, rbuf = 0;
    int tag = 99;
    int flag;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 2){
        cerr << "Error: This program requires exactly 2 processes!" <<endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Status status;
    MPI_Request request;
    
    sbuf = 10;
    if(rank == 0){
        MPI_Send(&sbuf, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
    }else if(rank == 1){
        MPI_Irecv(&rbuf, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &request);
        
        MPI_Cancel(&request);
        MPI_Wait(&request, &status);
        MPI_Test_cancelled( &status , &flag);
        cout << "Rank 1: Cancel success flag = " << (flag ? "true" : "false") << endl;
        if(flag){
            MPI_Irecv(&rbuf, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &request);
            MPI_Wait(&request, &status);
            cout << "Rank 1: Re-receive completed, rbuf = " << rbuf << endl;
        }else{
            cout << "Rank 1: Receive not cancelled, rbuf = " << rbuf << endl;
        }
    }

    MPI_Finalize();
    return 0;
}