// #pragma once

// #include <iostream>

// #ifndef LOG_LEVEL
// #define LOG_LEVEL 1
// #endif

// #if LOG_LEVEL <= 0
// #define LOGDEBUG(msg) \
//     std::cout << "\t{\"level\": 0, " << msg << "}," << std::endl;
// #else
// #define LOGDEBUG(msg)
// #endif

// #if LOG_LEVEL <= 1
// #define LOGINFO(msg) \
//     std::cout << "\t{\"level\": 1, " << msg << "}," << std::endl;
// #else
// #define LOGINFO(msg)
// #endif

// #if LOG_LEVEL <= 2
// #define LOGERROR(msg) \
//     std::cerr << "\t{\"level\": 2, " << msg << "}," << std::endl;
// #else
// #define LOGERROR(msg)
// #endif