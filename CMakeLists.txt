cmake_minimum_required(VERSION 2.8)
project(glre)
aux_source_directory(./src  SRC_LIST)
aux_source_directory(./glre INC_LIST)

set(MYLIB_VERSION_MAJOR 0)
set(MYLIB_VERSION_MINOR 1)
set(MYLIB_VERSION_PATCH 1)
set(MYLIB_VERSION_STRING ${MYLIB_VERSION_MAJOR}.${MYLIB_VERSION_MINOR}.${MYLIB_VERSION_PATCH})


SET( MAKE_SHARED   FALSE CACHE BOOL "Make a shared library instead of a static.")



include_directories ("${PROJECT_SOURCE_DIR}")


SET( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules"
                       "${CMAKE_MODULE_PATH}" )

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(LIBS ${LIBS} ${SDL2_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLEW_LIBRARIES})

find_package(GLU REQUIRED)
include_directories(${GLU_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLU_LIBRARIES})


FIND_PACKAGE(OpenGL REQUIRED)

IF(MAKE_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
ELSE(MAKE_SHARED)
    add_library(${PROJECT_NAME} STATIC ${SRC_LIST})
ENDIF(MAKE_SHARED)


set(LIBS ${PROJECT_NAME} z assimp rgui ${OPENGL_glu_LIBRARY}  ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${GLU_LIBRARIES} ${SDL2_LIBRARY}  )

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${MYLIB_VERSION_STRING}
                                           SOVERSION ${MYLIB_VERSION_MAJOR})

INSTALL( DIRECTORY glre DESTINATION include )

install(TARGETS glre
     RUNTIME DESTINATION bin
     LIBRARY DESTINATION lib
     ARCHIVE DESTINATION lib
)

#file(COPY resources/testskin.zip   DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY resources/example.xml    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})



add_executable(main main.cpp)
target_link_libraries (main ${LIBS})


ADD_DEFINITIONS (
    -std=c++11 # Or
    #-std=c++0x
    # Other flags
)
