#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv){
    int rank ,size;
    int i;
    double x;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 3){
        cerr << "The processes must equal three!" <<endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Status status;
    i = 1;
    x = 2.7;

    if(rank == 0){
        MPI_Send(&i, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        cout << "进程 0 向进程 2 发送整型数: " << i << endl;
    }else if(rank == 1){
        MPI_Send(&x, 1, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD);
        cout << "进程 0 向进程 2 发送整型数: " << x << endl;
    }else if(rank ==2){
        int a = 0;
        double b = 0;
        
        for(int j = 0;j < 2; ++j){
            MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            if(status.MPI_SOURCE == 0){
                MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
                cout << "进程 2 从进程 0 接收整型数: " << a << endl;
            }else{
                MPI_Recv(&b, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
                cout << "进程 2 从进程 1 接收整型数: " << b << endl;
            }
        }
    }

    MPI_Finalize();
    return 0;
}