# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

set(EDL_FILE "../fs.edl")

set(EDL_SEARCH_DIR "../../../edl")

oeedl_file(${EDL_FILE} enclave gen --edl-search-dir ${EDL_SEARCH_DIR})

add_enclave(TARGET fs_enc SOURCES enc.cpp ${gen})

target_include_directories(fs_enc PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/syscall/include)

target_link_libraries(fs_enc
    ${OESGXFSENCLAVE} oelibcxx oecpio oesyscallenclave oeenclave oehostfs)
