# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

oeedl_file(../epoll.edl enclave gen)

add_executable(epoll_enc enc.cpp ${gen})

maybe_build_using_clangw(epoll_enc)

target_include_directories(epoll_enc PRIVATE
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(epoll_enc
    oelibcxx oeenclave oehostepoll oesyscall oehostsock)
