cmake_minimum_required (VERSION 3.0)

project (Kuari_task)

if (${CMAKE_VERSION} VERSION_LESS "3.1")
    add_compile_options(-std=c++14)
else ()
    set (CMAKE_CXX_STANDARD 14)
    set (CMAKE_CXX_STANDARD_REQUIRED ON)
endif ()

set(ACE_LINK_LIB  "libACE.so")
set(ACE_INCLUDE_DIRS "/usr/local/include/ace")

include_directories (
    ${ACE_INCLUDE_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/tlv_lib"
    )
link_directories("/usr/local/lib/ace")

add_subdirectory (tlv_lib)
set (TLV_LIB ${TLV_LIB} tlv_lib)

add_executable (Test_tlv test_tlv_lib.cc)
add_executable (Bit_m test.cpp)
add_executable (Server_MT server_multithreaded.cc)
add_executable (Client client.cc)

target_link_libraries(Server_MT ${ACE_LINK_LIB})
target_link_libraries(Client ${ACE_LINK_LIB})

target_link_libraries (Test_tlv ${TLV_LIB})

