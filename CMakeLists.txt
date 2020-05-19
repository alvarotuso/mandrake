cmake_minimum_required(VERSION 3.15)
project(mandrake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
    mandrake
    src/main.cpp
    src/server/server.cpp
    src/model/app.cpp
    src/model/request.cpp
    src/model/response.cpp
    src/utils/iterators.cpp
)

set_target_properties(mandrake PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(mandrake pthread)