# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

oeedl_file(../epoll.edl host gen)

add_executable(epoll_host host.cpp ${gen})

target_include_directories(epoll_host PRIVATE
    ${PROJECT_SOURCE_DIR}/syscall/include
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(epoll_host oesyscallhost oehostapp)
