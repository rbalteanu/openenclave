# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/syscall.edl
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/edl/syscall.edl"
    COMPONENT OEHOSTVERIFY)
