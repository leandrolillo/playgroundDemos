#executable
set(MODULE_NAME roadFighter)
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
            ${CMAKE_SOURCE_DIR}/resources
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/axes.obj
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/axes.obj")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/axes.mtl
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/axes.mtl")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/corvette.obj
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/corvette.obj")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/corvette.mtl
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/corvette.mtl")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/texturedCube.obj
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/texturedCube.obj")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/texturedCube.mtl
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/texturedCube.mtl")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/palettesmall.png
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/palettesmall.png")
add_custom_command(
    TARGET ${MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/resources/audio/background.ogg
            "${CMAKE_BINARY_DIR}/${MODULE_NAME}/resources/background.ogg")
