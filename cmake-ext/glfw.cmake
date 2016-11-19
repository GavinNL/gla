include(ExternalProject)
 
set(glfw_PREFIX  ${CMAKE_BINARY_DIR}/cmake-ext/glfw-prefix)
set(glfw_INSTALL ${CMAKE_BINARY_DIR}/cmake-ext/glfw-install)

ExternalProject_Add(glfw
    PREFIX ${glfw_PREFIX}
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.2.1"
    INSTALL_DIR ${glfw_INSTALL}
    UPDATE_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${glfw_INSTALL}
)

include_directories(${glfw_INSTALL}/include)

if (LINUX)

endif()



if (UNIX AND NOT APPLE)
set(glfw_LIB ${glfw_INSTALL}/lib/libglfw3.a rt m dl Xrandr Xinerama Xxf86vm Xext Xcursor Xrender Xfixes X11 pthread xcb Xau Xdmcp GL)
endif()

if (MSVC)
set(glfw_LIB ${glfw_INSTALL}/lib/libglfw3.a rt m dl Xrandr Xinerama Xxf86vm Xext Xcursor Xrender Xfixes X11 pthread xcb Xau Xdmcp GL)
endif()

set(glm_LIB ${glm_INSTALL}/lib/libglm3.a rt m dl Xrandr Xinerama Xxf86vm Xext Xcursor Xrender Xfixes X11 pthread xcb Xau Xdmcp GL)
