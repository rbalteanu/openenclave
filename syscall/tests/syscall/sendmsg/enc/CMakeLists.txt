# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

oeedl_file(../sendmsg.edl enclave gen)

##==============================================================================
##
## sendmsg_enc:
##
##==============================================================================

add_executable(sendmsg_enc enc.c ../client.c ../server.c ${gen})

maybe_build_using_clangw(sendmsg_enc)

target_include_directories(sendmsg_enc PRIVATE
    ${PROJECT_SOURCE_DIR}/syscall/include
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(sendmsg_enc oehostsock oesyscall oeenclave)
