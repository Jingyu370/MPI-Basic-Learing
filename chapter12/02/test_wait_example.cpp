#include <iostream>
#include <vector>
#include "mpi.h"
using namespace std;

const int count = 20;

int main(int argc, char **argv){
    int rank, size;
    int tag = 20;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    vector<int> a(count);

    if(size != 2){
        cerr << "More than two processes!" << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Status status;
    MPI_Request request;
    int flag = 0;

    for(int i = 0;i < count ; ++i){
        a[i] = i + 1;
    }

    if(rank == 0){
        cout << "进程0发送"<<endl;
        MPI_Isend(&a[0] ,10, MPI_INT, 1, tag, MPI_COMM_WORLD, &request);

        while(!flag){
            MPI_Test(&request, &flag, &status);
            cout << "进程 0 等待发送完成..." << endl;
        }
        cout << "进程0非阻塞发送完成" <<endl;
    }else if(rank == 1){
        cout << "进程1接收"<<endl;
        MPI_Irecv(&a[0], 15, MPI_INT, 0, tag, MPI_COMM_WORLD, &request);

        MPI_Wait(&request, &status);
        cout << "进程1非阻塞接收完成" <<endl;
    }

    MPI_Finalize();
    return 0;
}