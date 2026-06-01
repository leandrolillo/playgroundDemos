#executable
set(MODULE_NAME objDemo)
add_executable(${MODULE_NAME})
set_target_properties(${MODULE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${MODULE_NAME}")

add_subdirectory(src)

#resources - clean and copy from scratch
add_custom_command( 
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources")
add_custom_command( 
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/resources
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources")
