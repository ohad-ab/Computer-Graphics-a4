cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 14)

SET(PROJ_NAME1 "Game")
project(${PROJ_NAME1})
file(GLOB GAME ${PROJ_NAME1}/*.cpp ${PROJ_NAME1}/*.h)

SET(PROJ_NAME2 "Mbrot")
project(${PROJ_NAME2})
file(GLOB MBROT ${PROJ_NAME2}/*.cpp ${PROJ_NAME2}/*.h)

SET(PROJ_NAME3 "Raytracing")
project(${PROJ_NAME3})
file(GLOB RAYS ${PROJ_NAME3}/*.cpp ${PROJ_NAME3}/*.h)

SET(PROJ_NAME4 "RubiksCube")
project(${PROJ_NAME4})
file(GLOB RUBIKS ${PROJ_NAME4}/*.cpp ${PROJ_NAME4}/*.h)

SET(PROJ_NAME5 "IK")
project(${PROJ_NAME5})
file(GLOB IK ${PROJ_NAME5}/*.cpp ${PROJ_NAME5}/*.h)


file(GLOB ENGINE Engine3D/*.cpp Engine3D/*.c  Engine3D/*.h)
file(GLOB DISPLAY DisplayGLFW/*.cpp DisplayGLFW/*.h)

include_directories(${PROJECT_SOURCE_DIR}/res/includes ${PROJECT_SOURCE_DIR}/Engine3D
 ${PROJECT_SOURCE_DIR}/DisplayGLFW)
#add_subdirectory(${PROJECT_SOURCE_DIR}/res)

add_library(Display ${DISPLAY})
add_library(Engine ${ENGINE})

add_executable(${PROJ_NAME1} ${GAME})
target_link_libraries(${PROJ_NAME1} Engine Display ${PROJECT_SOURCE_DIR}/res/libs/glew32s.lib ${PROJECT_SOURCE_DIR}/res/libs/glfw3.lib opengl32.lib)

add_executable(${PROJ_NAME2} ${MBROT})
target_link_libraries(${PROJ_NAME2} Engine Display ${PROJECT_SOURCE_DIR}/res/libs/glew32s.lib ${PROJECT_SOURCE_DIR}/res/libs/glfw3.lib opengl32.lib)

add_executable(${PROJ_NAME3} ${RAYS})
target_link_libraries(${PROJ_NAME3} Engine Display ${PROJECT_SOURCE_DIR}/res/libs/glew32s.lib ${PROJECT_SOURCE_DIR}/res/libs/glfw3.lib opengl32.lib)

add_executable(${PROJ_NAME4} ${RUBIKS})
target_link_libraries(${PROJ_NAME4} Engine Display ${PROJECT_SOURCE_DIR}/res/libs/glew32s.lib ${PROJECT_SOURCE_DIR}/res/libs/glfw3.lib opengl32.lib)

add_executable(${PROJ_NAME5} ${IK})
target_link_libraries(${PROJ_NAME5} Engine Display ${PROJECT_SOURCE_DIR}/res/libs/glew32s.lib ${PROJECT_SOURCE_DIR}/res/libs/glfw3.lib opengl32.lib)
