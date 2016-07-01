cmake_minimum_required(VERSION 2.8)

project(gla)

#aux_source_directory(./examples SRC_LIST)


FILE(GLOB_RECURSE HeaderFiles   "./gla/*" "./examples")
FILE(GLOB_RECURSE ResourceFiles "./resources/*")
FILE(GLOB_RECURSE OtherFiles    "./README" "Doxygen.cfg")

add_custom_target(headers   SOURCES ${HeaderFiles})
add_custom_target(resources SOURCES ${ResourceFiles})
add_custom_target(other     SOURCES ${OtherFiles})

set(MYLIB_VERSION_MAJOR 0)
set(MYLIB_VERSION_MINOR 1)
set(MYLIB_VERSION_PATCH 1)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})


SET( MAKE_SHARED   FALSE CACHE BOOL "Make a shared libary instead of a static.")


include_directories ("${PROJECT_SOURCE_DIR}")


#===========================================================================
# Compiler definitions
#===========================================================================
ADD_DEFINITIONS (
    -std=c++11 # Or  -std=c++0x
    # Other flags
)
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
include_directories(${GLU_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS} )

# A variable of all the libraries we need
set(LIBS_FULL
                  ${GLFW_STATIC_LIBRARIES}
                  ${OPENGL_glu_LIBRARY}
                  ${OPENGL_gl_LIBRARY}
                  ${GLEW_LIBRARIES}
                  #${GLU_LIBRARIES}
)



#===========================================================================
# BUILD OUTPUT FILES
# This is where we define what we are creating
#===========================================================================

# Create the a library file
# IF(MAKE_SHARED)
#    add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
# ELSE(MAKE_SHARED)
#    add_library(${PROJECT_NAME} STATIC ${SRC_LIST})
# ENDIF(MAKE_SHARED)

#set_target_properties(${PROJECT_NAME}  PROPERTIES VERSION ${MYLIB_VERSION_STRING}
#                                                SOVERSION ${MYLIB_VERSION_MAJOR})


#Create an exeutable
#        add_executable(main main.cpp)
#target_link_libraries (main ${LIBS_FULL})


#===========================================================================
# INSTALLATION DIRECTORIES
#===========================================================================
#Copy all files from the gla folder to the <installprefix>/include folder
# INSTALL( DIRECTORY gla DESTINATION include )
#
#
# # Copy all files in the resource directory to <installprefix>/share/<projectname>
# INSTALL(DIRECTORY     ${CMAKE_CURRENT_LIST_DIR}/resources
#        DESTINATION    share/${PROJECT_NAME})

#===========================================================================

add_executable(TextureArray
               examples/TextureArray.cpp)
target_link_libraries (TextureArray ${LIBS_FULL})

add_executable(HelloTriangle
               examples/HelloTriangle.cpp)
target_link_libraries (HelloTriangle ${LIBS_FULL})

add_executable(HelloTriangleVAO
               examples/HelloTriangleVAO.cpp)
target_link_libraries (HelloTriangleVAO ${LIBS_FULL})

add_executable(HelloTriangleEncapsulatedVAO
               examples/HelloTriangleEncapsulatedVAO.cpp)
target_link_libraries (HelloTriangleEncapsulatedVAO ${LIBS_FULL})

add_executable(Textures
               examples/Textures.cpp)
target_link_libraries (Textures ${LIBS_FULL})

#        add_executable(TransformSequence
#                       examples/TransformSequence.cpp)
#target_link_libraries (TransformSequence ${LIBS_FULL})

add_executable(UniformBuffer
      examples/UniformBuffer.cpp)
target_link_libraries (UniformBuffer ${LIBS_FULL})

add_executable(FrameBufferObject
      examples/FrameBufferObject.cpp)
target_link_libraries (FrameBufferObject ${LIBS_FULL})

add_executable(Tesselation
      examples/Tesselation.cpp)
target_link_libraries (Tesselation ${LIBS_FULL})


# This command b
#add_custom_command(
#    TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND rm resources
#    COMMAND ln -s ${PROJECT_SOURCE_DIR}/resources resources
#)


