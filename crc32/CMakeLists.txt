# CMakeList.txt : CMake project for CRC16, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


project(CRC32)

# Add source to this project's executable.
add_executable (crc32 "crc32.c" "crc32.h")

# TODO: Add tests and install targets if needed.
