cmake_minimum_required(VERSION 3.26)
project(Term__Project__System_Monitor_ C)

set(CMAKE_C_STANDARD 11)

add_executable(Term__Project__System_Monitor_
        CPU/CPUUtilities.c
        Memory/Memory.c)

# If you are using Windows and need to link with the pdh library for CPU monitoring
if (WIN32)
    target_link_libraries(Term__Project__System_Monitor_ pdh)
endif()
