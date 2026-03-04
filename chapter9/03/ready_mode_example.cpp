#include <iostream>
#include <vector>
#include "mpi.h"
using namespace std;

void test_rsend() {
    const int TEST_SIZE = 2000;
    int ierr, prev, next, count, tag, rank, size;
    MPI_Status status;
    MPI_Request request;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        cerr << "This test requires exactly 2 processes" << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    next = rank + 1;
    if (next >= size) next = 0;
    prev = rank - 1;
    if (prev < 0) prev = size - 1;

    if (rank == 0) {
        cout << " Rsend Test " << endl;
    }

    tag = 1456;
    count = TEST_SIZE / 3;

    vector<float> send_buf(TEST_SIZE);
    vector<float> recv_buf(TEST_SIZE);

    if (rank == 0) {
        MPI_Recv(MPI_BOTTOM, 0, MPI_INT, next, tag, MPI_COMM_WORLD, &status);
        
        cout << "Process " << rank << " post Ready send" << endl;
        
        MPI_Rsend(send_buf.data(), count, MPI_FLOAT, next, tag, MPI_COMM_WORLD);
    } else {
        cout << "process " << rank << " post a receive call" << endl;
        
        MPI_Irecv(recv_buf.data(), TEST_SIZE, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
        
        MPI_Send(MPI_BOTTOM, 0, MPI_INT, prev, tag, MPI_COMM_WORLD);
        
        MPI_Wait(&request, &status);
        
        cout << "Process " << rank << " Receive Rsend message from " << status.MPI_SOURCE << endl;
    }
}

int main(int argc, char** argv) {
    int ierr;

    MPI_Init(&argc, &argv);

    test_rsend();

    MPI_Finalize();

    return 0;
}