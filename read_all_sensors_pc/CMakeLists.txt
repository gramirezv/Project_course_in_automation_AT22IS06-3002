cmake_minimum_required(VERSION 3.10)
project(serial_read C)

set(CMAKE_C_STANDARD 99)

add_executable(read_sensors read_sensors.c)

# Link against the Windows API
target_link_libraries(read_sensors wsock32 ws2_32)
