#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, ierr;
    int n = 5;
    float outval = 1.0f, inval = 0.0f;
    MPI_Request req;
    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 1; i <= n; ++i) {
            MPI_Isend(&outval, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &req);
            MPI_Request_free(&req);
            
            MPI_Irecv(&inval, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, &req);
            MPI_Wait(&req, &status);
            
            cout << "Rank 0: Iteration " << i << ", received value = " << inval << endl;
            outval += 1.0f;
        }
    } else if (rank == 1) {
        MPI_Irecv(&inval, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, &status);
        cout << "Rank 1: Initial receive, value = " << inval << endl;

        for (int i = 1; i <= n-1; ++i) {
            outval = inval + 10.0f;
            MPI_Isend(&outval, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &req);
            MPI_Request_free(&req);
            
            MPI_Irecv(&inval, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &req);
            MPI_Wait(&req, &status);
            
            cout << "Rank 1: Iteration " << i << ", received value = " << inval << endl;
        }

        outval = inval + 10.0f;
        MPI_Isend(&outval, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, &status);
        cout << "Rank 1: Final send, value = " << outval << endl;
    }

    MPI_Finalize();
    return 0;
}