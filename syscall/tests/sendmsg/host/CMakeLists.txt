# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.


oeedl_file(../sendmsg.edl host gen --edl-search-dir "../../../edl")

add_executable(sendmsg_host host.c ../client.c ../server.c ${gen})

target_include_directories(sendmsg_host PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(sendmsg_host oesyscallhost oehostapp)
