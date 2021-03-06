# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

##==============================================================================
##
## Prefix where Open Enclave is installed.
##
##==============================================================================

set(PREFIX "${CMAKE_INSTALL_PREFIX}")

##==============================================================================
##
## Enclave compiler flags:
##
##==============================================================================

set(ENCLAVE_CINCLUDES
    "-I\${includedir}/openenclave/3rdparty/libc -I\${includedir}/openenclave/3rdparty -I\${includedir}")

set(ENCLAVE_CXXINCLUDES
    "-I\${includedir}/openenclave/3rdparty/libcxx ${ENCLAVE_CINCLUDES}")

set(ENCLAVE_CFLAGS_LIST
  -nostdinc
  -m64
  -fPIE
  -ftls-model=local-exec
  -fvisibility=hidden
  -fno-stack-protector
  -ffunction-sections
  -fdata-sections)

set(ENCLAVE_CFLAGS_CLANG_LIST ${ENCLAVE_CFLAGS_LIST} ${SPECTRE_MITIGATION_FLAGS})
list(JOIN ENCLAVE_CFLAGS_CLANG_LIST " " ENCLAVE_CFLAGS_CLANG)

list(JOIN ENCLAVE_CFLAGS_LIST " " ENCLAVE_CFLAGS_GCC)

##==============================================================================
##
## Enclave linker flags:
##
##==============================================================================

set(ENCLAVE_CLIBS_1
  -nostdlib
  -nodefaultlibs
  -nostartfiles
  -Wl,--no-undefined
  -Wl,-Bstatic
  -Wl,-Bsymbolic
  -Wl,--export-dynamic
  -Wl,-pie
  -Wl,--build-id
  -Wl,-z,noexecstack
  -Wl,-z,now
  -Wl,-gc-sections
  -L\${libdir}/openenclave/enclave
  -loeenclave
  -loecryptombed
  -lmbedx509
  -lmbedcrypto)

set(ENCLAVE_CLIBS_2 -loelibc -loesyscall -loecore)

set(ENCLAVE_CLIBS_LIST ${ENCLAVE_CLIBS_1} ${ENCLAVE_CLIBS_2})
list(JOIN ENCLAVE_CLIBS_LIST " " ENCLAVE_CLIBS)

set(ENCLAVE_CXXLIBS_LIST ${ENCLAVE_CLIBS_1} -loelibcxx ${ENCLAVE_CLIBS_2})
list(JOIN ENCLAVE_CXXLIBS_LIST " " ENCLAVE_CXXLIBS)

##==============================================================================
##
## Host compiler flags:
##
##==============================================================================

set(HOST_INCLUDES "-I\${includedir}")

set(HOST_CFLAGS_CLANG_LIST -fstack-protector-strong ${SPECTRE_MITIGATION_FLAGS})
list(JOIN HOST_CFLAGS_CLANG_LIST " " HOST_CFLAGS_CLANG)

set(HOST_CXXFLAGS_CLANG ${HOST_CFLAGS_CLANG})

set(HOST_CFLAGS_GCC_LIST -fstack-protector-strong -D_FORTIFY_SOURCE=2)
list(JOIN HOST_CFLAGS_GCC_LIST " " HOST_CFLAGS_GCC)

set(HOST_CXXFLAGS_GCC ${HOST_CFLAGS_GCC})

##==============================================================================
##
## Host linker flags:
##
##==============================================================================

if(HAS_QUOTE_PROVIDER)
    set(SGX_LIBS "-lsgx_enclave_common -lsgx_dcap_ql -lsgx_urts")
else()
    set(SGX_LIBS "-lsgx_enclave_common -lsgx_urts")
endif()

set(HOSTVERIFY_CLIBS "-rdynamic -Wl,-z,noexecstack -L\${libdir}/openenclave/host -loehostverify -ldl -lpthread")
set(HOST_CLIBS "-rdynamic -Wl,-z,noexecstack -L\${libdir}/openenclave/host -loehost -ldl -lpthread ${SGX_LIBS}")

set(HOSTVERIFY_CXXLIBS "${HOSTVERIFY_CLIBS}")
set(HOST_CXXLIBS "${HOST_CLIBS}")

##==============================================================================
##
## oeenclave-gcc.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oeenclave-gcc.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oeenclave-gcc.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oeenclave-gcc.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig")

##==============================================================================
##
## oeenclave-g++.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oeenclave-g++.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oeenclave-g++.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oeenclave-g++.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig")

##==============================================================================
##
## oehost-gcc.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oehost-gcc.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehost-gcc.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehost-gcc.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig")

##==============================================================================
##
## oehost-g++.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oehost-g++.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehost-g++.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehost-g++.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig")

##==============================================================================
##
## oehostverify-gcc.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oehostverify-gcc.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehostverify-gcc.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehostverify-gcc.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig"
    COMPONENT OEHOSTVERIFY)

##==============================================================================
##
## oehostverify-g++.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oehostverify-g++.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehostverify-g++.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehostverify-g++.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig"
    COMPONENT OEHOSTVERIFY)

##==============================================================================
##
## oeenclave-clang.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oeenclave-clang.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oeenclave-clang.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oeenclave-clang.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig")

##==============================================================================
##
## oeenclave-clang++.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oeenclave-clang++.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oeenclave-clang++.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oeenclave-clang++.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig")

##==============================================================================
##
## oehost-clang.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oehost-clang.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehost-clang.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehost-clang.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig")

##==============================================================================
##
## oehost-clang++.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oehost-clang++.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehost-clang++.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehost-clang++.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig")

##==============================================================================
##
## oehostverify-clang.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oehostverify-clang.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehostverify-clang.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehostverify-clang.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig"
    COMPONENT OEHOSTVERIFY)

##==============================================================================
##
## oehostverify-clang++.pc:
##
##==============================================================================

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/oehostverify-clang++.pc
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehostverify-clang++.pc
    @ONLY)

install(FILES
    ${CMAKE_BINARY_DIR}/output/share/pkgconfig/oehostverify-clang++.pc
    DESTINATION
    "${CMAKE_INSTALL_DATADIR}/pkgconfig"
    COMPONENT OEHOSTVERIFY)
