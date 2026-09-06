#sources
target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../base)

add_subdirectory(adapters)

target_sources(${MODULE_NAME} PUBLIC Breakout.cpp BreakoutStates.cpp)