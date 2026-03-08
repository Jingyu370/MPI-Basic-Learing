#include <iostream>
#include "mpi.h"
#include <cstdio>
using namespace std;

const int TOTAL_SIZE = 16;
const int MY_SIZE = TOTAL_SIZE / 4;
const int STEPS = 10;

int main(int argc, char** argv) {
    int myid, numprocs;
    int n, i, j;
    int begin_row = 0, end_row = 0;
    int up, down;
    const int tag1 = 3, tag2 = 4;
    
    MPI_Init(&argc, &argv);
    MPI_Status status[4];
    MPI_Request req[4];

    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    if (numprocs != 4) {
        if (myid == 0) {
            cout << "Error: Must run with 4 processes! (mpirun -np 4)" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    cout << "Process " << myid << " of " << numprocs << " is alive. Responsible for rows: " 
         << begin_row << " - " << end_row << endl;
    cout.flush();

    double a[TOTAL_SIZE + 2][MY_SIZE] = {0.0};
    double b[TOTAL_SIZE + 2][MY_SIZE] = {0.0};

    for (j = 0; j < MY_SIZE; j++) {
        a[0][j] = 8.0;
        a[TOTAL_SIZE + 1][j] = 8.0;
    }
    for (i = 0; i < TOTAL_SIZE + 2; i++) {
        a[i][0] = 8.0;
        a[i][MY_SIZE - 1] = 8.0;
    }

    if (myid == 0) {
        for (j = 0; j < MY_SIZE; j++) {
            a[1][j] = 8.0;
        }
    }

    if (myid == 3) {
        for (j = 0; j < MY_SIZE; j++) {
            a[TOTAL_SIZE][j] = 8.0;
        }
    }

    up = (myid > 0) ? myid - 1 : MPI_PROC_NULL;
    down = (myid < 3) ? myid + 1 : MPI_PROC_NULL;

    int rows_per_process = TOTAL_SIZE / numprocs;
    begin_row = myid * rows_per_process + 1;
    end_row = (myid + 1) * rows_per_process;
    if (myid == 0) begin_row = 2;
    if (myid == 3) end_row = TOTAL_SIZE - 1;

    for (n = 0; n < STEPS; n++) {
        for (i = begin_row; i <= end_row; i++) {
            for (j = 1; j < MY_SIZE - 1; j++) {
                b[i][j] = 0.25 * (
                    a[i + 1][j] + a[i - 1][j] +
                    a[i][j + 1] + a[i][j - 1]
                );
            }
        }

        MPI_Isend(&b[end_row][0], MY_SIZE, MPI_DOUBLE, down, tag1, MPI_COMM_WORLD, &req[0]);
        MPI_Isend(&b[begin_row][0], MY_SIZE, MPI_DOUBLE, up, tag2, MPI_COMM_WORLD, &req[1]);

        MPI_Irecv(&a[begin_row - 1][0], MY_SIZE, MPI_DOUBLE, up, tag1, MPI_COMM_WORLD, &req[2]);
        MPI_Irecv(&a[end_row + 1][0], MY_SIZE, MPI_DOUBLE, down, tag2, MPI_COMM_WORLD, &req[3]);

        for (i = 0; i < 4; i++) {
            MPI_Wait(&req[i], &status[i]);
        }

        for (i = begin_row; i <= end_row; i++) {
            for (j = 1; j < MY_SIZE - 1; j++) {
                a[i][j] = b[i][j];
            }
        }
    }

    cout << "===== Process " << myid << " Result =====" << endl;
    for (i = begin_row; i <= end_row; i++) {
        cout << "Process " << myid << " , Row: " << i << "  ";
        for (j = 0; j < MY_SIZE; j++) {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
    cout.flush();

    MPI_Finalize();
    return 0;
}