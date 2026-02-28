#include <mpi.h>
#include <iostream>
#include <cstring>

/* 
 * MPI主动退出执行的C++示例
 * 两种模式：
 * 1. 默认模式：进程0为主进程
 * 2. 指定模式：传入lastmaster参数，最后一个进程为主进程
*/

int main(int argc, char** argv) {
    int node, size, i;
    int masternode = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &node);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (i = 1; i < argc; ++i) {
        std::cerr << "myid=" << node << ",procs=" << size 
                  << ",argv[" << i << "]=" << argv[i] << std::endl;
        
        if (argv[i] && std::strcmp("lastmaster", argv[i]) == 0) {
            masternode = size - 1;
        }
    }

    if (node == masternode) {
        std::cerr << "myid=" << node << " is masternode Abort!" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 99);
    } else {
        std::cerr << "myid=" << node << " is not masternode Barrier!" << std::endl;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}