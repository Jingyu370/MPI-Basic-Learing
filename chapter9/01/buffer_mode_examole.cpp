#include <iostream>
#include "mpi.h"
using namespace std;
#define SIZE 6
static int src = 0;
static int dest = 1;

/*产生发送数据*/
void Generate_data(double *buffer, int buff_size);

/*标准接收*/
void Normal_Test_Recv(double *buffer, int buff_size);

/*缓存发送*/
void Buffered_Test_Send(double *buffer, int buffer_size);

int main(int argc, char **argv){
    int rank;
    double buffer[SIZE],*tmpbuffer,*tmpbuf;
    int tsize,bsize;
    char* Current_Test = nullptr;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == src){
        Generate_data(buffer,SIZE);
        MPI_Pack_size(SIZE,MPI_DOUBLE,MPI_COMM_WORLD, &bsize);

        tmpbuffer = (double*)malloc(bsize + 2 * MPI_BSEND_OVERHEAD);
        if(!tmpbuffer){
            cerr << "Could not allocate bsend buffer of size "<< bsize <<endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        MPI_Buffer_attach( tmpbuffer ,bsize + MPI_BSEND_OVERHEAD);
        Buffered_Test_Send(buffer,SIZE);
        MPI_Buffer_detach( &tmpbuf , &tsize);
    }else if(rank == dest){
        Normal_Test_Recv(buffer, SIZE);
    }else{
        cerr <<"*** This program uses exactly 2 processes! ***" << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Finalize();
    return 0;
}

void Generate_data(double *buffer, int buff_size){
    for(int i = 0;i < buff_size;i++){
        buffer[i] = (double)i + 1;
    }
}

void Normal_Test_Recv(double *buffer, int buff_size){
    MPI_Status status;
    double *b;

    b = buffer;
    MPI_Recv(b, (buff_size - 1),MPI_DOUBLE, src, 2000,MPI_COMM_WORLD,&status);
    cerr << "standard receive a message of " << buff_size - 1 << " data " << endl;
    for(int j = 0;j < buff_size - 1;j++){
        cerr << "buf[" << j <<"] = "<< b[j] <<endl;
    }
    b += buff_size - 1;
    MPI_Recv(b, 1, MPI_DOUBLE, src, 2000, MPI_COMM_WORLD, &status);
    cerr <<"standard receive a message of one data" << endl;
    cerr << "buf[0] = " << *b <<endl;
}

void Buffered_Test_Send(double* buffer, int buff_size) {
    void* bbuffer = nullptr;
    int size = 0;

    cerr << "Process " << src << ": buffered send message of " << buff_size - 1 << " data" << endl;
    for (int j = 0; j < buff_size - 1; j++) {
        cerr << "buf[" << j << "] = " << buffer[j] << endl;
    }

    MPI_Bsend(buffer, buff_size - 1, MPI_DOUBLE, dest, 2000, MPI_COMM_WORLD);

    buffer += buff_size - 1;

    cerr << "Process " << src << ": buffered send message of one data" << endl;
    cerr << "buf[0] = " << *buffer << endl;

    MPI_Bsend(buffer, 1, MPI_DOUBLE, dest, 2000, MPI_COMM_WORLD);

    MPI_Buffer_detach(&bbuffer, &size);
    
    //演示：缓存可以重复绑定
    MPI_Buffer_attach(bbuffer, size);
}