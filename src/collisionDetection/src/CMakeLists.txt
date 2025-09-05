#sources
target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${MODULE_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../base)

target_sources(${MODULE_NAME} PUBLIC CollisionDetectionDemo.cpp)

target_link_libraries(${MODULE_NAME} playground audioRunner openGLRunner physicsRunner)
