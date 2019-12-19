# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

oeedl_file(../poller.edl enclave gen --edl-search-dir "../../../edl")

add_executable(poller_enc
    enc.cpp
    ../client.cpp
    ../server.cpp
    ../poller.cpp
    ${gen})

maybe_build_using_clangw(poller_enc)

target_include_directories(poller_enc PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(poller_enc oelibcxx oeenclave oehostepoll oehostsock
    oesyscallenclave)
