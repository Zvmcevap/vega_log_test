#include "pybind11/pybind11.h"
#include "test_module.h"
#include <thread>
#include <chrono>

#include <mpi.h>

void run(int rank, int loop_count, int sleep_ms)
{
    double start = MPI_Wtime();
    std::string rank_str = "\"rank\": " + std::to_string(rank) + ",";
    // if (rank == 0)
    // {
    //     std::cout << "{" << std::endl;
    // }
    // std::cout << "\"start" << std::to_string(rank) << "\": {" << rank_str + ",\"start_time\": " + std::to_string(start) << "}," << std::endl;
    // MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
    {
        std::cout << "{\n\"logs\": \n[" << std::endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = 0; i < loop_count; ++i)
    {
        std::string time = std::to_string(MPI_Wtime());
        LOGDEBUG(rank_str + " \"message\": \"Debug message  :/\", \"loop_count\": " + std::to_string(i) + ", \"time\": " + time)
        LOGINFO(rank_str + " \"message\": \"Info message   :)\", \"loop_count\": " + std::to_string(i) + ", \"time\": " + time)
        LOGERROR(rank_str + " \"message\": \"Error message  :(\", \"loop_count\": " + std::to_string(i) + ", \"time\": " + time)
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    }

    if (rank == 0)
    {
        double end = MPI_Wtime();
        double elapsed = end - start;
        std::cout << "{\"time\": 0}]," << std::endl;
        std::cout << "\"end\": {" << rank_str + " \"ellapsed_time\": " + std::to_string(elapsed) << "}" << std::endl;
        std::cout << "}" << std::endl;
    }
}

PYBIND11_MODULE(test_module, m)
{
    m.def("run", &run);
}