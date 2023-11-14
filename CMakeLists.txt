cmake_minimum_required(VERSION 3.26)
project(Term__Project__System_Monitor_ C)

set(CMAKE_C_STANDARD 11)

add_executable(Term__Project__System_Monitor_
        CPU/CPUUtilities.h
        Visualizer.c
        CPU/CPUUtilities.c
        Memory/Memory.h
        Memory/Memory.c
        Disk_IO/DiskIO_monitoring.c
        Disk_IO/DiskIO_monitoring.h
)

target_link_libraries(Term__Project__System_Monitor_ pdh)
