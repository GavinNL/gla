#-------------------------------------------------
#
# Project created by QtCreator 2016-06-29T13:56:00
#
# This is just a testing QT Project to use while I
# do some tests on MSVC. Use CMake instead.
#-------------------------------------------------

QT -= core gui uitools widgets opengl svg testlib dbus concurrent network

TARGET   = 01_HelloTriangle

TEMPLATE = app

DEFINES += NOMINMAX

INCLUDEPATH += $$PWD/../

#SOURCES  += ../examples/01_HelloTriangle.cpp ../glad.cpp
#SOURCES  += ../examples/02_HelloTriangleIndexed.cpp ../glad.cpp
#SOURCES  += ../examples/03_HelloTriangleVertexArray.cpp ../glad.cpp
#SOURCES  += ../examples/04_HelloTexturedTriangle.cpp ../glad.cpp
#SOURCES  += ../examples/05_Sampler2DArray.cpp ../glad.cpp
#SOURCES  += ../examples/06_UniformBuffer.cpp ../glad.cpp
SOURCES  += ../examples/07_FrameBuffer.cpp ../glad.cpp

CONFIG   += c++11

win32:CONFIG(release, debug|release):    LIBS += -L$$PWD/../deps -lglfw3 -lopengl32 -lgdi32 -luser32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../deps -lglfw3 -lopengl32 -lgdi32 -luser32
else:unix:                               LIBS += -L$$PWD/../deps -lglfw3 -lopengl32 -lgdi32 -luser32
