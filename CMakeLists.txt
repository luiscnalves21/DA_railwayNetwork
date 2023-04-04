cmake_minimum_required(VERSION 3.23)
project(DA_railwayNetwork)

set(CMAKE_CXX_STANDARD 14)

add_executable(DA_railwayNetwork main.cpp src/GestaoR.cpp src/Menu.cpp data_structures/Graph.cpp data_structures/VertexEdge.cpp)
