cmake_minimum_required(VERSION 2.8)
project(gla)
aux_source_directory(./src         SRC_LIST)
aux_source_directory(./src/core    CORE_SRC_LIST)
aux_source_directory(./src/engine  ENGINE_SRC_LIST)
aux_source_directory(./src/utils   ENGINE_SRC_LIST)

aux_source_directory(./gla         INC_LIST)

set(MYLIB_VERSION_MAJOR 0)
set(MYLIB_VERSION_MINOR 1)
set(MYLIB_VERSION_PATCH 1)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})


SET( MAKE_SHARED   FALSE CACHE BOOL "Make a shared libary instead of a static.")



include_directories ("${PROJECT_SOURCE_DIR}")


SET( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules"
                       "${CMAKE_MODULE_PATH}" )

#find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS})
#set(LIBS ${LIBS} ${SDL2_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLEW_LIBRARIES})

find_package(GLU REQUIRED)
include_directories(${GLU_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLU_LIBRARIES})


#============================
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories( ${GLFW_INCLUDE_DIRS} )
SET( TEST   ${GLFW_INCLUDE_DIRS})

#target_link_libraries(simple ${GLFW_LIBRARIES})

#target_link_libraries(simple ${GLFW_STATIC_LIBRARIES})
#============================

FIND_PACKAGE(OpenGL REQUIRED)

IF(MAKE_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
ELSE(MAKE_SHARED)
    add_library(${PROJECT_NAME}engine STATIC ${ENGINE_SRC_LIST})
ENDIF(MAKE_SHARED)


set(LIBS_FULL     ${PROJECT_NAME}engine assimp       ${GLFW_STATIC_LIBRARIES} ${OPENGL_glu_LIBRARY}  ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${GLU_LIBRARIES}   )
set(LIBS_TEST     ${PROJECT_NAME}engine z assimp      ${GLFW_STATIC_LIBRARIES} ${OPENGL_glu_LIBRARY}  ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${GLU_LIBRARIES}   )
set(LIBS_CORE     ${PROJECT_NAME}engine ${GLFW_STATIC_LIBRARIES} ${OPENGL_glu_LIBRARY}  ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${GLU_LIBRARIES} )

#set_target_properties(${PROJECT_NAME}core     PROPERTIES VERSION ${MYLIB_VERSION_STRING}
#                                               SOVERSION ${MYLIB_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME}engine   PROPERTIES VERSION ${MYLIB_VERSION_STRING}
                                               SOVERSION ${MYLIB_VERSION_MAJOR})

INSTALL( DIRECTORY gla DESTINATION include )

#install(TARGETS glacore
#     RUNTIME DESTINATION bin
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib
#)

install(TARGETS glaengine
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION lib
     ARCHIVE DESTINATION lib
)

#file(COPY resources/testskin.zip   DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY resources/example.xml    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(main main.cpp)
target_link_libraries (main ${LIBS_FULL})

        add_executable(TextureArray
                       examples/TextureArray.cpp)
target_link_libraries (TextureArray ${LIBS_FULL})

        add_executable(HelloTriangle
                       examples/HelloTriangle.cpp)
target_link_libraries (HelloTriangle ${LIBS_CORE})

        add_executable(HelloTriangleVAO
                       examples/HelloTriangleVAO.cpp)
target_link_libraries (HelloTriangleVAO ${LIBS_CORE})

        add_executable(HelloTriangleEncapsulatedVAO
                       examples/HelloTriangleEncapsulatedVAO.cpp)
target_link_libraries (HelloTriangleEncapsulatedVAO ${LIBS_CORE})


        add_executable(Textures
                       examples/Textures.cpp)
target_link_libraries (Textures ${LIBS_CORE})

        add_executable(TransformSequence
                       examples/TransformSequence.cpp)
target_link_libraries (TransformSequence ${LIBS_FULL})

        add_executable(UniformBuffer
              examples/UniformBuffer.cpp)
target_link_libraries (UniformBuffer ${LIBS_FULL})

        add_executable(engine
              examples/engine.cpp)
target_link_libraries (engine ${LIBS_FULL})

add_executable(test test.cpp)
target_link_libraries (test ${LIBS_FULL})

#add_executable(nogui test.cpp)
#target_link_libraries (nogui ${LIBS_TEST})

ADD_DEFINITIONS (
    -std=c++11 # Or
    #-std=c++0x
    # Other flags
)
