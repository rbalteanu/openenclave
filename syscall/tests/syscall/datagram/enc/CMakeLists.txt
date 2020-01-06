# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.


oeedl_file(../test_datagram.edl
    enclave gen --edl-search-dir ../../../device/edl)

add_enclave(TARGET datagram_enc SOURCES enc.c ${gen})

target_include_directories(datagram_enc PRIVATE
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(datagram_enc oelibc oehostsock oesyscall oeenclave)
