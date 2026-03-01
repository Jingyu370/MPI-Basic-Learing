#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv){
    int totalsize = 16;
    int numprocs, myid;
    int steps = 10;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    MPI_Status status;

    int mysize = totalsize / numprocs;

    vector<vector<double>> a(mysize + 2, vector<double>(totalsize, 0.0));
    vector<vector<double>> b(mysize + 2, vector<double>(totalsize, 0.0));

    // 左右边界赋值为 8.0 
    for (int i = 0; i < mysize + 2; ++i) {
        a[i][0] = 8.0;
        a[i][totalsize - 1] = 8.0;
    }

    // 全局顶端边界
    if (myid == 0) {
        for (int j = 0; j < totalsize; ++j) a[1][j] = 8.0;
    }

    // 全局底端边界
    if (myid == 3) {
        for (int j = 0; j < totalsize; ++j) a[mysize][j] = 8.0;
    }
    
    for (int n = 0; n < steps; ++n) {
        // 向上接收/向下发送
        if (myid == 0) {
            MPI_Recv(&a[mysize + 1][0], totalsize, MPI_DOUBLE, myid + 1, 10, MPI_COMM_WORLD, &status);
        }
        else if (myid == 3) {
            MPI_Send(&a[1][0], totalsize, MPI_DOUBLE, myid - 1, 10, MPI_COMM_WORLD);
        }else{
            MPI_Sendrecv( &a[1][0] , totalsize , MPI_DOUBLE , myid - 1 , 10 , &a[mysize + 1][0] , totalsize , MPI_DOUBLE , myid + 1 , 10 , MPI_COMM_WORLD , &status);
        }
        // 向下接收/向上发送
        if (myid == 0) {
            MPI_Send(&a[mysize][0], totalsize, MPI_DOUBLE, myid + 1, 10, MPI_COMM_WORLD);
        }
        else if (myid == 3) {
            MPI_Recv(&a[0][0], totalsize, MPI_DOUBLE, myid - 1, 10, MPI_COMM_WORLD, &status);
        }else{
            MPI_Sendrecv( &a[mysize][0] , totalsize , MPI_DOUBLE , myid + 1 , 10 , &a[0][0] , totalsize , MPI_DOUBLE , myid - 1 , 10 , MPI_COMM_WORLD , &status);
        }

        // 确定计算范围 (排除全局固定边界)
        int begin_row = 1;
        int end_row = mysize;
        if (myid == 0) begin_row = 2;
        if (myid == 3) end_row = mysize - 1;

        // Jacobi 计算：看 A 算 B
        for (int i = begin_row; i <= end_row; ++i) {
            for (int j = 1; j < totalsize - 1; ++j) {
                b[i][j] = (a[i + 1][j] + a[i - 1][j] + a[i][j + 1] + a[i][j - 1]) * 0.25;
            }
        }

        // 更新数据：把 B 拷回 A
        for (int i = begin_row; i <= end_row; ++i) {
            for (int j = 1; j < totalsize - 1; ++j) {
                a[i][j] = b[i][j];
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);  //所有进程同步
    for (int rank = 0; rank < numprocs; ++rank) {
        if (myid == rank) {
            cout << "=== Rank " << myid << " 的计算结果 ===" << endl;
            for (int i = 1; i <= mysize; ++i) {
                cout << "Rank " << myid << " Row " << i << ": ";
                for (int j = 0; j < totalsize; ++j) cout << a[i][j] << " ";
                cout << endl;
            }
            cout << endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);  //等待当前进程打印完成
    }

    MPI_Finalize();
    return 0;
}