#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include <mpi.h>
#include <pybind11/pybind11.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL 0
#endif

// Compile-time filter for SPDLOG_* macros
#define SPDLOG_ACTIVE_LEVEL (LOG_LEVEL + 1)

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

// ----------------------------------------------------------------------------
// logger setup
// ----------------------------------------------------------------------------

static std::shared_ptr<spdlog::logger> make_logger_for_rank(int rank, bool async = false)
{
    const std::string rank_prefix = "{\"rank\": " + std::to_string(rank) + ", ";

    if (async)
    {
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
        stdout_sink->set_pattern(rank_prefix + "%v },");

        // Runtime level (must be consistent with LOG_LEVEL meaning)
        stdout_sink->set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
        static bool thread_pool_initialized = false;
        if (!thread_pool_initialized)
        {
            spdlog::init_thread_pool(8192, 1);
            thread_pool_initialized = true;
        }

        std::vector<spdlog::sink_ptr> sinks{stdout_sink};
        auto logger = std::make_shared<spdlog::async_logger>(
            "vega_logger",
            sinks.begin(),
            sinks.end(),
            spdlog::thread_pool(),
            spdlog::async_overflow_policy::block);

        return logger;
    }
    else
    {
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_sink_st>();
        stdout_sink->set_pattern(rank_prefix + "%v },");
        stdout_sink->set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
        auto logger = std::make_shared<spdlog::logger>("vega_logger", stdout_sink);
        logger->set_level(
            static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));

        return logger;
    }
}

// ----------------------------------------------------------------------------
// core function
// ----------------------------------------------------------------------------

void run(int rank, int loop_count, int sleep_ms, bool async_logger = false)
{
    auto logger = make_logger_for_rank(rank, async_logger);
    spdlog::set_default_logger(logger);

    const double start = MPI_Wtime();
    if (rank == 0)
    {
        std::cout << "{\n";
        std::cout << "\"start\": {"
                  << "\"rank\": " << rank << ", "
                  << "\"start_time\": " << start
                  << "},\n"
                  << "\"logs\": [\n";
    }

    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = 0; i < loop_count; ++i)
    {
        const double t = MPI_Wtime();
        const std::string time_str = std::to_string(t);

        SPDLOG_DEBUG(
            "\"message\": \"Debug message  :/\", "
            "\"loop_count\": {}, \"time\": {}",
            i, time_str);
        SPDLOG_INFO(
            "\"message\": \"Info message   :)\", "
            "\"loop_count\": {}, \"time\": {}",
            i, time_str);
        SPDLOG_ERROR(
            "\"message\": \"Error message  :(\", "
            "\"loop_count\": {}, \"time\": {}",
            i, time_str);

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    }

    // Make sure async logs are flushed before printing footer
    spdlog::default_logger()->flush();
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
    {
        const double end = MPI_Wtime();
        const double elapsed = end - start;

        // your hacky final entry to close the JSON array
        std::cout << "{\"time\": 0}],\n";
        std::cout << "\"end\": {"
                  << "\"rank\": " << rank << ", "
                  << "\"elapsed_time\": " << elapsed
                  << "}\n";
        std::cout << "}\n";
    }
}

// ----------------------------------------------------------------------------
// pybind11 module
// ----------------------------------------------------------------------------

PYBIND11_MODULE(test_module, m)
{
    m.def("run", &run, "Run MPI logging loop",
          pybind11::arg("rank"),
          pybind11::arg("loop_count"),
          pybind11::arg("sleep_ms"),
          pybind11::arg("async_logger"));
}
