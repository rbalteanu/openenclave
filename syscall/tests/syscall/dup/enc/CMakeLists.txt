# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.


oeedl_file(../test_dup.edl enclave gen --edl-search-dir ../../../device/edl)

add_enclave(TARGET dup_enc SOURCES enc.c main.c ${gen})

target_include_directories(dup_enc PRIVATE
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(dup_enc oelibc oehostfs oesyscall oeenclave)
