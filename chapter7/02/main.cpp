#include <mpi.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    int ierr = MPI_Init(&argc, &argv);
    if (ierr != MPI_SUCCESS) {
        std::cerr << "MPI 初始化失败！" << std::endl;
        return -1;
    }

    const int MAX_NAME_LEN = MPI_MAX_PROCESSOR_NAME;
    char name[MAX_NAME_LEN];
    int resultlen;
    int version, subversion;
    int errs = 0; 

    for (int i = 0; i < MAX_NAME_LEN; ++i) {
        name[i] = ' ';
    }

    MPI_Get_processor_name(name, &resultlen);

    MPI_Get_version(&version, &subversion);

    for (int i = resultlen; i < MAX_NAME_LEN; ++i) {
        if (name[i] != ' ' && name[i] != '\0') {
            errs++;
        }
    }

    if (errs > 0) {
        std::cout << "Non-blanks after name" << std::endl;
    } else {
        std::string processor_name(name, resultlen);
        std::cout << processor_name << " MPI version " << version << "." << subversion << std::endl;
    }

    MPI_Finalize();

    return 0;
}