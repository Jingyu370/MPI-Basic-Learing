#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "mpi.h"

using namespace std;

int main(int argc, char** argv) {
    int err = 0;
    double t1, t2;
    double tick;
    int i;

    MPI_Init(&argc, &argv);

    // 测试连续两次MPI_Wtime的合理性
    t1 = MPI_Wtime();
    t2 = MPI_Wtime();
    if (t2 - t1 > 0.1 || t2 - t1 < 0.0) {
        err++;
        cerr << "Two successive calls to MPI_Wtime gave strange results: (" 
             << t1 << ") (" << t2 << ")" << endl;
    }

    // 循环测试sleep(1)的计时准确性
    for (i = 0; i < 10; i++) {
        t1 = MPI_Wtime();
        sleep(1); 
        t2 = MPI_Wtime();

        if (t2 - t1 >= (1.0 - 0.01) && t2 - t1 <= 5.0) {
            break;
        }
        if (t2 - t1 > 5.0) {
            i = 9;
        }
    }

    if (i == 10) {
        err++;
        cerr << "Timer around sleep(1) did not give 1 second; gave " 
             << t2 - t1 << endl;
    }

    // 测试MPI_Wtick的返回值合理性
    tick = MPI_Wtick();
    if (tick > 1.0 || tick < 0.0) {
        err++;
        cerr << "MPI_Wtick gave a strange result: (" << tick << ")" << endl;
    }

    if (err == 0) {
    printf("All MPI timing tests passed successfully!\n");
    }
    
    MPI_Finalize();

    return err;
}