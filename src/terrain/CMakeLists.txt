#executable
set(MODULE_NAME terrainDemo)
add_executable(${MODULE_NAME} TerrainDemo.cpp)

#sources
target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../base)
target_link_libraries(${MODULE_NAME} playground openGLRunner audioRunner physicsRunner)

#resources
file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/resources")
#resources - clean and copy from scratch
add_custom_command( 
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf
            resources)
add_custom_command( 
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/../../resources
            resources)
