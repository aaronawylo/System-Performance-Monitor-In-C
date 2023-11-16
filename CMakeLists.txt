cmake_minimum_required(VERSION 3.26)
project(Term__Project__System_Monitor_ C)

set(CMAKE_C_STANDARD 11)

add_executable(Term__Project__System_Monitor_
        CPU/CPUUtilities.h
        CPU/CPUUtilities.c
        Visualizer.c
        Process/Process.h
        Process/Process.c
        Uptime/Uptime.c
        Uptime/Uptime.h
        Disk_IO/DiskIO_monitoring.c
        Disk_IO/DiskIO_monitoring.h
        Memory/Memory.c
        Network/Network.c
        Network/NetworkUtilities.h
)

target_link_libraries(Term__Project__System_Monitor_ pdh wevtapi)
