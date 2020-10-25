project (tlv_lib)

set(SOURCES "tlv_lib.cc")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(tlv_lib SHARED ${SOURCES})
target_include_directories(tlv_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
