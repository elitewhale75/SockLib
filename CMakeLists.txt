cmake_minimum_required(VERSION 3.22.1)
project(SockLib)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES})
set_target_properties(${PROJECT_NAME} 
    PROPERTIES PUBLIC_HEADER "include/SockLib/sock.h")

install (TARGETS ${PROJECT_NAME} 
    LIBRARY DESTINATION lib 
    PUBLIC_HEADER DESTINATION include/sock)
