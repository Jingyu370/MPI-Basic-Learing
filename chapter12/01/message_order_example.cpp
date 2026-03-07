#include <iostream>
#include "mpi.h"
using namespace std;

int main(int argc, char **argv){
    int rank;
    int *buf1, *buf2;
    char *buffer;

    MPI_Status status;
    int count = 10;
    int tag = 0;
    int bufsize = 2 * count * sizeof(int) + MPI_BSEND_OVERHEAD;

    buf1 = new int[count];
    buf2 = new int[count];
    for(int i = 0; i < count; i++){
        buf1[i] = i;
        buf2[i] = i + 100;
    }

    buffer = new char[bufsize];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0){
        MPI_Buffer_attach(buffer, bufsize);
    }

    if(rank == 0){
        cout << "Rank 0: 发送第一条消息 (buf1)" << endl;
        MPI_Bsend(buf1, count, MPI_INT, 1,tag, MPI_COMM_WORLD);

        cout << "Rank 0: 发送第二条消息 (buf2)" << endl;
        MPI_Bsend(buf2, count, MPI_INT, 1,tag, MPI_COMM_WORLD);

        MPI_Buffer_detach(buffer, &bufsize);
    }else if(rank == 1){
        cout << "Rank 1: 接收第一条消息" << endl;
        MPI_Recv(buf1, count,MPI_INT,0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        cout << "Rank 1: 接收第二条消息" << endl;
        MPI_Recv(buf2, count,MPI_INT,0, tag, MPI_COMM_WORLD, &status);

        cout << "Rank 1: 收到 buf1[0]=" << buf1[0] << ", buf2[0]=" << buf2[0] << endl;
    }

    delete[] buf1;
    delete[] buf2;
    if(rank == 0){
        delete[] buffer;
    }

    MPI_Finalize();
    return 0;
}