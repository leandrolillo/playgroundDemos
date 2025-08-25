FetchContent_Declare(
    playground
    GIT_REPOSITORY https://github.com/leandrolillo/playground.git
    GIT_TAG master
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(playground)


add_subdirectory(general)
add_subdirectory(collisionDetection)
add_subdirectory(physics)
#add_subdirectory(terrain)
add_subdirectory(obj)
add_subdirectory(roadFighter)