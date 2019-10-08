cmake_minimum_required(VERSION 3.10.2)
project(COMPRESSOR)

set(CMAKE_CXX_STANDARD 17)

set(SRC ./src/main.cpp ./src/fileReader.cpp)
set(EXEC cmprssr)

add_executable(${EXEC} ${SRC})
