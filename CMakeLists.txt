cmake_minimum_required(VERSION 3.3)

project(gla)

FILE(GLOB_RECURSE HeaderFiles   "./gla/*" "./examples")
FILE(GLOB_RECURSE ResourceFiles "./resources/*")
FILE(GLOB_RECURSE OtherFiles    "./README.md" "Doxygen.cfg" ".gitignore")

add_custom_target(headers   SOURCES ${HeaderFiles})
add_custom_target(resources SOURCES ${ResourceFiles})
add_custom_target(other     SOURCES ${OtherFiles})

# set(MYLIB_VERSION_MAJOR 0)
# set(MYLIB_VERSION_MINOR 1)
# set(MYLIB_VERSION_PATCH 1)
# set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})



set(BUILD_EXAMPLES ON CACHE BOOL "Build Examples")

include_directories ("${PROJECT_SOURCE_DIR}")

#===========================================================================
# Compiler definitions
#===========================================================================
if (UNIX)
    
ADD_DEFINITIONS (
    -std=c++11
    -Wfloat-equal
    -Werror=shadow
    -Wpointer-arith
    -Wcast-align
    -Wstrict-overflow=5
    -Wwrite-strings
    -Wcast-qual
    -Werror=switch-default
    -Wswitch-enum
    -Wunreachable-code
    -Werror=return-type
    -Werror=cast-qual
    -Werror=pedantic
    -pedantic-errors
  #  -Wfatal-errors
)

endif (UNIX)
#===========================================================================
# Find packages
#===========================================================================
SET( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules"
                       "${CMAKE_MODULE_PATH}" )

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS} )

# A variable of all the libraries we need
set(LIBS_FULL
    ${GLFW_STATIC_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    dl
    glfw
)


set(LIBS_GLAD
    ${GLFW_STATIC_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    dl
    glfw
)

set(LIBS
    ${LIBS_GLAD}
)


function(BuildExample base_name)
  add_executable(${base_name}  examples/${base_name}.cpp glad.cpp)
  target_link_libraries (${base_name} ${LIBS})
endfunction()


BuildExample(01_01_HelloTriangle)
BuildExample(01_02_HelloTriangleMultiBuffer)
BuildExample(02_HelloTriangleIndexed)
BuildExample(03_HelloTriangleVertexArray)
BuildExample(04_HelloTexturedTriangle)
BuildExample(05_Sampler2DArray)
BuildExample(05_02_SamplerCube)
BuildExample(06_UniformBuffer)
BuildExample(07_01_FrameBuffer)
BuildExample(07_02_RenderToTexture)
BuildExample(08_MeshBuffer)
BuildExample(09_InstanceRendering)
BuildExample(10_01_Shadows)
BuildExample(10_02_PointShadows)
BuildExample(11_01_Filters)
BuildExample(11_02_Sampler2dFilter)

# This command b
add_custom_command(
   TARGET 01_01_HelloTriangle POST_BUILD
   COMMAND rm resources || true
   COMMAND ln -s ${PROJECT_SOURCE_DIR}/resources resources)



