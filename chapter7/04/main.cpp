#include <iostream>
#include <cstring>
#include "mpi.h"
using namespace std;

int main(int argc, char **argv){
    int rank, value, size;
    MPI_Status status;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD , &size);

    do{
        if(rank == 0){
            cerr << "Please give new value = ";
            cin >> value;
            cerr << rank <<" read <- <-"<< value<<endl;
            if(size > 1){
               MPI_Send( &value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
               cerr << rank <<" send "<< value << "->-> " << rank+1 <<endl;
            }
        }else {
            MPI_Recv( &value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
            cerr << rank <<" receive "<< value << "<-<- " << rank-1 <<endl;

            if(rank < size - 1){
                MPI_Send( &value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                cerr << rank <<" send "<< value << "->-> " << rank+1 <<endl;
            }
        }

        MPI_Barrier( MPI_COMM_WORLD);
    }while(value >= 0);
    
    MPI_Finalize();
    return 0;
}