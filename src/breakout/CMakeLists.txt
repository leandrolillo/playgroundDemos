#executable
set(MODULE_NAME breakout)
add_executable(${MODULE_NAME})
set_target_properties(${MODULE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${MODULE_NAME}")

add_subdirectory(src)

#resources - clean and copy from scratch (unfortunately it has to be in the same folder as the target creation
add_custom_command( 
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources")
add_custom_command( 
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/level-0.json
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/level-0.json")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/level-1.json
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/level-1.json")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/level-2.json
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/level-2.json")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/resources/images/skybox/top.png
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/background.png")


target_link_libraries(${MODULE_NAME} playground openGLRunner audioRunner physicsRunner)