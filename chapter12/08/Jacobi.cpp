#include <iostream>
#include <vector>
#include <iomanip>
#include "mpi.h"

using namespace std;

const int totalsize = 16; 
const int mysize = totalsize / 4; 
const int steps = 10;

int main(int argc, char** argv) {
    int myid, numprocs;
    int i, j, n;
    int upper, under;
    int tag1 = 3, tag2 = 4;
    int begin_row, end_row; 

    vector<vector<double>> a(mysize + 2, vector<double>(totalsize, 0.0));
    vector<vector<double>> b(mysize + 2, vector<double>(totalsize, 0.0));

    MPI_Request req[4];
    MPI_Status status[4];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    if (numprocs != 4) {
        if (myid == 0) cerr << "Error: 必须使用4个进程运行！" << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    cout << "Process " << myid << " of " << numprocs << " is alive" << endl;

    for (j = 0; j < totalsize; ++j) {
        a[0][j] = 8.0;
        a[mysize + 1][j] = 8.0;
    }

    if (myid == 0) {
        for (j = 0; j < totalsize; ++j) {
            a[1][j] = 8.0;
        }
    }

    if (myid == 3) {
        for (j = 0; j < totalsize; ++j) {
            a[mysize][j] = 8.0;
        }
    }

    for (i = 0; i < mysize + 2; ++i) {
        a[i][0] = 8.0;
        a[i][totalsize - 1] = 8.0;
    }

    if (myid > 0) {
        upper = myid - 1;
    } else {
        upper = MPI_PROC_NULL;
    }

    if (myid < 3) {
        under = myid + 1;
    } else {
        under = MPI_PROC_NULL;
    }

    begin_row = 1;
    end_row = mysize;

    if (myid == 0) {
        begin_row = 2;
    }

    if (myid == 3) {
        end_row = mysize - 1;
    }

    MPI_Send_init(&b[end_row][0], totalsize, MPI_DOUBLE, under, tag1, MPI_COMM_WORLD, &req[0]);
    MPI_Send_init(&b[begin_row][0], totalsize, MPI_DOUBLE, upper, tag2, MPI_COMM_WORLD, &req[1]);

    MPI_Recv_init(&a[0][0], totalsize, MPI_DOUBLE, upper, tag1, MPI_COMM_WORLD, &req[2]);
    MPI_Recv_init(&a[mysize + 1][0], totalsize, MPI_DOUBLE, under, tag2, MPI_COMM_WORLD, &req[3]);

    for (n = 0; n < steps; ++n) {
        for (j = 1; j < totalsize - 1; ++j) {
            b[begin_row][j] = 0.25 * (a[begin_row + 1][j] + a[begin_row - 1][j] + a[begin_row][j + 1] + a[begin_row][j - 1]);
            b[end_row][j] = 0.25 * (a[end_row + 1][j] + a[end_row - 1][j] + a[end_row][j + 1] + a[end_row][j - 1]);
        }

        MPI_Startall(4, req);

        for (i = begin_row + 1; i < end_row; ++i) {
            for (j = 1; j < totalsize - 1; ++j) {
                b[i][j] = 0.25 * (a[i + 1][j] + a[i - 1][j] + a[i][j + 1] + a[i][j - 1]);
            }
        }

        MPI_Waitall(4, req, status);

        for (i = begin_row; i <= end_row; ++i) {
            for (j = 1; j < totalsize - 1; ++j) {
                a[i][j] = b[i][j];
            }
        }
    }

    if (myid == 0) cout << "===== 迭代完成，各进程结果） =====" << endl;
    MPI_Barrier(MPI_COMM_WORLD);

    for (i = begin_row; i <= end_row; ++i) {
        cout << "Process " << myid << " Row " << (myid * mysize + i - 1) << ": ";
        for (j = 1; j < totalsize - 1; ++j) {
            cout << fixed << setprecision(2) << a[i][j] << " ";
        }
        cout << endl;
    }

    for (int k = 0; k < 4; ++k) {
        MPI_Request_free(&req[k]);
    }

    MPI_Finalize();
    return 0;
}