cmake_minimum_required(VERSION 3.26)
project(Term__Project__System_Monitor_ C)
set(CMAKE_CXX_STANDARD 11)
include_directories("C:/raylib/raylib/src")
link_directories("C:/raylib/raylib/src")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CPU)


add_executable(Term__Project__System_Monitor_
        CPU/CPUUtilities.h
        CPU/CPUUtilities.c
        Memory/Memory.h
        Memory/Memory.c
        Visualizer.c
        Process/Process.h
        Process/Process.c
        Disk_IO/DiskIO_monitoring.c
        Disk_IO/DiskIO_monitoring.h
        Network/Network.c
        Network/NetworkUtilities.h
        Disk_IO/DiskIO_monitoring.c
        raylib/raygui.c
        raylib/raygui.h
        raylib/style_cyber.h
        User/CurrentUser.c
        User/CurrentUser.h
)

target_link_libraries(Term__Project__System_Monitor_ raylib winmm pdh wevtapi)
