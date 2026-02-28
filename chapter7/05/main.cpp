#include <iostream>
#include <cstdlib>
#include <cstring>
#include "mpi.h"
using namespace std;

void Hello();

int main(int argc, char **argv){
    int me, namelen, size;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&me); 
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    
    if(size < 2){
        cerr << "systest requires at least 2 processes"<<endl;
        MPI_Abort(MPI_COMM_WORLD , 1);
    }

    MPI_Get_processor_name(processor_name , &namelen);
    cout << "Process " << me <<" is alive on "<< processor_name << endl << flush;
    MPI_Barrier(MPI_COMM_WORLD);

    Hello();

    MPI_Finalize();
    return 0;
}

/*任意两个进程间交换问候信息 问候信息由发送进程标识和接收进程标识组成*/
void Hello(){
    int nproc ,me;
    int type = 1;
    int buffer[2], node;

    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    if(me == 0){
        cout << "\nHello test from all to all\n" << flush;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    
    for(node = 0;node < nproc; node++){
        if(node != me){
            buffer[0] = me;
            buffer[1] = node;
            MPI_Send(buffer, 2, MPI_INT, node, type, MPI_COMM_WORLD);
            MPI_Recv(buffer, 2, MPI_INT, node, type, MPI_COMM_WORLD, &status);

        if((buffer[0] != node) || (buffer[1] != me)){
            cerr << "Hello: "<< buffer[0] << " !=" << node <<" or "<< buffer[1] <<"!= "<< me <<endl;
            cout << "Mismatch on hello process ids: node = "<< node << endl;
        }
            cout << "Hello from " << me << " to "<< node << "\n" << flush;
        }
    }
}
