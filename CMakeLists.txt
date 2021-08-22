cmake_minimum_required(VERSION 3.0.0)
project(a.out VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")

set(CMAKE_CXX_FALGS "$ {CMAKE_CXX_FLAGS} $ {GCC_COVERAGE_COMPILE_FLAGS}") 


include(CTest)
enable_testing()

add_executable(a.out timeseries.cpp anomaly_detection_util.cpp SimpleAnomalyDetector.cpp MainTrain.cpp HybridAnomalyDetector.cpp minCircle.cpp CLI.cpp Server.cpp)

  



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
