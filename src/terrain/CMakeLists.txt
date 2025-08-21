set(MODULE_NAME terrainDemo)
add_executable(${MODULE_NAME} main.cpp)

add_subdirectory(../base)
add_subdirectory(../inputController)

target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${MODULE_NAME} physics core math openGL audio test)