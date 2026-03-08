#include <mpi.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm comm = MPI_COMM_WORLD;

    float a = 0.0f, b = 0.0f;
    MPI_Request req1, req2;
    MPI_Status status;

    if (rank == 0) {
        a = 1.0f;
        b = 2.0f;

        MPI_Isend(&a, 1, MPI_REAL, 1, 0, comm, &req1);
        sleep(1);
        MPI_Isend(&b, 1, MPI_REAL, 1, 0, comm, &req2);

    } else if (rank == 1) {
        MPI_Irecv(&a, 1, MPI_REAL, 0, MPI_ANY_TAG, comm, &req1);
        MPI_Irecv(&b, 1, MPI_REAL, 0, 0, comm, &req2);
    }

    MPI_Wait(&req1, &status);
    if (rank == 1) {
        std::cout << "进程1接收a = " << a << "（M1，即使M2先到）" << std::endl;
    }

    MPI_Wait(&req2, &status);
    if (rank == 1) {
        std::cout << "进程1接收b = " << b << "（M2）" << std::endl;
    }

    MPI_Finalize();
    return 0;
}