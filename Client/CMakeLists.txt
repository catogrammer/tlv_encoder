project (Client)

set(SOURCES client.cc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable (Client ${SOURCES})
target_link_libraries(Client ${ACE_LINK_LIB})

