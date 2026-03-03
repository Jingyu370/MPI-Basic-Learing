#include <iostream>
#include <cstring>
#include <string>
#include "mpi.h"
using namespace std;

#define MSG_EXIT 1
#define MSG_PRINT_ORDERED 2
#define MSG_PRINT_UNORDERED 3

int master_io(void);
int slave_io(void);

int main(int argc, char**argv){
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(rank == 0){
        master_io();
    }else{
        slave_io();
    }

    MPI_Finalize();
    return 0;
}


int master_io(void){
    int i,size,nslave,firstmsg;
    char buf[256], buf2[256];
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    nslave = size - 1;

    while(nslave > 0){
        MPI_Recv(buf, 256 ,MPI_CHAR, MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        switch(status.MPI_TAG){
            case MSG_EXIT: nslave--;break;

            case MSG_PRINT_UNORDERED: 
            cout << buf << endl;
            break;

            case MSG_PRINT_ORDERED:
            firstmsg = status.MPI_SOURCE;
            for(i = 1;i < size;i++){
                if(i == firstmsg){
                    cout << buf << endl;
                }else {
                    MPI_Recv(buf2, 256,MPI_CHAR, i , MSG_PRINT_ORDERED, MPI_COMM_WORLD, &status);
                    cout << buf2 << endl;
                }
            }
            break;
        }
    }

    return 0;
}

int slave_io(void){
    char buf[256];
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    string hello_msg = "Hello from slave " + to_string(rank) + " , ordered print\n";
    strncpy(buf, hello_msg.c_str(), sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    MPI_Send(buf, strlen(buf) + 1, MPI_CHAR, 0, MSG_PRINT_ORDERED, MPI_COMM_WORLD);

    string goodbye_msg = "Goodbye from slave " + to_string(rank) + " , ordered print\n";
    strncpy(buf, goodbye_msg.c_str(), sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    MPI_Send(buf, strlen(buf) + 1, MPI_CHAR, 0, MSG_PRINT_ORDERED, MPI_COMM_WORLD);

    string exit_unordered_msg = "I'm exiting (" + to_string(rank) + "),unordered print\n";
    strncpy(buf, exit_unordered_msg.c_str(), sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    MPI_Send(buf, strlen(buf) + 1, MPI_CHAR, 0, MSG_PRINT_UNORDERED, MPI_COMM_WORLD);

    MPI_Send(buf, 0, MPI_CHAR, 0, MSG_EXIT, MPI_COMM_WORLD);

    return 0;
}