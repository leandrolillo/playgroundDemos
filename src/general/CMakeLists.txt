#executable
set(MODULE_NAME generalDemo)
add_executable(${MODULE_NAME} GeneralDemo.cpp)

#sources
target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../base)
target_link_libraries(${MODULE_NAME} playground openGLRunner audioRunner)

#resources
add_custom_command( 
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/../../resources
            resources)
