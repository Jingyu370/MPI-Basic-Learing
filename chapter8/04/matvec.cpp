#include <iostream>
#include <vector>
#include "mpi.h"
using namespace std;

const int MAX_ROWS = 1000;
const int MAX_COLS = 1000;

int main(int argc, char **argv){
    int myid, master, numprocs, ierr;
    int rows = 100, cols = 100;
    MPI_Status status;

    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    master = 0;

    vector<vector<double>> a(MAX_ROWS, vector<double>(MAX_COLS));
    vector<double> b(MAX_COLS);
    vector<double> c(MAX_ROWS);
    vector<double> buffer(MAX_COLS);
    double ans;

    int numsent = 0, numrcvd = 0;
    int sender, anstype, row;

    if (myid == master) {
        // 主进程：初始化矩阵 A 和向量 b
        for (int j = 0; j < cols; ++j) {
            b[j] = 1.0;
        }
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                a[i][j] = i + 1;
            }
        }

        numsent = 0;
        numrcvd = 0;

        MPI_Bcast(b.data(), cols, MPI_DOUBLE, master, MPI_COMM_WORLD);

        for(int i = 0;i < min(numprocs - 1, rows);++i){
            for(int j = 0;j < cols;++j){
                buffer[j] = a[i][j];
            }
            MPI_Send(buffer.data(), cols, MPI_DOUBLE, i + 1, i + 1, MPI_COMM_WORLD);
            numsent++;
        }

        for(int i = 0;i < rows; ++i){
            MPI_Recv(&ans, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            sender= status.MPI_SOURCE;
            anstype = status.MPI_TAG;
            c[anstype - 1] = ans;

            if(numsent < rows){
                int next_row = numsent;
                for(int j = 0; j < cols; ++j){
                    buffer[j] = a[next_row][j];
                }
                MPI_Send(buffer.data(), cols, MPI_DOUBLE, sender, next_row + 1, MPI_COMM_WORLD);
                numsent++;
            }else {
                double dummy = 0.0;
                MPI_Send(&dummy , 1, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD);
            }
        }

        for(int i = 0; i < rows; ++i){
            cout << "c[" << i << "] = " << c[i] << endl;
        }
    }else{
            MPI_Bcast(b.data(), cols, MPI_DOUBLE, master, MPI_COMM_WORLD);
            while (true) {
            // 接收主进程发来的一行数据或终止消息
            MPI_Recv(buffer.data(), cols, MPI_DOUBLE, master, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            row = status.MPI_TAG;

            if (row == 0) {
                // 收到终止消息，退出
                break;
            }

            // 计算该行与向量 b 的点积
            ans = 0.0;
            for (int i = 0; i < cols; ++i) {
                ans += buffer[i] * b[i];
            }

            // 把结果发回主进程，tag 是行号
            MPI_Send(&ans, 1, MPI_DOUBLE, master, row, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}