#include <iostream>
#include <vector>
#include "mpi.h"
using namespace std;

#define MSG_EXIT 1
#define MSG_PRINT_ORDERED 2
#define MSG_PRINT_UNNORDERED 3

int master_io(void){
    int i,j,size,nslave,firstmsg;
    char buf[256], buf2[256];
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    nslave = size - 1;

    while(nslave > 0){
        MPI_Recv(buf, MPI_CHAR, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&stautus);
        
    }
}
int main(int argc, char**argv){
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(rank == 0){
        master_io();
    }else{
        slave_io();
    }
    return 0;
}