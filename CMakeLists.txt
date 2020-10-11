cmake_minimum_required (VERSION 3.0)

project (Kuari_task)

if (${CMAKE_VERSION} VERSION_LESS "3.1")
    add_compile_options(-std=c++14)
else ()
    set (CMAKE_CXX_STANDARD 14)
    set (CMAKE_CXX_STANDARD_REQUIRED ON)
endif ()

# find_package (ACE REQUIRED)

include_directories (
    ${ACE_INCLUDE_DIRS} 
    "${CMAKE_CURRENT_SOURCE_DIR}/tlv_llib"
    )

set (TLV_LIB ${TLV_LIB} tlv_lib)

add_executable (Test_tlv test_tlv_lib.cc)
add_executable (Bit_m test.cpp)

# add_executable (Server serever_multithreaded.cc)
# add_executable (Client client.cc)

# target_link_libraries (Server ${TLV_LIB} ${ACE_LIBS})
# target_link_libraries (Client ${TLV_LIB} ${ACE_LIBS})
