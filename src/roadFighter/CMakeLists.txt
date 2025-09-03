#executable
set(MODULE_NAME roadFighter)
add_executable(${MODULE_NAME} RoadFighter.cpp)

#sources
target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../base)
target_link_libraries(${MODULE_NAME} playground openGLRunner audioRunner physicsRunner)

set_target_properties(${MODULE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${MODULE_NAME}")

#resources - clean and copy from scratch
add_custom_command( 
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf resources)
add_custom_command( 
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/../../resources
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources")
