cmake_minimum_required (VERSION 3.0)

project (Kuari_task)

if (${CMAKE_VERSION} VERSION_LESS "3.1")
    add_compile_options(-std=c++14)
else ()
    set (CMAKE_CXX_STANDARD 14)
    set (CMAKE_CXX_STANDARD_REQUIRED ON)
endif ()

set (ACE_LINK_LIB  "libACE.so")
set (ACE_INCLUDE_DIRS "/usr/local/include/ace")
set (ACE_LINK_LIBS "/usr/local/lib/ace")

include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/General
    ${ACE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/tlv_lib
    )

link_directories (
    ${ACE_LINK_LIBS}
    )

add_subdirectory (tlv_lib)
set (TLV_LIB ${TLV_LIB} tlv_lib)

add_subdirectory (Client)
add_subdirectory (Server)

add_executable (Test_tlv test_tlv_lib.cc)
target_link_libraries (Test_tlv ${TLV_LIB})


