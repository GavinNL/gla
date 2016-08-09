cmake_minimum_required(VERSION 2.8)

project(gla)

FILE(GLOB_RECURSE HeaderFiles   "./gla/*" "./examples")
FILE(GLOB_RECURSE ResourceFiles "./resources/*")
FILE(GLOB_RECURSE OtherFiles    "./README" "Doxygen.cfg" ".gitignore")

add_custom_target(headers   SOURCES ${HeaderFiles})
add_custom_target(resources SOURCES ${ResourceFiles})
add_custom_target(other     SOURCES ${OtherFiles})

set(MYLIB_VERSION_MAJOR 0)
set(MYLIB_VERSION_MINOR 1)
set(MYLIB_VERSION_PATCH 1)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})


include_directories ("${PROJECT_SOURCE_DIR}")

#===========================================================================
# Compiler definitions
#===========================================================================
ADD_DEFINITIONS (
    -std=c++11 )
#===========================================================================
# Find packages
#===========================================================================
#SET( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules"
#                       "${CMAKE_MODULE_PATH}" )

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS} )

# A variable of all the libraries we need
set(LIBS_FULL
    ${GLFW_STATIC_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${GLEW_LIBRARIES}
)

set(LIBS_GLAD
    ${GLFW_STATIC_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
)


#  add_executable(TextureArray
#                 examples/TextureArray.cpp)
#  target_link_libraries (TextureArray ${LIBS_FULL})
#
#  add_executable(HelloTriangle
#                 examples/HelloTriangle.cpp)
#  target_link_libraries (HelloTriangle ${LIBS_FULL})
#
#  add_executable(HelloTriangleVAO
#                 examples/HelloTriangleVAO.cpp)
#  target_link_libraries (HelloTriangleVAO ${LIBS_FULL})
#
#  add_executable(HelloTriangleEncapsulatedVAO
#                 examples/HelloTriangleEncapsulatedVAO.cpp)
#  target_link_libraries (HelloTriangleEncapsulatedVAO ${LIBS_FULL})
#
#  add_executable(Textures
#                 examples/Textures.cpp)
#  target_link_libraries (Textures ${LIBS_FULL})
#
#  #        add_executable(TransformSequence
#  #                       examples/TransformSequence.cpp)
#  #target_link_libraries (TransformSequence ${LIBS_FULL})
#
#  add_executable(UniformBuffer
#        examples/UniformBuffer.cpp)
#  target_link_libraries (UniformBuffer ${LIBS_FULL})
#
#  add_executable(FrameBufferObject
#        examples/FrameBufferObject.cpp)
#  target_link_libraries (FrameBufferObject ${LIBS_FULL})
#
#  add_executable(Tesselation
#        examples/Tesselation.cpp)
#  target_link_libraries (Tesselation ${LIBS_FULL})
#
#  add_executable(Shadows
#        examples/Shadows.cpp)
#  target_link_libraries (Shadows ${LIBS_FULL})
#

        add_executable(01_HelloTriangle
              examples/01_HelloTriangle.cpp glad.c)
target_link_libraries (01_HelloTriangle ${LIBS_GLAD})

        add_executable(02_HelloTriangleIndexed
              examples/02_HelloTriangleIndexed.cpp glad.c)
target_link_libraries (02_HelloTriangleIndexed ${LIBS_GLAD})

        add_executable(03_HelloTriangleVertexArray
              examples/03_HelloTriangleVertexArray.cpp glad.c)
target_link_libraries (03_HelloTriangleVertexArray ${LIBS_GLAD})

        add_executable(04_HelloTexturedTriangle
              examples/04_HelloTexturedTriangle.cpp glad.c)
target_link_libraries (04_HelloTexturedTriangle ${LIBS_GLAD})

       add_executable(05_Sampler2DArray
             examples/05_Sampler2DArray.cpp glad.c)
target_link_libraries(05_Sampler2DArray ${LIBS_GLAD})
# This command b
#add_custom_command(
#    TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND rm resources || true
#    COMMAND ln -s ${PROJECT_SOURCE_DIR}/resources resources
#)


