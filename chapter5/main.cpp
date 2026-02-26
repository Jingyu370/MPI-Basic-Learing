#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv) {
    // 1. 初始化 MPI
    int rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) {
        cerr << "MPI_Init failed" << endl;
        return 1;
    }

    // 2. 定义变量
    int myid, numprocs, namelen, ierr;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    // 3. 获取进程信息
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Get_processor_name(processor_name, &namelen);

    // 4. 输出信息（和 Fortran 例子对应）
    cout << "Hello World! Process " << myid 
              << " of " << numprocs 
              << " on " << processor_name << endl;

    // 5. 结束 MPI
    MPI_Finalize();

    return 0;
}