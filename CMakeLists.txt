cmake_minimum_required(VERSION 3.22.1)
project(IPCLib)
add_library(${PROJECT_NAME} ipc.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ipc.h)
install (TARGETS ${PROJECT_NAME} DESTINATION lib
    PUBLIC_HEADER DESTINATION include/ipclib)
